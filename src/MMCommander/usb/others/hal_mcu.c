/***********************************************************************************
  Filename:     hal_mcu.c

  Description:  hal mcu library

***********************************************************************************/


#include "hal_types.h"
#include "hal_mcu.h"

#include "clock.h"

/***********************************************************************************
* @fn          halMcuInit
*
* @brief       Set system clock to run from crystal oscillator (XOSC)
*
* @param       none
*
* @return      none
*/
void halMcuInit(void)
{
    clockSetMainSrc(CLOCK_SRC_XOSC);
}


/***********************************************************************************
* @fn          halMcuWaitUs
*
* @brief       Busy wait function. Waits the specified number of microseconds. Use
*              assumptions about number of clock cycles needed for the various
*              instructions.
*
*              NB! This function is highly dependent on architecture and compiler!
*
* @param       uint16 usec - number of microseconds delay
*
* @return      none
*/
#pragma optimize=none
void halMcuWaitUs(uint16 usec)
{
    usec>>= 1;          // Divide by 2 to achieve better resolution

    while(usec > 1)     // compare: ~13 cycles
    {
        asm("NOP");     // 1 cycle/NOP
        asm("NOP");
        asm("NOP");
        asm("NOP");
        asm("NOP");
        asm("NOP");
        asm("NOP");
        asm("NOP");
        asm("NOP");
        asm("NOP");

        usec--;        // decr: ~7 cycles
    }
}

/***********************************************************************************
* @fn          halMcuWaitMs
*
* @brief       Busy wait function. Waits the specified number of milliseconds. Use
*              assumptions about number of clock cycles needed for the various
*              instructions.
*
*              NB! This function is highly dependent on architecture and compiler!
*
* @param       uint16 msec - number of milliseconds delay
*
* @return      none
*/
#pragma optimize=none
void halMcuWaitMs(uint16 msec)
{
    while(msec--)
        halMcuWaitUs(1000);
}

/***********************************************************************************
* @fn          halMcuSetLowPowerMode
*
* @brief       Sets the MCU in a low power mode. Will turn global interrupts on at
*              the same time as entering the LPM mode. The MCU must be waken from
*              an interrupt (status register on stack must be modified).
*
*              NB! This function is highly dependent on architecture and compiler!
*
* @param       uint8 mode - power mode
*
* @return      none
*/
void halMcuSetLowPowerMode(uint8 mode)
{
  // comment: not yet implemented
  //HAL_ASSERT(FALSE);
}


/******************************************************************************
* @fn          halMcuReset
*
* @brief
*              Resets the MCU. This utilize the watchdog timer as there is no
*              other way for a software reset. The reset will not occur until
*              approx. 2 ms. NB: The function will not return! (hangs until reset)
*
* @param       none
*
* @return      none
*
******************************************************************************/
void halMcuReset(void)
{
    const uint8 WDT_INTERVAL_MSEC_2=   0x03;   // after ~2 ms

    WDCTL = ((WDCTL & 0xFC) | (WDT_INTERVAL_MSEC_2 & 0x03));
    // Start watchdog
    WDCTL &= ~0x04;     // Select watchdog mode
    WDCTL |= 0x08;      // Enable timer
    while(1);           // Halt here until reset
}


/***********************************************************************************
  Copyright 2007 Texas Instruments Incorporated. All rights reserved.

  IMPORTANT: Your use of this Software is limited to those specific rights
  granted under the terms of a software license agreement between the user
  who downloaded the software, his/her employer (which must be your employer)
  and Texas Instruments Incorporated (the "License").  You may not use this
  Software unless you agree to abide by the terms of the License. The License
  limits your use, and you acknowledge, that the Software may not be modified,
  copied or distributed unless embedded on a Texas Instruments microcontroller
  or used solely and exclusively in conjunction with a Texas Instruments radio
  frequency transceiver, which is integrated into your product.  Other than for
  the foregoing purpose, you may not use, reproduce, copy, prepare derivative
  works of, modify, distribute, perform, display or sell this Software and/or
  its documentation for any purpose.

  YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE
  PROVIDED “AS IS” WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
  INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE,
  NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL
  TEXAS INSTRUMENTS OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT,
  NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER
  LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
  INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE
  OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT
  OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
  (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

  Should you have any questions regarding your right to use this Software,
  contact Texas Instruments Incorporated at www.TI.com.
***********************************************************************************/