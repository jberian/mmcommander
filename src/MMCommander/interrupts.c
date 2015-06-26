#include "globals.h"
#include "ioCCxx10_bitdef.h"
#include "ioCC1110.h"
#include "crc_4b6b.h"
#include "interrupts.h"
#include "medtronicRF.h"
#include "usb_uart.h"
#include "hal_uart.h"
#include "txFilter.h"

// Globals
unsigned long timeCounter;

#pragma vector = P1INT_VECTOR
__interrupt void PORT1_ISR(void)
{
  int i,j;
  int modeChange;
  char txString[2];
  
  // Clear Port 1 Interrupt Flag
  P1IFG = 0;
  IRCON2 &= 0xF7;
   
  modeChange = 1;
  for (j=0; j<0x20; j++) {
    for (i=0; i<0xFFFF; i++) asm("nop");
    if (P1_2 == 1) {
      modeChange = 0;
      j=0x20;
    }
  }
  
  if (modeChange == 1) {
    if (txFilterEnabled == 1) {
      txFilterEnabled = 0;
      P1_1 = 1;
      txString[0] = 0x13;
    } else {
      txFilterEnabled = 1;
      P1_1 = 0;
      txString[0] = 0x03;
    }
    halUartWrite((uint8 const *)txString,1);
  }

  // Clear Port 1 Interrupt Flag
  P1IFG = 0;  
  IRCON2 &= ~0x04;
}

void enablePushButtonInt (void)
{
  // Initialize Filter to enabled.
  P1_0 = 0;
  txFilterEnabled = 1;
  
  // Clear any pending Port 1 Interrupt Flag (IRCON2.P1IF = 0)
  P1IFG = 0;
  IRCON2 &= 0xF7;
  
  // Enable interrupt on falling edges
  PICTL |= 0x02;
  
  // Enable global interrupt (IEN0.EA = 1) and Port 1 Interrupt (IEN2.P1IE = 1)
  P1IEN = 0x04;
  EA = 1; IEN2 |= IEN2_P1IE;  
}

#pragma vector = T1_VECTOR
__interrupt void TIMER1_ISR(void)
{
  
  // Stop Timer 1
  T1CTL &= 0xFC;

  // Clear Timer 1 Interrupt Flag
  P1IFG = 0;  
  IRCON &= ~0x02;
  
  // Re-calibrate;
  timeCounter++;
  
  if (timeCounter >= 0x0005D00) {
    RFST = RFST_SIDLE;
    RFST = RFST_SRX;
    timeCounter = 0;
  }
   
  // Clean interrupts and start Timer 1 in Free Run mode
  T1CTL = 0x01;
}

void stopTimerInt (void)
{
  // Stop Timer 1
  T1CTL &= 0xFC;
  
   // Reset Timer 1 Counter
  T1CNTL = 0x00;
  
  // Disable Timer 1 interrupt
  IEN1 &= ~0x02;
}

void resetTimerCounter (void)
{
  timeCounter = 0;
  T1CNTL = 0x00;
}

void enableTimerInt (void)
{
 
  // Stop Timer 1
  T1CTL = 0x0C;

  // Set Timer 1 timeout value
  T1CC0H = 0xFF;
  T1CC0L = 0xFF;
  
  // Reset Timer 1 Counter
  T1CNTL = 0x00;
  timeCounter = 0;
 
  // Set Timer 1 mode 
  T1CCTL0 = 0x44; 
 
  // Clear any pending Timer 1 Interrupt Flag
  IRCON &= ~0x02;
  
  // Enable global interrupt (IEN0.EA = 1) and Timer 1 Interrupt (IEN1.1 = 1)
  EA = 1; IEN1 |= 0x02;  
  
  // Start Timer 1
  T1CTL = 0x0E;
}