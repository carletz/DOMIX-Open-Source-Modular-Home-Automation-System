# Box for DIN modules

The boxes for the various modules were designed based on the standard dimensions of commercial modules. The height, width, and depth were adapted to those commonly available on the market.

The surface thickness of the modules is 1.5 mm, making them sturdy enough for initial handling and light enough to print quickly. I personally printed them with e-Sun PLA+ filament; I haven't tried other brands or materials.

The lower board is mated to the module by interposing the PCB with the back of the module and the front. Inside the front, I used threaded brass inserts, hot-fitted into the structure, and then secured everything with M3x7 mm screws from side to side. This solution makes the whole thing solid and convenient for attaching to the electrical panel.
The module is locked to the DIN rail using a clip, identical for all modules, which can be locked using a flat screwdriver.

<div align="center">
    <img width="70%" src="../Images/Board/M1/M1%20-%203D%20Exploded.png">
</div>

The front part fits into the front part and provides the basic information for user interaction with the system in front of the electrical panel. To print it, I used an AMS with at least two colors; I didn't want to overly burden the graphics. Unfortunately, given the small size of the components, it's necessary to adjust the printing parameters to optimize the final result. The part must be printed with the graphics facing the print bed.
It's also possible to print fronts without any graphics or holes. The system will then be controlled only via a web interface or MQTT. The "bare" front files are included in the documentation.

The connection identification plates, however, should be printed with the numbers or letters facing upward. In the graphic file, they are 0.4 mm thick. Ideally, these should also be printed in at least two colors.

To print all the modules approximately 500g of filament were used.


