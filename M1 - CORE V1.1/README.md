# Module M1 CORE

<div align="center">
    <img width="70%" src="/./Images/Board/M1/M1%20-%20Pinout.png">
</div>

<div align="center">
    <img width="40%" src="/./Images/Board/M1/M1%20-%203D%20Board.png">
    <img width="40%" src="/./Images/Board/M1/M1%20-%203D%20Module.png">
</div>


## Overview

DOMIX M1 is the heart of the DOMIX ecosystem: a modular and expandable core controller for smart home and building automation systems. It is designed as the central processing and communication hub, capable of bridging Ethernet, Zigbee, RS485, and I2C-based expansion modules within a single, compact hardware platform. The project focuses on reliability, modularity, and protocol interoperability.

| Main info	| M1 |  |
|--------|----------|-----------|
| Lower module | Dimension	| 67 x 84.4 mm|
|              | Layer	  |      4        |
| Upper module | Dimension	| 67 x 41 mm|
|              | Layer	   |     2        |
| Middle module V1.0 | Dimension	| 67.5 x 59.6 mm |
|              | Layer	  |     2         |
| Middle module V1.1 | Dimension | 67.5 x 59.6 mm |
|              | Layer	  |      2        |


## Design Philosophy
The DOMIX M1 Core Module is built around three key principles:
-	Modularity – clear separation between core logic, expansion boards, and user interface
-	Protocol bridging – seamless coexistence of IP-based, fieldbus, and wireless networks (Zigbee)
-	Industrial robustness – protected interfaces, proper power design, and EMI-aware layout
The system is not intended to be a "one-board-does-all" device, but rather a scalable automation backbone that can grow with the user's needs.

## System Architecture
At a high level, DOMIX M1 is composed of three main hardware layers:
1.	Lower PCB – Core Module: MCU, networking, power management, and industrial interfaces
2.	Middle PCB – Expansion Backplane: I2C distribution and mechanical stacking interface
3.	Upper PCB – Display & User Interface: Local control, status indication, and user feedback
This stacked architecture allows the system to evolve without redesigning the core, ensuring long-term maintainability and upgradeability. To increase the number of I2C buses present, there is also a version 1.1 of the DOMIX M1 module for which the use of the TCA9548APWR chip allows up to 8 new buses.
To summarize, version 1.0 has one I2C bus available for communication, while version 1.1 can use up to 8 different buses allowing multiple slave devices to communicate. We chose to have two different middle boards while keeping both the lower and upper ones identical.

## Core Microcontroller
The heart of the system is an ESP32-S3-WROOM-1-N8R2 module, featuring a dual-core Xtensa LX7 MCU with integrated Wi-Fi and USB support, along with external PSRAM for advanced applications. The native USB-C interface enables easy flashing, debugging, and power delivery.
The ESP32-S3 serves multiple critical roles:
-	Main application processor for automation logic
-	Protocol bridge between Ethernet, Zigbee, RS485, and I²C
-	Logic controller for coordinating all expansion modules
Why didn't I choose another microcontroller? Well, the ESP32-S3 is now a more than robust and solid chip, both for programming and for interacting with the outside world. It's robust and simple enough to program compared to many other major chips, and it's fast enough to handle the tasks of this project. I didn't choose other chips from the Espressif family because none, in my opinion, have the required features together like the S3 series. An STM32 chip could certainly have been used, but I find it difficult to program, and that's not the sole purpose of this work. It probably would have been faster, but in terms of interaction, it would have been complicated to manage.

## Power Supply Architecture
DOMIX M1 supports multiple power sources with proper isolation and protection. The input power accepts 9–12V DC external supply with reverse polarity and overcurrent protection, followed by step-down DC/DC conversion to 5V. Internal rails include a 5V system rail and a 3.3V regulated rail for logic and RF circuits. Automatic USB-C power selection during programming ensures stable operation in permanent installations while remaining developer-friendly.
Do not connect both external power supply and USB-C cable!

## Ethernet Connectivity
Wired networking is provided by the W5500 Ethernet controller, featuring a hardware TCP/IP stack that significantly reduces MCU load and improves determinism. The module supports 10/100 Mbps Ethernet with an SPI interface to the ESP32, and includes an integrated RJ45 connector with magnetics (HR911105A). Activity LED indicator it’s also repeated on the front of the module for greater visibility.
Ethernet is my karma; no one can convince me otherwise when it comes to data transmission and the intrinsic security of a LAN connection. 
Obviously, we're not talking about LORA modules or other long-range needs, but for me, the only solution at home is Ethernet. Heavy walls, irregular shapes, external interference, anything can interact with your wireless systems. Some might say, "Why Zigbee?" Well, if something has to stay, it's better than a jumble of IP addresses on your router, right? Separate communications on separate systems, and only one IP to monitor for logs or troubleshooting. Don't you agree? I'm convinced of this; based on my experience, just remembering all the IP addresses connected to the various devices (yes, I have them in my house, too) and accessing them individually is a pain. 
That's why the cable, once installed and tested, can't fail.

## Zigbee Wireless Interface
Wireless mesh networking is handled by the RF-BM-2652P2 Zigbee module, based on the TI CC2652 family. It features an external SMA antenna connector, dedicated UART communication with the ESP32, and status LEDs for network and message activity. This allows DOMIX M1 to act as a Zigbee coordinator, router, or gateway, bridging Zigbee devices with Ethernet/IP systems. The chip firmware is described in the software chapter of this guide.
The choice for this chip was also made considering the work already done both for my POE coordinator [POE-Zigbee-Coordinator](https://github.com/carletz/POE-Zigbee-Coordinator) and above all the extremely functional firmware available on the network. I didn't want to waste a good portion of the work already done, which you can find here, and above all a functional and good solution for monitoring all our Zigbee components.

## RS485 Industrial Interface
For field-level and industrial communication, the board integrates a MAX13487E RS485 transceiver with half-duplex differential communication, TVS protection for ESD and surge immunity, and resettable fuses on signal lines. Typical use cases include Modbus RTU devices, distributed sensors and actuators, and long-distance wired communication.
Serial connection is forever... you can connect anything here, communicate with anything, and above all, why not? You could connect two appropriately programmed DOMIX M1s in parallel at a considerable distance. They could be connected via Ethernet, but this isn't always possible. In any case, any type of sensor, controller, or actuator usually has the ability to communicate via serial, which is why it's there.

## I2C Expansion Bus
DOMIX M1 exposes 2 I2C buses designed for expansion and modularity. A vertical expansion PCB acts as an I2C backplane, supporting multiple external boards such as digital I/O modules, relay modules, sensor interfaces, and energy metering modules. The separation between core and expansion logic ensures scalability without requiring redesign of the core system.

##	User Interface & Display Module
An optional Upper PCB provides local interaction and feedback through a 1.3" OLED display (128×64, SH1106, I2C), three physical buttons (UP / DOWN / OK), an ADS1115 ADC for button multiplexing, and status LEDs for power, Ethernet, Zigbee network, and activity. This enables local status visualization, menu-driven configuration, and manual interaction without requiring external software.
The display module, as with all other upper PCB configurations, is not necessary. Its use can be chosen and adapted to your needs. Personally, I believe that a good local display, even if limited, is useful in certain cases. This is why it was added to the system, but it absolutely does not preclude the functionality of all modules, it’s at your choice.

## Mechanical Structure
The system uses a stacked PCB architecture with the lower PCB containing core electronics, a vertical PCB providing mechanical and electrical interconnect, and an upper PCB housing the human interface. This structure minimizes footprint, improves maintainability, and allows for future UI or expansion board development.



