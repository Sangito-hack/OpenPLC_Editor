#include <stdlib.h>
extern "C" {
#include "openplc.h"
}
#include "Arduino.h"

//OpenPLC HAL for CANADUINO PLC Series 500
//https://universal-solder.ca/product/canaduino-plc-diy-500-ac-wifi-mega2560/
//https://universal-solder.ca/product/plc-diy-kit-canaduino-500-24-arduino/

/************************PINOUT CONFIGURATION*************************
Digital In: 22, 23, 24, 25, 26, 27, 28, 29        (%IX0.0 - %IX0.7)
            30, 31, 32, 33, 34, 36, 36, 37        (%IX1.0 - %IX1.7)
			
Digital Out: 38, 39, 40, 41, 42, 43, 44, 45       (%QX0.0 - %QX0.7)
             46, 47, 48, 49                       (%QX1.0 - %QX1.3)
			 
Analog In: A0, A1, A2, A3, A4, A5, A6, A7         (%IW0 - %IW7)
           A8, A9, A10, A11, A12, A13, A14, A15   (%IW8 - %IW15)
		   
Analog Out: 2, 3, 4, 5, 6, 7, 8, 9                (%QW0 - %QW7)
			
*********************************************************************/

//Define the number of inputs and outputs for this board (mapping for the Arduino Mega)
#define NUM_DISCRETE_INPUT         16
#define NUM_ANALOG_INPUT           16
#define NUM_DISCRETE_OUTPUT        12
#define NUM_ANALOG_OUTPUT          8

//Create the I/O pin masks
//const PROGMEM uint8_t pinMask_DIN[] = {62, 63, 64, 65, 66, 67, 68, 69, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52};
uint8_t pinMask_DIN[] = {22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37};
uint8_t pinMask_AIN[] = {A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15};
uint8_t pinMask_DOUT[] = {38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49};
uint8_t pinMask_AOUT[] = {2, 3, 4, 5, 6, 7, 8, 9};

void hardwareInit()
{
    for (int i = 0; i < NUM_DISCRETE_INPUT; i++)
    {
		uint8_t pin = pinMask_DIN[i];
        //pinMode(pin, INPUT);
        pinMode(pin, INPUT_PULLUP);
    }
    
    for (int i = 0; i < NUM_ANALOG_INPUT; i++)
    {
		uint8_t pin = pinMask_AIN[i];
        pinMode(pin, INPUT);
    }
    
    for (int i = 0; i < NUM_DISCRETE_OUTPUT; i++)
    {
		uint8_t pin = pinMask_DOUT[i];
        pinMode(pin, OUTPUT);
    }

    for (int i = 0; i < NUM_ANALOG_OUTPUT; i++)
    {
		uint8_t pin = pinMask_AOUT[i];
        pinMode(pin, OUTPUT);
    }
}

void updateInputBuffers()
{
    for (int i = 0; i < NUM_DISCRETE_INPUT; i++)
    {
		uint8_t pin = pinMask_DIN[i];
        if (bool_input[i/8][i%8] != NULL) 
            *bool_input[i/8][i%8] = digitalRead(pin);
    }
    
    for (int i = 0; i < NUM_ANALOG_INPUT; i++)
    {
		uint8_t pin = pinMask_AIN[i];
        if (int_input[i] != NULL)
            *int_input[i] = (analogRead(pin) * 64);
    }
}

void updateOutputBuffers()
{
    for (int i = 0; i < NUM_DISCRETE_OUTPUT; i++)
    {
		uint8_t pin = pinMask_DOUT[i];
        if (bool_output[i/8][i%8] != NULL) 
            digitalWrite(pin, *bool_output[i/8][i%8]);
    }
    for (int i = 0; i < NUM_ANALOG_OUTPUT; i++)
    {
		uint8_t pin = pinMask_AOUT[i];
        if (int_output[i] != NULL) 
            analogWrite(pin, (*int_output[i] / 256));
    }
}
