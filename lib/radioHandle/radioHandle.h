#ifndef RADIOHANDLE_H
#define RADIOHANDLE_H

#include <Arduino.h> // for byte data type from TEA5767 library
#include <Wire.h> // for I2C communication
#include <TEA5767.h>
#include <freertos/FreeRTOS.h> // mulithreading
#include <freertos/task.h> // for tasks
#include "hardware.h" // access to hardware components
#include "bluetooth.h" // for Bluetooth control

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
void signalStrengthLED(); // lights up the internal LED based on the signal strength
void handleRadioControl(); // handles radio control using the buttons
void handleRadioControlTask(void *parameter); // task to handle radio control multithreaded
void increaseRadioFrequency();
void decreaseRadioFrequency();
void setRadioFrequency(float freq);
void searchRadioStations(); // searches for radio stations and stores them in the stations array
void printRadioStations(); // prints the radio stations to the serial monitor
int32_t get_sound_data(Frame *data, int32_t frameCount);
void passAudioToBluetooth(); // passes audio data from get_sound_data to the Bluetooth speaker

// getters
const char* getCurrentStation();
float getFrequency();
short getSignalLevel();

#endif