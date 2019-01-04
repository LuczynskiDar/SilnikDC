# DC Engine driver ( Sterownik prędkości silnika DC )

> DC Engine driver is a [post graduate thesis](https://github.com/LuczynskiDar/SilnikDC/blob/master/Praca%20Podyplomowa%20-%20Sterownik%20Silnika%20-%20Dariusz%20%C5%81uczy%C5%84ski.pdf) done for AGH Univerisity of Science and Technology.

## Purpose of project

The purose of project was to use ECAD, EAGLE to design a circuit analog with
PCBA, then wirte a C program for Atmega32 to dirive a DC Enginge.

### Block diagram
![Block diagram](https://github.com/LuczynskiDar/SilnikDC/blob/master/Img/bloc_diagram.PNG)

### Schematic draing

![Schematics](https://github.com/LuczynskiDar/SilnikDC/blob/master/Img/schematic.PNG)

### PCBA
![PCBA view](https://github.com/LuczynskiDar/SilnikDC/blob/master/Img/PCBA_1.PNG)

### PCBA Drawing
![PCBA drawing view](https://github.com/LuczynskiDar/SilnikDC/blob/master/Img/PCBA.PNG)

### Design Approach

- zostanie wykorzystany wewnętrzny zegar mikroprocesora, bez posiłkowania się
zewnętrznym rezonatorem kwarcowym;
- sterowanie będzie obywało się za pomocą czterech przycisków, a będzie realizować
inkrementację i dekrementację sygnału PWM, a także zmianę wyjścia, na którym jest
on podawany;
- do sterowania nie zostanie wykorzystany mechanizm przerwań;
- zostanie wykorzystany tryb Fast PWM;
- oprogramowanie zostanie napisane w języku C;

### Design Verification
![Press S1](https://github.com/LuczynskiDar/SilnikDC/blob/master/Img/press_s1.PNG)

![Press S5](https://github.com/LuczynskiDar/SilnikDC/blob/master/Img/press_s5.PNG)

