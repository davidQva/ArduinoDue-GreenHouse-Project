#include "at91sam3x8.h"
#include "system_sam3x.h"
#include "core_cm3.h"
#include "handlers.h"

//-----Startup config setup function-----
void config()
{
  *AT91C_PIOD_CODR = (1 << 2); // 0x400E1434, clear output data reg. Clear OE

  //-----pmc----- d,c
  *AT91C_PMC_PCER = (1 << 14); // Peripheral Clock Reg: piod
  *AT91C_PMC_PCER = (1 << 13); // Peripheral Clock Reg: pioc

  //--------OE------ pin 27
  *AT91C_PIOD_PER = (1 << 2);  // set PD3(Pin27) on, bit 3 in port D
  *AT91C_PIOD_OER = (1 << 2);  // Enable output line
  *AT91C_PIOD_SODR = (1 << 2); // set PD3(Pin27) to 1, bit 2 in port D

  //-------lcd------ pin 46,47,48,49
  *AT91C_PIOC_PER = (1 << 17);   // set PD3(Pin27) on, bit 3 in port D
  *AT91C_PIOC_OER = (1 << 17);   // Enable output line
  *AT91C_PIOC_PPUDR = (1 << 17); // Disable pull-up register

  *AT91C_PIOC_PER = (1 << 16);   // set PD3(Pin27) on, bit 3 in port D
  *AT91C_PIOC_OER = (1 << 16);   // Enable output line
  *AT91C_PIOC_PPUDR = (1 << 16); // Disable pull-up register

  *AT91C_PIOC_PER = (1 << 15);   // set PD3(Pin27) on, bit 3 in port D
  *AT91C_PIOC_OER = (1 << 15);   // Enable output line
  *AT91C_PIOC_PPUDR = (1 << 15); // Disable pull-up register

  *AT91C_PIOC_PER = (1 << 14);   // set PD3(Pin27) on, bit 3 in port D
  *AT91C_PIOC_OER = (1 << 14);   // Enable output line
  *AT91C_PIOC_PPUDR = (1 << 14); // Disable pull-up register

  //-------lcd------ pin 25
  *AT91C_PIOD_PER = (1 << 0);   // set PD0(Pin25) on, bit 3 in port D
  *AT91C_PIOD_OER = (1 << 0);   // Enable output line
  *AT91C_PIOD_PPUDR = (1 << 0); // Disable pull-up register

  //----bus----- pin
  *AT91C_PIOC_PER = (0xFF << 2);   // set 34-41
  *AT91C_PIOC_PPUDR = (0xFF << 2); // Disable pull-up register

  //-------led------ pin 28
  *AT91C_PIOD_PER = (1 << 3);   // set PD3(Pin28) on, bit 3 in port D
  *AT91C_PIOD_OER = (1 << 3);   // Enable output line
  *AT91C_PIOD_PPUDR = (1 << 3); // Disable pull-up register

  //------enable keypad pins------ row pins (34,35,36,37) col pins (39,40,41)
  //-------------------------rows----------------
  *AT91C_PIOC_PER = (1 << 2); // set PD3(Pin34) on, bit 2 in port C
  *AT91C_PIOC_PER = (1 << 3); // set PD3(Pin35) on, bit 3 in port C
  *AT91C_PIOC_PER = (1 << 4); // set PD3(Pin36) on, bit 4 in port C
  *AT91C_PIOC_PER = (1 << 5); // set PD3(Pin37) on, bit 5 in port C

  //------------------------col------------------
  *AT91C_PIOC_PER = (1 << 7); // set PD3(Pin39) on, bit 7 in port C
  *AT91C_PIOC_PER = (1 << 8); // set PD3(Pin40) on, bit 8 in port C
  *AT91C_PIOC_PER = (1 << 9); // set PD3(Pin41) on, bit 9 in port C

  //----------------------lcd----------------------------------
  //----------------------eneable OE-pin and turning it off
  *AT91C_PIOC_PER = (1 << 12);  // set PD3(Pin51) on, bit 2 in port C
  *AT91C_PIOC_OER = (1 << 12);  // Enable output line
  *AT91C_PIOC_SODR = (1 << 12); // set PD3(Pin51) to 1, bit 2 in port D
}

//-----WRITE CONTROL. AKTIVE LOW!!!, 1=>OFF 0=>ON-----
void LCD_WR(int x) // WRITE CONTROL. AKTIVE LOW!!!, 1=>OFF 0=>ON
{
  if (x == 1)
    *AT91C_PIOC_SODR = (1 << 17); // set PC17(Pin46)=>utsignal=1
  else
    *AT91C_PIOC_CODR = (1 << 17); // clear PC17(Pin46), output data reg.=> utsignal=0
}

//-----READ CONTROL. AKTIVE LOW!!!, 1=>OFF 0=>ON-----
void LCD_RD(int x)
{
  if (x == 1)
    *AT91C_PIOC_SODR = (1 << 16); // set PC16(Pin47)=>utsignal=1
  else
    *AT91C_PIOC_CODR = (1 << 16); // clear PC16(Pin47), output data reg.=> utsignal=0
}

//-----CHIP ENABLE. AKTIVE LOW!!!, 1=>OFF 0=>ON, CE=CS-----
void LCD_CE(int x)
{
  if (x == 1)
    *AT91C_PIOC_SODR = (1 << 15); // set PC15(Pin48)=>utsignal=1
  else
    *AT91C_PIOC_CODR = (1 << 15); // clear PC15(Pin48), output data reg.=> utsignal=0
}

//-----COMMAND/DATA SELECT or REGISTER SELECT. 1=> READ, 0=> WRITE-----
void LCD_CD(int x)
{
  if (x == 1)
    *AT91C_PIOC_SODR = (1 << 14); // set PC14(Pin49)=>utsignal=1
  else
    *AT91C_PIOC_CODR = (1 << 14); // clear PC14(Pin49), output data reg.=> utsignal=0
}

//-----RESET LCD. AKTIVE LOW!!! Low -> RA6963 will be reset.?? sida 9-----
void LCD_RESET(int x)
{
  if (x == 1)
    *AT91C_PIOD_SODR = (1 << 0); // set PD0(Pin25)=>utsignal=1
  else
    *AT91C_PIOD_CODR = (1 << 0); // clear PD0(Pin25), output data reg.=> utsignal=0
}

//-----LCD READ OR WRITE. 1=>WRITE, 0=>READ-----
void BUS_R_W(int x)
{
  if (x == 1)
    *AT91C_PIOC_OER = (0xFF << 2); // set 34-41 =>OUT=1
  else
    *AT91C_PIOC_ODR = (0xFF << 2); // set 34-41 OUT=0
}

//-----1=> OFF 0=>ON 74HC245-----
void skalbagge_LCD(int x) // 1=> OFF 0=>ON 74HC245
{
  if (x == 1)
    *AT91C_PIOC_SODR = (1 << 12); // set PC12(Pin51)=>utsignal=1
  else
    *AT91C_PIOC_CODR = (1 << 12); // clear PC12(Pin51), output data reg.=> utsignal=0
}

//-----1=> READ, 0=> WRITE 74HC245-----
void skalbagge_LCD_DIR(int x)
{
  if (x == 1)
  {
    *AT91C_PIOC_OER = (1 << 13);  // Enable
    *AT91C_PIOC_SODR = (1 << 13); // set PC13(Pin50)=>utsignal=1
  }
  else
    *AT91C_PIOC_OER = (1 << 13); // Enable
  *AT91C_PIOC_CODR = (1 << 13);  // clear PC13(Pin50), output data reg.=> utsignal=0}
}

//-----1=>OFF 0=>ON 74HC245-----
void skalbagge_Kpad(int x)
{
  if (x == 1)
    *AT91C_PIOD_SODR = (1 << 2); // set PD3(Pin27)=>utsignal=1
  else
    *AT91C_PIOD_CODR = (1 << 2); // 0x400E1434, clear PD3(Pin27), output data reg.=> utsignal=0
}

//-----timer clock 1 to timer counter,TC-----
void tempSensorInit(void)
{
  *AT91C_PMC_PCER = (1 << 12); // Power Management Controller
  *AT91C_PMC_PCER = (1 << 27); // Timer Counter 0 s.46
  *AT91C_TC0_CCR = (5 << 0);   // start clock, software trigger 101
  *AT91C_TC0_CMR = (*AT91C_TC0_CMR & 0xfff0fff8) | 0x60000;

  *AT91C_PIOB_PER = (1 << 25); // Pin Enable Register, Pin 2=PB25
  *AT91C_PIOB_PPUDR = (1 << 25);

  *AT91C_PIOB_OER = (1 << 25);  // Set pin2
  *AT91C_PIOB_SODR = (1 << 25); // clear pin2
  *AT91C_PIOB_CODR = (1 << 25);

  tSens = 1;
  while (tSens == 1)
  {
  }

  *AT91C_PIOB_SODR = (1 << 25); // clear pin2

  *AT91C_TC0_SR;
  NVIC_ClearPendingIRQ(27);
  NVIC_SetPriority(27, 1);
  NVIC_EnableIRQ(27); // Timer counter, configure for rising edge
}

//-----Initialize Servo,Period 20ms, Duty-----
void Init_Servo() // Initialize Servo,Period 20ms, Duty
{
  *AT91C_PMC_PCER1 = (1 << 4);      // Timer Counter 1
  *AT91C_PIOB_PDR = (1 << 17);      // PIO Disable Register
  *AT91C_PIOB_ABMR = (1 << 17);     // Activate peripheral B to control the pin 1=B
  *AT91C_PMC_PCER = (1 << 12);      // Power Management Controller
  *AT91C_PIOB_PPUDR = (1 << 17);    // PullUp Disable Register
  *AT91C_PWMC_ENA = (1 << 1);       // PWM Enable Register
  *AT91C_PWMC_CH1_CMR = 5;          // Chanel Mode Register => 32
  *AT91C_PWMC_CH1_CPRDR = (0xCD14); // PWM Channel Period Register
  *AT91C_PWMC_CH1_CDTYR = (0xE5B);  // Beauuty cycle
}
