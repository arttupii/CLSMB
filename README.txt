Closed loop step motor controller

You can use AS5040 or standard rotary encoder (HN3806-AB-400N)


1. Use AS5040 Rotary Sensor 
    Link to test video:
    https://www.youtube.com/watch?v=YhEo3L9fRq0


    Architecture of closed loop step motor controller:

                 +--------------+         +------------+
                 | +----------+ |         |            |
      stepInPin--->|          | |    +--->| Step Motor |
                 | | Arduino  | |    |    |            |
      dirInPin---->| Pro Mini | |    |    |            |
                 | |          | |    |    +------+-----+
      enInPin----->|          | |    |           |
                 | +----------+ |    |           |
                 |              |    |           |
                 | +----------+ |    |           |
                 | | A4988    |------+           |
                 | +----------+ |                |
                 |              |                |
                 | +----------+ |                |
                 | | AS5040   |<-----------------+ Loop back
                 | +----------+ |    (step/dir signal)
                 |              |
                 +--------------+


             
    Component list:
    - A4988  https://www.banggood.com/5Pcs-3D-Printer-A4988-Reprap-Stepper-Motor-Driver-Module-p-952527.html?rmmds=myorder&cur_warehouse=CN
    - Pro Mini ATMEGA328P https://www.banggood.com/3Pcs-Pro-Mini-ATMEGA328P-3-3V-8M-Interactive-Media-Upgrade-Version-For-Arduino-p-1193820.html?rmmds=myorder&cur_warehouse=CN
    - AS5040 Rotary Sensor https://ams.com/as5040
    - 220uF
    - 100nF
    - 2.54mm Female Header Jumper Connectors Socket
    - 2.54mm Single Row Male Pin Header Strip
    - Step motor (nema23)
    - 6x3mm Diametriclly magnetised magnet https://www.ebay.com/itm/10-Magnets-6x3-mm-Neodymium-Disc-small-DIAMETRIC-craft-magnet-disk-6mm-dia-x-3mm/162072923403?ssPageName=STRK%3AMEBIDX%3AIT&_trksid=p2060353.m2749.l2649
    - PCB boards from ITEAD (https://www.itead.cc/open-pcb/pcb-prototyping.html) 

    Photos:
    https://github.com/arttupii/CLSMB/tree/master/photos

    Schematic&Gerber files:
    https://github.com/arttupii/CLSMB/tree/master/eagleFiles

2. Use standard rotary encoder (HN3806-AB-400N or similar)

    ->Modify config.h file and enable QUADRATURE_MODE flag
    
    
    Architecture of closed loop step motor controller:

                                       +---------+--------+
                   +--------------+    | Rotary  |  Step  |
      stepIn------>|3             |    | encoder |  Motor |
                   |   Arduino    |    |         |        |
      dirIn------->|6  Pro Mini   |    |  A   B  |        |
                   |   or nano    |    +---------+--------+
      EnIn-------->|7             |       |   |     ^^^^
                   |             4|<------+   |     ||||
      LED---------<|8             |           |     ||||
                   |             5|<----------+     ||||
                   |              |                 ||||
                   |  9  10   11  |                 ||||
                   +--------------+                 ||||
                      |   |   |                     ||||
                      |   |   |                     ||||
                      V   V   V                     ||||                                              
                   +---------------+                ||||
                   |               |                ||||
                   |  Standard    A|-----red--------+|||
                                  _|                 ||| 
                   |  Stepmotor   A|-----blue--------+||
                                   |                  ||  
                   |  controller  B|-----green--------+|
                   |              _|                   |
                   |  e.g. TB6600 B|-----black---------+
                   |               |
                   |               |
                   +---------------+                   

                   
Library:
https://github.com/arttupii/ArduinoHelperLibrary
