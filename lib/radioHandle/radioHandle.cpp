#include "radioHandle.h"

RadioHandle::RadioHandle() {
    // default values (known radio station Europa FM (pe aceeasi frecventa cu tine))
    snprintf(currentStation, sizeof(currentStation), "107.5 FM");
    frequency = 107.5;
}

void RadioHandle::setupRadio() {
    Wire.begin(TEA5767_SDA, TEA5767_SCL); // start I2C bus with radio pins
    // joystick.setupJoystick(); // for controlling the radio
}

void RadioHandle::initRadio() {
    setupRadio();
    radio.setFrequency(frequency);  // set initial frequency to 107.5 FM
    Serial.println(F("Radio initialized!"));
    Serial.println(F("Current station:"));
    Serial.println(frequency);
}

void RadioHandle::signalStrengthLED() {
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

void RadioHandle::increaseFrequency() {
    frequency += 0.1;
    if (frequency > 108.0) {
        frequency = 87.5;
    }
    radio.setFrequency(frequency);
    snprintf(currentStation, sizeof(currentStation), "%.1f FM", frequency);
    Serial.printf("Frequency increased to: %.1f FM\n", frequency);
}

void RadioHandle::decreaseFrequency() {
    frequency -= 0.1;
    if (frequency < 87.5) {
        frequency = 108.0;
    }
    radio.setFrequency(frequency);
    snprintf(currentStation, sizeof(currentStation), "%.1f FM", frequency);
    Serial.printf("Frequency decreased to: %.1f FM\n", frequency);
}

void RadioHandle::setFrequency(float freq) {
    if (freq >= 87.5 && freq <= 108.0) {
        frequency = freq;
        radio.setFrequency(frequency);
        snprintf(currentStation, sizeof(currentStation), "%.1f FM", frequency);
        Serial.printf("Frequency set to: %.1f FM\n", frequency);
    } else {
        Serial.println(F("Invalid frequency. Please enter a value between 87.5 and 108.0 MHz."));
    }
}

void RadioHandle::searchRadioStations() {
    int foundStations = radio.init(3);
    int stationCount = 0;

    Serial.println(F("Searching for radio stations..."));
    for (int i = 0; i < foundStations && stationCount < MAX_RADIO_STATIONS; i++) {
        float station = radio.nextStation();
        if (station == 0.0) {
            break;
        }

        stations[stationCount].frequency = station;
        Serial.printf("Station %d: %.1f FM\n", stationCount + 1, station);
        stationCount++;
    }
}

void RadioHandle::printRadioStations() {
    Serial.println(F("Radio stations:"));
    for (int i = 0; i < MAX_RADIO_STATIONS; ++i) {
        Serial.printf("Station %d: %.1f FM\n", i + 1, stations[i].frequency);
    }
}

void RadioHandle::joystickRadioControl() {
    // THIS FUNCTION DOESN'T WORK YET
    JoystickHandle joystick;
    joystick.setupJoystick();
    joystick.readJoystick();

    int x, y;
    uint8_t sw; // switch
    joystick.getJoystickValues(x, y, sw);

    if (sw == HIGH) {
        // toggle mute
        radio.setMuted(!radio.isMuted());
    }

    // prevent accidental tuning
    const int deadZone = 100;

    if (x < (512 - deadZone)) {
        // tune down
        Serial.println(F("Tuning down"));
        radio.previousStation();
    } else if (x > (512 + deadZone)) {
        // tune up
        Serial.println(F("Tuning up"));
        radio.nextStation();
    }
}

int32_t RadioHandle::get_sound_data(Frame *data, int32_t frameCount) {
    // this function is supposed to get sound data from TEA5767 using I2S
    // TODO
    return frameCount;
}

void RadioHandle::passAudioToBluetooth() {
    // this function is supposed to pass audio data from get_sound_data to the Bluetooth speaker
    BluetoothA2DPSource bluetoothRadioSource;
    bluetoothRadioSource.start("Radio");
}

// getters
const char* RadioHandle::getCurrentStation() const {
    return currentStation;
}

float RadioHandle::getFrequency() const {
    return frequency;
}

short RadioHandle::getSignalLevel() const {
    return getSignalLevel();
}