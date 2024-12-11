#ifndef RADIOHANDLE_H
#define RADIOHANDLE_H

#include <Wire.h>
#include <TEA5767.h>
#include "hardware.h"
#include "bluetooth.h"
#include "joystickHandle.h"

class RadioHandle {
public:
    RadioHandle();
    
    // methods
    void setupRadio(); // setup radio pins
    void initRadio();
    void tuneRadio(float freq);
    void searchRadioStations();
    void handleRadioControl();
    void passAudioToBluetooth(); // pass audio to Bluetooth speaker
    int32_t get_sound_data(Frame *data, int32_t frameCount); // get sound data from radio using I2S

    // getters
    const char* getCurrentStation() const;
    float getFrequency() const;

private:
    TEA5767 radio;
    BluetoothA2DPSource bluetoothRadioSource;
    JoystickHandle joystick;
    char currentStation[16];
    float frequency;
};

#endif