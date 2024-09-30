#include "at91sam3x8.h"
#include "system_sam3x.h"
#include "core_cm3.h"

#include "datetime.h"
#include "display.h"

int row;
int col;
int value;
int z;

//-----Function for keypad buttons-----
int keypad(void)
{

  value = -1;

  *AT91C_PIOC_OER = (7 << 7);  // enables outputs, make all column pin as output
  *AT91C_PIOD_CODR = (1 << 2); // clerar OE key bus, pin27, PD2
  *AT91C_PIOC_ODR = (15 << 2); // disables outputs, main purpose enables inputs //make all row pin as inputs
  *AT91C_PIOC_SODR = (7 << 7); // set all column pin as high

  for (int i = 0; i < 3; i++)
  {                                    // for loop för columner
    *AT91C_PIOC_CODR = (1 << (7 + i)); // clearar alla värden för PC 7-9
    for (int j = 0; j < 4; j++)
    { // for loop rows
      if ((*AT91C_PIOC_PDSR & (1 << (2 + j))) == 0)
      {

        switch (i)
        {
        case 0:
          col = 3;
          break;
        case 1:
          col = 2;
          break;
        case 2:
          col = 1;
          break;
        }
        switch (j)
        {
        case 0:
          row = 4;
          break;
        case 1:
          row = 3;
          break;
        case 2:
          row = 2;
          break;
        case 3:
          row = 1;
          break;
        }
        value = row * 3 + col - 3;

        z = milliseconds;
        while (180 + z > milliseconds)
        {
        };
        break;
      }
    }
    *AT91C_PIOC_SODR = (1 << (7 + i)); // set the column again
  }
  *AT91C_PIOC_ODR = (7 << 7); // make all colum pin as input

  return value;
}

//-----Print the keypad number to display-----
void Print_Nr_From_Keypad(void)
{
  int pressedButton = keypad();
  if (pressedButton != -1)
  {
    switch (pressedButton)
    {
    case 1:
      Write_Data_2_Display(0x11);
      break;
    case 2:
      Write_Data_2_Display(0x12);
      break;
    case 3:
      Write_Data_2_Display(0x13);
      break;
    case 4:
      Write_Data_2_Display(0x14);
      break;
    case 5:
      Write_Data_2_Display(0x15);
      break;
    case 6:
      Write_Data_2_Display(0x16);
      break;
    case 7:
      Write_Data_2_Display(0x17);
      break;
    case 8:
      Write_Data_2_Display(0x18);
      break;
    case 9:
      Write_Data_2_Display(0x19);
      break;
    case 10:
      Write_Data_2_Display(0xA);
      break;
    case 11:
      Write_Data_2_Display(0x10);
      break;
    case 12:
      Write_Data_2_Display(0x3);
      break;
    }
    Write_Command_2_Display(0xC0); // write and increment
  }
}