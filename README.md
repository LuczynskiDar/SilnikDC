# DC Engine driver ( Sterownik prędkości silnika DC )

> DC Engine driver is a [post graduate thesis](https://github.com/LuczynskiDar/SilnikDC/blob/master/Praca%20Podyplomowa%20-%20Sterownik%20Silnika%20-%20Dariusz%20%C5%81uczy%C5%84ski.pdf) done for AGH Univerisity of Science and Technology.

## Purpose of project

The purose of project was to use ECAD, EAGLE to design a circuit analog with
PCBA, then wirte a C program for Atmega32 to dirive a DC Enginge.

### Design approach

- For the project purposes will be used an mocro's internal clock,instead of using more reliable external crystal.
- Drivig the engine will be done on the basis of 4 switches, 2 for changing engine directions (Left/Right),
  which is actually changing micro's GPIO  and 2 responsible for incrementing and decrementing PWM signal.
- Interruption mechanism won't be used.
- I will use built in Fast PWM mechanism.
- The software will be done in C, usng Atmega Studio 7.
- The code will be debugged and uploaded thoreugh JTAG with MK7 Dragn programmer.
- Thce code will be validated with an eval board, Kamami ZL3.

### Block diagram

![Block diagram](https://github.com/LuczynskiDar/SilnikDC/blob/master/Img/bloc_diagram.PNG)

### Eagle ECAD designer

There are 2 Eagle project files:

- [Sterownik_DC.sch](https://github.com/LuczynskiDar/SilnikDC/blob/master/Sterownik_DC.sch) 
- [Sterownik_DC.brd](https://github.com/LuczynskiDar/SilnikDC/blob/master/Sterownik_DC.brd)

#### Schematic drawing

![Schematics](https://github.com/LuczynskiDar/SilnikDC/blob/master/Img/schematic.PNG)

#### Material List (BOM)

![BOM](https://github.com/LuczynskiDar/SilnikDC/blob/master/Img/bom.PNG)

#### Printed wiring board

![PCBA view](https://github.com/LuczynskiDar/SilnikDC/blob/master/Img/PCBA_1.PNG)

#### PWB Drawing

![PCBA drawing view](https://github.com/LuczynskiDar/SilnikDC/blob/master/Img/PCBA.PNG)

### C Code

I created 2 files,

- [constants.h](https://github.com/LuczynskiDar/SilnikDC/blob/master/Sterownik_DC/constants.h), which contains definition for an internal clock flags,
- [sterownikdc.c](https://github.com/LuczynskiDar/SilnikDC/blob/master/Sterownik_DC/sterownikdc.c), which contains other methods along with main loop ```while(1)```.

### Design Verification

Code verificationn was performed on eval board, all 4 swithches were working properly.

- Set the dc engine to turn left, "l" on the display.

![Press S1](https://github.com/LuczynskiDar/SilnikDC/blob/master/Img/press_s1.PNG)

- Set the dc engine to turn right, "r" on te display.

![Press S5](https://github.com/LuczynskiDar/SilnikDC/blob/master/Img/press_s5.PNG)