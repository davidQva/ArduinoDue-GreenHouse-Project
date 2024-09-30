#ifndef _handlers_h
#define _handlers_h

#include "at91sam3x8.h"
#include "system_sam3x.h"
#include "core_cm3.h"
#include <stdio.h>

void Delay(int Value);
void SysTick_Handler();
void TC0_Handler(void);
void ADC_Handler(void);
void sunTracker();

extern int weekdays[5][7];

typedef struct LinkedList 
{
  unsigned int id;//epochtime is ID
  float sensorData;
  struct LinkedList *next;
}LinkedList;

extern int tempFlag;
extern unsigned int Flag;
extern unsigned int pFlag;
extern unsigned int Flag2;
extern  int tTemp;
extern  int tempStatus;
extern  int tStatus;
extern  int tSens;
extern int fullDayTime;
extern double voltOne;
extern double voltTwo;
extern int v1Flag;
extern int v2Flag;
extern double tempVolt;
extern int servoValue;
extern int sunTimer;
extern int pointPos;
#endif