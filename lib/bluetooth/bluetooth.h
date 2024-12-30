#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include "hardware.h"
#include "joystickHandle.h"
#include <Wire.h>
#include <AudioTools.h>
#include <BluetoothA2DPSink.h>
#include <BluetoothA2DPSource.h>
#include <Audio.h>

class BluetoothHandle {
public:
    BluetoothHandle();
    ~BluetoothHandle() = default;

    void initializeBluetoothSpeaker(); // ESP32 as A2DP sink (external speaker)
    void playThroughBluetoothSpeaker(); // play audio through other Bluetooth speaker

    void handleBluetoothControl(); // handle Bluetooth control

    // getters
    bool isConnected() const; // returns true if some a2dp device is connected
    const char* getDeviceName() const; // returns the device name that is connected

    // tasks
    static void handleBluetoothControlTask(void *pvParameters);
    void startHandleBluetoothControlTask();

private:
    static void connectionStateCallback(bool connected); // callback for connection state

    bool connected;
    Audio audio; // to play audio on MAX98357A amplifier
    BluetoothA2DPSink btAudioSink; // sink for bluetooth speaker output
    BluetoothA2DPSource btAudioSource; // source for bluetooth audio input
    I2SStream i2s;
    const char* deviceName; // Bluetoothinatorul by default
};

#endif