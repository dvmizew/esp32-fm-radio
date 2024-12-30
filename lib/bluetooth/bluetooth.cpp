#include "bluetooth.h"

BluetoothHandle::BluetoothHandle() 
    : btAudioSink(i2s), 
      btAudioSource(), 
      connected(false), 
      deviceName(DEVICE_NAME) {}

void BluetoothHandle::initializeBluetoothSpeaker() {
    Serial.begin(115200);

    audio.setPinout(AMP_BLCK, AMP_LRC, AMP_DIN);
    audio.setVolume(100);

    // getting metadata from A2DP source
    btAudioSink.set_avrc_metadata_callback([](uint8_t id, const uint8_t *value) {
        if (value == nullptr || strlen((const char *)value) == 0) {
            return;
        }
        
        if (id == ESP_AVRC_MD_ATTR_TITLE) { // title metadata
            Serial.print(F("Title: "));
            Serial.println((const char *)value);
        } else if (id == ESP_AVRC_MD_ATTR_ARTIST) { // artist metadata
            Serial.print(F("Artist: "));
            Serial.println((const char *)value);
        } else if (id == ESP_AVRC_MD_ATTR_ALBUM) { // album metadata
            Serial.print(F("Album: "));
            Serial.println((const char *)value);
        } else if (id == ESP_AVRC_MD_ATTR_NUM_TRACKS) { // number of tracks metadata
            Serial.print(F("Number of tracks: "));
            Serial.println((const char *)value);
        } else if (id == ESP_AVRC_MD_ATTR_GENRE) { // genre metadata
            Serial.print(F("Genre: "));
            Serial.println((const char *)value);
        } else if (id == ESP_AVRC_MD_ATTR_PLAYING_TIME) { // playing time metadata
            Serial.print(F("Playing time: "));
            Serial.println((const char *)value);
        } else if (id == ESP_AVRC_MD_ATTR_TRACK_NUM) { // track number metadata
            Serial.print(F("Track number: "));
            Serial.println((const char *)value);
        }
    });

    btAudioSink.set_avrc_connection_state_callback(BluetoothHandle::connectionStateCallback);
    
    // btAudioSink.set_auto_reconnect(true, 1000); // auto reconnect after 1 second
    btAudioSink.set_pin_config(amp_config); // pin config for MAX98357A amplifier
    btAudioSink.start(deviceName); // start Bluetooth sink with device name
    Serial.println(F("Bluetoothinatorul is now discoverable"));
}

void BluetoothHandle::playThroughBluetoothSpeaker() {
    // btAudioSource.start("Bluetoothinatorul");
    // btAudioSource.write_data();
}

void BluetoothHandle::handleBluetoothControl() {
    // avrcp commands using the joystick
    JoystickHandle joystick;
    joystick.setupJoystick();

    while (true) {
        joystick.readJoystick();

        int x, y;
        uint8_t sw; // switch
        joystick.getJoystickValues(x, y, sw);

        // Adjust the threshold values based on the test results
        const int thresholdLow = 1000;
        const int thresholdHigh = 3000;

        if (sw == LOW) {
            // pause/play
            btAudioSink.pause();
            Serial.println(F("Pause/Play"));
        }
        if (x < thresholdLow) {
            // previous track
            btAudioSink.previous();
            Serial.println(F("Previous track"));
        } else if (x > thresholdHigh) {
            // next track
            btAudioSink.next();
            Serial.println(F("Next track"));
        }
        vTaskDelay(100 / portTICK_PERIOD_MS); // 100 ms delay
    }
}

void BluetoothHandle::handleBluetoothControlTask(void *pvParameters) {
    BluetoothHandle *bluetoothHandle = static_cast<BluetoothHandle*>(pvParameters);
    while (true) {
        bluetoothHandle->handleBluetoothControl();
    }
    vTaskDelete(NULL);
}

void BluetoothHandle::startHandleBluetoothControlTask() {
    xTaskCreate(&BluetoothHandle::handleBluetoothControlTask, "BluetoothControlTask", 2048, this, 1, NULL);
}

// getters
bool BluetoothHandle::isConnected() const {
    return connected;
}

const char* BluetoothHandle::getDeviceName() const {
    return deviceName; // return this device's name
}

void BluetoothHandle::connectionStateCallback(bool connected) {
    pinMode(INTERNAL_LED, OUTPUT);
    if (connected) {
        Serial.println(F("Connected to A2DP source"));
        digitalWrite(INTERNAL_LED, HIGH);
    } else {
        Serial.println(F("Disconnected from A2DP source"));
        digitalWrite(INTERNAL_LED, LOW);
    }
}