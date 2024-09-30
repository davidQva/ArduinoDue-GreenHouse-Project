#ifndef _display_h
#define _display_h

#include "at91sam3x8.h"
#include "system_sam3x.h"
#include "core_cm3.h"

void Write_Command_2_Display(unsigned char Command);
void Write_Data_2_Display(unsigned char Data);
unsigned char Read_Status_Display(void);
void setAPointer(int x, int y);
void Clear_Display(void);
void Init_Display(void);
void writeTime();
void writeDate();
void print();
void writeSunPos();
void writeLogTimeStamp();

extern short int lowAA[16];
#endif