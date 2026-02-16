# DOMIX - Open Source Modular Home Automation System

<div align="center">
    <img width="50%" src="/Logo/domixOK.png">
</div>

## 📋 Table of Contents

- [Introduction](#-introduction)
- [The DOMIX Manifesto](#-the-domix-manifesto)
- [System Architecture](#-system-architecture)
- [Available Modules](#-available-modules)
- [Technical Specifications](#-technical-specifications)
- [Getting Started](#-getting-started)
- [Software & Programming](#-software--programming)
- [Comparison with Other Systems](#-comparison-with-other-systems)
- [License & Disclaimer](#-license--disclaimer)
- [Documentation, Contributing, Contact & Acknowledgments](#-documentation)


---
> [!NOTE]
> **Please read the Full Technical Manual:** [Technical Documentation PDF](docs/DOMIX_Technical_Manual.pdf) to understand the project's full potential and features, below is just a summary of the project's features.
---


## 🎯 Introduction

For several years, I have been managing the equipment in my home using automated systems that report all necessary information to a central controller, both for operational control and for data analysis.

These tools have only recently evolved, perhaps a decade or so for the consumer sector. Logic controllers, on the other hand, have been around for decades, but I don't think they'll easily become usable by a simple enthusiast like me, both due to their cost and their architecture.

As I've evolved my systems day by day, I've realized—unfortunately, too late, given that the learning curve for self-taught software is always very steep—that the best system, in my opinion, is always a centralized one.
After experimenting with both centralized and distributed approaches, the conclusion is clear: having everything converges into a single control point offers decisive advantages.

Many commercial solutions offer hybrid approaches, but excluding well-known and expensive systems, I could not find a simple solution capable of offering everything within a single device. From this need, the DOMIX project was born combining prior experience in electronics and automation into a single coherent ecosystem.

The idea is to concentrate the control logic inside a dedicated electrical cabinet, interfacing with actuators and safety switches belonging to the main electrical panel. This cabinet is not a conventional electrical panel, but a control cabinet designed to host automation logic and low-voltage control devices. The electrical power panel is not part of the DOMIX project; it relies on qualified electricians to connect the DOMIX system to the real world.

Yes, I know what you're thinking: who knows how many wires I have to run in my house! Or: how can I modify an existing system now? Well, I agree with you, this system is ideal if you're starting from scratch. If I don't yet have any conduit or corrugated pipes inside my walls, that's true, but in my opinion, even on a small scale, it can be adapted to many situations. On the other hand, imagination is a big part of the job of those who start thinking up similar systems on their own.

By centralizing all control wiring—both inputs (buttons, switches) and outputs (actuators, relays, contactors)—the system remains organized, flexible, and ready for complex logic and future expansion.

### Key Features

DOMIX is an **open-source, modular, DIN-rail home automation system** designed for centralized control and monitoring. Born from years of personal experience with home automation, DOMIX combines the structured approach of industrial systems with the flexibility of DIY platforms.

- ✅ Fully modular DIN-rail design
- ✅ Centralized logic with decentralized I/O
- ✅ Native Ethernet + Zigbee support
- ✅ ESPHome-based programming
- ✅ Open hardware & software
- ✅ No vendor lock-in
- ✅ Low-voltage safety (12V DC max)

## 📜 The DOMIX Manifesto

DOMIX is built on **10 core principles**:

1. **Open by Design** - All schematics and documentation are open source
2. **No Competition with Industrial Systems** - Research platform, not certified product
3. **Research, Not Certification** - Built for learning and experimentation
4. **Modular or Nothing** - Only M1 is mandatory, everything else is optional
5. **Cabinet-Oriented, Low-Voltage Only** - Never drives power loads directly
6. **Ethernet First, Always** - Wired reliability over wireless dependency
7. **Open Ecosystem Integration** - Works with Home Assistant, ESPHome, MQTT
8. **Zigbee as a First-Class Citizen** - Native Zigbee coordinator capability
9. **Fully Customizable by Design** - Firmware and logic can be freely modified
10. **Built for People Who Want to Understand** - Transparency over abstraction

> [!IMPORTANT]
> **DOMIX exists because building things is still fun. If it teaches something, it has succeeded.**


## 🏗️ System Architecture

<div align="center">
    <img width="100%" src="/Images/3D Total 2.png">
</div>

### Modular Design

DOMIX uses a **centralized core (M1)** with specialized I/O expansion modules connected via **I²C bus**. All modules mount on standard 35mm DIN rail.

### Connection System

- **Power & Data:** 8-pin IDC flat cables
- **I²C Bus:** Two independent buses (expandable to 8 with M1 v1.1)
- **Communications:** Ethernet (primary), Zigbee, RS485

### I²C Address Management

The system carefully manages I²C address space across three ranges:

| Range | Used For | Max Devices |
|-------|----------|-------------|
| 0x20–0x27 | PCA9554A, PCA9535 | 8 |
| 0x38–0x3F | PCA9554A, PCF8574, PCA9535 | 8 |
| 0x48–0x4B | ADS1115 (ADC) | 4 |

## 🧩 Available Modules

### Core Module

| Module | Function | DIN Width | Description |
|--------|----------|-----------|-------------|
| **M1** | Core/Gateway | 4 modules | ESP32-S3, Ethernet, Zigbee, RS485, I²C hub |

<div align="center">
    <img width="40%" src="/Images/Board/M1/M1%20-%203D%20Module.png">
</div>

### I/O Modules

| Module | Function | DIN Width | I/O Count | Description |
|--------|----------|-----------|-----------|-------------|
| **M2** | General I/O | 2 modules | 8 in / 8 out | Optocoupled inputs, Darlington outputs |
| **M3** | Door/Window Contacts | 2 modules | 16 analog in | Analog multiplexer for reed switches |
| **M7** | Digital Output | 2 modules | 16 out | General purpose 12V outputs |
| **M8** | Digital Input | 2 modules | 16 in | General purpose optocoupled inputs |

<div align="center">
    <img width="22%" src="/Images/Board/M2/M2%20-%203D%20Module.png">
    <img width="22%" src="/Images/Board/M3/M3%20-%203D%20Module.png">
    <img width="22%" src="/Images/Board/M7/M7%20-%203D%20Module.png">
    <img width="22%" src="/Images/Board/M8/M8%20-%203D%20Module.png">
</div>

### Relay Modules

| Module | Function | DIN Width | Relays | Description |
|--------|----------|-----------|--------|-------------|
| **M4** | 6-Relay | 2 modules | 6 | 10A @ 250VAC relays |
| **M5** | 12-Relay | 4 modules | 12 | 10A @ 250VAC relays |

<div align="center">
    <img width="25%" src="/Images/Board/M4/M4%20-%203D%20Module.png">
    <img width="25%" src="/Images/Board/M5/M5%20-%203D%20Module.png">
</div>

### Specialized Modules

| Module | Function | DIN Width | Description |
|--------|----------|-----------|-------------|
| **M6** | Energy Meter | 4 modules | 10-channel current clamp meter |
| **M9** | Sprinkler Controller | 4 modules | 8-zone sprinkler control with 24VAC output |

<div align="center">
    <img width="25%" src="/Images/Board/M6/M6%20-%203D%20Module.png">
    <img width="25%" src="/Images/Board/M9/M9%20-%203D%20Module.png">
</div>

## 🔧 Technical Specifications

### M1 Core Module

- **MCU:** ESP32-S3-WROOM-1-N8R2 (dual-core, Wi-Fi, USB-C)
- **Ethernet:** W5500 controller, 10/100 Mbps
- **Zigbee:** RF-BM-2652P2 (TI CC2652, external antenna)
- **RS485:** MAX13487E transceiver with TVS protection
- **Power:** 9-12V DC input, 5V/3.3V internal rails
- **I²C:** 2 buses (expandable to 8 with TCA9548A)
- **Display:** Optional 1.3" OLED (128×64, SH1106)

### Power Requirements

- **System Logic:** 3.3V @ <500mA
- **Relay Coils:** 5V (total system max 2A)
- **External Control:** 12V DC

### Mechanical

- **PCB Layers:** 2-4 layer depending on module
- **Enclosure:** 3D-printed, standard DIN dimensions
- **Connection:** IDC flat cables (1mm pitch)
- **Terminals:** Removable screw blocks (max 1.5mm²)

## 🚀 Getting Started

### Prerequisites

- Basic electronics knowledge
- Soldering skills
- 3D printer (for enclosures)
- ESPHome environment
- 12V DC power supply

### Building Your First System

1. **Minimum Configuration:**
   - 1× M1 Core module
   - 12V DC power supply
   - Ethernet cable

2. **Recommended Starter Kit:**
   - 1× M1 Core
   - 2× M2 General I/O
   - 1× M4 6-Relay
   - Power supply + cables

3. **Assembly Steps:**
   - Configure I²C addresses on expansion modules
   - Program ESP32 via USB-C with ESPHome
   - Flash Zigbee firmware to RF-BM-2652P2
   - Connect modules with IDC cables
   - Mount on DIN rail

### I²C Address Planning

**Critical:** Plan addresses before assembly to avoid conflicts.

Example configuration:
```
M1: Core (no address)
M2 #1: 0x20
M2 #2: 0x21
M4: 0x38
M3: 0x39 + ADS1115 @ 0x48
```

Check section 4.4 of manual for detailed address planning guidelines.

## 💻 Software & Programming

### ESPHome Configuration

DOMIX uses **ESPHome** for firmware. For me, ESPHome was an obvious choice, convenient and quick. 
I'm not a great programmer and I didn't want to get lost in a maze of lines of code, perhaps already flawed from the start, which would have wasted a lot of time.

The example in the Software folder is simply a basic configuration for all the modules you've created. The correct configuration for your system is entirely up to you. 
There's no single solution; the system must be adapted and remodeled. I've simply tried to provide a foundation, as modular as possible. 
Again, everything can and must be adapted to use my modules.

> [!WARNING]
> ESPHome version 2026 is **<ins>not compatible</ins>** with the external Zigbee component, so if you need to use it you must compile it with a maximum version of 2025. I used 2025.7.5. If you don't need it, you can use the latest version.

### Zigbee Setup

The RF-BM-2652P2 module requires Z-Stack firmware. See [POE Zigbee Coordinator](https://github.com/carletz/POE-Zigbee-Coordinator) for flashing instructions.

### Integration Examples

- **Home Assistant:** MQTT/Native API
- **Node-RED:** MQTT broker
- **OpenHAB:** MQTT binding
- **Custom:** Direct ESPHome API

## 📊 Comparison with Other Systems

| Feature | DOMIX | KNX | Shelly/Sonoff | Industrial PLC |
|---------|-------|-----|---------------|----------------|
| **Architecture** | Centralized core | Fully distributed | Device-based | Centralized |
| **DIN Rail** | Native | Yes | Partial | Yes |
| **Programming** | ESPHome (open) | ETS (licensed) | App/Cloud | Vendor IDE |
| **Vendor Lock-in** | None | High | Medium-High | High |
| **Entry Cost** | Low-Medium | High | Low | Very High |
| **Wireless** | Zigbee native | Optional | Wi-Fi | Rare |
| **Certification** | None (research) | Full | Consumer | Industrial |

### Where DOMIX Makes Sense

✅ You want full control over firmware and logic  
✅ You prefer wired reliability  
✅ You need clean DIN-rail cabinet integration  
✅ You want to understand and customize your system  
✅ Energy monitoring is important  

### Where DOMIX Is NOT Suitable

❌ Formal building certification required  
❌ Extremely large installations (100+ nodes)  
❌ Plug-and-play is the primary goal  
❌ Production environment without validation  

## 📄 License & Disclaimer

**Version:** 1.2  
**Last Updated:** March 2026  
**DOMIX © 2026 by carletz**  
Licensed under [CC BY-NC-SA 4.0](https://creativecommons.org/licenses/by-nc-sa/4.0/)

### Terms

- ✅ **Attribution** required
- ✅ **Share Alike** - derivative works must use same license
- ❌ **Non-Commercial** - commercial use not permitted
- ✅ Modifications and derivatives allowed

### Disclaimer

This project is provided **as-is** for educational and experimental purposes.

- ⚠️ Not certified for production use
- ⚠️ All electrical connections must be done by qualified personnel
- ⚠️ No warranties or guarantees provided
- ⚠️ User assumes all risks

**Always follow local electrical codes and safety regulations.**

## 📚 Documentation

- **Full Manual:** [Technical Documentation PDF](https://github.com/carletz/DOMIX-Open-Source-Modular-Home-Automation-System/tree/main/Docs/DOMIX_Technical_Documentation_EN.pdf)
- **Schematics, Gerbers, Pick and place and Interactive BOM:** Available in each module directory
- **BOM:** Available in [BOM](https://github.com/carletz/DOMIX-Open-Source-Modular-Home-Automation-System/tree/main/BOM) directory
- **3D Models:** Available in [3D Print files](https://github.com/carletz/DOMIX-Open-Source-Modular-Home-Automation-System/tree/main/3D%20Print%20files) directory
- **ESPHome Examples:** Available in [Software](https://github.com/carletz/DOMIX-Open-Source-Modular-Home-Automation-System/tree/main/Software)

## 🤝 Contributing

Contributions are welcome! Please:

1. Fork the repository
2. Create a feature branch
3. Commit your changes
4. Open a pull request

## 📞 Contact & Support

- **GitHub Issues:** For bug reports and feature requests
- **Oshwlab Stars 2026 Page:** [DOMIX](oshwlab.com/carletz.slug/domix-domotic-module-for-smart-home-co)
- **Related Projects:**
  - [POE Zigbee Coordinator](https://github.com/carletz/POE-Zigbee-Coordinator)
  - [MultiSensor](https://github.com/carletz/MultiSensor)
  - [Sprinkler Boy](https://github.com/carletz/Sprinkler-Boy)

## 🙏 Acknowledgments

- ESPHome community
- Home Assistant project
- Zigbee2MQTT / Z-Stack firmware by Koenkk
- zig-star.com for Zigbee tools

---

**Built with curiosity. Shared with transparency. Used with responsibility.**

If DOMIX teaches you something, it has succeeded. ✨
