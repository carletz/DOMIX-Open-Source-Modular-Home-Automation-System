# Module M5 12 RELAY

## Overview
The DOMIX M5 module is conceptually identical to the DOMIX M4 module, with the difference in the number of outputs which in this case is equal to 12. Also, the characteristics of the designed relays are the same, in order to try to standardize the components to be purchase.

|Main info|	| M5|
|------|--------|-----|
|Lower module|	Dimension|	67 x 84.4 mm|
|	|Layer	|2|
|Upper module|	Dimension|	67 x 41 mm|
|	|Layer	|2|
|Vertical module|	Dimension|	50.5 x 24 mm|
|	|Layer	|2|

## System Architecture
For all possible modules the system is always the same, 3 stacked pcb that allows for modularity and easy maintenance if necessary. See DOMIX M1 module for more information.
The DOMIX M5 system includes a PCA9535 integrated circuit that allows you to control the gate of each 12 N-channel mosfet connected directly to the coils of 12 10A miniature pcb power relay. In order to choose the correct address for the chip, a dip switch called SW1 manages the correct I2C address on channel 1 of the M1 module's control bus. The following summarizes the possible (but non-overlapping) addresses that can be managed by the integrated circuit itself:

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

It is highly recommended that you first create a sort of table where you can know how many and which modules will need to be paired so that you can correctly manage all the addresses and avoid overlaps.

The use and the functionality of this module is exactly the same of DOMIX M4 module. Check the consideration inside the relative paragraph.

## Power Supply Architecture
The module's control logic is powered by the IDC connector and therefore by the DOMIX M1 core module. A 3V3 power supply is required to operate the integrated circuit of the PCA9535 chip, while the 5V power supply in the cable is used for the relay coils circuit, which is separated by the mosfet from the logic circuit at 3V3. The integrated circuit has a decoupling capacitor, and the external contacts are completely separated from the inside of the logical part of the board. 

## User Interface & Display Module
A user interface made with a display exactly identical to the DOMIX M1 module is foresee on the project. The use of it, by the way, should be extremely calculated due to the lack of possibilities of the I2C address available on the bus. Adding another I2C screen could be dangerous and not correct, any user should, in my opinion, calculate the correct number of modules and use just one display where I can always represent the output of my system through ESPHome. This, in any case, should be studied case by case.

## Mechanical Structure
As like all the other DOMIX module, the DOMIX M5 system uses a stacked PCB architecture with the lower PCB containing core electronics, a vertical PCB providing mechanical and electrical interconnect, and an upper PCB housing the human interface. This structure minimizes footprint, improves maintainability, and allows for future UI or expansion board development.
