#include "bluetooth.h"

BluetoothHandle::BluetoothHandle() 
    : btAudioSink(i2s), 
      btAudioSource(), 
      connected(false), 
      deviceName("Bluetoothinatorul") {}

void BluetoothHandle::initializeBluetoothSpeaker() {
    // configure I2S for MAX98357A amplifier
    auto config = i2s.defaultConfig(); // get default I2S configuration
    config.pin_bck = AMP_SCK; // set BCK pin
    config.pin_ws = AMP_CS; // set LRCK pin
    config.pin_data = AMP_SDA; // set DATA pin

    i2s.begin(config); // initialize I2S with configuration
    Serial.begin(115200);
    Serial.println("I2S stream started");

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