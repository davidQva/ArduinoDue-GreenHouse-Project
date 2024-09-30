#include "at91sam3x8.h"
#include "system_sam3x.h"
#include "core_cm3.h"
#include "display.h"
#include <stdlib.h>
#include "sensor.h"
#include "datetime.h"

int tempFlag;
int tTemp;
int tempStatus;
int tSens;
int tStatus;
double voltOne;
double voltTwo;
int v1Flag;
int v2Flag;
char sunLocationText[10] = {"10Azimuth:"};
int *pointPos;
int lampMinutes;
int lampHours;
int sunMinutes;
int sunHours;

double tempVolt; // temporaryVolt if it is smaller than 0.3=sun not found
int servoValue;  // deg in 3. 180/3=60. 3deg/sec
int sunTimer;    // cleared at 06:00
int lampTimer;   // how long lamp has been on /sec
int fullDayTime;
int weekdays[5][7];
// arr for 7days min[0][x],max[1][x],avg[2][x],timestamp 3=minTS, 4=maxTS

unsigned int Flag = 0x11; // Clear screen and go to mainmenu
unsigned int *pFlag = &Flag;
unsigned int Flag2;

#define FLAG_SUNLOCATED (1 << 5)    // if tempVolt>0.3
#define SPEEDMODE (Flag & (1 << 6)) // FLAG 1
#define FLAG_TAKE_TEMP (1 << 7)     // FLAG 2

void Delay(int Value) // Delay function
{
  for (int i = 0; i < Value; i++)
    asm("nop");
}

//-----Systick increment each ms-----
void SysTick_Handler()
{
  milliseconds++;

  //-----SPEEDMODE ON-----
  if (SPEEDMODE)
  {

    if (milliseconds % 1 == 0) // seconds counter to epoc value
    {
      epochTime += 1;
    }

    if (milliseconds % 1 == 0)
    {
      fullDayTime++;
    }

    if (tTemp == 1)
    {
      tempStatus++;
    }

    if (tempStatus == 1800)
    {
      tTemp = 0;
      tempStatus = 0;
    }

    if (tSens == 1)
    {
      tStatus++;
    }

    if (tStatus == 27)
    {
      tSens = 0;
      tStatus = 0;
    }
  }

  //-----NORMAL SPEED-----
  if (~Flag & (1 << 6))
  {

    if (milliseconds % 1000 == 0) // Seconds
    {
      epochTime += 1;
    }

    if (milliseconds % 1000 == 0)
    {
      fullDayTime++;
    }

    if (tTemp == 1)
    {
      tempStatus++;
    }

    if (tempStatus == 1000)
    {
      tTemp = 0;
      tempStatus = 0;
    }

    if (tSens == 1)
    {
      tStatus++;
    }

    if (tStatus == 15)
    {
      tSens = 0;
      tStatus = 0;
    }
  }

  //-----Suntracker-----
  if (tempVolt > 0.3) // Sun NOT found
  {
    if (~SPEEDMODE)
    {
      if (milliseconds % 1000 == 0) // each sec add
      {
        servoValue += 80;
        Flag &= ~FLAG_SUNLOCATED;
        lampTimer++;
        lampMinutes = lampTimer / 60;
        lampHours = lampTimer / 3600;
      }
    }

    if (SPEEDMODE)
    {

      if (milliseconds % 1 == 0) // each sec add
      {
        servoValue += 1;
        Flag &= ~FLAG_SUNLOCATED;
        lampTimer++;
        lampMinutes = lampTimer / 60;
        lampHours = lampTimer / 3600;
      }
    }
  }
  else if ((milliseconds % 1000 == 0) && ~SPEEDMODE)
  {
    Flag |= FLAG_SUNLOCATED;
    sunTimer++;
    sunMinutes = sunTimer / 60;
    sunHours = sunTimer / 3600;
  }
  else if ((milliseconds % 1 == 0) && SPEEDMODE)
  {
    Flag |= FLAG_SUNLOCATED;
    sunTimer++;
    sunMinutes = sunTimer / 60;
    sunHours = sunTimer / 3600;
  }

  if (secondsIntoDay < 21600 || secondsIntoDay > 79200) // reset timer at 0600
  {
    sunTimer = 0;
    lampTimer = 0;
    sunMinutes = 0;
    sunHours = 0;
    lampMinutes = 0;
    lampHours = 0;
  }

  if (~SPEEDMODE && (milliseconds % 60000 == 0))
    Flag2 |= FLAG_TAKE_TEMP;

  if (SPEEDMODE && (milliseconds % 60 == 0))
  {
    Flag2 |= FLAG_TAKE_TEMP;
  }
}

//-----Handler for photosensor-----
void ADC_Handler(void)
{

  int x = *AT91C_ADCC_SR;

  if ((x & (1 << 1)) == (1 << 1))
  {
    voltOne = *AT91C_ADCC_CDR1 * (3.3 / 0xFFF);
    v1Flag = 1;
  }

  *AT91C_ADCC_IDR = (3 << 1);
}

//-----Handler for temp-----
void TC0_Handler(void)
{
  tempFlag = 1;
  *AT91C_TC0_IDR = (1 << 6);
}

//-----Function to track the sun-----
void sunTracker()
{
  if ((voltOne < 3.3) && voltOne > 0)
  {
    tempVolt = voltOne;
  }

  *AT91C_PWMC_CH1_CDTYR = (1838 + servoValue); // 0 deg+servoValue/sec increment

  if (servoValue > 4200) // 0-180 deg
  {
    servoValue = 0;
  }

  if (secondsIntoDay > 21600 && secondsIntoDay < 79200 && (~Flag & FLAG_SUNLOCATED))
  {
    *AT91C_PIOD_SODR = (1 << 3);
  }
  else if (secondsIntoDay > 21600 && secondsIntoDay < 79200 && (Flag & FLAG_SUNLOCATED))
  {
    *AT91C_PIOD_CODR = (1 << 3);
  }

  else if (secondsIntoDay < 21600 || secondsIntoDay > 79200)
  {
    if (epochTime % 1 == 0)
      *AT91C_PIOD_SODR = (1 << 3);

    if (epochTime % 2 == 0)
      *AT91C_PIOD_CODR = (1 << 3);
  }

  //-----Print in main menu-----
  if ((Flag & 0xF) == 1)
  {
    print(sunLocationText, 1, 11);

    int tmpPos;

    if (Flag & FLAG_SUNLOCATED)
    {
      tmpPos = (servoValue * 0.04325); // 0-180,increment in 3

      // Print sunlocation in main
      setAPointer(9, 11);
      Write_Data_2_Display(0x10 + (tmpPos / 100));
      Write_Command_2_Display(0xC0);
      Write_Data_2_Display(0x10 + (tmpPos / 10) % 10);
      Write_Command_2_Display(0xC0);
      Write_Data_2_Display(0x10 + (tmpPos % 10));
      Write_Command_2_Display(0xC0);
    }

    // hours and minutes of lamp in a day
    setAPointer(34, 7);
    unsigned char hh = ((unsigned char)lampHours); // read value of "hours" adress
    Write_Data_2_Display(0x10 + (hh / 10));        // div 10 by 10
    Write_Command_2_Display(0xC0);
    Write_Data_2_Display(0x10 + (hh % 10)); // mod 10 for single digits
    Write_Command_2_Display(0xC0);

    setAPointer(36, 7); // print a colon between the hours and minutes
    Write_Data_2_Display(0x1A);
    Write_Command_2_Display(0xC0);

    setAPointer(37, 7);
    unsigned char mm = ((unsigned char)lampMinutes); // read value of "minutes" adress
    Write_Data_2_Display(0x10 + (mm / 10) % 6);
    Write_Command_2_Display(0xC0);
    Write_Data_2_Display(0x10 + (mm % 10));
    Write_Command_2_Display(0xC0);

    // hours and minutes of sun in a day
    setAPointer(34, 9);
    unsigned char hh2 = ((unsigned char)sunHours); // read value of "hours" adress
    Write_Data_2_Display(0x10 + (hh2 / 10));       // div 10 by 10
    Write_Command_2_Display(0xC0);
    Write_Data_2_Display(0x10 + (hh2 % 10)); // mod 10 for single digits
    Write_Command_2_Display(0xC0);

    setAPointer(36, 9); // print a colon between the hours and minutes
    Write_Data_2_Display(0x1A);
    Write_Command_2_Display(0xC0);

    setAPointer(37, 9);
    unsigned char mm2 = ((unsigned char)sunMinutes); // read value of "minutes" adress
    Write_Data_2_Display(0x10 + (mm2 / 10) % 6);
    Write_Command_2_Display(0xC0);
    Write_Data_2_Display(0x10 + (mm2 % 10));
    Write_Command_2_Display(0xC0);
  }
}
