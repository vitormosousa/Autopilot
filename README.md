# Autopilot

Seatalk 2 is a 9-bit half-duplex bus with a speed of 4800 baud.
Extensive documentation is available at http://www.thomasknauf.de/seatalk.htm.

This project aims to make it possible to send commands directly to the autopilot, replacing the command.

The interface is made of any PC (raspberry) with “node red”. The hardware is an arduino nano and level shifter.

The arduino software is based on an adaptation of the "SoftwareSerial9" library
