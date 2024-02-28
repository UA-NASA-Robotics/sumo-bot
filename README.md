# Sumo Bot ESP32 code

This program is for our Sumo Bot outreach robot. The microcontroller is an ESP32, and we are using the Bluepad32 library to control the robot with an XBOX ONE controller. 

## Control Scheme

A, B, X, Y are used to move the bot forward, backward, turn left, and turn right.

Up and Down on the D-Pad adjusts the zero offset.

## Current Issues

The XBOX controller doesn't like to stay connected to the ESP32, so our program crashes when it loses connection.