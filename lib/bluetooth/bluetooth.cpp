#include "bluetooth.h"

BluetoothHandle::BluetoothHandle() 
    : btAudioSink(i2s), 
      btAudioSource(), 
      connected(false), 
      deviceName("Bluetoothinatorul") {}

void BluetoothHandle::initializeBluetoothSpeaker() {
    Serial.begin(115200);

    audio.setPinout(AMP_BLCK, AMP_LRC, AMP_DIN);
    audio.setVolume(100);

    // getting metadata from A2DP source
    btAudioSink.set_avrc_metadata_callback([](uint8_t id, const uint8_t *value) {
        if (id == ESP_AVRC_MD_ATTR_TITLE) { // title metadata
            Serial.print("Title: ");
            Serial.println((const char *)value);
        } else if (id == ESP_AVRC_MD_ATTR_ARTIST) { // artist metadata
            Serial.print("Artist: ");
            Serial.println((const char *)value);
        } else if (id == ESP_AVRC_MD_ATTR_ALBUM) { // album metadata
            Serial.print("Album: ");
            Serial.println((const char *)value);
        }
    });

    btAudioSink.set_avrc_connection_state_callback(BluetoothHandle::connectionStateCallback);
    
    // btAudioSink.set_auto_reconnect(true, 1000); // auto reconnect after 1 second
    btAudioSink.set_pin_config(amp_config); // pin config for MAX98357A amplifier
    btAudioSink.start(deviceName); // start Bluetooth sink with device name
    Serial.println("Bluetoothinatorul is now discoverable");
}

void BluetoothHandle::playThroughBluetoothSpeaker() {
    // btAudioSource.start("Bluetoothinatorul");
    // btAudioSource.write_data();
}

void BluetoothHandle::handleBluetoothControl() {
   // TODO
}

// getters
bool BluetoothHandle::isConnected() const {
    return connected;
}

const char* BluetoothHandle::getDeviceName() const {
    return deviceName; // return this device's name
}

void BluetoothHandle::connectionStateCallback(bool connected) {
    if (connected) {
        Serial.println("Connected to A2DP source");
    } else {
        Serial.println("Disconnected from A2DP source");
    }
}