#include "bluetooth.h"
#include <BluetoothA2DPSource.h>

BluetoothA2DPSource btAudio;

void setupBluetooth() {
    btAudio.start("ESP32 Radio");
}

void bluetoothSpeaker() {
    // TODO
}

void playThroughSpeaker() {
    // TODO
}