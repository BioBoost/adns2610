# ADNS2610 Optical Sensor Mbed Library

This is a driver for the ADNS2610 optical sensor often found in optical mice. These can be used as small camera's for motion detection or service meter counting (for example gas or electricity).

## Based on

This driver is based on the Arduino code of [https://conorpeterson.wordpress.com/2010/06/04/optical-mouse-hacking-part-1/](https://conorpeterson.wordpress.com/2010/06/04/optical-mouse-hacking-part-1/).

## Warning

Make sure to connect the SDIO and SCLK to 5V tolerant pins on the Mbed platform. Digital pins on K64F for example are 5V tolerant.