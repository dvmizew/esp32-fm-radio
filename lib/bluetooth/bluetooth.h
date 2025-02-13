#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include "hardware.h"
#include "displayHandle.h" // for displaying avrc metadata on the OLED display
#include <Wire.h>
// #include <AudioTools.h>
#include <BluetoothA2DPSink.h>
#include <BluetoothA2DPSource.h>
// #include <Audio.h>

#define VOL_STEP 5

// global variables
extern I2SStream i2s; // for configuring the I2S stream for the amplifier
extern BluetoothA2DPSink btAudioSink; // use ESP32 as speaker
// extern BluetoothA2DPSource btAudioSource; // play through an external speaker
// extern Audio audio; // for tone control (bass, mid-range, treble)
// also used for the old way of playing audio through the amplifier, which crashes after a while
extern bool connected;
extern bool bluetoothSpeakerInitialized;
extern const char* deviceName;

extern String currentTitle;
extern String currentArtist;
extern String currentAlbum;

// function prototypes
void initializeBluetoothSpeaker(); // ESP32 as A2DP sink (external speaker)
void deinitializeBluetoothSpeaker(); // deinitialize the Bluetooth speaker
void disconnectBluetoothSinkDevice(); // disconnect the device from the sink
void disableBluetoothSpeaker(); // disable the Bluetooth speaker
void playNextTrack(); // play the next track
void playPreviousTrack(); // play the previous track
void togglePlayback(); // pause or play the track
void volumeUp(); // increase the volume
void volumeDown(); // decrease the volume
void volumeChange(int volume); // change the volume
void startBluetoothTask(); // Start the Bluetooth control and display task
void bluetoothTask(void *pvParameters); // Task function for Bluetooth control and display

// getters
bool bluetoothIsConnected(); // returns true if some a2dp device is connected
const char* getBluetoothDeviceName(); // returns the device name that is connected
void connectionStateCallback(bool isConnected); // callback for connection state

#endif