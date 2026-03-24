// modbus_slave.h
// Complete Modbus RTU Slave for ESPHome / RP2040
// Transceiver: MAX13487E (auto-direction, no DE/RE pin)
//
// Supported Function Codes:
// FC01 (0x01) — Read Coils
// FC02 (0x02) — Read Discrete Inputs
// FC03 (0x03) — Read Holding Registers
// FC04 (0x04) — Read Input Registers
// FC05 (0x05) — Write Single Coil
// FC06 (0x06) — Write Single Register
// FC0F (0x0F) — Write Multiple Coils
// FC10 (0x10) — Write Multiple Registers
// FC11 (0x11) — Report Slave ID
// FC16 (0x16) — Mask Write Register
// FC17 (0x17) — Read/Write Multiple Registers
//
// Data tables:
// modbus_regs[] — Holding Registers (R/W from master)
// modbus_input_regs[] — Input Registers (R only, written by sensors)
// modbus_coils[] — Coils (R/W from master)
// modbus_discrete[] — Discrete Inputs (R only, written by sensors)

#pragma once
#include "esphome.h"

// ──────────────────────────────────────────
// Configuration
// ──────────────────────────────────────────
#ifndef MODBUS_SLAVE_ADDRESS
  #define MODBUS_SLAVE_ADDRESS  0x01  // fallback if not passed by build_flags
#endif
#define MODBUS_NUM_REGS         32    // Holding Registers
#define MODBUS_NUM_INPUT_REGS   32    // Input Registers
#define MODBUS_NUM_COILS        32    // Coils
#define MODBUS_NUM_DISCRETE     32    // Discrete Inputs
#define MODBUS_TIMEOUT_MS       5     // ms silence = end of frame
#define MODBUS_SLAVE_ID_STR     "RP2040-SENSOR-NODE-v1.0"

// ──────────────────────────────────────────
// Shared data tables with yaml
// ──────────────────────────────────────────

// Holding Registers — R/W — written by the master or updated by sensors
uint16_t modbus_regs[MODBUS_NUM_REGS] = {0};

// Input Registers — Read Only from the master — Updated only internally
// Use these for sensor values ​​that the master should not be able to modify
uint16_t modbus_input_regs[MODBUS_NUM_INPUT_REGS] = {0};

// Coils — R/W — master-controllable output bits
bool modbus_coils[MODBUS_NUM_COILS] = {0};

// Discrete Inputs — Read Only from the master — Boolean sensor states
bool modbus_discrete[MODBUS_NUM_DISCRETE] = {0};

// ──────────────────────────────────────────
// Statistic
// ──────────────────────────────────────────
struct ModbusStats {
  uint32_t frames_ok     = 0;
  uint32_t frames_crc    = 0;
  uint32_t frames_error  = 0;
  uint32_t last_fc       = 0;
} modbus_stats;

// ──────────────────────────────────────────
// CRC16 Modbus
// ──────────────────────────────────────────
uint16_t modbus_crc16(const uint8_t *buf, uint8_t len) {
  uint16_t crc = 0xFFFF;
  for (uint8_t i = 0; i < len; i++) {
    crc ^= buf[i];
    for (uint8_t j = 0; j < 8; j++)
      crc = (crc & 0x0001) ? (crc >> 1) ^ 0xA001 : crc >> 1;
  }
  return crc;
}

// ──────────────────────────────────────────
// Send reply (adds CRC automatically)
// ──────────────────────────────────────────
void modbus_send(UARTComponent *uart, uint8_t *buf, uint8_t len) {
  uint16_t crc = modbus_crc16(buf, len);
  for (uint8_t i = 0; i < len; i++) uart->write_byte(buf[i]);
  uart->write_byte(crc & 0xFF);
  uart->write_byte((crc >> 8) & 0xFF);
  uart->flush();  // waits for complete physical transmission
                  // required for MAX13487E auto-direction
}

// ─────────────────────────────────────────
// Modbus Exception Response
// Standard Exception Codes:
// 0x01 — Illegal Function
// 0x02 — Illegal Data Address
// 0x03 — Illegal Data Value
// 0x04 — Slave Device Failure
// ─────────────────────────────────────────
void modbus_send_error(UARTComponent *uart, uint8_t fc, uint8_t code) {
  uint8_t resp[3];
  resp[0] = MODBUS_SLAVE_ADDRESS;
  resp[1] = fc | 0x80;
  resp[2] = code;
  modbus_send(uart, resp, 3);
  modbus_stats.frames_error++;
  ESP_LOGW("modbus", "Exception FC=0x%02X code=0x%02X", fc, code);
}

// ──────────────────────────────────────────
// Utility: Pack bool array into bytes
// LSB first: coil/discrete 0 → bit0 byte0
//            coil/discrete 7 → bit7 byte0
//            coil/discrete 8 → bit0 byte1 ...
// ──────────────────────────────────────────
void pack_bits(uint8_t *dest, const bool *src, uint16_t start, uint16_t qty) {
  uint8_t byte_count = (qty + 7) / 8;
  for (uint8_t b = 0; b < byte_count; b++) {
    dest[b] = 0;
    for (uint8_t bit = 0; bit < 8; bit++) {
      uint16_t idx = start + b * 8 + bit;
      if (idx < start + qty && (src[idx]))
        dest[b] |= (1 << bit);
    }
  }
}

// ──────────────────────────────────────────
// Utility: unpack bytes into bool arrays
// ──────────────────────────────────────────
void unpack_bits(bool *dest, const uint8_t *src, uint16_t start, uint16_t qty) {
  for (uint16_t i = 0; i < qty; i++) {
    uint8_t byte_idx = i / 8;
    uint8_t bit_idx  = i % 8;
    dest[start + i] = (src[byte_idx] >> bit_idx) & 0x01;
  }
}

// ──────────────────────────────────────────
// Process received Modbus frame
// ──────────────────────────────────────────
void modbus_process_frame(UARTComponent *uart, uint8_t *frame, uint8_t len) {

  if (len < 4) return;

  // Ignore frames not addressed to this slave
  // addr 0x00 = broadcast: no response but execute the command
  bool is_broadcast = (frame[0] == 0x00);
  if (!is_broadcast && frame[0] != MODBUS_SLAVE_ADDRESS) return;

  // Check CRC
  uint16_t crc_recv = frame[len-2] | ((uint16_t)frame[len-1] << 8);
  uint16_t crc_calc = modbus_crc16(frame, len - 2);
  if (crc_recv != crc_calc) {
    modbus_stats.frames_crc++;
    ESP_LOGW("modbus", "CRC error recv=0x%04X calc=0x%04X len=%d", crc_recv, crc_calc, len);
    return;
  }

  uint8_t  fc         = frame[1];
  uint16_t start_addr = (len > 3) ? ((frame[2] << 8) | frame[3]) : 0;
  uint16_t quantity   = (len > 5) ? ((frame[4] << 8) | frame[5]) : 0;

  modbus_stats.frames_ok++;
  modbus_stats.last_fc = fc;
  ESP_LOGD("modbus", "FC=0x%02X addr=%d qty=%d", fc, start_addr, quantity);

 // ═════════════════════════════════════════
  // FC01 — Read Coils
  // Master reads the coils (Boolean R/W outputs)
  // ════════════════════════════════════════
  if (fc == 0x01) {

    if (quantity == 0 || quantity > 2000)          { modbus_send_error(uart, fc, 0x03); return; }
    if (start_addr + quantity > MODBUS_NUM_COILS)  { modbus_send_error(uart, fc, 0x02); return; }

    uint8_t byte_count = (quantity + 7) / 8;
    uint8_t resp[3 + byte_count];
    resp[0] = MODBUS_SLAVE_ADDRESS;
    resp[1] = 0x01;
    resp[2] = byte_count;
    pack_bits(&resp[3], modbus_coils, start_addr, quantity);
    if (!is_broadcast) modbus_send(uart, resp, 3 + byte_count);
  }

  // ════════════════════════════════════════
  // FC02 — Read Discrete Inputs
  // Master reads digital inputs (R only)
  // e.g., PIR, LD2410 presence, sensor thresholds
  // ════════════════════════════════════════
  else if (fc == 0x02) {

    if (quantity == 0 || quantity > 2000)             { modbus_send_error(uart, fc, 0x03); return; }
    if (start_addr + quantity > MODBUS_NUM_DISCRETE)  { modbus_send_error(uart, fc, 0x02); return; }

    uint8_t byte_count = (quantity + 7) / 8;
    uint8_t resp[3 + byte_count];
    resp[0] = MODBUS_SLAVE_ADDRESS;
    resp[1] = 0x02;
    resp[2] = byte_count;
    pack_bits(&resp[3], modbus_discrete, start_addr, quantity);
    if (!is_broadcast) modbus_send(uart, resp, 3 + byte_count);
  }

  // ════════════════════════════════════════
  // FC03 — Read Holding Registers
  // Master reads R/W registers (16 bits)
  // e.g., setpoints, configurations, sensor values
  // ════════════════════════════════════════
  else if (fc == 0x03) {

    if (quantity == 0 || quantity > 125)            { modbus_send_error(uart, fc, 0x03); return; }
    if (start_addr + quantity > MODBUS_NUM_REGS)    { modbus_send_error(uart, fc, 0x02); return; }

    uint8_t resp[3 + quantity * 2];
    resp[0] = MODBUS_SLAVE_ADDRESS;
    resp[1] = 0x03;
    resp[2] = quantity * 2;
    for (uint16_t i = 0; i < quantity; i++) {
      resp[3 + i*2]     = (modbus_regs[start_addr + i] >> 8) & 0xFF;
      resp[3 + i*2 + 1] =  modbus_regs[start_addr + i] & 0xFF;
    }
    if (!is_broadcast) modbus_send(uart, resp, 3 + quantity * 2);
  }

  // ════════════════════════════════════════
  // FC04 — Read Input Registers
  // Master reads R registers only (16 bits)
  // Use them for "pure" sensor values
  // that the master must not be able to overwrite
  // ════════════════════════════════════════
  else if (fc == 0x04) {

    if (quantity == 0 || quantity > 125)                  { modbus_send_error(uart, fc, 0x03); return; }
    if (start_addr + quantity > MODBUS_NUM_INPUT_REGS)    { modbus_send_error(uart, fc, 0x02); return; }

    uint8_t resp[3 + quantity * 2];
    resp[0] = MODBUS_SLAVE_ADDRESS;
    resp[1] = 0x04;
    resp[2] = quantity * 2;
    for (uint16_t i = 0; i < quantity; i++) {
      resp[3 + i*2]     = (modbus_input_regs[start_addr + i] >> 8) & 0xFF;
      resp[3 + i*2 + 1] =  modbus_input_regs[start_addr + i] & 0xFF;
    }
    if (!is_broadcast) modbus_send(uart, resp, 3 + quantity * 2);
  }

  // ════════════════════════════════════════
  // FC05 — Write Single Coil
  // 0xFF00 = ON, 0x0000 = OFF
  // ════════════════════════════════════════
  else if (fc == 0x05) {

    if (start_addr >= MODBUS_NUM_COILS)   { modbus_send_error(uart, fc, 0x02); return; }
    uint16_t val = (frame[4] << 8) | frame[5];
    if (val != 0xFF00 && val != 0x0000)   { modbus_send_error(uart, fc, 0x03); return; }

    modbus_coils[start_addr] = (val == 0xFF00);
    if (!is_broadcast) modbus_send(uart, frame, len - 2);  // echo
  }

  // ════════════════════════════════════════
  // FC06 — Write Single Register
  // ════════════════════════════════════════
  else if (fc == 0x06) {

    if (start_addr >= MODBUS_NUM_REGS)  { modbus_send_error(uart, fc, 0x02); return; }

    modbus_regs[start_addr] = (frame[4] << 8) | frame[5];
    if (!is_broadcast) modbus_send(uart, frame, len - 2);  // echo
  }

  // ════════════════════════════════════════
  // FC0F (15) — Write Multiple Coils
  // ════════════════════════════════════════
  else if (fc == 0x0F) {

    if (quantity == 0 || quantity > 1968)              { modbus_send_error(uart, fc, 0x03); return; }
    if (start_addr + quantity > MODBUS_NUM_COILS)      { modbus_send_error(uart, fc, 0x02); return; }
    uint8_t byte_count = frame[6];
    if (byte_count != (quantity + 7) / 8)              { modbus_send_error(uart, fc, 0x03); return; }

    unpack_bits(modbus_coils, &frame[7], start_addr, quantity);

    uint8_t resp[6];
    resp[0] = MODBUS_SLAVE_ADDRESS;
    resp[1] = 0x0F;
    resp[2] = (start_addr >> 8) & 0xFF;
    resp[3] =  start_addr & 0xFF;
    resp[4] = (quantity >> 8) & 0xFF;
    resp[5] =  quantity & 0xFF;
    if (!is_broadcast) modbus_send(uart, resp, 6);
  }

  // ════════════════════════════════════════
  // FC10 (16) — Write Multiple Registers
  // ════════════════════════════════════════
  else if (fc == 0x10) {

    if (quantity == 0 || quantity > 123)            { modbus_send_error(uart, fc, 0x03); return; }
    if (start_addr + quantity > MODBUS_NUM_REGS)    { modbus_send_error(uart, fc, 0x02); return; }
    uint8_t byte_count = frame[6];
    if (byte_count != quantity * 2)                 { modbus_send_error(uart, fc, 0x03); return; }

    for (uint16_t i = 0; i < quantity; i++)
      modbus_regs[start_addr + i] = (frame[7 + i*2] << 8) | frame[8 + i*2];

    uint8_t resp[6];
    resp[0] = MODBUS_SLAVE_ADDRESS;
    resp[1] = 0x10;
    resp[2] = (start_addr >> 8) & 0xFF;
    resp[3] =  start_addr & 0xFF;
    resp[4] = (quantity >> 8) & 0xFF;
    resp[5] =  quantity & 0xFF;
    if (!is_broadcast) modbus_send(uart, resp, 6);
  }

  // ════════════════════════════════════════
  // FC11 (17) — Report Slave ID
  // The master asks for information about this slave
  // ════════════════════════════════════════
  else if (fc == 0x11) {

    const char *id_str  = MODBUS_SLAVE_ID_STR;
    uint8_t id_len      = strlen(id_str);
    // reply: addr + fc + byte_count + slave_addr + run_indicator + id_str
    uint8_t resp[4 + id_len];
    resp[0] = MODBUS_SLAVE_ADDRESS;
    resp[1] = 0x11;
    resp[2] = 2 + id_len;       // byte count
    resp[3] = MODBUS_SLAVE_ADDRESS;
    resp[4] = 0xFF;              // run indicator: 0xFF = running
    for (uint8_t i = 0; i < id_len; i++) resp[5 + i] = id_str[i];
    if (!is_broadcast) modbus_send(uart, resp, 4 + id_len);
  }

  // ════════════════════════════════════════
  // FC16 (22) — Mask Write Register
  // Change only certain bits of a register:
  // result = (current AND and_mask) OR (or_mask AND NOT and_mask)
  // ════════════════════════════════════════
  else if (fc == 0x16) {

    if (len < 8)                          { modbus_send_error(uart, fc, 0x03); return; }
    if (start_addr >= MODBUS_NUM_REGS)    { modbus_send_error(uart, fc, 0x02); return; }

    uint16_t and_mask = (frame[4] << 8) | frame[5];
    uint16_t or_mask  = (frame[6] << 8) | frame[7];
    modbus_regs[start_addr] = (modbus_regs[start_addr] & and_mask) | (or_mask & ~and_mask);

    if (!is_broadcast) modbus_send(uart, frame, len - 2);
  }

  // ════════════════════════════════════════
  // FC17 (23) — Read/Write Multiple Registers
  // Write and read in a single command
  // First write, then read and respond
  // ════════════════════════════════════════
  else if (fc == 0x17) {

    if (len < 9)  { modbus_send_error(uart, fc, 0x03); return; }

    uint16_t read_addr  = (frame[2] << 8) | frame[3];
    uint16_t read_qty   = (frame[4] << 8) | frame[5];
    uint16_t write_addr = (frame[6] << 8) | frame[7];
    uint16_t write_qty  = (frame[8] << 8) | frame[9];
    uint8_t  byte_count = frame[10];

    if (read_qty  == 0 || read_qty  > 125)               { modbus_send_error(uart, fc, 0x03); return; }
    if (write_qty == 0 || write_qty > 121)               { modbus_send_error(uart, fc, 0x03); return; }
    if (read_addr  + read_qty  > MODBUS_NUM_REGS)        { modbus_send_error(uart, fc, 0x02); return; }
    if (write_addr + write_qty > MODBUS_NUM_REGS)        { modbus_send_error(uart, fc, 0x02); return; }
    if (byte_count != write_qty * 2)                     { modbus_send_error(uart, fc, 0x03); return; }

    // First write
    for (uint16_t i = 0; i < write_qty; i++)
      modbus_regs[write_addr + i] = (frame[11 + i*2] << 8) | frame[12 + i*2];

    // Then read and answer
    uint8_t resp[3 + read_qty * 2];
    resp[0] = MODBUS_SLAVE_ADDRESS;
    resp[1] = 0x17;
    resp[2] = read_qty * 2;
    for (uint16_t i = 0; i < read_qty; i++) {
      resp[3 + i*2]     = (modbus_regs[read_addr + i] >> 8) & 0xFF;
      resp[3 + i*2 + 1] =  modbus_regs[read_addr + i] & 0xFF;
    }
    if (!is_broadcast) modbus_send(uart, resp, 3 + read_qty * 2);
  }

  // ════════════════════════════════════════
  // Function code not supported
  // ════════════════════════════════════════
  else {
    modbus_send_error(uart, fc, 0x01);
  }
}

// ──────────────────────────────────────────
// Poll UART — called every 10ms from the yaml loop
// ──────────────────────────────────────────
static uint8_t  rx_buf[256];
static uint8_t  rx_len       = 0;
static uint32_t last_byte_ms = 0;

void modbus_slave_poll(UARTComponent *uart) {
  uint32_t now = millis();

  while (uart->available()) {
    uint8_t b;
    if (uart->read_byte(&b)) {
      if (rx_len < sizeof(rx_buf))
        rx_buf[rx_len++] = b;
      last_byte_ms = now;
    }
  }

  if (rx_len > 0 && (now - last_byte_ms) >= MODBUS_TIMEOUT_MS) {
    modbus_process_frame(uart, rx_buf, rx_len);
    rx_len = 0;
    // Discard any bytes that arrived during response transmission.
    // The master may have re-sent the same request (send_count > 1)
    // while we were busy in flush(). Answering those stale duplicates
    // would confuse the master and cause it to log 0 for that cycle.
    delay(2);
    while (uart->available()) { uint8_t dummy; uart->read_byte(&dummy); }
  }
}