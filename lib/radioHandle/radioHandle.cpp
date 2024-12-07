#include <TEA5767.h>
#include "radioHandle.h"
#include "hardware.h"

TEA5767 radio;
String currentStation = "87.5 FM";
int volume = 0;

void initRadio() {
    radio.init();
    radio.setFrequency(87.5);
}

void tuneRadio(float frequency) {
    if (frequency >= 87.5 && frequency <= 108.0) {  // TEA5767 frequency range
        radio.setFrequency(frequency);
        currentStation = String(frequency) + " FM";
        updateDisplay();
    } else {
        Serial.println("Invalid frequency!");
    }
}

void handleRadioControl() {
    if (digitalRead(JOYSTICK_Y) == LOW) {
        Serial.println("Joystick button pressed!");
        if (digitalRead(JOYSTICK_X) == HIGH) {
            volume++;
            if (volume > MAXVOLUME) 
                volume = MAXVOLUME;
            Serial.print("Volume increased: ");
            Serial.println(volume);
        } 
        else { 
            if (volume > 0) 
                volume--;
            Serial.print("Volume decreased: ");
            Serial.println(volume);
        }
        updateDisplay();
    }
}
