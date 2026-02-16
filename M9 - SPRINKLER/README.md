# Module M9 SPRINKLER

---
<div align="center">
    <img width="70%" src="/./Images/Board/M9/M9%20-%203D%20Pinout.png">
</div>

<div align="center">
    <img width="40%" src="/./Images/Board/M9/M9%20-%203D%203D%20Board.png">
    <img width="40%" src="/./Images/Board/M9/M9%20-%203D%203D%20Module.png">
</div>
---

## Overview
The M9 module is a special module that can drive 24VAC loads,  typically used for irrigation valve coils. This module is based on another project of mine called [SprinklerBoy](https://github.com/carletz/Sprinkler-Boy). 

|Main info|	| M9|
|------|--------|-----|
|Lower module|	Dimension|	67 x 84.4 mm|
|	|Layer	|2|
|Upper module|	Dimension|	67 x 41 mm|
|	|Layer	|2|
|Vertical module|	Dimension|	50.5 x 24 mm|
|	|Layer	|2|


## System Architecture
For all possible modules the system is always the same, 3 stacked pcb that allows for modularity and easy maintenance if necessary. See DOMIX M1 module for more information.
The DOMIX M9 system includes a PCA9554A integrated circuit that allows you to control 8 MAC97A8 triac circuits. A dip switch called SW1 manages the correct I2C address on channel 1 of the M1 module's control bus. The following summarizes the possible (but non-overlapping) addresses that can be managed by the integrated circuit itself:

|PCA9554A| | | |	
|-----|-----|----|----| 
|Position of SW1 pins| | | |
|Address|	A2|	A1|	A0|
|0x38|	0|	0|	0|
|0x39|	0|	0|	1|
|0x3A|	0|	1|	0|
|0x3B|	0|	1|	1|
|0x3C|	1|	0|	0|
|0x3D|	1|	0|	1|
|0x3E|	1|	1|	0|
|0x3F|	1|	1|	1|

The same table is reported as a silkscreen on the back of the PCB itself.

The buttons on the top module are controlled by a PCF8574M/TR which allows the triac controls to be directly operated to open and close the output contacts, a typical function of an irrigation controller.As before here the addresses that can be managed by the integrated circuit itself:

|PCF8574| | | |	
|-----|-----|----|----| 
|Position of SW2 pins| | | |
|Address|	A2|	A1|	A0|
|0x38|	0|	0|	0|
|0x39|	0|	0|	1|
|0x3A|	0|	1|	0|
|0x3B|	0|	1|	1|
|0x3C|	1|	0|	0|
|0x3D|	1|	0|	1|
|0x3E|	1|	1|	0|
|0x3F|	1|	1|	1|

The same table is reported as a silkscreen on the back of the PCB itself.

It is highly recommended that you first create a sort of table where you can know how many and which modules will need to be paired so that you can correctly manage all the addresses and avoid overlaps.

## Power Supply Architecture
The module's control logic is powered by the IDC connector and therefore by the DOMIX M1 core module. A 3V3 power supply is required to operate the integrated circuits, while the 5V power supply in the cable is not used on this board. Each integrated circuit has a decoupling capacitor where necessary. 

It’s necessary to connect an external 24VAC power supply which is necessary for managing the external coils. For this particular module, the AC connection is necessary because the circuit relies on power triacs that can only handle this type of input. The valve common terminal has its own COM connector with two available outputs.

## User Interface & Display Module
The user interface designed for this module is equipped with 8 latch switches that allow you to control the opening and closing of each output. An LED allows you to immediately view the output status. Depending on how the buttons are then programmed, different effects can be obtained. In fact, this user interface is implicitly disconnected from the direct output; it must be programmed.

## Mechanical Structure
As like the M1 module, the DOMIX M8 system uses stacked PCB architecture with the lower PCB containing core electronics, a vertical PCB providing mechanical and electrical interconnect, and an upper PCB housing the human interface. This structure minimizes footprint, improves maintainability, and allows for future UI or expansion board development.

