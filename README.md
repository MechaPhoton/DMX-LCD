# simple solution for 2 axis motion control via DMX.
this project consists of a breakout board for w5500-evb-pico and associated firmware.
with this system you can:

control each axis with 16bit DMX
home each axis with DMX control channel
set IP address, universe, and start channel with LCD
set high and low motion limits with LCD
Set microstep settings for each axis with LCD
quickly and easily plug in all your wires with common connections

BOM:
wiznet w5500-EVB-pico
fystec mini 12864 v2.1
2x polulu a4988 carrier 
12v to 5v regulator

2x 10 pin IDC male header 
2x 3 pin jst-xh header
2x 4 pin jst-xh header
2 pin jst-xh header
2.1x5.5 barrel jack breadboard compatible. 
female pins 2.54mm spacing
male pins   2.54mm spacing

also included is a simplified file for LCD menu for managing basic DMX connection info for projects with no motors. 

sACN protocol functionality WIP
makes use of olikraus' u8g2 and MUIU8g2, adafruit NEOPIXEL, Rui Seixas versatile rotary encoder, natcl artnet. 
