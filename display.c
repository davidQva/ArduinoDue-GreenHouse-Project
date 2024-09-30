#include "system_sam3x.h"
#include "at91sam3x8.h"
#include "core_cm3.h"

#include "handlers.h"
#include "config.h"
#include "menu.h"
#include "datetime.h"

// Low Adress array value, value of x for each row
short int lowAA[16] = {0x0, 0x28, 0x50, 0x78, 0xA0, 0xC8, 0xF0,
                       0x18, 0x40, 0x68, 0x90, 0xB8, 0xE0,
                       0x8, 0x30, 0x58};
typedef struct pointer
{
  unsigned char lowA, highA;
} pointer;

//-----Set adress pointer. (low adress, high adress) see display data sheet for display chip p.17-----
pointer saP = {.lowA = 0, .highA = 0};

//-----Read status  of display (check if ready)-----
unsigned char Read_Status_Display(void) // Read status  of display
{
  unsigned char temp;

  BUS_R_W(0);
  skalbagge_Kpad(1);
  skalbagge_LCD(0);
  skalbagge_LCD_DIR(1);
  LCD_CD(1);
  LCD_CE(0);
  LCD_RD(0);
  LCD_WR(1);
  Delay(15);
  temp = (*AT91C_PIOC_PDSR & (0xFF << 2));
  LCD_CE(1);
  LCD_RD(1);
  skalbagge_LCD(1);
  skalbagge_LCD_DIR(1);
  return (temp);
}

//-----Write command to LCD steps-----
void Write_Command_2_Display(unsigned char Command) // Write command to LCD Display
{
  // wait until OK
  while (((Read_Status_Display() & (0xF3 << 1)) == (0x3 << 2)))
  {
  }
  // enable output and clear
  *AT91C_PIOC_OER = (0xFF << 2);
  *AT91C_PIOC_CODR = (0xFF << 2);
  *AT91C_PIOC_SODR = (unsigned int)Command << 2; //;Set ouput (turn on led)

  skalbagge_LCD_DIR(0);
  skalbagge_Kpad(1);
  skalbagge_LCD(0);
  BUS_R_W(1);
  LCD_CD(1);
  LCD_CE(0);
  LCD_WR(0);
  Delay(15);
  LCD_CE(1);
  LCD_WR(1);
  skalbagge_LCD(1);
  BUS_R_W(0);
}

//-----Write data to LCD steps-----
void Write_Data_2_Display(unsigned char Data)
{
  // wait until OK
  while (((Read_Status_Display() & (0xF3 << 1)) == (0x3 << 2)))
  {
  }
  // enable output and clear
  *AT91C_PIOC_OER = (0xFF << 2);
  *AT91C_PIOC_CODR = (0xFF << 2);
  *AT91C_PIOC_SODR = (unsigned int)Data << 2;
  skalbagge_LCD_DIR(0);
  skalbagge_Kpad(1);
  skalbagge_LCD(0);
  BUS_R_W(1);
  LCD_CD(0);
  LCD_CE(0);
  LCD_WR(0);
  Delay(15);
  LCD_CE(1);
  LCD_WR(1);
  skalbagge_LCD(1);
  BUS_R_W(0);
}

//-----Clears the LCD-----
void Init_Display(void) // Initialize LCD Display
{
  LCD_RESET(1 << 0);
  Delay(10);
  LCD_RESET(1 << 0);

  Write_Data_2_Display(0x00);
  Write_Data_2_Display(0x00);
  Write_Command_2_Display(0x40); // Set text home address
  Write_Data_2_Display(0x00);
  Write_Data_2_Display(0x40);
  Write_Command_2_Display(0x42); // Set graphic home address
  Write_Data_2_Display(0x28);    // initial value 0x1e(30 vågrätt)
  Write_Data_2_Display(0x00);
  Write_Command_2_Display(0x41); // Set text area
  Write_Data_2_Display(0x1e);
  Write_Data_2_Display(0x00);
  Write_Command_2_Display(0x43); // Set graphic area
  Write_Command_2_Display(0x80); // text mode
  Write_Command_2_Display(0x94); // Text on graphic off
}

//-----Clears the LCD-----
void Clear_Display(void) // Clear the LCD
{
  while (((Read_Status_Display() & (0xF3 << 1)) == (0x3 << 2)))
  {
  }
  Write_Data_2_Display(0x00);
  Write_Data_2_Display(0x00);
  Write_Command_2_Display(0x24); // X,Y adress

  for (int i = 0; i < 16 * 40; i++)
  {
    Write_Data_2_Display(0x00);
    Write_Data_2_Display(0x00);
    Write_Command_2_Display(0xC0);
  }
  Write_Data_2_Display(0x00);
  Write_Data_2_Display(0x00);
  Write_Command_2_Display(0x24);
}

//-----Set the pointer on the LCD-----
void setAPointer(int x, int y) // Välj rad(y) och kolumn(x) att skriva i
{
  if (y < 16 && x < 40) // limit rows and columns 40 columns per row, LCD = 16 rows max
  {
    switch (y)
    {
    case 1 ... 6:
      saP.highA = 0;
      saP.lowA = lowAA[y] + x;
      if (x > 16 && y == 6)
      {
        saP.highA = 1;
        saP.lowA = x - 16;
      }
      break;

    case 7 ... 12:
      saP.highA = 1; // y = 1 == 256
      saP.lowA = lowAA[y] + x;
      break;

    case 13 ... 16: // y = 2 == 512
      saP.highA = 2;
      saP.lowA = lowAA[y] + x;
      break;

    default:
      saP.highA = 0;
      saP.lowA = x;
      break;
    }
  }
  Write_Data_2_Display(saP.lowA);
  Write_Data_2_Display(saP.highA);
  Write_Command_2_Display(0x24);
}

//-----print a text by using the function "SetAPointer"-----
void print(char *text, int x, int y)
{
  char *temp = text;
  setAPointer(x, y);
  char size = ((text[0] - 0x30) * 10 + (text[1] - 0x30)); // Determine the size by the first 2 element in the array.

  for (int i = 2; i < size; i++) // print the characters after the size number.
  {
    Write_Data_2_Display((int)(temp[i] - 0x20)); // (-) 32 in ASCI to get the correct value.
    Write_Command_2_Display(0xC0);
  }
}

//-----writes the time as HH:MM:SS-----
void writeTime() // writes the time as HH:MM:SS
{
  setAPointer(5, 3);
  unsigned char hh = ((unsigned char)*&hours); // read value of "hours" adress
  Write_Data_2_Display(0x10 + (hh / 10));      // div 10 by 10
  Write_Command_2_Display(0xC0);
  Write_Data_2_Display(0x10 + (hh % 10)); // mod 10 for single digits
  Write_Command_2_Display(0xC0);

  setAPointer(7, 3); // print a colon between the hours and minutes
  Write_Data_2_Display(0x1A);
  Write_Command_2_Display(0xC0);

  setAPointer(8, 3);
  unsigned char mm = ((unsigned char)*&minutes); // read value of "minutes" adress
  Write_Data_2_Display(0x10 + (mm / 10));
  Write_Command_2_Display(0xC0);
  Write_Data_2_Display(0x10 + (mm % 10));
  Write_Command_2_Display(0xC0);

  setAPointer(10, 3); // print a colon between the minutes and seconds
  Write_Data_2_Display(0x1A);
  Write_Command_2_Display(0xC0);

  setAPointer(11, 3);
  unsigned char ss = ((unsigned char)*&seconds); // read value of seconds adress
  Write_Data_2_Display(0x10 + (ss / 10));
  Write_Command_2_Display(0xC0);
  Write_Data_2_Display(0x10 + (ss % 10));
  Write_Command_2_Display(0xC0);
}

//-----writes the date as YYYY:MM:DD-----
void writeDate()
{
  setAPointer(27, 3);
  short yyyy = ((short)*&year); // read value of "year"
  Write_Data_2_Display(0x10 + (yyyy / 1000) % 10);
  Write_Command_2_Display(0xC0);
  Write_Data_2_Display(0x10 + ((yyyy / 100) % 10));
  Write_Command_2_Display(0xC0);
  Write_Data_2_Display(0x10 + ((yyyy / 10) % 10));
  Write_Command_2_Display(0xC0);
  Write_Data_2_Display(0x10 + (yyyy % 10));
  Write_Command_2_Display(0xC0);

  // setAPointer(9,1); // print a colon between the years and months
  Write_Data_2_Display(0x1A);
  Write_Command_2_Display(0xC0);

  // setAPointer(10,1);
  unsigned char mm = ((unsigned char)*&month); // read value of "month"
  Write_Data_2_Display(0x10 + (mm / 10));
  Write_Command_2_Display(0xC0);
  Write_Data_2_Display(0x10 + (mm % 10));
  Write_Command_2_Display(0xC0);

  // setAPointer(12,1); // print a colon between the months and days
  Write_Data_2_Display(0x1A);
  Write_Command_2_Display(0xC0);

  // setAPointer(13,1);
  unsigned char dd = ((unsigned char)*&DOM); // read value of "Day of month (DOM)"
  Write_Data_2_Display(0x10 + (dd / 10));
  Write_Command_2_Display(0xC0);
  Write_Data_2_Display(0x10 + (dd % 10));
  Write_Command_2_Display(0xC0);
}

//-----writes the sun position-----
void writeSunPos()
{
  setAPointer(9, 5);
  int tmp = ((int)*&pointPos);
  Write_Data_2_Display(0x10 + (tmp / 10));
  Write_Command_2_Display(0xC0);
  Write_Data_2_Display(0x10 + (tmp % 10));
  Write_Command_2_Display(0xC0);
  Write_Data_2_Display(0x00);
  Write_Command_2_Display(0xC0);
  Write_Data_2_Display(0x23);
  Write_Command_2_Display(0xC0);
}

//-----writes the time as HHMM for the time stamp---- (with the HHMM representation because of limited space)
void writeLogTimeStamp(int x, int y, int time)
{

  int StampMinutes = ((((unsigned int)time % 86400) % 3600) / 60);
  int StampHours = (((unsigned int)time % 86400) / 3600);

  setAPointer(x, y);
  unsigned char hh = StampHours;          // read value of "hours" adress
  Write_Data_2_Display(0x10 + (hh / 10)); // div 10 by 10
  Write_Command_2_Display(0xC0);
  Write_Data_2_Display(0x10 + (hh % 10)); // mod 10 for single digits
  Write_Command_2_Display(0xC0);

  setAPointer(x + 2, y);
  unsigned char mm = StampMinutes; // read value of "minutes" adress
  Write_Data_2_Display(0x10 + (mm / 10));
  Write_Command_2_Display(0xC0);
  Write_Data_2_Display(0x10 + (mm % 10));
  Write_Command_2_Display(0xC0);
}