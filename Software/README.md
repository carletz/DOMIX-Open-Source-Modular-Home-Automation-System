# DOMIX ESPHome Software

> ESPHome firmware for the DOMIX modular home automation system.  
> The project is split into two independent firmware targets: **M** (Main/Cabinet module) and **S** (Sensor node).

---

## Repository Structure

```
Software/
├── M/                          # Firmware for DOMIX M1 (ESP32-based cabinet controller)
│   ├── main.yaml               # Entry point — include the packages you need
│   ├── hardware/               # Hardware abstraction packages
│   │   ├── data.yaml           # ⭐ Master substitutions file (IP, MQTT, I2C addresses…)
│   │   ├── esp.yaml            # ESP32 board definition
│   │   ├── ethernet.yaml       # Wired Ethernet (W5500 or similar)
│   │   ├── i2c.yaml            # I²C bus — use with M1 v1.0
│   │   ├── i2c_switch.yaml     # I²C bus with switch — use with M1 v1.2 (mutually exclusive with i2c.yaml)
│   │   ├── mqtt.yaml           # MQTT broker connection
│   │   ├── web_server.yaml     # Built-in ESPHome web server
│   │   ├── ota.yaml            # Over-the-air update support
│   │   ├── restart.yaml        # Safe restart button entity
│   │   ├── info.yaml           # Device info sensors (uptime, IP…)
│   │   ├── log.yaml            # Log level configuration
│   │   ├── screen.yaml         # OLED screen (SH1106 128×64 on M1)
│   │   ├── time.yaml           # SNTP time synchronisation
│   │   ├── uart.yaml           # UART for RS485 / serial bridge
│   │   └── zigbee.yaml         # Zigbee coordinator (CC2652, max ESPHome 2025.x)
│   ├── module/                 # One YAML file per expansion module
│   │   ├── m1.yaml             # M1 — OLED display + analogue button menu
│   │   ├── m2.yaml             # M2 — 8-input / 8-output I/O (PCA9554)
│   │   ├── m3.yaml             # M3 — Dry contacts + ADS1115 analogue inputs
│   │   ├── m4.yaml             # M4 — 6-relay output board
│   │   ├── m4_shutters.yaml    # M4 — 6-relay configured as 3 shutter pairs
│   │   ├── m5.yaml             # M5 — 12-relay output board
│   │   ├── m5_shutters.yaml    # M5 — 12-relay configured as 6 shutter pairs
│   │   ├── m6.yaml             # M6 — Single-phase energy meter (ZMPT101B + ADS1115)
│   │   ├── m7.yaml             # M7 — Generic output board
│   │   ├── m8.yaml             # M8 — Generic input board
│   │   └── m9.yaml             # M9 — Sprinkler/valve controller
│   └── sensor/
│       ├── wire.yaml           # DS18B20 1-wire temperature sensor (optional, on M1)
│       └── modbus_master.yaml  # ⭐ Reusable template — reads one DOMIX S sensor node via RS485
│
└── S/                          # Firmware for DOMIX S (RP2040-based room sensor node)
    ├── main.yaml               # Entry point — include the sensor packages you need
    ├── hardware/               # Hardware abstraction packages
    │   ├── data.yaml           # ⭐ Master substitutions file (Modbus address, sensor I2C addresses, IR codes…)
    │   ├── rp2040.yaml         # RP2040 board definition
    │   ├── i2c.yaml            # I²C bus for sensors
    │   ├── modbus.yaml         # Modbus RTU slave (RS485 to M1)
    │   ├── modbus_interval.yaml# Modbus polling interval
    │   ├── modbus_slave.h      # C++ helper for Modbus slave register mapping
    │   ├── serial.yaml         # UART / serial bridge settings
    │   └── log.yaml            # Log level configuration
    ├── sensor/                 # One YAML file per physical sensor
    │   ├── bme280.yaml         # Temperature / Humidity / Pressure (I2C)
    │   ├── bh1750.yaml         # Ambient light in lux (I2C)
    │   ├── scd41.yaml          # CO₂ photoacoustic sensor (I2C)
    │   ├── sgp41.yaml          # VOC + NOx index sensor (I2C)
    │   ├── ld2410.yaml         # 24 GHz mmWave presence radar (UART)
    │   ├── pir.yaml            # PIR passive motion sensor (GPIO)
    │   ├── remote_tran.yaml    # IR transmitter — NEC protocol (current)
    │   ├── remote_tran_old.yaml# IR transmitter — legacy variant
    │   ├── remote_rec.yaml     # IR receiver (optional, commented out by default)
    │   ├── screen.yaml         # SSD1306/SH1106 OLED screen output
    │   └── ir_transmitter_rp2040.h  # C++ NEC IR transmission helper
    └── font/                   # Embedded fonts for the OLED screen
        ├── Roboto-Regular.ttf
        ├── Roboto-Medium.ttf
        └── materialdesignicons-webfont.ttf
```

---

## Quick Start — M Firmware (Cabinet Controller)

### 1. Configure `hardware/data.yaml`

This is the **only file you must edit** before compiling. It contains all substitutions used across every package:

| Section | Key parameters |
|---|---|
| Board identity | `device_name`, `friendly_name`, `mqtt_name` |
| Network | `ethernet_IP`, `ethernet_gateway`, `ethernet_subnet` |
| MQTT | `mqtt_broker`, `mqtt_port`, `mqtt_username`, `mqtt_password` |
| Web server | `web_username`, `web_password` |
| Time | `timezone` (e.g. `Europe/Rome`) |
| OTA | `ota_password` |
| Module addresses | `m1_screen_address`, `m2_i2c_1`, `m3_i2c_1`, … (one block per module) |

> ⚠️ **Every I2C address must be unique on the bus.** Failing to set correct addresses will prevent the system from starting.

### 2. Edit `main.yaml` — select your modules

`main.yaml` uses ESPHome's `packages` / `!include` mechanism. Comment out any module you are not physically installing:

```yaml
packages:
  esp:        !include hardware/esp.yaml
  data:       !include hardware/data.yaml     # always required
  ethernet:   !include hardware/ethernet.yaml
  mqtt:       !include hardware/mqtt.yaml
  # Choose ONE of the two i2c packages depending on your M1 board revision:
  i2c:        !include hardware/i2c.yaml        # M1 v1.0
  # i2c:      !include hardware/i2c_switch.yaml # M1 v1.2

  m1:   !include module/m1.yaml       # OLED display + buttons
  m2:   !include module/m2.yaml       # 8-in / 8-out I/O
  # m3, m4, m5, m6 … add as needed
```

To add **S sensor nodes** (room sensors communicating over RS485), use the reusable `modbus_master.yaml` template — one block per sensor node, each with a unique Modbus address:

```yaml
  slave_living:
    <<: !include
      file: sensor/modbus_master.yaml
      vars:
        s_name: living
        s_name_upper: Living
        s_modbus_addr: "0x01"
        s_controller_id: modbus_slave1

  slave_bedroom:
    <<: !include
      file: sensor/modbus_master.yaml
      vars:
        s_name: bedroom
        s_name_upper: Bedroom
        s_modbus_addr: "0x02"
        s_controller_id: modbus_slave2
```

### 3. Flash via USB-C

Connect the USB-C port on the M1 board (do **not** connect external power simultaneously) and run:

```bash
esphome run M/main.yaml
```

Subsequent updates can be done OTA over the network.

---

## Quick Start — S Firmware (Room Sensor Node)

The S node runs on a Raspberry Pi RP2040. Its role is to collect all room sensor data and expose it to the M1 master over **Modbus RTU (RS485)**.

### 1. Configure `hardware/data.yaml`

| Parameter | Description |
|---|---|
| `modbus_address` | **Must be unique** for every S node on the RS485 bus (e.g. `0x01`, `0x02`, …) |
| `bh1750_addr` | `0x23` (ADDR=LOW) or `0x5C` (ADDR=HIGH) |
| `bme280_addr` | `0x76` or `0x77` depending on SDO pin |
| `nec_address` | NEC device address of the IR-controlled appliance |
| `nec_digit_0…9` | NEC command codes for digit keys |
| `nec_muto`, `nec_power`, … | NEC codes for special IR commands |

### 2. Edit `main.yaml` — enable/disable sensors

Comment out sensors not physically present on your daughter board:

```yaml
packages:
  - !include hardware/rp2040.yaml
  - !include hardware/data.yaml
  - !include hardware/modbus.yaml
  - !include sensor/bme280.yaml     # Temperature / Humidity / Pressure
  - !include sensor/bh1750.yaml     # Light
  - !include sensor/scd41.yaml      # CO₂
  - !include sensor/sgp41.yaml      # VOC / NOx
  - !include sensor/ld2410.yaml     # mmWave presence
  - !include sensor/pir.yaml        # PIR motion
  - !include sensor/remote_tran.yaml # IR transmitter
  # - !include sensor/remote_rec.yaml  # IR receiver (optional)
  - !include sensor/screen.yaml     # OLED display
```

### 3. Flash via USB-C

The RP2040 enumerates natively as a USB device. Flash with:

```bash
esphome run S/main.yaml
```

> Do **not** connect the external 12 V supply and USB-C simultaneously.

---

## Modbus Register Map (S node → M1 master)

The M1 reads sensor data from each S node using standard Modbus RTU function codes. The `modbus_master.yaml` template on the M side maps the following registers:

| Type | FC | Address | Entity |
|---|---|---|---|
| Input Register | FC04 | 0 | Temperature (×0.1 °C) |
| Input Register | FC04 | 1 | Humidity (×0.1 %) |
| Input Register | FC04 | 2 | Pressure (×0.1 hPa) |
| Input Register | FC04 | 3 | Brightness (lux) |
| Input Register | FC04 | 4 | VOC Index |
| Input Register | FC04 | 5 | NOx Index |
| Input Register | FC04 | 6 | CO₂ (ppm) |
| Input Register | FC04 | 7 | Detected Distance (cm) |
| Discrete Input | FC02 | 0 | PIR Movement |
| Discrete Input | FC02 | 1 | Presence (radar) |
| Discrete Input | FC02 | 2 | Movement (radar) |
| Discrete Input | FC02 | 3 | Static Presence (radar) |
| Discrete Input | FC02 | 4 | CO₂ Alarm |
| Discrete Input | FC02 | 5 | VOC Alarm |
| Discrete Input | FC02 | 6 | Temperature Alarm |
| Discrete Input | FC02 | 7 | Active Alarm |
| Coil | FC01/FC05 | 1 | Reset CO₂ Baseline |
| Holding Register | FC03/FC06 | 0 | IR Command (0=idle, 1–999=channel, 1001–1006=special) |
| Holding Register | FC03/FC06 | 10 | Temperature Setpoint (×10, °C) |
| Holding Register | FC03/FC06 | 11 | CO₂ Setpoint (ppm) |
| Holding Register | FC03/FC06 | 12 | Current Time (HHMM) |
| Holding Register | FC03/FC06 | 13 | Display Command (0=OK, 1=VENT, 2=HEAT, 3=COOL) |

---

## MQTT Topic Convention

All entities publish and subscribe under the pattern:

```
{mqtt_name}/{ModuleName}/{EntityName}/state     ← published by the device
{mqtt_name}/{ModuleName}/{EntityName}/command   ← subscribed by the device
```

Example for sensor node `Living`:
```
DOMIX/Living sensor/Temperature/state    → "22.4"
DOMIX/Living sensor/CO2/state            → "612"
DOMIX/Living sensor/IR/command           → "MUTO" | "1" | "34" | "ACCENDI"
```

Example for M2 I/O module:
```
DOMIX/M2_1/Output 1/state    → "ON" / "OFF"
DOMIX/M2_1/Output 1/command  → "ON" / "OFF"
DOMIX/M2_1/Input 1/state     → "ON" / "OFF"
```

---

## M1 Display & Button Menu

The M1 module includes a SH1106 128×64 OLED and a 3-button analogue interface (Up / Down / OK) read via an ADS1115 ADC. The menu system defined in `module/m1.yaml` provides:

- **Home screen** — shows "DOMIX" header and current time; auto-returns after 10 s of inactivity.
- **Main menu (level 0)** — lists all installed modules (M2 I/O, M3 Contacts, M4 Relays…).
- **Submenu (level 1)** — shows live input/output states for the selected module.
- **Edit mode (level 2)** — allows toggling individual outputs directly from the panel.

Button voltage thresholds (ADS1115, gain 6.144 V):

| Button | Voltage range |
|---|---|
| Up | 2.5 V – 2.9 V |
| Down | 0.4 V – 0.65 V |
| OK | 1.5 V – 1.75 V |

---

## Zigbee Coordinator Note

The Zigbee module (CC2652) requires its own dedicated firmware to act as a Zigbee2MQTT coordinator. Refer to the [POE-Zigbee-Coordinator](https://github.com/carletz/POE-Zigbee-Coordinator) project for the flashing procedure.

> ⚠️ **ESPHome 2026.x is not compatible with the external Zigbee component.** If you need Zigbee, compile with ESPHome ≤ 2025.x (tested on **2025.7.5**). If Zigbee is not required, use the latest ESPHome version.

---

## External Components

The `M/module/.esphome/external_components/` directory contains a cached copy of the **ZMPT101B** voltage sensor ESPHome component (used by the M6 energy meter module). This is automatically referenced during compilation and does not require manual installation.

---

## License

DOMIX © 2026 by carletz — licensed under [CC BY-NC-SA 4.0](https://creativecommons.org/licenses/by-nc-sa/4.0/).  
For educational and experimental use. Validate thoroughly before any production deployment.
