CAN-viewer
==========

A simple CAN to USB bridge for STM32F407.

It reads two CAN interfaces, CAN1 (PA11/12) and CAN2 (PB12/13). When registering some activity on any CAN bus, it receives data and sends it to the USB via VCP (virtual COM port).

You can watch received data in Putty or similar terminal; or write your own app.

----- Detailed description -----
1. Open /EWARM/CAN_reader.eww in IAR (version 6.30 is good)
2. Compile it and load to the target STM32F407VGT device
3. Make two hardware CAN interfaces (schematics and board files are in the /interface folder)
4. Connect it to the CAN bus of the car, and USB to client USB jack on STM32F407 board
5. Open putty or similar app
6. Do some activity on the car (open the door, press any button, start the engine, etc) and watch data packets in console log.