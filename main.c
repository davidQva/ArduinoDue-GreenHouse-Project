#include "system_sam3x.h"
#include "at91sam3x8.h"
#include "core_cm3.h"

#include "datetime.h"
#include "handlers.h"
#include "display.h"
#include "keypad.h"
#include "sensor.h"
#include "config.h"
#include "menu.h"
#include "linkedList.h"

#define FLAG_SUNLOCATED (1 << 5)   // if sun is found set bit 5 in flag
#define FLAG_SPEEDMODE_ON (1 << 6) // flag is set if speedmode is ON
#define FLAG_UPDATE (1 << 7)       // flag is set when going between menues
#define FLAG_TAKE_TEMP (1 << 7)    // flag2 bit 7 is set once every min

//-----Text to print in mainmenu and alarm
char speedModeText[12] = {"12Speedmode:"};
char Alert[29] = {"29!Warning! check temperature"};
char AlertConfirm[22] = {"22#->Confirm and Clear"};
char speedMButton[5] = {"05(6)"};

//----variables for 7dayarray
int currentMin = 99;
int currentMax = 0;
int j = -1;
int i;

int main()
{
  SystemInit();

  SysTick_Config((int)(SystemCoreClock * 0.001)); // 1ms = Normal mode

  //-----In/Out pin setup-----
  config();
  Init_Display();
  Clear_Display();

  //-----temp initilazation-----
  tempSensorInit();
  start_temp();

  //-----light initilazation-----
  Init_lightsensor();
  readLight();

  //-----servo initilazation-----
  Init_Servo();

  //-----create linkedlist-----
  struct LinkedList *list;
  list = NULL;

  while (1)
  {
    //-----check leap and it ended up here-----
    if (leap == 0)
      yearMonth(*&DOY);
    else
      leapyearMonth(*&DOY);

    //-----Date & Time------
    clockWeekday();
    currentYear();
    monthDay();

    //------Start tracking sun and print pos-----
    readLight();
    sunTracker();

    //-----delete last node-----
    if (Flag2 & (1 << 0))
    {
      freeList(list);
      Flag2 ^= (1 << 0);
    }

    //-----read-temp continously-----
    tempReady();

    //-----if temp-alarm go to case 4-----
    if ((tempVal > tempMaxLimit && tempMaxLimit != 0) || (tempVal < tempMinLimit && tempMaxLimit != 0) && tempVal != 0)
    {
      Flag = (Flag & 0xF0) | (1 << 2);
      Clear_Display();
    }

    //-----Flag choose menu-----
    switch ((Flag & 0xF))
    {
    case 1:
      mainMenu();
      print(speedModeText, 1, 8);
      print(speedMButton, 1, 9);
      break;
    case 2:
      settingsMenu();
      break;
    case 3:
      logMenu();
      break;
    case 4:
      print(Alert, 6, 7);
      print(AlertConfirm, 6, 9);
      if (keypad() == 12) // clear alarm with #
      {
        Flag = (Flag & 0xF0) | (17 << 0);
      } // go to main and cleardisplay
      break;
    }

    if (Flag2 & (1 << 7))
    {
      // reset temp values newday in weekdays
      if (j != weekday && j != -1)
      {
        currentMin = 99;
        currentMax = 0;
        weekdays[2][j] = (int)tempVal;
      }

      // add a value at startup
      if (j == -1)
      {
        currentMin = 99;
        currentMax = 0;
        weekdays[2][j + 2] = (int)tempVal;
      }

      // step in weekday array
      j = weekday;

      // check for new min or max temp
      if (currentMin >= tempVal)
      {
        currentMin = (int)tempVal;
        weekdays[0][j] = (int)tempVal;
        weekdays[3][j] = (int)epochTime;
      }
      if (currentMax <= tempVal)
      {
        currentMax = (int)tempVal;
        weekdays[1][j] = (int)tempVal;
        weekdays[4][j] = (int)epochTime;
      }

      // save average
      weekdays[2][j] = (((weekdays[2][j] + (int)tempVal)) / 2);

      // add Node in linked list
      insertFirst(&list, readSensor(epochTime));

      // toggle 1min temp flag
      Flag2 ^= (1 << 7);
    }

    // Speedmode ON
    if ((Flag & FLAG_SPEEDMODE_ON) && (Flag & FLAG_UPDATE))
    {
      SysTick_Config((int)(SystemCoreClock * 0.000555556));
      Flag ^= FLAG_UPDATE;   
      Init_lightsensor();
      tempSensorInit();
      start_temp();
    }

    // Speedmode OFF
    if ((Flag & ~FLAG_SPEEDMODE_ON) && (Flag & FLAG_UPDATE))
    {
      SysTick_Config((int)(SystemCoreClock * 0.001));
      Flag ^= FLAG_UPDATE;    
      Init_lightsensor();
      tempSensorInit();
      start_temp();
    }
  }
}
