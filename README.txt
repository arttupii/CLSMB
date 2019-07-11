Closed loop step motor controller

Two modes are supported. Step loss compensation and load position control mode.
You can select mode from config.h file.
     
    Architecture of closed loop step motor controller:

                                             +---------+--------+
                        +---------------+    | Rotary  |  Step  |
                        |               |    | encoder |  Motor |
                        |               |    |         |        |
                        |               |    |         |        |
           stepIn------>|D8             |    |         |        |
                        |   Arduino     |    |         |        |
           dirIn------->|D9 Nano        |    |  A   B  |        |
                        |               |    +---------+--------+
           EnIn-------->|D10            |       |   |     ^^^^
                        |               |IN_A   |   |     ||||
           LED---------<|D13          D2|<------+   |     ||||
                        |               |           |     ||||
        _________       |               |IN_B       |     ||||
        FEED HOLD------<|D7           D3|<----------+     ||||
                        |               |                 ||||
                        |  A2  A1  A0   |                 ||||
                        +---------------+                 ||||
                           |   |   |                      ||||
                           |   |   |                      ||||
                           V   V   V                      ||||
                        +---------------+                 ||||
                        | !EN DIR  STEP |                 ||||
                        |               |                 ||||
                        |  Standard    A|-----red---------+|||
                        |              _|                  ||| 
                        |  Stepmotor   A|-----blue---------+||
                        |               |                   ||  
                        |  controller  B|-----green---------+|
                        |              _|                    |
                        |  e.g. TB6600 B|-----black----------+
                        |               |
                        |               |
                        +---------------+                   

Block diagram (STEP_LOSS_COMPENSATION-mode)
                   +----------------------------------+  
                   |   +----------+                   |  
 Input(Step,En,Dir)|   | Input    |                   |                                             
 ------------------+-->| counter  |                   |                                                  
                       |          |                   |              +--------------+       +--------------+
                   +---|          |                   |              |              |       | Step Motor   |
                   |   +----------+                   V    (Switch)  | Step         |       |              |
                   |                                  +----O         | Motor        |------>|              |
                   |   +----------+   +----------+         ------O-->| Driver       |       |              |
                   +-->|Deviation |   |          |    +----O  ^      | (e.g TB6600) |       |              |
                       |counter   |-->| >n steps |    ^       |      |              |       |              |
                   +-->|          |   |          |    |       |      +--------------+       +--------------+
                   |   +----------+   +---------+     |       |                             | Rotary       |
                   |                        |         |       |                             | encorer      |
                   |                        V         |       |                             |              |
                   |   +----------+      +----------------+   |                             |              |
                   +---|Rotation  |      |                |   |                             +--------------+
                       |position  |      | Fix position   |   |                                    |
                       |counter   |      | (Generate      |---+                                    |     
                   +-->|          |      |step,dir pulses)|                                        |     
                   |   +----------+      +----------------+                                        |     
                   |                                                                               |  
                   +-------------------------------------------------------------------------------+


Block diagram (LOAD_POSITION_CONTROL-mode)
                                                         
                       +----------+                      
 Input(Step,En,Dir)    | Input    |                                                                 
 --------------------->| counter  |                                                                      
                       |          |                                  +--------------+       +--------------+
                   +---|          |                                  |              |       | Step Motor   |
                   |   +----------+                                  | Step         |       |              |
                   |                                                 | Motor        |------>|              |
                   |   +----------+   +----------+    +------------->| Driver       |       |              |
                   +-->|Deviation |   |          |    |              | (e.g TB6600) |       |              |
                       |counter   |-->| >n steps |    |              |              |       |              |
                   +-->|          |   |          |    |              +--------------+       +--------------+
                   |   +----------+   +---------+     |                                     | Rotary       |
                   |                        |         |                                     | encorer      |
                   |                        V         |                                     |              |
                   |   +----------+      +----------------+                                 |              |
                   +---|Rotation  |      |                |                                 +--------------+
                       |position  |      | Fix position   |                                        |
                       |counter   |      | (Generate      |                                        |     
                   +-->|          |      |step,dir pulses)|                                        |     
                   |   +----------+      +----------------+                                        |     
                   |                                                                               |  
                   +-------------------------------------------------------------------------------+


The modes explained: https://www.linearmotiontips.com/how-does-closed-loop-stepper-control-work/
                                    
Hox!
 - Because of speed optimizations you cannot change pinout without code changes. 
 - FEED HOLD is open collector output. With this pin you can ask "feed hold" from CNC machine controller
        steps loss --> FEED HOLD is LOW.
        normally ---> FEED HOLD is open
 - LOAD_POSITION_CONTROL mode needs something. "Direct control" causes too much vibrations. 


 Compatible with:
     * Arduino nano and Arduino Pro Mini 
     * a standard step motor controller (e.g. a4988 and TB6600)
     * a standard rotary incremental encoder (e.g HN3806-AB-400N, AS5040 or similar)
               +---+   +---+   +---+   +---+
            A -+   +---+   +---+   +---+

              ---+   +---+   +---+   +---+  
            B    +---+   +---+   +---+   +--
     * NEMA xx Stepper motor
 
Demo video: 
https://youtu.be/jLogS5AQ-yQ (AS5040 encoder and A4988 step motor controller)
https://youtu.be/jxansaLNb5I (Serial Plotter view. #define ENABLE_PRINTS 100)

Useful links:
https://www.banggood.com/NANO-IO-Shield-Expansion-Board-Nano-V3-Improved-Version-No-Cable-For-Arduino-p-1010994.html?rmmds=search&cur_warehouse=CN
https://www.banggood.com/TB6600-Upgraded-Stepper-Motor-Driver-Controller-for-4A-940V-TTL-32-Micro-Step-2-or-4-Phase-of-4257-Stepper-Motor-3D-Printer-CNC-Part-p-1453122.html?rmmds=search&cur_warehouse=CN
https://www.banggood.com/3D-Printer-42-Stepper-Motor-Drive-Expansion-Board-8825-A4988-p-1028319.html?rmmds=search&cur_warehouse=CN
https://www.banggood.com/Geeetech-Stepper-Driver-A4988-With-Heatsink-And-Sticker-For-3D-Printer-p-1229214.html?rmmds=search&cur_warehouse=CN
