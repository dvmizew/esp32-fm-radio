#ifndef RADIOHANDLE_H
#define RADIOHANDLE_H

#include <Wire.h>
#include <TEA5767.h>

class RadioHandle {
public:
    RadioHandle();
    
    // methods
    void setupRadio(); // setup radio pins
    void initRadio();
    void tuneRadio(float freq);
    void searchRadioStations();
    void handleRadioControl();

    // getters
    const char* getCurrentStation() const;
    float getFrequency() const;

private:
    TEA5767 radio;
    char currentStation[16];
    float frequency;
};

#endif