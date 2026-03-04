# 📦 BOM — Bill of Materials

> **DOMIX** · Open Source Modular Home Automation System

This folder contains the **Bill of Materials (BOM)** for all DOMIX hardware modules. Each `.xlsx` file lists the components required to build a specific PCB, including part references, quantities, manufacturer part numbers, LCSC/JLCPCB codes and estimated costs.

All BOM files were exported from **EasyEDA** and are optimized for ordering directly via **LCSC** or assembly via **JLCPCB**.

---

## 📁 Folder Structure

```
BOM/
├── README.md                                              # This file
│
│   ── Main Modules (M) ──
├── BOM_M1 - CORE V1.1_M1 - Core Module V1.1.xlsx         # ESP32 core controller
├── BOM_M2 - I_O_M2 - I_O Module.xlsx                     # I/O expansion module
├── BOM_M3 - CONTACT_M3 - Contact Module.xlsx             # Dry contact inputs
├── BOM_M4 - 6 RELAY_M4 - 6 Relay Module.xlsx            # 6-channel relay board
├── BOM_M5 - 12 RELAY_M5 - 12 Relay Module.xlsx          # 12-channel relay board
├── BOM_M6 - ENERGY_M6 - Energy Meter Module.xlsx        # Energy monitoring module
├── BOM_M7 - OUTPUT_M7 - Output Module.xlsx              # Digital output module
├── BOM_M8 - INPUT_M8 - Input Module.xlsx                # Digital input module
├── BOM_M9 - SPRINKLER_M9 - Sprinkler Module.xlsx        # Irrigation/sprinkler controller
│
│   ── Sensor PCBs (S) ──
├── BOM_S  - CORE SENSOR_S - CORE SENSOR PCB.xlsx        # Core sensor board
├── BOM_S1 - CEILING SENSOR_S1 - CEILING PCB.xlsx        # Ceiling-mount sensor PCB (PIR)
├── BOM_S2 - WALL SENSOR_S2 - WALL PCB.xlsx             # Wall-mount sensor PCB
│
│   ── Upper (Display) Boards (U) ──
├── BOM_U1 - CORE - UPPER V1.1_M1 - Display Module V1.1.xlsx
├── BOM_U2 - I_O - UPPER_M2 - Display Module.xlsx
├── BOM_U3-4 - MULTI - UPPER_M3-4 - Display Module.xlsx
├── BOM_U5-6 - MULTI - UPPER_M5-6 - Display Module.xlsx
├── BOM_U7 - OUTPUT - UPPER_M7 - Display Module.xlsx
├── BOM_U8 - INPUT - UPPER_M8 - Display Module.xlsx
├── BOM_U9 - SPRINKLER - UPPER_M9 - Upper Module.xlsx
│
│   ── Vertical / Middle Interface Boards (V) ──
├── BOM_V1 - CORE - MIDDLE V1.0_M1 - Middle Module V1.0.xlsx
├── BOM_V1 - CORE - MIDDLE V1.2_M1 - Middle Module V1.2.xlsx
├── BOM_V2 - I_O - VERTICAL_M2 - Vertical Module.xlsx
├── BOM_V3-4-5-6 - MULTI - VERTICAL_M3-4-5-6 - Vertical Module.xlsx
├── BOM_V7 - OUTPUT - VERTICAL_M7 - Vertical Module.xlsx
├── BOM_V8 - INPUT - VERTICAL_M8 - Vertical Module.xlsx
└── BOM_V9 - SPRINKLER - VERTICAL_M9 - Vertical Module.xlsx
```

---

## 🧩 Modules Overview

### Main Modules (M)

| File | Description | Component Types | Total Parts |
|---|---|:---:|:---:|
| M1 — Core V1.1 | Central ESP32-based controller. Includes tactile switches, capacitors, regulators, connectors | 51 | ~101 |
| M2 — I/O | I/O expansion with terminal blocks and resettable fuse | 10 | ~15 |
| M3 — Contact | Multi-channel dry contact input board (33× decoupling caps) | 15 | ~130 |
| M4 — 6 Relay | 6-channel relay board with AO3400A MOSFETs and 1N4007F flyback diodes | 12 | ~37 |
| M5 — 12 Relay | 12-channel relay board, same topology as M4 scaled up | 12 | ~69 |
| M6 — Energy Meter | Multi-channel energy monitoring (current/voltage sensing) | 33 | ~109 |
| M7 — Output | Digital output expansion module | 8 | ~10 |
| M8 — Input | Digital input expansion module | 9 | ~20 |
| M9 — Sprinkler | Irrigation controller with TVS protection, fuse holders | 18 | ~80 |

### Sensor PCBs (S)

| File | Description | Component Types | Total Parts |
|---|---|:---:|:---:|
| S — Core Sensor | Main sensor carrier board (temp, humidity, ambient light…) | 41 | ~64 |
| S1 — Ceiling | Ceiling-mount PCB with HC-SR501 PIR motion sensor | 17 | ~24 |
| S2 — Wall | Wall-mount sensor PCB with MOSFET level shifting | 14 | ~21 |

### Upper / Display Boards (U)

| File | Description | Component Types | Total Parts |
|---|---|:---:|:---:|
| U1 — Core Upper V1.1 | Status LEDs, tactile nav buttons, 2×5 header | 11 | ~19 |
| U2 — I/O Upper | 16× red SMD LEDs with resistor arrays | 4 | ~21 |
| U3-4 — Multi Upper | OLED display (0.96", SSD1306 compatible) + 2×4 header | 2 | ~2 |
| U5-6 — Multi Upper | OLED display (1.3", SH1106 compatible) + 2×4 header | 2 | ~2 |
| U7 — Output Upper | 16× red SMD LEDs, 4× 680Ω resistor arrays | 3 | ~21 |
| U8 — Input Upper | 16× red SMD LEDs, 4× 270Ω resistor arrays | 3 | ~21 |
| U9 — Sprinkler Upper | 8× LED indicators + pushbuttons for zone control | 4 | ~19 |

### Vertical / Middle Interface Boards (V)

These are mechanical/electrical interface boards used to stack or interconnect modules. They mainly contain 2.54mm headers, IDC connectors, and pin sockets.

| File | Description |
|---|---|
| V1 — Core Middle V1.0 | IDC 2×4 + female socket connectors |
| V1 — Core Middle V1.2 | Updated version with 16× 2.2kΩ pull-up resistors |
| V2 — I/O Vertical | 2×10 right-angle pin headers + IDC connectors |
| V3-4-5-6 — Multi Vertical | Shared vertical board for relay/contact modules |
| V7 — Output Vertical | Right-angle 2×10 headers |
| V8 — Input Vertical | Right-angle 2×10 headers |
| V9 — Sprinkler Vertical | IDC 2×4 + 2×8 right-angle headers |

---

## 📋 BOM Column Reference

Each BOM file contains the following columns (EasyEDA export format):

| Column | Description |
|---|---|
| `No.` | Row number |
| `Quantity` | Number of parts required |
| `Comment` | Value or part comment (e.g. `100nF`, `10kΩ`, `G2FE-05HMV1`) |
| `Designator` | PCB reference designators (e.g. `C1, C2, R3`) |
| `Footprint` | PCB land pattern / package |
| `Value` | Electrical value |
| `Manufacturer Part` | Manufacturer Part Number (MPN) |
| `Manufacturer` | Component manufacturer |
| `Supplier Part` | LCSC Part Number (e.g. `C14663`) |
| `Supplier` | Supplier name (typically `LCSC`) |
| `LCSC Stock` | Stock quantity on LCSC at export time |
| `LCSC Price` | Unit price on LCSC (USD) at export time |
| `Primary Category` | LCSC component category |
| `Secondary Category` | LCSC component sub-category |
| `JLCPCB Stock` | Stock available for JLCPCB SMT assembly |
| `JLCPCB Price` | Price for JLCPCB SMT assembly |

---

## 🛒 Ordering Guidelines

- **LCSC** is the primary supplier — all BOM files include LCSC part numbers (`Supplier Part` column). Upload the `.xlsx` directly on [lcsc.com](https://www.lcsc.com) using the BOM import tool.
- **JLCPCB SMT Assembly** — the `JLCPCB Stock` and `JLCPCB Price` columns allow you to use these BOMs directly for [JLCPCB's assembly service](https://jlcpcb.com/smt-assembly).
- Always verify stock and pricing before ordering — values in the BOM reflect the status at export time and may have changed.
- Some components (e.g. HC-SR501 PIR module in S1) have no supplier part listed and must be sourced separately (AliExpress or local distributors).

---

## ⚙️ How to Use

1. Identify the module(s) you want to build
2. Open the corresponding BOM `.xlsx` file
3. Cross-reference the `Designator` values against the schematic in the `/Schematic` folder
4. Use the Gerber files in the `/PCB` folder to fabricate the board
5. Import the BOM into LCSC or JLCPCB for ordering / assembly

---

## 🔀 Building Multiple Modules Together — BOM Optimization

If you plan to build more than one module at a time, it is strongly recommended to **merge the individual BOMs and consolidate quantities** before ordering. Many components are shared across modules, and ordering them in bulk significantly reduces cost and shipping overhead.

### Why it matters

Many passive components (resistors, capacitors, connectors) appear in 4 to 9 different modules. For example:

| LCSC Part | Value / Description | Shared across |
|---|---|:---:|
| C14663 / C38141 | 100nF decoupling capacitor (C0603 / C0805) | 6–10 modules |
| C29718 / C2906982 | 10kΩ resistor array | 7–9 modules |
| C9139 | IDC 2×4 2.54mm connector | 7 modules |
| C389517 | MHT192CRCT red LED 0603 | 6 modules |
| C577602 | KF2EDGR-2.54-10P terminal block | 4 modules |
| C369168 | JK-MSMD050-30 resettable fuse | 4 modules |
| C255606 | PCA9535PW I²C I/O expander | 4 modules |

Ordering these separately per module means paying multiple shipping minimums and potentially not reaching the quantity discounts available on LCSC (usually at 10, 50, or 100 pieces).

### How to merge BOMs

The quickest approach is to use a spreadsheet:

1. Open all the BOM `.xlsx` files you need
2. Copy all data rows (excluding headers) into a single sheet
3. Group rows by `Supplier Part` (LCSC code) and sum the `Quantity` column
4. Add a small buffer (5–10%) for SMD components to account for placement losses
5. Use the merged sheet to place a single LCSC order

Alternatively, tools like [KiCad BOM plugins](https://github.com/SchrodingersGat/KiBoM), [InteractiveHtmlBom](https://github.com/openscopeproject/InteractiveHtmlBom), or a simple Python/Excel script can automate this consolidation.

### Tips

- Check the `JLCPCB Stock` column before ordering: parts with high JLCPCB stock are good candidates for SMT assembly rather than hand-soldering.
- Some components appear with slightly different LCSC codes across modules (e.g. two different `100nF` C0603 parts) — verify they are electrically equivalent before substituting one for the other.
- Vertical (V) and Upper (U) boards share almost exclusively passive connectors and headers, making them the easiest to batch-order together.

---

*Part of the [DOMIX Home Automation System](https://github.com/carletz/DOMIX-Open-Source-Modular-Home-Automation-System) · Made with ❤️ for the open hardware community*
