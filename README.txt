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
      dirIn------->|D9  Pro Mini   |    |  A   B  |        |
      ____         |   or nano     |    +---------+--------+
      EnIn-------->|D10            |       |   |     ^^^^
                   |             D4|<------+   |     ||||
      LED---------<|D13            |           |     ||||
                   |             D5|<----------+     ||||
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
 - This has not been tested yet. (My encoder is still somewhere between China and Finland.)

Library:
https://github.com/arttupii/ArduinoHelperLibrary
