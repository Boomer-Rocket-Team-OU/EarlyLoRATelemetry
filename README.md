# Early LoRa Telemetry Implementation

This is a repository dedicated towards constructing an early varient of LoRa Transceiever based communication. The intent of this code is to communicate essential information such as:
- Battery life (voltage in a LiPo Battery)
- Temperature
- Altitude
- Acceleration
- Velocity

And more forms of information - in other words, this code is intenteded to serve as sensor suite for grabbing information from the sensors and then providing them to a base station. 

## Software Configuration

The following is how the code is configured.

- telemetry.ino [payload code, intended to be loaded onto a arduino compatible board such as ESP-32 or Teensy]
- telemetryReceiever.ino [base station code, intended to be ran on a raspberry pi or similar system configured to be listening from the ground port.]

## Hardware Listing

- Any LoRa Reciever
- Any Barometer
- Any IMU (possibly two for consistency and error handling)
- Any microcontroller board (ESP-32, Teensy)
- Any compute board (rasperry pi, libre lepotato, Whatever alabama used like 4 years ago)

## Development Environment 

For the sake of development on this project, I am using the following hardware and software configuration on my system to perform the task of coding:

### Machine dependant

- CPU Architecture: x86 - x64 (note this should also be possible on ARM based machines)
- IDE(s) - Arduino IDE for the sake of interfacing with hardware , VSCode for configuration and git related tasks
 
 ### Libraries

 - List here as they are used
 