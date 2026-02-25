# ESPHome software

For me, ESPHome was an obvious choice, convenient and quick. I'm not a great programmer and I didn't want to get lost in a maze of lines of code, perhaps already flawed from the start, which would have wasted a lot of time. I've already used ESPHome in many other projects with excellent results, so I'd like to repeat the same approach in this project.
You're all familiar with ESPHome, so this isn't the section to explain how it works or how to use it. For that, I refer you to the application's excellent website, located at https://esphome.io/.

I created a project with a basic structure that is absolutely not intended to be the only possible solution; on the contrary, it aims to be the inspiration for many possible solutions that can be implemented on the system.

## Project structure
The structure includes a hardware section where all the possible blocks relating to the various basic components of ESPHome have been divided, with the configuration updated with the hardware characteristics of the system. The sensor folder currently only includes the 1-wire module, which can be configured if necessary, while the module section includes the basic definitions of all modules, also including alternative examples, which must be configured by the user according to their needs.

```
Software/
├── hardware/
│   ├── data.yaml
│   ├── esp.yaml
│   ├── ethernet.yaml
│   ├── i2c.yaml
│   ├── info.yaml
│   ├── log.yaml
│   ├── mqtt.yaml
│   ├── ota.yaml
│   ├── restart.yaml
│   ├── screen.yaml
│   ├── serial.yaml
│   ├── time.yaml
│   ├── web_server.yaml
│   └── zigbee.yaml
├── module/
│   ├── m1.yaml
│   ├── m2.yaml
│   ├── m3.yaml
│   ├── m4.yaml
│   ├── m5.yaml
│   ├── m6.yaml
│   ├── m7.yaml
│   ├── m8.yaml
│   ├── m9.yaml
├── sensor/
│   └── wire.yaml
└── main.yaml
```

In the main.yaml file, you can select the modules you're interested in and the hardware components you intend to use during the build. Along with these, you'll need to customize the data.yaml file, which contains all the necessary substitutions to best customize your firmware. In fact, within this file you need to set the I2C addresses corresponding to each module. Without this, the system will not work.
Once the programming is set up you can upload using the USB-C port on the DOMIX M1 board that connects directly to the microcontroller, as the controller is natively built in. Please do not connect the external power supply while programming the module.

## Considerations about Zigbee firmware
Special consideration should be made for the Zigbee module. It must be programmed with a specific firmware that allows you to create a coordinator that can communicate with your central server via Zigbee2MQTT. The procedure is explained very well in another project of mine at the link: [POE-Zigbee-Coordinator](https://github.com/carletz/POE-Zigbee-Coordinator). I always thank [ZigStar](https://zig-star.com/) and [Z-Stack-firmware](https://github.com/Koenkk/Z-Stack-firmware/tree/dev/coordinator/Z-Stack_3.x.0/bin) for creating the specific tools and firmware. 
This setting allows you to automatically load the firmware. If that doesn't work, you can follow the script steps manually and use the buttons on the board for the Zigbee chip. Furthermore, if you encounter further problems, the board's direct programming pins are also exposed.

> [!WARNING]
> ESPHome version 2026, at the time this guide was written, is not compatible with the external Zigbee component, so if you need to use it you must compile it with a maximum version of 2025. I used 2025.7.5. If you don't need it, you can use the latest version.

