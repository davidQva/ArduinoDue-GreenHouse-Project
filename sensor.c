#include "at91sam3x8.h"
#include "system_sam3x.h"
#include "core_cm3.h"
#include "datetime.h"
#include "handlers.h"
#include "display.h"

int tempVal; // global value, stores temp

#define SPEEDMODE (Flag & (1 << 6)) // FLAG 1

//-----------------------------Temperature sensor-----------------------------

//-----Initialize temp sensor-----
void start_temp(void) // Start tempsensor
{

  tempFlag = 0;
  *AT91C_PIOB_OER = (1 << 25);  // Set pin2
  *AT91C_PIOB_SODR = (1 << 25); // clear pin2
  *AT91C_PIOB_CODR = (1 << 25);

  tSens = 1;
  while (tSens == 1)
  {
  } // delay 15ms

  *AT91C_PIOB_SODR = (1 << 25); // set pin2
  *AT91C_PIOB_CODR = (1 << 25); // clear

  Delay(50);

  *AT91C_PIOB_SODR = (1 << 25); // set pin2
  *AT91C_PIOB_ODR = (1 << 25);  // out disable, set as input

  *AT91C_TC0_SR; // (TC0) Status Register

  *AT91C_TC0_CCR = (1 << 2); //(TC0) Channel Control Register, reset interrupt
  *AT91C_TC0_IER = (1 << 6); //(TC0) Interrupt Enable Register, LDRBS, RB s.917
}

//-----Read temp-----
double readTemp(void) // Measure the temp between falling edge and rising edge
{
  int A = *AT91C_TC0_RA;
  int B = *AT91C_TC0_RB;
  double temp = (((B - A) / (5 * 42)) - 273.15); // T(?C) = [tDx(?s) / timeout multiplier(?s/?K)] - 273.15?K

  return temp;
}

//-----Check if temp is ready to read-----
void tempReady()
{
  if (tempFlag == 1)
  {
    tempVal = readTemp();
    start_temp();
  }
}

//-----Print temp-----
void writeTemp(int x, int y, int temp)
{
  setAPointer(x, y);
  int tmp = ((int)*&temp);
  Write_Data_2_Display(0x10 + (tmp / 10));
  Write_Command_2_Display(0xC0);
  Write_Data_2_Display(0x10 + (tmp % 10));
  Write_Command_2_Display(0xC0);
  Write_Data_2_Display(0x00);
  Write_Command_2_Display(0xC0);
}

//-----------------------------Light sensor--------------------------------

//-----Initialize-----
void Init_lightsensor(void) // Readwrite!! INIT
{
  *AT91C_PMC_PCER = (1 << 11); // Peripheral Clock Enable Register
  *AT91C_PMC_PCER1 = (1 << 5);
  *AT91C_PIOA_PPUDR = (3 << 3); // (PIOA) Pull-up Disable Register
  *AT91C_PIOA_PER = (3 << 3);   // (PIOA) PIO Enable Register
  *AT91C_PIOA_ODR = (3 << 3);   // (PIOA) Output Disable Register
  *AT91C_ADCC_MR = (1 << 9);    // (ADCC) ADC Mode Register
}

//-----Read light-----
void readLight(void)
{
  *AT91C_ADCC_CHER = (3 << 1); // (ADCC) ADC Channel Enable Register
  *AT91C_ADCC_CR = (1 << 1);   // (ADCC) ADC Control Register
  *AT91C_ADCC_SR; // (ADCC) ADC Status Register
  NVIC_ClearPendingIRQ(37);
  NVIC_SetPriority(37, 1);
  NVIC_EnableIRQ(37);
  *AT91C_ADCC_IER = (3 << 1); // (ADCC) ADC Interrupt Enable Register
  *AT91C_ADCC_CR = (1 << 1);  // (ADCC) ADC Control Register
}