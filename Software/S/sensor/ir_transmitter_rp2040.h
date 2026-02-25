#pragma once
#include "esphome.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"

// ──────────────────────────────────────────────────────────────
//  IR Transmitter for RP2040 — NEC and Samsung protocols
//  IR LED Pin: Configured in remote_tran.yaml (GPIO3)
// ──────────────────────────────────────────────────────────────

// PIO program: generates 38kHz bursts for N cycles, then silence.
// The OSR register contains: [31:16] = burst cycles, [15:0] = silent cycles
// PIO clock frequency = 38000 * 2 = 76000 Hz → each tick = 1/(76kHz)

static const uint16_t ir_pio_program_instructions[] = {
  // .wrap_target
  0x80a0, //  0: pull   block           ; wait data
  0x6040, //  1: out    y, 16           ; y = burst cycle
  0x6020, //  2: out    x, 16           ; x = silence cycle
  // burst loop
  0xe001, //  3: set    pins, 1         ; LED ON
  0x0044, //  4: jmp    y--, 4  [0]     ; burst cycle counter (with delay)
  // silence loop
  0xe000, //  5: set    pins, 0         ; LED OFF
  0x0046, //  6: jmp    x--, 6  [0]     ; counts silence cycles
  // .wrap → back to 0
};

// Carrier frequency 38kHz → period = 26.3µs
// We use PIO clock = 76kHz (2 ticks per half-period)
// Each "burst cycle" = 1 tick ON + 1 tick OFF (toggling managed in the loop)
// To simplify: we use a 1MHz clock and count µs directly

// ── Timing NEC (µs) ──────────────────────
#define NEC_HDR_MARK   9000
#define NEC_HDR_SPACE  4500
#define NEC_BIT_MARK    562
#define NEC_ONE_SPACE  1687
#define NEC_ZER_SPACE   562
#define NEC_TRAIL_MARK  562

// ── Timing Samsung (µs) ──────────────────
#define SAM_HDR_MARK   4500
#define SAM_HDR_SPACE  4500
#define SAM_BIT_MARK    562
#define SAM_ONE_SPACE  1687
#define SAM_ZER_SPACE   562
#define SAM_TRAIL_MARK  562

// ── Carrier 38kHz ────────────────────────
#define CARRIER_HZ     38000
#define CARRIER_PERIOD_US  (1000000 / CARRIER_HZ)  // ~26µs

class IRTransmitterRP2040 : public Component {
 public:
  uint8_t pin_;

  IRTransmitterRP2040(uint8_t pin) : pin_(pin) {}

  void setup() override {
    pinMode(pin_, OUTPUT);
    digitalWrite(pin_, LOW);
    ESP_LOGI("ir_tx", "IR Transmitter ready on GPIO%d", pin_);
  }

  // ── Funzioni di basso livello ─────────────────────────────

  void mark(uint32_t us) {
    // Burst at 38kHz for 'us' microseconds
    uint32_t cycles = us / CARRIER_PERIOD_US;
    for (uint32_t i = 0; i < cycles; i++) {
      digitalWrite(pin_, HIGH);
      delayMicroseconds(CARRIER_PERIOD_US / 2);
      digitalWrite(pin_, LOW);
      delayMicroseconds(CARRIER_PERIOD_US / 2);
    }
  }

  void space(uint32_t us) {
    digitalWrite(pin_, LOW);
    delayMicroseconds(us);
  }

  // ── Send bit LSB-first (32 bit) ─────────────────────────

  void send_bits(uint32_t data, uint8_t nbits,
                 uint32_t bit_mark, uint32_t one_space, uint32_t zero_space) {
    for (int i = 0; i < nbits; i++) {
      mark(bit_mark);
      if (data & 1)
        space(one_space);
      else
        space(zero_space);
      data >>= 1;
    }
  }

  // ── Protocol NEC ───────────────────────────────────────
  // Frame: header | address(8) | ~address(8) | command(8) | ~command(8) | trail
  void send_nec(uint16_t address, uint16_t command) {
    ESP_LOGI("ir_tx", "NEC → addr=0x%04X cmd=0x%04X", address, command);

    // Header
    mark(NEC_HDR_MARK);
    space(NEC_HDR_SPACE);

    // NEC extended (16-bit address) or standard (8-bit + complement)
    if (address > 0xFF) {
      // Extended NEC: address a 16 bit
      send_bits(address, 16, NEC_BIT_MARK, NEC_ONE_SPACE, NEC_ZER_SPACE);
    } else {
      // Standard NEC: address + ~address
      send_bits(address & 0xFF, 8, NEC_BIT_MARK, NEC_ONE_SPACE, NEC_ZER_SPACE);
      send_bits(~address & 0xFF, 8, NEC_BIT_MARK, NEC_ONE_SPACE, NEC_ZER_SPACE);
    }

    // Command + ~command
    send_bits(command & 0xFF, 8, NEC_BIT_MARK, NEC_ONE_SPACE, NEC_ZER_SPACE);
    send_bits(~command & 0xFF, 8, NEC_BIT_MARK, NEC_ONE_SPACE, NEC_ZER_SPACE);

    // Trail
    mark(NEC_TRAIL_MARK);
    space(40000);  // final pause ~40ms
  }

  // ── Protocoll Samsung ───────────────────────────────────
  // Frame: header | address(8) | address(8) | command(8) | ~command(8) | trail
  // (Samsung repeats the address instead of the complement)
  void send_samsung(uint8_t address, uint8_t command) {
    ESP_LOGI("ir_tx", "Samsung → addr=0x%02X cmd=0x%02X", address, command);

    // Header
    mark(SAM_HDR_MARK);
    space(SAM_HDR_SPACE);

    // Address repeated twice
    send_bits(address, 8, SAM_BIT_MARK, SAM_ONE_SPACE, SAM_ZER_SPACE);
    send_bits(address, 8, SAM_BIT_MARK, SAM_ONE_SPACE, SAM_ZER_SPACE);

    // Command + ~command
    send_bits(command, 8, SAM_BIT_MARK, SAM_ONE_SPACE, SAM_ZER_SPACE);
    send_bits(~command & 0xFF, 8, SAM_BIT_MARK, SAM_ONE_SPACE, SAM_ZER_SPACE);

    // Trail
    mark(SAM_TRAIL_MARK);
    space(40000);
  }
};

// Global instance accessible from YAML lambdas
IRTransmitterRP2040 *ir_tx = nullptr;
