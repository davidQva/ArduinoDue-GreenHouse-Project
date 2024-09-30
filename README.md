# Smart Greenhouse Project

## Description

The Smart Greenhouse project aims to monitor and control the climate of a greenhouse. The system controls temperature and light intensity, ensuring that the temperature is maintained between 20°C and 25°C. It's supposed to provide a light-to-darkness ratio to promote healthy plant growth.

This project is implemented using the **Microchip SAM3X8E** ARM embedded platform (Arduino Due) and developed on **IAR**. The project integrates various components including sensors, a motorized mirror system, and a display.

## System Components

- **Microcontroller**: Microchip (Atmel) SAM3X8E – ARM embedded platform (Arduino Due)
- **Keypad**: Port I/O for user input 4*4 keypad - AT91 Keyboard interface
- **Photosensor**: A/D converter for light measurement -CDS PHOTORESISTORS
PGMXXX-MP Series
- **Temperature Sensor**: Time-based monitoring of the greenhouse temperature-MAX6575H-MAX6575L
- **RC Servo Motor**: Pulse Width Modulation (PWM) for controlling mirror positioning - HS-55 MICRO LITE SERVO MOTOR
- **Switch**: Hardware interrupt for system control
- **Display**: Port I/O for user interface and data presentation -HS-55 MICRO LITE SERVO 
- **LEDs**: For simulating the lighting system in the greenhouseSchool Project 

To run this you would need to have the hardware components and the software tools installed. The code is just supposed to be used as an example in implementing this kind of hardware to ARM platform.