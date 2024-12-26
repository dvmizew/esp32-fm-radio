#include "radioHandle.h"

RadioHandle::RadioHandle() {
    // default values (lowest frequency that TEA5767 supports)
    snprintf(currentStation, sizeof(currentStation), "87.5 FM");
    frequency = 87.5;
}

void RadioHandle::setupRadio() {
    Wire.begin(TEA5767_SDA, TEA5767_SCL); // start I2C bus with radio pins
    Wire.setClock(100000); // radio clock
    joystick.setupJoystick(); // for controlling the radio
}

void RadioHandle::initRadio() {
    setupRadio();
    radio.init();
    radio.setFrequency(frequency);  // Set initial frequency to 87.5 FM
    Serial.println(F("Radio initialized!"));
    Serial.println(F("Current station:"));
    Serial.println(frequency);
}

void RadioHandle::tuneRadio(float freq) {
    if (freq >= 87.5 && freq <= 108.0) {
        radio.setFrequency(freq);  // Set the radio to the desired frequency
        snprintf(currentStation, sizeof(currentStation), "%.1f FM", freq);
    } else {
        Serial.println(F("Invalid frequency!"));
    }
}

void RadioHandle::searchRadioStations() {
    for (float i = 87.5; i <= 108.0; i += 0.1) {
        radio.setFrequency(i);
        Serial.print(F("Searching station at "));
        Serial.print(i);
        tuneRadio(i); // iterate through all frequencies
        delay(1000);
    }
}

void RadioHandle::handleRadioControl() {
    // THIS FUNCTION DOESN'T WORK YET
    joystick.readJoystick();
    
    int x, y, sw;
    joystick.getJoystickValues(x, y, sw);
    
    if (sw == HIGH) {
        // toggle mute
        radio.setMute(!radio.getMute());
    }

    // prevent accidental tuning
    const int deadZone = 100;

    if (x < (512 - deadZone)) {
        // tune down
        if (frequency > 87.5) {
            frequency -= 0.1;
            tuneRadio(frequency);
            Serial.println(F("Tuning down..."));
            Serial.printf("Frequency: %.1f FM\n", frequency);
        }
    } else if (x > (512 + deadZone)) {
        // tune up
        if (frequency < 108.0) {
            frequency += 0.1;
            tuneRadio(frequency);
            Serial.println(F("Tuning up..."));
            Serial.printf("Frequency: %.1f FM\n", frequency);
        }
    }
}

int32_t RadioHandle::get_sound_data(Frame *data, int32_t frameCount) {
    // this function is supposed to get sound data from TEA5767 using I2S
    // TODO
    return frameCount;
}

void RadioHandle::passAudioToBluetooth() {
    // this function is supposed to pass audio data from get_sound_data to the Bluetooth speaker
    bluetoothRadioSource.start("Radio");
}

// getters
const char* RadioHandle::getCurrentStation() const {
    return currentStation;
}

float RadioHandle::getFrequency() const {
    return frequency;
}