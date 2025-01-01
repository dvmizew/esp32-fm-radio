#include "bluetooth.h"

// global variables
I2SStream i2s; // for configuring the I2S stream for the amplifier
BluetoothA2DPSink btAudioSink(i2s); // use ESP32 as speaker
// Audio audio; // for tone control
// BluetoothA2DPSource btAudioSource; // play through an external speaker
bool connected = false;
bool bluetoothSpeakerInitialized = false;
const char* deviceName = DEVICE_NAME;

void initializeBluetoothSpeaker() {
    if (!bluetoothSpeakerInitialized) {
        Serial.begin(115200);

        // old way, sounds better but crashes after a while
        // audio.setPinout(AMP_BLCK, AMP_LRC, AMP_DIN);
        // audio.setVolume(100);

        // initialize I2SStream
        auto cfg = i2s.defaultConfig(); // get the default configuration for I2S
        cfg.pin_bck = AMP_BLCK; // set the bit clock pin
        cfg.pin_ws = AMP_LRC; // set the word select pin
        cfg.pin_data = AMP_DIN; // set the data pin
        cfg.bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT; // set the bits per sample
        cfg.channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT; // set the channel format
        cfg.use_apll = false;
        i2s.begin(cfg);

        // hopefully this will make the sound better
        // set it to 0 to disable, 100 to max
        // const int8_t gainLowPass = 5;   // increase bass
        // const int8_t gainBandPass = 3;  // mid-range
        // const int8_t gainHighPass = 4;  // increase treble
        // audio.setTone(gainLowPass, gainBandPass, gainHighPass);

        btAudioSink.start(deviceName); // start the Bluetooth audio sink and now it's discoverable

        // getting metadata from A2DP source (external device)
        btAudioSink.set_avrc_metadata_callback([](uint8_t id, const uint8_t *value) {
            if (value == nullptr || strlen((const char *)value) == 0) {
                return;
            }

            switch (id) {
                case ESP_AVRC_MD_ATTR_TITLE:
                    Serial.print(F("Title: "));
                    break;
                case ESP_AVRC_MD_ATTR_ARTIST:
                    Serial.print(F("Artist: "));
                    break;
                case ESP_AVRC_MD_ATTR_ALBUM:
                    Serial.print(F("Album: "));
                    break;
                case ESP_AVRC_MD_ATTR_GENRE:
                    Serial.print(F("Genre: "));
                    break;
                case ESP_AVRC_MD_ATTR_PLAYING_TIME:
                    Serial.print(F("Playing time: "));
                    break;
                case ESP_AVRC_MD_ATTR_TRACK_NUM:
                    Serial.print(F("Track number: "));
                    break;
                default:
                    return;
            }
            Serial.println((const char *)value);
        });

        btAudioSink.set_avrc_connection_state_callback(connectionStateCallback);
        btAudioSink.set_auto_reconnect(true, 1000); // auto reconnect after 1 second
        Serial.printf_P(PSTR("%s is now discoverable\n"), DEVICE_NAME);
        bluetoothSpeakerInitialized = true;
    }
}

void deinitializeBluetoothSpeaker() {
    Serial.println(F("Stopping Bluetooth audio sink..."));
    btAudioSink.stop();
    Serial.println(F("Stopping I2S..."));
    i2s.end();
    Serial.println(F("Bluetooth speaker deinitialized"));
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
    btAudioSink.pause();
}

void volumeUp() {
    int volume = btAudioSink.get_volume();
    if (volume < 100) {
        volume += VOL_STEP;
        btAudioSink.set_volume(volume);
    }
}

void volumeDown() {
    int volume = btAudioSink.get_volume();
    if (volume > 0) {
        volume -= VOL_STEP;
        btAudioSink.set_volume(volume);
    }
}

void volumeChange(int volume) {
    // volume range is 0-100
    btAudioSink.set_volume(volume);
}

void joystickHandleBluetoothControl() {
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

void handleBluetoothControlTask(void *pvParameters) {
    while (true) {
        joystickHandleBluetoothControl();
    }
    vTaskDelete(NULL);
}

void startHandleBluetoothControlTask() {
    xTaskCreate(&handleBluetoothControlTask, "BluetoothControlTask", 2048, NULL, 1, NULL);
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