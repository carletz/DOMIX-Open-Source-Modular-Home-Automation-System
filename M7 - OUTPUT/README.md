# Module M7 OUTPUT

## Overview
The DOMIX M7 module is intended to be a generic Output module, useful for normal commands and controls.

|Main info|	| M7 |
|------|--------|-----|
|Lower module|	Dimension|	32 x 84.4 mm|
|	|Layer	|2|
|Upper module|	Dimension|	32 x 41 mm|
|	|Layer	|2|
|Vertical module|	Dimension|	50.5 x 24 mm|
|	|Layer	|2|

## System Architecture
For all possible modules the system is always the same, 3 stacked pcb that allows for modularity and easy maintenance if necessary.
The DOMIX M7 system includes a PCA9535 integrated circuit that allows you to control 16 output channels. SW1 is a dip switch that manages the correct I2C address on channel 1 of the M1 module's control bus. The following summarizes the possible (but non-overlapping) addresses that can be managed by the integrated circuit itself:

|PCA9535| | | |	
|-----|-----|----|----| 
|Position of SW1 pins| | | |
|Address|	A2|	A1|	A0|
|0x20|	0|	0|	0|
|0x21|	0|	0|	1|
|0x22|	0|	1|	0|
|0x23|	0|	1|	1|
|0x24|	1|	0|	0|
|0x25|	1|	0|	1|
|0x26|	1|	1|	0|
|0x27|	1|	1|	1|


The same table is reported as a silkscreen on the back of the PCB itself.

The output channels have a ULN2803-A, a module with 8 Darlington arrays that allows you to use the 12VDC voltage and bring it to the external controls, without affecting the internal control logic, being completely decoupled, as the M3 module.

## Power Supply Architecture
The module's control logic is powered by the IDC connector and therefore by the DOMIX M1 core module. A 3V3 power supply is required to operate the integrated circuits, while the 5V power supply in the cable is not used on this board. Each integrated circuit has a decoupling capacitor where necessary, while the ULN2803-A is connected to 12VDC as a reference for the output signals.
For the above, it is therefore necessary to also connect the 12VDC to make the output connections work. This power supply can be the same one supplied to the M1 module.

## User Interface & Display Module
For simplicity's sake, in this case, we chose to simply display LEDs at the top that indicate whether individual channels are activated or not. These LEDs are connected to the system's 3V3 logic and indicate channel activation.

## Mechanical Structure
As like the M1 module, the DOMIX M7 system uses a stacked PCB architecture with the lower PCB containing core electronics, a vertical PCB providing mechanical and electrical interconnect, and an upper PCB housing the human interface. This structure minimizes footprint, improves maintainability, and allows for future UI or expansion board development.
