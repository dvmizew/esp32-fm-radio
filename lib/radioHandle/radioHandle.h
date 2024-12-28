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
    char name[32];
    float frequency;
};

class RadioHandle {
public:
    RadioHandle();
    ~RadioHandle() = default;
    
    // methods
    void setupRadio(); // setup radio pins
    void initRadio();
    void signalStrengthLED();
    void searchRadioStations();
    void printRadioStations();
    void handleRadioControl();
    void passAudioToBluetooth(); // pass audio to Bluetooth speaker
    int32_t get_sound_data(Frame *data, int32_t frameCount); // get sound data from radio using I2S

    // getters
    const char* getCurrentStation() const;
    float getFrequency() const;
    short getSignalLevel() const;

private:
    TEA5767 radio;
    BluetoothA2DPSource bluetoothRadioSource;
    JoystickHandle joystick;
    char currentStation[16];
    float frequency;

    radioStation stations[MAX_RADIO_STATIONS];
};

#endif