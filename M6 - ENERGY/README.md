# Module M6 ENERGY METER

<div align="center">
    <img width="70%" src="/./Images/Board/M6/M6%20-%20Pinout.png">
</div>

<div align="center">
    <img width="40%" src="/./Images/Board/M6/M6%20-%203D%20Board.png">
    <img width="40%" src="/./Images/Board/M6/M6%20-%203D%20Module.png">
</div>


## Overview
The DOMIX M6 module measures electrical consumption using clamp meters. It also measures the current mains voltage to accurately calculate the power used at the measurement points.

|Main info|	| M6|
|------|--------|-----|
|Lower module|	Dimension|	67 x 84.4 mm|
|	|Layer	|2|
|Upper module|	Dimension|	67 x 41 mm|
|	|Layer	|2|
|Vertical module|	Dimension|	50.5 x 24 mm|
|	|Layer	|2|

## System Architecture
The architecture, with three PCBs connected together, is the same as the previous modules. The base has the main task of implementing the measurement logic, the vertical part is responsible for communication with the main module, and the upper part is responsible for the display.

Voltage measurement is performed via the ZMPT101B single-phase voltage transformer (AC voltage max. 250 volts) present at the input, which is then recalculated through an operational amplifier that makes it available as an analog output. The output signal is sinusoidal, and the average value of the waveform (DC component) is 1/2 VDC. The output signal can be adjusted via the trimmer on the module.

The current measurement is also carried out through a circuit with an amplifier which is however managed by an analog multiplexer CD74HC4067SM96 which probes all 10 available channels through the use of a PCA9554APW,118 which in turn switches between all the possibilities.

As said the DOMIX M6 system includes a PCA9554A integrated circuit that allows you to control the 4 switches of analog multiplexer for scan 10 analog channel, all for input purposes. The address can be chosen through the dip switch on the side of the board and the following summarizes the possible (but non-overlapping) addresses that can be managed by the integrated circuit itself:

|PCA9535| | | |	
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

The output of the analog multiplexer is connected to an ADS1115 that, though the I2C connection forward to the DOMIX M1 module the data read. For this chip there is a dedicated part on the front of pcb where it is necessary to weld the correct pad for addressing correctly the chip on the bus. In this case the table can be represented as follows:

|Address	|Pad||
|----|-----|-----|
|0x48|	GND| pad welded|
|0x49|	3V3| pad welded|
|0x4A|	SDA| pad welded|
|0x4B|	SCL| pad welded|

As always a correct design of the I2C addresses is necessary to avoid any overlapping and therefore malfunction of the system

## Power Supply Architecture
The module's control logic is powered by the IDC connector and therefore by the DOMIX M1 core module. The operational amplifier that measures the correct mains voltage requires 5V, which is always supplied from the same connector. The rest of the control logic operates at 3V3.

## User Interface & Display Module
A user interface made with a display exactly identical to the DOMIX M1 module is foresee on the project. The use of it, by the way, should be extremely calculated due to the lack of possibilities of the I2C address available on the bus. Adding another I2C screen could be dangerous and not correct, any user should, in my opinion, calculate the correct number of modules and use just one display where I can always represent the output of my system through ESPHome. This, in any case, should be studied case by case.

## Mechanical Structure
As like all the other DOMIX module, the DOMIX M6 system uses a stacked PCB architecture with the lower PCB containing core electronics, a vertical PCB providing mechanical and electrical interconnect, and an upper PCB housing the human interface. This structure minimizes footprint, improves maintainability, and allows for future UI or expansion board development.

