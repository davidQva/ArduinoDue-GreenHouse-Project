#include "system_sam3x.h"
#include "at91sam3x8.h"
#include "core_cm3.h"
#include "stdio.h"
#include "handlers.h"
#include "datetime.h"
#include "display.h"
#include "sensor.h"
#include "keypad.h"

//-----text to display in main menu-----
char menu[10] = {"10**Menu**"};
char date[7] = {"07Date:"};
char time[7] = {"07Time:"};
char temp[7] = {"07Temp:"};
char pSun[18] = {"18Position of sun:"};
char settingsB[13] = {"13Settings(*)"};
char templogB[12] = {"12(#)TempLog"};
char setDate[12] = {"12yyyy:mm:dd"};
char setTime[7] = {"07hh:mm"};
char mainB[9] = {"09Main(#)"};
char speedModeON[5] = {"05ON "};
char speedModeOFF[5] = {"05OFF"};
char timerLamp[12] = {"12Lamp Timer"};
char timerSun[11] = {"11Sun Timer"};

//-----log text-----
char logMenuText[5] = {"05Log"};
char logWeekdays[35] = {"35Mon  Tue  Wen  Thu  Fri  Sat  Sun"};
char logMin[5] = {"05Min"};
char logMax[5] = {"05Max"};
char logAvg[5] = {"05Avg"};
char mainLogB[9] = {"09Main(*)"};

//-----alarm values-----
int tempMinLimit;
int tempMaxLimit;

//-----Print variables-----
short tempPrintVal;
short tempX;
short tempY;

//-----temp for 7-day arr-----
int tempMonth;
int tempYear;
int tempDay;

int daysGoneMonth(int mon);

#define CLEAR_SCREEN (1 << 4)

//-----if keypad num 6 = speedmode-----
void speedModeButton()
{
  if (keypad() == 6)
    Flag ^= (3 << 6);
}

//------Main menu start-----
void mainMenu()
{
  //-----Static-----
  if (Flag & (1 << 4)) // clear screen
  {
    Clear_Display();
    print(menu, 16, 1);
    print(temp, 1, 6);
    print(settingsB, 1, 14);
    print(templogB, 29, 14);
    print(timerLamp, 23, 7);
    print(timerSun, 24, 9);
    Flag ^= (1 << 4);
  }

  //-----print ON or OFF in main-----
  if (~Flag & (1 << 6))
    print(speedModeOFF, 11, 8);
  else
    print(speedModeON, 11, 8);

  //-----print-----
  writeDate();
  writeTime();

  //-----update temp lagom-----
  if ((milliseconds % 100 == 0) && (Flag & ~(1 << 6))) // updates every minute
    writeTemp(6, 6, tempVal);
  else if (Flag & (1 << 6))
    writeTemp(6, 6, tempVal);

  //-----go to settings or log menu-----
  keypad();
  speedModeButton();

  if (*&value == 10)
  {
    Flag = 0x12; // go to case:2, set clear screen
  }
  else if (*&value == 12)
  {
    Flag |= 0x13; // go to case:3, set clear screen
  }
}

//-----Log start-----
void logMenu()
{
  if (Flag & (1 << 4)) // clear screen & print static text
  {
    Clear_Display();
    print(mainLogB, 1, 14);
    print(logMenuText, 18, 1);
    print(logWeekdays, 5, 3);
    print(logMin, 1, 5);
    print(logMax, 1, 8);
    print(logAvg, 1, 11);
    Flag ^= (1 << 4);
    goto intro;
  }

  //-----print 1 time and then once a minute-----
  if (Flag2 & (1 << 7))
  {
  intro:
    
    tempX = 0;
    tempY = 5;

    //-----magic temp printer-----
    for (int y = 0; y < 3; y++)
    {
      for (int x = 1; x < 8; x++)
      {
        tempPrintVal = weekdays[y][x % 7];
        writeTemp(tempX += 5, tempY, tempPrintVal);
      }
      tempX = 0;
      tempY += 3;
    }
    tempX = 0;
    tempY = 6;

    for (int y = 3; y < 5; y++)
    {
      for (int x = 1; x < 8; x++)
      {
        writeLogTimeStamp(tempX += 5, tempY, weekdays[y][x % 7]);
      }
      tempX = 0;
      tempY += 3;
    }
  }
  keypad();

  if (*&value == 10) // Button go to main
  {
    Flag = (Flag & 0xF0) | (17 << 0);
  }
}

//-----Settings menu start-----
void settingsMenu()
{
  if (Flag & (1 << 4)) // clear screen
  {
    Clear_Display();
    Flag ^= (1 << 4); // clear bit 4
    char changeDate[17] = {"17(1) Change Date"};
    char changeTime[17] = {"17(2) Change Time"};
    char changeTempAlarm[23] = {"23(3) Change Alarm Temp"};
    print(changeDate, 1, 4);
    print(changeTime, 1, 6);
    print(changeTempAlarm, 1, 8);
    print(mainB, 1, 14);
  }

  int tempkey = keypad(); // if we dont do this, choosing menu bugs

  //-----Set Temp Alarm-----
  if (tempkey == 3)
  {
  startSetTempLimit:
    Clear_Display();
    char clearMinLimit[4] = {"04  "};
    char clearMaxLimit[4] = {"04  "};
    char confirm[23] = {"23(*) confirm (#) retry"};
    char setTempLimit[15] = {"15Min:   Max:  "};
    print(clearMinLimit, 10, 8);
    print(clearMaxLimit, 17, 8);
    print(setTempLimit, 6, 8);
    setAPointer(10, 8);

    short tempLimit[4];

    int counter = 0;

    while (counter < 5)
    {
      int key = NULL;

      int pressedButton = keypad();
      if (pressedButton != -1 && pressedButton != 10 && pressedButton != 12)
      {
        switch (pressedButton)
        {
        case 1:
          Write_Data_2_Display(0x11);
          key = 1;
          break;
        case 2:
          Write_Data_2_Display(0x12);
          key = 2;
          break;
        case 3:
          Write_Data_2_Display(0x13);
          key = 3;
          break;
        case 4:
          Write_Data_2_Display(0x14);
          key = 4;
          break;
        case 5:
          Write_Data_2_Display(0x15);
          key = 5;
          break;
        case 6:
          Write_Data_2_Display(0x16);
          key = 6;
          break;
        case 7:
          Write_Data_2_Display(0x17);
          key = 7;
          break;
        case 8:
          Write_Data_2_Display(0x18);
          key = 7;
          break;
        case 9:
          Write_Data_2_Display(0x19);
          key = 9;
          break;
        case 10:
          Write_Data_2_Display(0xA);
          key = 10;
          break;
        case 11:
          Write_Data_2_Display(0x10);
          key = 0;
          break;
        case 12:
          Write_Data_2_Display(0x3);
          key = 12;
          break;
        }

        Write_Command_2_Display(0xC0); // write and increment
        tempLimit[counter] = key;

        counter++;

        if (counter == 2)
        {
          setAPointer(17, 8);
        }
      }

      if (counter == 4) // confirm retry option
      {
        print(confirm, 11, 9);

        if (pressedButton == 10)
          break;

        if (pressedButton == 12)
          goto startSetTempLimit;
      }
    }
    tempMinLimit = 10 * tempLimit[0];
    tempMinLimit += tempLimit[1];

    tempMaxLimit = 10 * tempLimit[2];
    tempMaxLimit += tempLimit[3];

    if (tempMinLimit > tempMaxLimit)
    {
      goto startSetTempLimit;
    }
    Flag = 0x12;
  }

  //-----Set Date-----
  if (tempkey == 1)
  {
  retry:
    Clear_Display();
    char clearDate[17] = {"17               "};
    char confirm[23] = {"23(*) confirm (#) retry"};
    char clearConfirm[23] = {"23                     "};
    print(clearDate, 1, 4);
    print(setDate, 6, 7);
    setAPointer(6, 7);

    short Date[8];

    for (int i = 0; i <= 7; i++)
      Date[i] = NULL;

    int counter = 0;

    while (counter < 9)
    {
      int key = NULL;

      int pressedButton = keypad();
      if (pressedButton != -1)
      {
        switch (pressedButton)
        {
        case 1:
          Write_Data_2_Display(0x11);
          key = 1;
          break;
        case 2:
          Write_Data_2_Display(0x12);
          key = 2;
          break;
        case 3:
          Write_Data_2_Display(0x13);
          key = 3;
          break;
        case 4:
          Write_Data_2_Display(0x14);
          key = 4;
          break;
        case 5:
          Write_Data_2_Display(0x15);
          key = 5;
          break;
        case 6:
          Write_Data_2_Display(0x16);
          key = 6;
          break;
        case 7:
          Write_Data_2_Display(0x17);
          key = 7;
          break;
        case 8:
          Write_Data_2_Display(0x18);
          key = 8;
          break;
        case 9:
          Write_Data_2_Display(0x19);
          key = 9;
          break;
        case 10:
          Write_Data_2_Display(0xA);
          key = 10;
          break;
        case 11:
          Write_Data_2_Display(0x10);
          key = 0;
          break;
        case 12:
          Write_Data_2_Display(0x3);
          key = 12;
          break;
        }
        Write_Command_2_Display(0xC0); // write and increment
        Date[counter] = key;

        counter++;

        if (counter == 4 || counter == 6)
        {
          Write_Data_2_Display(0x1A);
          Write_Command_2_Display(0xC0);
        }

        if (counter == 8)
        {
          while (keypad() != 10)
          {
            print(confirm, 18, 7);
            if (keypad() == 12)
            {
              print(clearConfirm, 18, 7);
              goto retry;
            }
            Delay(20);
          }
        }
      }
    }
    tempYear = 0;
    int x = 1000;

    for (int a = 0; a < 4; a++)
    {
      tempYear += x * Date[a];
      x /= 10;
    }

    if (tempYear > 2040 || tempYear < 1970)
      goto retry;

    tempMonth = 0;
    tempMonth = 10 * Date[4];
    tempMonth += Date[5];

    if (tempMonth > 12 || tempMonth < 1)
      goto retry;

    tempDay = 0;
    tempDay = 10 * Date[6];
    tempDay += Date[7];

    switch (tempMonth)
    {
    case 1:
      if (tempDay > 31 || tempDay < 1)
        goto retry;
      else
        break;

    case 2:
      if (tempYear % 4 == 0 && ((tempDay > 29) || (tempDay < 1)))
        goto retry;
      else if (tempYear % 4 != 0 && ((tempDay > 28) || (tempDay < 1)))
        goto retry;
      else
        break;

    case 3:
      if (tempDay > 31 || tempDay < 1)
        goto retry;
      else
        break;

    case 4:
      if (tempDay > 30 || tempDay < 1)
        goto retry;
      else
        break;

    case 5:
      if (tempDay > 31 || tempDay < 1)
        goto retry;
      else
        break;

    case 6:
      if (tempDay > 30 || tempDay < 1)
        goto retry;
      else
        break;

    case 7:
      if (tempDay > 31 || tempDay < 1)
        goto retry;
      else
        break;

    case 8:
      if (tempDay > 31 || tempDay < 1)
        goto retry;
      else
        break;

    case 9:
      if (tempDay > 30 || tempDay < 1)
        goto retry;
      else
        break;

    case 10:
      if (tempDay > 31 || tempDay < 1)
        goto retry;
      else
        break;

    case 11:
      if (tempDay > 30 || tempDay < 1)
        goto retry;
      else
        break;

    case 12:
      if (tempDay > 31 || tempDay < 1)
        goto retry;
      else
        break;
    }

    // calculation that determines the total seconds of the date since 1970 the first one is based on if it's a leap year the "else" is based on non-leap year
    if (!((tempYear) % 4) && (((tempYear) % 100) || !((tempYear) % 400)))
      epochTime = ((tempYear - 1970) * (365 * 86400) + (daysGoneMonth(tempMonth) * 86400) + (tempDay * 86400) + ((((tempYear - 1968) / 4) - 1) * 86400) + secondsIntoDay);
    else
      epochTime = ((tempYear - 1970) * (365 * 86400) + (daysGoneMonth(tempMonth) * 86400) + (tempDay * 86400) + (((tempYear - 1968) / 4) * 86400) + secondsIntoDay);

    Flag = 0x12; // succesfuly entered date
  }

  //-----SET TIME-----
  if (tempkey == 2)
  {
  startSetTime:
    Clear_Display();   
    char confirm[23] = {"23(*) confirm (#) retry"};  
    print(setTime, 6, 7);
    setAPointer(6, 7);

    short time[4];

    for (int i = 0; i <= 4; i++)
      time[i] = NULL;

    int counter = 0;

    while (counter < 5)
    {
      int key = NULL;

      int pressedButton = keypad();
      if (pressedButton != -1)
      {

        switch (pressedButton)
        {
        case 1:
          Write_Data_2_Display(0x11);
          key = 1;
          break;
        case 2:
          Write_Data_2_Display(0x12);
          key = 2;
          break;
        case 3:
          Write_Data_2_Display(0x13);
          key = 3;
          break;
        case 4:
          Write_Data_2_Display(0x14);
          key = 4;
          break;
        case 5:
          Write_Data_2_Display(0x15);
          key = 5;
          break;
        case 6:
          Write_Data_2_Display(0x16);
          key = 6;
          break;
        case 7:
          Write_Data_2_Display(0x17);
          key = 7;
          break;
        case 8:
          Write_Data_2_Display(0x18);
          key = 7;
          break;
        case 9:
          Write_Data_2_Display(0x19);
          key = 9;
          break;
        case 10:
          Write_Data_2_Display(0xA);
          key = 10;
          break;
        case 11:
          Write_Data_2_Display(0x10);
          key = 0;
          break;
        case 12:
          Write_Data_2_Display(0x3);
          key = 12;
          break;
        }
        Write_Command_2_Display(0xC0); // write and increment
        time[counter] = key;

        counter++;

        if (counter == 2)
        {
          Write_Data_2_Display(0x1A);
          Write_Command_2_Display(0xC0);
        }
      }
      if (counter == 4) // confirm retry option
      {
        print(confirm, 18, 7);
        if (keypad() == 12)
        {
          goto startSetTime;
        }
        if (keypad() == 10)
          break;
      }
    }

    int tempHour = 0;

    tempHour = 10 * time[0];
    tempHour += time[1];

    if (tempHour > 23 || tempHour < 0)
      goto startSetTime;

    int tempMinute = 0;
    tempMinute = 10 * time[2];
    tempMinute += time[3];

    if (tempMinute > 60 || tempMinute < 0)
      goto startSetTime;

    epochTime += ((tempHour * 3600) + (tempMinute * 60) - secondsIntoDay);

    Flag = 0x12; // succesfuly set date
    Delay(100);
  }
  //-----END SET TIME-----  

  if (tempkey == 12) // Button go to main
  {
    Flag = 0x91; // Set FLAG_UPDATE, CLEARSCREEN, GO TO MAIN MENU
  }
}


// Determines the elapsed days of the year based on which month that gets inputted in set_date for leap year and non-leap year
int daysGoneMonth(int mon)
{
  // int yer;
  int daysGone;

  daysGone = tempMonth;
  int elapsedDays = 0;

  char t = 0;

  if (tempYear % 4 == 0)
    t = 1;

  switch (daysGone)
  {
  case 1:
    elapsedDays = 1;
    break;
  case 2:
    elapsedDays = 31;
    break;
  case 3:
    if (t == 0)
      elapsedDays = 59;
    else
      elapsedDays = 60;
    break;
  case 4:
    if (t == 0)
      elapsedDays = 90;
    else
      elapsedDays = 91;
    break;
  case 5:
    if (t == 0)
      elapsedDays = 120;
    else
      elapsedDays = 121;
    break;
  case 6:
    if (t == 0)
      elapsedDays = 151;
    else
      elapsedDays = 152;
    break;
  case 7:
    if (t == 0)
      elapsedDays = 181;
    else
      elapsedDays = 182;
    break;
  case 8:
    if (t == 0)
      elapsedDays = 212;
    else
      elapsedDays = 213;
    break;
  case 9:
    if (t == 0)
      elapsedDays = 243;
    else
      elapsedDays = 244;
    break;
  case 10:
    if (t == 0)
      elapsedDays = 273;
    else
      elapsedDays = 274;
    break;
  case 11:
    if (t == 0)
      elapsedDays = 304;
    else
      elapsedDays = 305;
    break;
  case 12:
    if (t == 0)
      elapsedDays = 334;
    else
      elapsedDays = 335;
    break;
  }
  return elapsedDays - 1;
}