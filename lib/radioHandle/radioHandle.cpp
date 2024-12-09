#include <TEA5767.h>
#include "radioHandle.h"
#include "hardware.h"

extern TEA5767 radio;
char currentStation[16] = "87.5 FM";
float frequency = 87.5;
int volume = 0;

unsigned long lastSeekTime = 0;
unsigned long seekDelay = 1000;

void initRadio() {
    radio.init();  // Initialize the radio with the I2C bus
    radio.setFrequency(frequency);  // Set initial frequency to 87.5 FM
    Serial.println(F("Radio initialized!"));
}

void tuneRadio(float freq) {
    if (freq >= 87.5 && freq <= 108.0) {
        radio.setFrequency(freq);  // Set the radio to the desired frequency
        snprintf(currentStation, sizeof(currentStation), "%.1f FM", freq);
        updateDisplay();  // Update the OLED display with the current station
    } else {
        Serial.println(F("Invalid frequency!"));
    }
}

void adjustVolume(bool increase) {
    if (increase) {
        volume++;
        Serial.print(F("Volume: "));
        Serial.println(volume);
        if (volume > MAXVOLUME) 
            volume = MAXVOLUME;
    } 
    else {
        if (volume > 0) 
            Serial.print(F("Volume: "));
            Serial.println(volume);
            volume--;
    }
    updateDisplay();
}

void handleRadioControl() {
    // volume control
    static unsigned long lastVolumeChange = 0;
    if (digitalRead(JOYSTICK_Y) == LOW && (millis() - lastVolumeChange > debounceDelay)) {
        lastVolumeChange = millis();
        if (digitalRead(JOYSTICK_X) == HIGH)
            adjustVolume(true);  // Increase volume
        else
            adjustVolume(false);  // Decrease volume
    }

    // frequency control
    static unsigned long lastFrequencyChange = 0;
    if (digitalRead(JOYSTICK_X) == LOW && (millis() - lastFrequencyChange > debounceDelay)) {
        lastFrequencyChange = millis();
        if (digitalRead(JOYSTICK_Y) == HIGH)
            tuneRadio(frequency + 0.1);  // increase frequency
        else
            tuneRadio(frequency - 0.1);  // decrease frequency
    }

    // seek up
    if (digitalRead(JOYSTICK_SW) == LOW && (millis() - lastSeekTime > seekDelay)) {
        radio.seekUp();
        lastSeekTime = millis();
        updateDisplay();
    }
}