#ifndef _config_h
#define _config_h

#include "at91sam3x8.h"
#include "system_sam3x.h"
#include "core_cm3.h"

void config();
void skalbagge_LCD_DIR(int x);
void skalbagge_Kpad(int x);
void skalbagge_LCD(int x);
void LCD_RESET(int x);
void BUS_R_W(int x);
void LCD_WR(int x);
void LCD_RD(int x);
void LCD_CE(int x);
void LCD_CD(int x);
void tempSensorInit(void);
void Init_Servo();
#endif