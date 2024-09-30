#ifndef _sensor_h
#define _sensor_h

#include "at91sam3x8.h"
#include "system_sam3x.h"
#include "core_cm3.h"

void start_temp();
void tempReady();
double readTemp();
void writeTemp();
void readLight(void);
void Init_lightsensor(void);

extern int tempVal;
extern int tempCounter;
#endif