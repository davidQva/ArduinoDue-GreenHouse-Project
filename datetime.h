#ifndef _datetime_h
#define _datetime_h

#include "at91sam3x8.h"
#include "system_sam3x.h"
#include "core_cm3.h"

void clockWeekday();
int currentYear();
int isLeap(int x);
void monthDay();
void yearMonth(int y);
void leapyearMonth(int y);
void epochTimeConvert();

extern unsigned long milliseconds;
extern unsigned long epochTime; 
extern unsigned long epochConvert;
extern unsigned int minutes;
extern unsigned int hours;
extern int secondsIntoDay;
extern int daysSinceEpoch;
extern int weekday;
extern int DOY;
extern int DOM;
extern int dayMonth;
extern int leapdays;
extern int year;
extern int month;
extern int seconds;
extern int y;
extern int leap;
#endif