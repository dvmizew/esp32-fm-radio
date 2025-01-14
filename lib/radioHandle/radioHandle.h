#ifndef RADIOHANDLE_H
#define RADIOHANDLE_H

#include <Arduino.h> // for byte data type from TEA5767 library
#include <Wire.h> // for I2C communication
#include <TEA5767.h>
#include <freertos/FreeRTOS.h> // mulithreading
#include <freertos/task.h> // for tasks
#include "hardware.h" // access to hardware components
// #include "displayHandle.h" // for displaying radio information

#define MAX_RADIO_STATIONS 20

// struct to store radio station data when we search
struct radioStation {
    char name[20];
    float frequency;
};

void setupRadio();
void initRadio();
void enableRadio();
void disableRadio();
void handleRadioControl(); // handles radio control using the buttons
void handleRadioControlTask(void *parameter); // task to handle radio control multithreaded
void increaseRadioFrequency();
void decreaseRadioFrequency();
void setRadioFrequency(float freq);
void searchRadioStations(); // searches for radio stations and stores them in the stations array
void printRadioStations(); // prints the radio stations to the serial monitor

// getters
const char* getCurrentStation();
float getFrequency();
short getSignalLevel();

#endif