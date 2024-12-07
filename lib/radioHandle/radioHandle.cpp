#include <TEA5767.h>
#include "radioHandle.h"
#include "hardware.h"

TEA5767 radio;
char currentStation[16] = "87.5 FM";
float frequency = 87.5;
int volume = 0;

void initRadio() {
    radio.init();
    radio.setFrequency(87.5);
}

void tuneRadio(float frequency) {
    if (frequency >= 87.5 && frequency <= 108.0) {
        radio.setFrequency(frequency);
        snprintf(currentStation, sizeof(currentStation), "%.1f FM", frequency);
        updateDisplay();
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
    if (digitalRead(JOYSTICK_Y) == LOW) {
        if (digitalRead(JOYSTICK_X) == HIGH)
            adjustVolume(true);
        else
            adjustVolume(false);
    }
}