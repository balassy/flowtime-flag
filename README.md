# Flow Time Flag

An IoT device powered by an ESP8266 microcontroller that signals [flow time](https://en.wikipedia.org/wiki/Flow_(psychology)) with a flag.

## How it works

This code connects to the specified time server to retrieve the current time adjusted to your current time zone and daylight saving preferences. Based on the specified time range for flow time it rotates a servo motor.

## Getting started

### Hardware

TBD

### Enclosure

TBD

### Software

Load the project into the Arduino IDE (or Visual Studio Code) and import the [WiFiManager library](https://github.com/tzapu/WiFiManager).

Update the parameters in the `config.h` file and upload the sketch to your ESP8266.

### Usage

If you have not connected your ESP8266 to the network before, when first boot up it will create a new WiFi network with the parameters specified in `config.h`. By default the network name is `FlowTimeFlag`.

Connect to this WiFi network with the password specified in `config.h` (by default `FlowTime!`).

Open your favorite web browser and navigate to `http://192.168.4.1`, and configure the WiFi connection. Upon save your device will restart and automatically connect to the specified WiFi network.

## Acknowledgements

Thanks to [Andreas Spiess](https://github.com/SensorsIot) for sharing his [NTP library for ESP8266](https://github.com/SensorsIot/NTPtimeESP), though I had to change his code to make it more reliable.

Thanks to [Roman Minyaylov](https://github.com/roman-minyaylov) for sharing his [NodeMCU Fritzing part](https://github.com/roman-minyaylov/fritzing-parts). I copied the current version of that part into the `./doc/wiring/esp8266-nodemcu-v3` folder.


## About the author

This project is maintained by [György Balássy](https://linkedin.com/in/balassy).
