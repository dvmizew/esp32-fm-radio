#include "radioHandle.h"

static char currentStation[10] = "107.5 FM";
static float frequency = 107.5;
static radioStation stations[MAX_RADIO_STATIONS]; // array of radio stations found during search
static TEA5767 radio;
static TaskHandle_t radioControlTaskHandle = NULL;

void setupRadio() {
    // start I2C bus with radio pins
    Wire.begin(TEA5767_SDA, TEA5767_SCL); // start I2C bus with radio pins
}

void initRadio() {
    // start radio module with default settings
    setupRadio();
    radio.setFrequency(frequency);  // set initial frequency to 107.5 FM
    Serial.println(F("Radio initialized!"));
    Serial.println(F("Current station:"));
    Serial.println(frequency);
}

void enableRadio() {
    setupRadio();
    initRadio();
    radio.setMuted(false);
    radio.setStandby(false);
    if (radioControlTaskHandle == NULL) {
        xTaskCreate(handleRadioControlTask, "RadioControlTask", 2048, NULL, 1, &radioControlTaskHandle);
    }
}

void disableRadio() {
    radio.setMuted(true);
    radio.setFrequency(0.0);
    radio.setStandby(true);
}

void signalStrengthLED() {
    pinMode(INTERNAL_LED, OUTPUT);
    while (true) {
        // this function lights up the LED based on the signal strength
        int signalLevel = radio.getSignalLevel();
        int brightness = 0;

        if (signalLevel == 0) {
            // no signal
            brightness = 0;
        } else if (signalLevel == 1) {
            // weak signal
            brightness = 128; // 50% brightness
        } else {
            // strong signal
            brightness = 255; // 100% brightness
        }
        analogWrite(INTERNAL_LED, brightness);
        delay(100);
    }
}

void handleRadioControl() {
    setupButtons();
    unsigned long nextButtonPressTime = 0;
    unsigned long prevButtonPressTime = 0;
    bool nextButtonHeld = false;
    bool prevButtonHeld = false;
    bool radioEnabled = true;

    while (true) {
        if (digitalRead(PLAY_BUTTON) == LOW) {
            if (radioEnabled) {
                disableRadio();
            } else {
                enableRadio();
            }
            radioEnabled = !radioEnabled;
            vTaskDelay(pdMS_TO_TICKS(BUTTON_DEBOUNCE_DELAY));
        }
        handleButtonPress(NEXT_BUTTON, nextButtonHeld, nextButtonPressTime, increaseRadioFrequency, volumeUp);
        handleButtonPress(PREV_BUTTON, prevButtonHeld, prevButtonPressTime, decreaseRadioFrequency, volumeDown);
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void handleRadioControlTask(void *parameter) {
    handleRadioControl();
    vTaskDelete(NULL); // delete the task when done
}

void increaseRadioFrequency() {
    frequency += 0.1;
    if (frequency > 108.0) {
        frequency = 87.5;
    }
    radio.setFrequency(frequency);
    snprintf(currentStation, sizeof(currentStation), "%.1f FM", frequency);
    Serial.printf_P(PSTR("Frequency increased to: %.1f FM\n"), frequency);
}

void decreaseRadioFrequency() {
    frequency -= 0.1;
    if (frequency < 87.5) {
        frequency = 108.0;
    }
    radio.setFrequency(frequency);
    snprintf(currentStation, sizeof(currentStation), "%.1f FM", frequency);
    Serial.printf_P(PSTR("Frequency decreased to: %.1f FM\n"), frequency);
}

void setRadioFrequency(float freq) {
    if (freq >= 87.5 && freq <= 108.0) {
        frequency = freq;
        radio.setFrequency(frequency);
        snprintf(currentStation, sizeof(currentStation), "%.1f FM", frequency);
        Serial.printf_P(PSTR("Frequency set to: %.1f FM\n"), frequency);
    } else {
        Serial.println(F("Invalid frequency. Please enter a value between 87.5 and 108.0 MHz."));
    }
}

void searchRadioStations() {
    int foundStations = radio.init(3); // search for radio stations with a signal level of 3
    int stationCount = 0;

    Serial.println(F("Searching for radio stations..."));
    for (int i = 0; i < foundStations && stationCount < MAX_RADIO_STATIONS; i++) {
        float station = radio.nextStation();
        if (station == 0.0) {
            break;
        }

        stations[stationCount].frequency = station;
        Serial.printf_P(PSTR("Station %d: %.1f FM\n"), stationCount + 1, station);
        stationCount++;
    }
}

void printRadioStations() {
    Serial.println(F("Radio stations:"));
    for (int i = 0; i < MAX_RADIO_STATIONS; ++i) {
        Serial.printf("Station %d: %.1f FM\n", i + 1, stations[i].frequency);
    }
}

int32_t get_sound_data(Frame *data, int32_t frameCount) {
    // this function is supposed to get sound data from TEA5767 using I2S
    // TODO
    return frameCount;
}

void passAudioToBluetooth() {
    // this function is supposed to pass audio data from get_sound_data to the Bluetooth speaker
    BluetoothA2DPSource bluetoothRadioSource;
    bluetoothRadioSource.start(DEVICE_NAME);
}

// getters
const char* getCurrentStation() {
    return currentStation;
}

float getFrequency() {
    return frequency;
}

short getSignalLevel() {
    return radio.getSignalLevel();
}