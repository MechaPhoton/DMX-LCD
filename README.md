# DMX-LCD
simple solution for 2 axis motion control via DMX.
this project consists of a breakout board for w5500-evb-pico and associates firmware. 

with this system you can:
control each axis with 16bit DMX
set IP address, universe, and start channel
set high and low motion limits. 
home each axis with hall effect sensor via DMX. 





LCD menu for managing basic DMX connection info

written for fystec mini 12864 v2.1 LCD and wiznet evb w5500 pico

relatively easy to adapt to any 128x64 LCD

sACN protocol functionality WIP

makes use of olikraus' u8g2 and MUIU8g2, adafruit NEOPIXEL, Rui Seixas versatile rotary encoder, natcl artnet. 
