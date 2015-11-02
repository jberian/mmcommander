#include "hal_types.h"
#include "hal_defs.h"
#include "hal_cc8051.h"
#include "ioCCxx10_bitdef.h"
#include "ioCC1110.h"
#include "constants.h"
#include "globals.h"
#include "configuration.h"

/***********************************************************************************
* LOCAL FUNCTIONS
*/
void configureIO (void)
{
    /* Set function of pin to general purpose I/O */
    P1SEL &= BIT0;
    P1SEL &= BIT1;

    /* Write value to pin. Note that this is a direct bit access.
     * Also note that the value is set before the pin is configured
     * as an output, as in some cases the value of the output needs
     * to be defined before the output is driven (e.g. to avoid
     * conflicts if the signal is shared with an other device)
     */
    P1_0 = 0;
    P1_1 = 0;

    /* Change direction to output */
    P1DIR |= BIT0;
    P1DIR |= BIT1;
}

void configureOsc (void)
{
  SLEEP &= ~OSC_PD_BIT;     // powering down all oscillators
  while(!XOSC_STABLE);      // waiting until the oscillator is stable
  asm("NOP");
  CLKCON &= 0xF8;
  CLKCON &= ~MAIN_OSC_BITS; // starting the Crystal Oscillator
  SLEEP |= OSC_PD_BIT;      // powering down the unused oscillator
}

void configureMedtronicRFMode (void)
{
  SYNC1 = 0xFF; SYNC0 = 0x00;
  PKTLEN = 0xFF;
  PKTCTRL1 = 0x00; PKTCTRL0 = 0x00;
  ADDR = 0x00;
  CHANNR = 0x00;
  FSCTRL1 = 0x06; FSCTRL0 = 0x00;
  if (_USA_FREQUENCY_MODE_ == 0 ) {
    FREQ2 = 0x24;  
    FREQ1 = 0x2E; 
    FREQ0 = 0x38; 
  } else {
    FREQ2 = 0x26;
    FREQ1 = 0x30;
    FREQ0 = 0x00;
  }
  MDMCFG4 = 0xB9; 
  MDMCFG3 = 0x66; 
  MDMCFG2 = 0x33; 
  MDMCFG1 = 0x61; 
  MDMCFG0 = 0xE6; 
  DEVIATN = 0x15;
  MCSM2 = 0x07; MCSM1 = 0x30; MCSM0 = 0x18;
  FOCCFG = 0x17;
  BSCFG = 0x6C;
  AGCCTRL2 = 0x03; AGCCTRL1 = 0x40; AGCCTRL0 = 0x91;
  FREND1 = 0x56; FREND0 = 0x11;
  FSCAL3 = 0xE9; FSCAL2 = 0x2A; FSCAL1 = 0x00; FSCAL0 = 0x1F;
  TEST2 = 0x88; TEST1 = 0x31; TEST0 = 0x09;
  PA_TABLE7 = 0x00; PA_TABLE6 = 0x00; PA_TABLE5 = 0x00; PA_TABLE4 = 0x00;
  PA_TABLE3 = 0x00; PA_TABLE2 = 0x00; PA_TABLE1 = 0x50; PA_TABLE0 = 0x00;

  RFTXRXIE = 0;
}


void initGlobals (void)
{
  int i = 0;
  
  for (i=0; i<SIZE_OF_UART_RX_BUFFER; i++) {
    uartRxBuffer[i] = 0x00;
    uartTxBuffer[i] = 0x00;
  }
  uartTxLength = 0;
  uartTxIndex  = 0;
  uartRxIndex  = 0;
}