Closed loop step motor controller

Use standard rotary encoder (HN3806-AB-400N or similar)

     
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
      LED---------<|D13          D4|<------+   |     ||||
                   |               |           |     ||||
   _________       |               |IN_B       |     ||||
   FEED HOLD------<|D7           D5|<----------+     ||||
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

Hox!
 - Because of speed optimizations you cannot change pinout without code changes. 
 - This has not been tested yet. (My encoder is still somewhere between China and my home.)
 - FEED HOLD is open collector output. With this pin you can ask "feed hold" from CNC machine controller
        steps loss --> FEED HOLD is LOW.
        normally ---> FEED HOLD is open

Library:
https://github.com/arttupii/ArduinoHelperLibrary

Useful links:
https://www.banggood.com/NANO-IO-Shield-Expansion-Board-Nano-V3-Improved-Version-No-Cable-For-Arduino-p-1010994.html?rmmds=search&cur_warehouse=CN
