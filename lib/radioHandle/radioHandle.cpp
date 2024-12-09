#include <Wire.h>
#include <TEA5767.h>
#include "radioHandle.h"
#include "hardware.h"

TEA5767 radio;
char currentStation[16] = "87.5 FM";
float frequency = 87.5;

void setupRadio() {
    Wire.begin(TEA5767_SDA, TEA5767_SCL); // start I2C bus with radio pins
    Wire.setClock(100000); // radio clock
}

void initRadio() {
    setupRadio();
    radio.init();
    radio.setFrequency(frequency);  // Set initial frequency to 87.5 FM
    Serial.println(F("Radio initialized!"));
}

void tuneRadio(float freq) {
    if (freq >= 87.5 && freq <= 108.0) {
        radio.setFrequency(freq);  // Set the radio to the desired frequency
        snprintf(currentStation, sizeof(currentStation), "%.1f FM", freq);
    } 
    else {
        Serial.println(F("Invalid frequency!"));
    }
}

void handleRadioControl() {
    // TODO
}