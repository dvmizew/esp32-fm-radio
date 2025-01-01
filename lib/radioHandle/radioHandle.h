#ifndef RADIOHANDLE_H
#define RADIOHANDLE_H

#include <Arduino.h> // for byte data type from TEA5767 library
#include <Wire.h>
#include <TEA5767.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "hardware.h"
#include "bluetooth.h"
#include "joystickHandle.h"

#define MAX_RADIO_STATIONS 20

struct radioStation {
    char name[20];
    float frequency;
};

void setupRadio();
void initRadio();
void enableRadio();
void disableRadio();
void signalStrengthLED();
void increaseRadioFrequency();
void decreaseRadioFrequency();
void setRadioFrequency(float freq);
void searchRadioStations();
void printRadioStations();
void joystickRadioControl();
int32_t get_sound_data(Frame *data, int32_t frameCount);
void passAudioToBluetooth();

// getters
const char* getCurrentStation();
float getFrequency();
short getSignalLevel();

#endif