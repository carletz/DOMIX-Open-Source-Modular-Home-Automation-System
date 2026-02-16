# Module M2 GENERAL I/O

## Overview
The DOMIX M2 module is intended to be a generic I/O module, useful for normal commands and controls.

|Main info|	M2| |
|------|------|------|
|Lower module	|Dimension	|32 x 84.4 mm|
|	|Layer	|2|
|Upper module	|Dimension	|32 x 41 mm|
|	|Layer	|2|
|Vertical module|	Dimension	|50.5 x 24 mm|
|	|Layer	|2|

## System Architecture
For all possible modules the system is always the same, 3 stacked pcb that allows for modularity and easy maintenance if necessary. See DOMIX M1 module for more information.
The DOMIX M2 system includes a PCA9535 integrated circuit that allows you to control 16 channels, in this case 8 input channels and 8 output channels. SW1 is a dip switch that manages the correct I2C address on channel 1 of the M1 module's control bus. The following summarizes the possible (but non-overlapping) addresses that can be managed by the integrated circuit itself:

|PCA9535	||||
|------|------|------|--------|
|Position of SW1 pins|||
|Address	|A2	|A1	|A0|
|0x20	|0|	0|	0|
|0x21	|0|	0|	1|
|0x22	|0|	1|	0|
|0x23	|0|	1|	1|
|0x24	|1|	0|	0|
|0x25	|1|	0|	1|
|0x26	|1|	1|	0|
|0x27	|1|	1|	1|


The same table is reported as a silkscreen on the back of the PCB itself.
It is highly recommended that you first create a sort of table where you can know how many and which modules will need to be paired so that you can correctly manage all the addresses and avoid overlaps.
The input channels are controlled by two TLP290-4 optocouplers that allow the isolation of the external control from the internal management part, sending a digital signal to the communication micro.
The output channels, however, have a ULN2803-A, a module with 8 Darlington arrays that allows you to use the 12VDC voltage and bring it to the external controls, without affecting the internal control logic, being completely decoupled.

## Power Supply Architecture
The module's control logic is powered by the IDC connector and therefore by the DOMIX M1 core module. A 3V3 power supply is required to operate the integrated circuits, while the 5V power supply in the cable is not used on this board. Each integrated circuit has a decoupling capacitor where necessary, while the ULN2803-A is connected to 12VDC as a reference for the output signals. For uniformity, the infrared LEDs of the TLP290-4 are also powered by 12VDC, so external controls will also be powered by this power supply.
For the above, it is therefore necessary to also connect the 12VDC to make both the input and output connections work. This power supply can be the same one supplied to the M1 module.

## User Interface & Display Module
For simplicity's sake, in this case, we chose to simply display LEDs at the top that indicate whether individual channels are activated or not. These LEDs are connected to the system's 3V3 logic and indicate channel activation. The input channels are at the top, and the output channels are at the bottom.

## Mechanical Structure
As like the M1 module, the DOMIX M2 system uses a stacked PCB architecture with the lower PCB containing core electronics, a vertical PCB providing mechanical and electrical interconnect, and an upper PCB housing the human interface. This structure minimizes footprint, improves maintainability, and allows for future UI or expansion board development.


