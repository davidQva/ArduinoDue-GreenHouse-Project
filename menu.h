#ifndef _menu_h
#define _menu_h

#include "system_sam3x.h"
#include "at91sam3x8.h"
#include "core_cm3.h"

void mainMenu();
void settingsMenu();
void speedModeButton();
void logMenu();

extern char menu[];
extern char date[];
extern char time[];
extern char temp[];
extern char pSun[];
extern char settingsB[];
extern char templogB[];
extern char setDate[];
extern int tempMinLimit;
extern int tempMaxLimit;
extern int tempMonth;
extern int tempYear;
extern int tempDay;
#endif