#ifndef _keypad_h
#define _keypad_h

#include "at91sam3x8.h"
#include "system_sam3x.h"
#include "core_cm3.h"

void Print_Nr_From_Keypad(void);

int keypad(void);
extern int value;
extern int col;
extern int row;
extern int z;
#endif