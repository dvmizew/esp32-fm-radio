#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include "hardware.h"
#include <Wire.h>
#include <BluetoothA2DPSink.h>
#include <BluetoothA2DPSource.h>

class BluetoothHandle {
public:
    BluetoothHandle();
    void initializeBluetoothSpeaker(); // ESP32 as A2DP sink (external speaker)
    void playThroughBluetoothSpeaker(); // play audio through other Bluetooth speaker

    // getters
    bool isConnected() const; // returns true if some a2dp device is connected
    const char* getDeviceName() const; // returns the device name that is connected

private:
    static void connectionStateCallback(bool connected); // callback for connection state

    bool connected;
    BluetoothA2DPSink btAudioSink; // sink for bluetooth speaker output
    BluetoothA2DPSource btAudioSource; // source for bluetooth audio input
    I2SStream i2s;
    const char* deviceName; // Bluetoothinatorul by default
};

#endif