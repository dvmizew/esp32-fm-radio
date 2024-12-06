#ifndef RADIO_H
#define RADIO_H

#include <TEA5767.h>
#include <displayHandle.h>
#include <hardware.h>

extern int volume;
extern String currentStation;
void initRadio();
void tuneRadio(float frequency);
void handleRadioControl();

#endif