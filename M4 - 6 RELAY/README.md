# Module M4 6 RELAY

## Overview
The DOMIX M4 module is intended to be a relay module with a very generic use.

|Main info|	| M4 |
|------|--------|-----|
|Lower module|	Dimension|	32 x 84.4 mm|
|	|Layer	|2|
|Upper module|	Dimension|	32 x 41 mm|
|	|Layer	|2|
|Vertical module|	Dimension|	50.5 x 24 mm|
|	|Layer	|2|

By using relays, I wanted to give the possibility of having an open contact available, without voltage, that can be used for example in the following ways:
-	Open contacts on equipment such as boilers, air conditioners, HVAC systems, and similar;
-	Open contact on power contactors, such as pumps;
-	Any type of connection, whether or not it requires voltage at the connections.

## System Architecture
For all possible modules the system is always the same, 3 stacked pcb that allows for modularity and easy maintenance if necessary. See DOMIX M1 module for more information.
The DOMIX M4 system includes a PCA9554A integrated circuit that allows you to control the gate of each 6 N-channel mosfet connected directly to the coils of 6 10A miniature pcb power relay. In this case, in order to choose the correct address for the chip, a dip switch called SW1 manages the correct I2C address on channel 1 of the M1 module's control bus. The following summarizes the possible (but non-overlapping) addresses that can be managed by the integrated circuit itself:

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

It is highly recommended that you first create a sort of table where you can know how many and which modules will need to be paired so that you can correctly manage all the addresses and avoid overlaps.

The system basically controls the contacts on the relay and in particular the characteristic of these components is to have a contact capable of withstanding a maximum of 10A at 250VAC or 10A at 30VDC. The coil is actuated at 5V to reduce the load on the contacts of PCA9554A itself through the mosfet.

At this point, someone might jump up on their chair. But wasn't it said that the entire system had to be completely decoupled from any dangerous live voltages? Yes, it’s correct, but at the same time, some appliances need to have this type of contact under voltage (for example, my boiler use 230VAC on his contact) and therefore I could not avoid to install a module with this type of solution.

The part which can withstand a maximum of 230VAC and 10A is only related to the connections between the contacts of the relays and the connector to the external world. On the pcb the tracks will be without silkscreen so the user can add solder to increase the physical thickness of the traces if needed.

Always remember that dealing with mains voltage is never a joke and that any operation related to it must always be carried out by qualified personnel. If in doubt, it is always best to contact an electrician.

## Power Supply Architecture
The module's control logic is powered by the IDC connector and therefore by the DOMIX M1 core module. A 3V3 power supply is required to operate the integrated circuit of the PCA9554A chip, while the 5V power supply in the cable is used for the relay coils circuit, which is separated by the mosfet from the logic circuit at 3V3. The integrated circuit has a decoupling capacitor, and the external contacts are completely separated from the inside of the logical part of the board. 

## User Interface & Display Module
A user interface made with a display exactly identical to the DOMIX M3 module is foresee on the project. The use of it, by the way, should be extremely calculated due to the lack of possibilities of the I2C address available on the bus. Adding another I2C screen could be dangerous and not correct, any user should, in my opinion, calculate the correct number of modules and use just one display where I can always represent the output of my system through ESPHome. This, in any case, should be studied case by case.

## Mechanical Structure
As like all the other DOMIX module, the DOMIX M4 system uses a stacked PCB architecture with the lower PCB containing core electronics, a vertical PCB providing mechanical and electrical interconnect, and an upper PCB housing the human interface. This structure minimizes footprint, improves maintainability, and allows for future UI or expansion board development.



