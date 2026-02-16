# Module M3 CONTACT

##	Overview
The DOMIX M3 module is intended to be an analogic contact input module, useful for door, windows, and similar reed or switch contact reading.

|Main info|	M3|  |
|------|--------|-----|
|Lower module|	Dimension|	67 x 84.4 mm|
|	|Layer	|2|
|Upper module|	Dimension|	67 x 41 mm|
|	|Layer	|2|
|Vertical module|	Dimension|	50.5 x 24 mm|
|	|Layer	|2|


## System Architecture
For all possible modules the system is always the same, 3 stacked pcb that allows for modularity and easy maintenance if necessary. See DOMIX M1 module for more information.
The DOMIX M3 system includes a PCA9554A integrated circuit that allows you to control the 4 switches of analog multiplexer for scan 16 analog channel, all for input purposes. Due to physical restraint on the pcb for choosing the right address there are several pads to be welded during preparation. The following summarizes the possible (but non-overlapping) addresses that can be managed by the integrated circuit itself:

|PCA9554A| | | |	
|-----|-----|----|----| 
|Jumper to be welded| | | |
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

The analog multiplexer, which reads the contact values, is a CD74HC4067PWR chip. The output of this component is connected to an ADS1115 that, though the I2C connection forward to the DOMIX M1 module the data read. Also for this chip there is a dedicated part on the back of pcb where it is necessary to weld the correct pad for addressing correctly the chip on the bus. In this case the table can be represented as follows:

|Address	|Pad||
|----|-----|-----|
|0x48|	GND| pad welded|
|0x49|	3V3| pad welded|
|0x4A|	SDA| pad welded|
|0x4B|	SCL| pad welded|


> [!NOTE] 
> Only one pad have to be welded to grant the correct address on the bus. The pads that are not needed should be leave as is.

It is highly recommended that you first create a sort of table where you can know how many and which modules will need to be paired so that you can correctly manage all the addresses and avoid overlaps.
The control logic is quite simple: a BC847B NPN transistor receives the signal from the outside on the base pin. The emitter is connected to GND while on the collector I read the presence or absence of the 3V3 signal, appropriately filtered with an R-C system. The signal is then send to a channel of the multiplexer.

On the ADS1115 I’ll read a signal which will be on the upper or lower part of the digital conversion. On ESPHome a proper zone of reading should be implemented.

## Power Supply Architecture
The module's control logic is powered by the IDC connector and therefore by the DOMIX M1 core module. A 3V3 power supply is required to operate the integrated circuits, while the 5V power supply in the cable is not used on this board. Each integrated circuit has a decoupling capacitor where necessary, the external contacts are connected to the 12VDC power. This power supply can be the same one supplied to the DOMIX M1 module.

## User Interface & Display Module
In this case a user interface is represented by a small 0.96 128x64 SSD1315 chip I2C screen. This is not essential and could be avoided mounting on the module, the choice can be made at any time.

The HS HS96L03W2C03 OLED display module typically operates on an I2C address of 0x3C or 0x3D. It uses the SSD1306 driver, and the specific 7-bit address is determined by the SA0 bit (address pin), which is often configurable via a solder jumper on the back of the module. 

Jumper Setting: Modifying the zero-ohm resistor (jumper) on the back allows switching between potential addresses. 

Keep in mind always that the screen operates on the I2C bus number 2 so the address should be choose accordingly.

## Mechanical Structure
As like the DOMIX M1 and M2 module, the DOMIX M3 system uses a stacked PCB architecture with the lower PCB containing core electronics, a vertical PCB providing mechanical and electrical interconnect, and an upper PCB housing the human interface. This structure minimizes footprint, improves maintainability, and allows for future UI or expansion board development.

