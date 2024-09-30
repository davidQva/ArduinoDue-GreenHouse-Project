#include "menu.h"

unsigned int hours;
unsigned int minutes;
unsigned int epochTime = 0x69A57FB5;
unsigned int epochConvert;
unsigned int milliseconds;

int secondsIntoDay;
int daysSinceEpoch;
int year;
int month;
int DOY;
int DOM;
int dayYear;
int weekday;
int leapdays;
int seconds;
int leap;
int y;
int day;

//-----time of the day (%=mod, /=div)-----
void clockWeekday()
{
  secondsIntoDay = (unsigned int)epochTime % 86400;
  daysSinceEpoch = (unsigned int)epochTime / 86400;

  seconds = secondsIntoDay % 60;
  minutes = (secondsIntoDay % 3600) / 60;
  hours = secondsIntoDay / 3600;
  weekday = (daysSinceEpoch + 4) % 7;
  leapdays = (*&year - 1970) / 4;
  DOY = (daysSinceEpoch % 365) - leapdays;
}

//-----converts to seconds based on the values below-----
void epochTimeConvert()
{
  epochConvert = (*&daysSinceEpoch * 86400) + (*&hours * 3600) + (*&minutes * 60) + (*&seconds);
}

//-----returns current year-----
int currentYear() // Current year
{

  year = epochTime / (86400 * 365);
  year = (year + 1970);
  return (year);
}

//-----Check if leap year-----
int isLeap(int x)
{
  if (!((year) % 4) && (((year) % 100) || !((year) % 400)))
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

//-----Non-leap year month-----
void yearMonth(int y)
{

  dayYear = y;

  switch (dayYear)
  {
  case 1 ... 31:
    month = 1;
    break;
  case 32 ... 59:
    month = 2;
    break;
  case 60 ... 90:
    month = 3;
    break;
  case 91 ... 120:
    month = 4;
    break;
  case 121 ... 151:
    month = 5;
    break;
  case 152 ... 181:
    month = 6;
    break;
  case 182 ... 212:
    month = 7;
    break;
  case 213 ... 243:
    month = 8;
    break;
  case 244 ... 273:
    month = 9;
    break;
  case 274 ... 304:
    month = 10;
    break;
  case 305 ... 334:
    month = 11;
    break;
  case 335 ... 365:
    month = 12;
    break;
  }
}

//-----Leap year month-----
void leapyearMonth(int y) // Leap year month.
{

  dayYear = y;

  switch (dayYear)
  {
  case 1 ... 31:
    month = 1;
    break;
  case 32 ... 60:
    month = 2;
    break;
  case 61 ... 91:
    month = 3;
    break;
  case 92 ... 121:
    month = 4;
    break;
  case 122 ... 152:
    month = 5;
    break;
  case 153 ... 182:
    month = 6;
    break;
  case 183 ... 213:
    month = 7;
    break;
  case 214 ... 244:
    month = 8;
    break;
  case 245 ... 274:
    month = 9;
    break;
  case 275 ... 305:
    month = 10;
    break;
  case 306 ... 335:
    month = 11;
    break;
  case 336 ... 366:
    month = 12;
    break;
  }
}

//-----Day of month-----
void monthDay()
{
  int test = *&month;
  short t = isLeap(*&year);
  DOM = 0;

  switch (test)
  {
  case 1:
    DOM = (*&DOY);
    break;
  case 2:
    if (t == 0)
      DOM = (*&DOY - 30);
    else
      DOM = (*&DOY - 30);
    break;
  case 3:
    if (t == 0)
      DOM = (*&DOY - 58);
    else
      DOM = (*&DOY - 59);
    break;
  case 4:
    if (t == 0)
      DOM = (*&DOY - 89);
    else
      DOM = (*&DOY - 90);
    break;
  case 5:
    if (t == 0)
      DOM = (*&DOY - 119);
    else
      DOM = (*&DOY - 120);
    break;
  case 6:
    if (t == 0)
      DOM = (*&DOY - 150);
    else
      DOM = (*&DOY - 151);
    break;
  case 7:
    if (t == 0)
      DOM = (*&DOY - 180);
    else
      DOM = (*&DOY - 181);
    break;
  case 8:
    if (t == 0)
      DOM = (*&DOY - 211);
    else
      DOM = (*&DOY - 212);
    break;
  case 9:
    if (t == 0)
      DOM = (*&DOY - 242);
    else
      DOM = (*&DOY - 243);
    break;
  case 10:
    if (t == 0)
      DOM = (*&DOY - 272);
    else
      DOM = (*&DOY - 273);
    break;
  case 11:
    if (t == 0)
      DOM = (*&DOY - 303);
    else
      DOM = (*&DOY - 304);
    break;
  case 12:
    if (t == 0)
      DOM = (*&DOY - 333);
    else
      DOM = (*&DOY - 334);
    break;
  }
}
