#ifndef RADIO_H
#define RADIO_H

#include "displayHandle.h"

#define MAXVOLUME 10

extern int volume;
extern char currentStation[16];
extern float frequency;

void initRadio();
void tuneRadio(float frequency);
void handleRadioControl();
void adjustVolume(bool increase);

#endif