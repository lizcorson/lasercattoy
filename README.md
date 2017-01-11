# lasercattoy
A remote-controlled laser cat toy.

## Overview
This project will use MQTT to connect a wireless joystick controller to a a laser cat toy.

## Status
Rather than using an esp8266, I've switched to an Adafruit M0 WiFi board, which has multiple analog inputs built in. This allows me to get a much smaller form factor for the joystick, since I don't need a protoboard with an analog multiplexer.

I'm currently using MQTT as an intermediary between the joystick and the laser.

## Parts List

Electronic parts:
- [Adafruit M0 WiFi](https://www.adafruit.com/products/3010)
- [Adafruit Feather Huzzah](https://www.adafruit.com/product/2821)
- [Joystick module](https://www.adafruit.com/products/512)
- [Laser diode](https://www.adafruit.com/products/1054)
- [Laser mount](https://www.adafruit.com/products/1094)
- 2x [micro servo](https://www.adafruit.com/products/169)
- [Pan-tilt mount](https://www.adafruit.com/products/1968)
