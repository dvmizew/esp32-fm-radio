#include "bluetooth.h"

I2SStream i2s; // for configuring the I2S stream for the amplifier
BluetoothA2DPSink btAudioSink(i2s); // use ESP32 as a speaker using the I2S stream
bool connected = false;
bool bluetoothSpeakerInitialized = false;
const char* deviceName = DEVICE_NAME;

TaskHandle_t bluetoothTaskHandle = nullptr;

// global variables to store metadata
String currentTitle = "";
String currentArtist = "";
String currentAlbum = "";

void initializeBluetoothSpeaker() {
    if (!bluetoothSpeakerInitialized) {
        Serial.println(F("Initializing Bluetooth speaker..."));

        Serial.printf(PSTR("Heap before Bluetooth init: %u\n"), ESP.getFreeHeap());

        auto cfg = i2s.defaultConfig();
        cfg.pin_bck = AMP_BLCK;
        cfg.pin_ws = AMP_LRC;
        cfg.pin_data = AMP_DIN;
        cfg.bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT;
        cfg.channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT;
        cfg.use_apll = true;
        i2s.begin(cfg);

        Serial.println(F("Starting Bluetooth audio sink..."));
        btAudioSink.start(deviceName); // start Bluetooth audio sink and make it discoverable

        // metadata callback
        btAudioSink.set_avrc_metadata_callback([](uint8_t id, const uint8_t *value) {
            if (value != nullptr && strlen((const char *)value)) {
                switch (id) {
                    case ESP_AVRC_MD_ATTR_TITLE:
                        currentTitle = (const char *)value;
                        Serial.printf_P(PSTR("Title: %s\n"), value);
                        break;
                    case ESP_AVRC_MD_ATTR_ARTIST:
                        currentArtist = (const char *)value;
                        Serial.printf_P(PSTR("Artist: %s\n"), value);
                        break;
                    case ESP_AVRC_MD_ATTR_ALBUM:
                        currentAlbum = (const char *)value;
                        Serial.printf_P(PSTR("Album: %s\n"), value);
                        break;
                    default:
                        break;
                }
            } else {
                Serial.println(F("Metadata value is null or empty"));
            }
        });

        btAudioSink.set_avrc_connection_state_callback(connectionStateCallback);
        btAudioSink.set_auto_reconnect(true, 1000); // auto reconnect after 1 second
        Serial.printf_P(PSTR("%s is now discoverable\n"), DEVICE_NAME);
        bluetoothSpeakerInitialized = true;

        // start Bluetooth control and display task
        startBluetoothTask();
        Serial.printf("Heap after Bluetooth init: %u\n", ESP.getFreeHeap());
    } else {
        Serial.println(F("Bluetooth speaker already initialized"));
    }
}

void startBluetoothTask() {
    // there i start the bluetooth task which will handle the bluetooth controls and display the metadata on the OLED display
    if (bluetoothTaskHandle == nullptr) {
        xTaskCreatePinnedToCore(
            bluetoothTask,
            "BluetoothTask",
            4096,
            nullptr,
            1,
            &bluetoothTaskHandle,
            1 // assign to core 1
        );
    }
}

void bluetoothTask(void *pvParameters) {
    setupButtons();
    unsigned long nextButtonPressTime = 0;
    unsigned long prevButtonPressTime = 0;
    bool nextButtonHeld = false;
    bool prevButtonHeld = false;

    while (true) {
        // bluetooth button controls
        if (digitalRead(PLAY_BUTTON) == LOW) {
            togglePlayback();
            vTaskDelay(pdMS_TO_TICKS(BUTTON_DEBOUNCE_DELAY));
        }
        handleButtonPress(NEXT_BUTTON, nextButtonHeld, nextButtonPressTime, playNextTrack, volumeUp);
        handleButtonPress(PREV_BUTTON, prevButtonHeld, prevButtonPressTime, playPreviousTrack, volumeDown);

        // display avrc metadata on the OLED display
        if (isDisplayInitialized()) {
            clearAndUpdate();
            display.printf("%s\n\n", getBluetoothDeviceName());
            display.printf("Connected: %s\n", bluetoothIsConnected() ? "Yes" : "No");
            display.printf("Title: %s\n", currentTitle.c_str());
            display.printf("Artist: %s\n", currentArtist.c_str());
            display.printf("Album: %s\n", currentAlbum.c_str());
            display.display();
        }

        vTaskDelay(pdMS_TO_TICKS(1000)); // update display every second
    }
}

void deinitializeBluetoothSpeaker() {
    if (bluetoothSpeakerInitialized) {
        Serial.println(F("Stopping Bluetooth audio sink..."));

        if (bluetoothTaskHandle != nullptr) {
            vTaskDelete(bluetoothTaskHandle);
            bluetoothTaskHandle = nullptr;
        }

        btAudioSink.end();
        Serial.println(F("Stopping I2S..."));
        i2s.end();

        esp_bt_controller_disable(); // reset the Bluetooth stack
        esp_bt_controller_enable(ESP_BT_MODE_CLASSIC_BT);

        bluetoothSpeakerInitialized = false;

        Serial.printf("Heap after Bluetooth deinit: %u\n", ESP.getFreeHeap());
    } else {
        Serial.println(F("Bluetooth speaker was not initialized"));
    }
}

void disconnectBluetoothSinkDevice() {
    btAudioSink.disconnect();
}

void disableBluetoothSpeaker() {
    disconnectBluetoothSinkDevice();
    deinitializeBluetoothSpeaker();
}

void playNextTrack() {
    btAudioSink.next();
}

void playPreviousTrack() {
    btAudioSink.previous();
}

void togglePlayback() {
    esp_a2d_audio_state_t audioState = btAudioSink.get_audio_state();
    if (audioState == ESP_A2D_AUDIO_STATE_STOPPED || audioState == ESP_A2D_AUDIO_STATE_REMOTE_SUSPEND)
        btAudioSink.play();
    else if (audioState == ESP_A2D_AUDIO_STATE_STARTED)
        btAudioSink.pause();
}

void volumeUp() {
    int volume = btAudioSink.get_volume();
    if (volume < 100) {
        volume += VOL_STEP;
        volumeChange(volume);
    }
}

void volumeDown() {
    int volume = btAudioSink.get_volume();
    if (volume > 0) {
        volume -= VOL_STEP;
        volumeChange(volume);
    }
}

void volumeChange(int volume) {
    // volume range is 0-100
    btAudioSink.set_volume(volume);
}

void handleBluetoothControl() {
    setupButtons();
    unsigned long nextButtonPressTime = 0;
    unsigned long prevButtonPressTime = 0;
    bool nextButtonHeld = false;
    bool prevButtonHeld = false;

    while (true) {
        if (digitalRead(PLAY_BUTTON) == LOW) {
            togglePlayback();
            vTaskDelay(pdMS_TO_TICKS(BUTTON_DEBOUNCE_DELAY));
        }
        handleButtonPress(NEXT_BUTTON, nextButtonHeld, nextButtonPressTime, playNextTrack, volumeUp);
        handleButtonPress(PREV_BUTTON, prevButtonHeld, prevButtonPressTime, playPreviousTrack, volumeDown);
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

bool bluetoothIsConnected() {
    return connected;
}

const char* getBluetoothDeviceName() {
    return deviceName; // return this device's name
}

void connectionStateCallback(bool isConnected) {
    pinMode(INTERNAL_LED, OUTPUT);
    if (isConnected) {
        Serial.println(F("Connected to A2DP source"));
        digitalWrite(INTERNAL_LED, HIGH);
        connected = true;
    } else {
        Serial.println(F("Disconnected from A2DP source"));
        digitalWrite(INTERNAL_LED, LOW);
        connected = false;
    }
}