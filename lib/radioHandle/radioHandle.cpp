#include "radioHandle.h"

TEA5767 radio;
String currentStation = "87.5 FM";

int volume = 0; // initial volume

void initRadio() {
    radio.init();
    radio.setFrequency(87.5);
}

void tuneRadio(float frequency) {
    radio.setFrequency(frequency);
    currentStation = String(frequency) + " FM";
    updateDisplay();
}

void handleRadioControl() {
    if (digitalRead(JOYSTICK_Y) == LOW) {
        if (digitalRead(JOYSTICK_X) == HIGH) {
            volume++;
            radio.setVolume(volume);
            updateDisplay();
        } 
        else {
            volume--;
            if (volume < 0) volume = 0;
            radio.setVolume(volume);
            updateDisplay();
        }
    }
}
