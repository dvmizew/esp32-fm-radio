#ifndef RADIO_H
#define RADIO_H

#include "displayHandle.h"

#define MAXVOLUME 10

extern int volume;
extern String currentStation;
void initRadio();
void tuneRadio(float frequency);
void handleRadioControl();

#endif