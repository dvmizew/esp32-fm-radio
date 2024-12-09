#include "bluetooth.h"
#include <BluetoothA2DPSource.h> // for transmitting audio and metadata to A2DP sink
#include <BluetoothA2DPSink.h> // for receiving audio and metadata from A2DP source

void initializeBluetoothSpeaker() {
    // this function makes ESP32 an A2DP sink (Bluetooth speaker)
    I2SStream i2s; // I2S Stream to write audio data to I2S bus
    BluetoothA2DPSink btAudioSink(i2s); // Bluetooth Sink to read audio data from A2DP source
    
    // set up I2S bus for MAX98357A amplifier
    auto config = i2s.defaultConfig(); // get default I2S config
    config.pin_bck = AMP_SCK; // set BCK pin (bit clock)
    config.pin_ws = AMP_CS; // set LRCK pin (word select)
    config.pin_data = AMP_SDA; // set DATA pin

    i2s.begin(config); // start I2S stream

    // get metadata from A2DP source and print it
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

    // control A2DP source using AVRCP
    // TODO

    btAudioSink.start("Bluetoothinatorul");
}

void playThroughBluetoothSpeaker() {
    // this function plays audio data through external Bluetooth speaker
    // TODO
    BluetoothA2DPSource btAudioSource; // Bluetooth Source to write audio data to A2DP sink
}