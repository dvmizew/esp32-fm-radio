#ifndef RADIO_H
#define RADIO_H

extern char currentStation[16];
extern float frequency;

void setupRadio();
void initRadio();
void tuneRadio(float frequency);
void handleRadioControl();

#endif