# DOMIX Sensor Modules – S Family

The **DOMIX sensor family** is a set of standalone multi-sensor nodes designed to complement the main DOMIX cabinet system. Unlike the DIN-rail modules (M1–M9), sensor modules are intended for room-level deployment and communicate with the DOMIX M1 via **RS485**. Each node is built around a shared **core board (S)** and a sensor daughter board — **S1** for ceiling installation or **S2** for wall / 503-box installation.

The two daughter boards carry identical sensor sets but differ in mechanical packaging. The S2 variant additionally includes a small **OLED display** and a **rotary encoder** for local interaction.

---

## Table of Contents

- [S – Core Sensor Board](#s--core-sensor-board)
- [S1 – Ceiling Daughter Board](#s1--ceiling-daughter-board)
- [S2 – Wall Daughter Board (503-box)](#s2--wall-daughter-board-503-box)
- [Variant Configuration Guide](#variant-configuration-guide)
- [Choosing the Right Variant](#choosing-the-right-variant)

---

## S – Core Sensor Board

### Overview

The **DOMIX S Core** board is the shared intelligence of all DOMIX sensor nodes. It provides:

- Microcontroller (RP2040)
- Power supply chain (12V → 5V → 3.3V)
- USB-C programming interface
- RS485 communication
- 20-pin expansion header for S1 or S2 daughter boards

<div align="center">
    <img width="40%" src="/./Images/Board/S/S%20-%20CORE%20SENSOR%20PCB.png">
</div>

The same PCB is used regardless of installation type; only the daughter board and 3D-printed enclosure change.

### System Architecture

| Component | Part | Notes |
|-----------|------|-------|
| MCU | Raspberry Pi RP2040 | Core microcontroller |
| Flash | W25Q128JVSIQ (16 MB) | External SPI flash |
| Oscillator | 12 MHz crystal | System clock |
| RS485 Transceiver | MAX13487EESA+T | With SN74LVC1G125DCKR buffer |
| RS485 Protection | TVS + resettable fuses | Bus line protection |
| RS485 Termination | 120 Ω resistor | Solder only on the **last** sensor on bus |

The RP2040 exposes USB D+/D− directly on the USB-C connector, allowing firmware flashing without additional hardware.

The 20-pin header carries: `3V3`, `5V`, `GND`, I2C bus, UART, and GPIO lines to the daughter board. A supplementary **6-pin power header** provides additional `3V3 / 5V / GND` connections for the ceiling board only.

### Power Supply Architecture

| Stage | Component | Function |
|-------|-----------|----------|
| Input | 2-pin screw terminal | 12V DC input |
| 12V → 5V | TPS54302DDCR | Buck converter |
| 5V → 3.3V | Linear regulator | 3.3V rail |
| Power path | WPM2015-3 P-MOSFET | Auto-selection between 12V ext. and USB-C 5V |

> ⚠️ **Warning:** Do **not** connect both the external 12V supply and the USB-C cable simultaneously.

---

## S1 – Ceiling Daughter Board

### Overview

The **S1** daughter board is designed for ceiling installation in a dedicated 3D-printed enclosure. It hosts a comprehensive set of environmental and presence sensors, plus IR transmit/receive circuits for controlling legacy infrared devices.

The board connects to the S Core via the 20-pin header and receives a supplementary 3-pin power connector.

> **Note:** The S1 comes in two sub-variants: **S1a** (mmWave radar) and **S1b** (PIR sensor). Only one presence sensor can be mounted at a time due to physical constraints.

<div align="center">
    <img width="40%" src="/./Images/Board/S/S1a%20-%203D%20Board.png">
    <img width="40%" src="/./Images/Board/S/S1b%20-%203D%20Board.png">
</div>

### Sensors and Peripherals

| Sensor / Component | Interface | Function |
|-------------------|-----------|----------|
| HLK-LD2410C-P (24 GHz mmWave radar) | UART | Static presence + motion detection — **S1a only** |
| PIR HC-SR501 | Digital (3-pin header) | Motion detection — **S1b only** |
| BME280 | I2C | Temperature, humidity, barometric pressure |
| BH1750 | I2C | Ambient light (lux) |
| SCD41 | I2C | CO₂ (ppm), temperature, humidity (photoacoustic) |
| SGP41 | I2C | VOC and NOx index |
| TSOP31240 | — | 38 kHz IR receiver for remote control decoding |
| SFH 4546 × 3 + BSS138BK MOSFET | — | High-power IR transmitter array |

All I2C devices share the SDA/SCL bus from the S Core.

### Enclosure

The S1 assembly is housed in a **custom 3D-printed ceiling enclosure**, flush-mounted with openings for each sensor and the IR window. The S Core sits inside and connects to the S1 via the 20-pin header.

<div align="center">
    <img width="40%" src="/./Images/Board/S/S1%20-%203D%20Explode.png">
</div>
---

## S2 – Wall Daughter Board (503-box)

### Overview

The **S2** daughter board carries the same core sensor suite as S1, but is designed for a standard **503 electrical wall box** — the same form factor used for wall switches and sockets. This makes it ideal for retrofitting into existing rooms without ceiling access.

Compared to S1, S2 adds a local user interface option (OLED + rotary encoder), but trades off the **SCD41**, **BH1750**, and **TSOP31240 IR receiver** due to space constraints.

> **Note:** The S2 comes in two sub-variants: **S2a** (display + encoder) and **S2b** (extended sensors).

<div align="center">
    <img width="40%" src="/./Images/Board/S/S2a%20-%203D%20Board.png">
    <img width="40%" src="/./Images/Board/S/S2b%20-%203D%20Board.png">
</div>

### Sensors and Peripherals

| Sensor / Component | Interface | Notes |
|-------------------|-----------|-------|
| HLK-LD2410C-P (24 GHz mmWave) | UART | Presence detection |
| BME280 | I2C | Temperature, humidity, pressure |
| SGP41 | I2C | VOC and NOx index |
| SCD41 | I2C | CO₂ — **S2b only** |
| BH1750 | I2C | Ambient light — **S2b only** |
| OLED HS96L03W2C03 | I2C (0x3C) | Local display — **S2a only** |
| Rotary Encoder EC11L1525G01 | GPIO 16/17/18 | Local navigation — **S2a only** |
| SFH 4546 × 3 IR TX | — | IR transmitter array |

> The OLED default I2C address is `0x3C`, configurable via solder jumper.

### Enclosure

The S2 is designed to fit inside a standard **503 flush wall box**. The S Core mounts behind the S2 daughter board; the assembly slides in from the front with the display and encoder accessible through the cover plate. No ceiling modification required.

---

## Variant Configuration Guide

All variants are built on the same principle: **one S Core board + one of four daughter board configurations**.

| Variant | Installation | LD2410C | PIR | BME280 | SCD41 | SGP41 | BH1750 | OLED + Encoder | IR RX | IR TX |
|---------|-------------|:-------:|:---:|:------:|:-----:|:-----:|:------:|:--------------:|:-----:|:-----:|
| **S Core + S1a** | Ceiling – mmWave | ✓ | – | ✓ | ✓ | ✓ | ✓ | – | ✓ | ✓ |
| **S Core + S1b** | Ceiling – PIR | – | ✓ | ✓ | ✓ | ✓ | ✓ | – | ✓ | ✓ |
| **S Core + S2a** | Wall 503 – Display | ✓ | – | ✓ | – | ✓ | – | ✓ | – | ✓ |
| **S Core + S2b** | Wall 503 – Sensors | ✓ | – | ✓ | ✓ | ✓ | ✓ | – | – | ✓ |

<div align="center">
    <img width="40%" src="/./Images/Board/S/S1a%20-%203D%20Module.png">
    <img width="40%" src="/./Images/Board/S/S1b%20-%203D%20Module.png">
</div>
<div align="center">
    <img width="40%" src="/./Images/Board/S/S2a%20-%203D%20Module.png">
    <img width="40%" src="/./Images/Board/S/S2b%20-%203D%20Module.png">
</div>

> All variants include the S Core with RP2040, RS485, USB-C, and 12V power supply.  
> ✓ = included · – = not available in this variant  
> Individual sensors are always optional — the table reflects the **maximum** possible configuration.

---

## Choosing the Right Variant

| Use Case | Recommended Variant | Reason |
|----------|--------------------|---------| 
| Living room / bedroom occupancy automation | **S1a or S2a** (mmWave) | PIR cannot reliably detect stationary presence |
| Corridor, entrance, secondary rooms | **S1b** (PIR) | Simpler, lower cost; static detection not needed |
| Air quality priority (bedroom, office, kitchen) | **S2b** | Richest dataset: CO₂, light, temp, humidity, VOC |
| Local display / thermostat / manual controls | **S2a** | OLED + encoder for direct interaction; trades SCD41 + BH1750 |

All variants are:
- **Powered** by 12V DC on the S Core (fully autonomous)
- **Connected** via two-wire RS485 to the DOMIX M1
- **Flashable** via USB-C
