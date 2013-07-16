CAN-viewer
==========

A simple CAN to USB bridge for STM32F407.

It reads two CAN interfaces, CAN1 (PA11/12) and CAN2 (PB12/13). When registering some activity on any CAN bus, it receives data and sends it to the USB via VCP (virtual COM port).

You can watch received data in Putty or similar terminal; or write your own app.
