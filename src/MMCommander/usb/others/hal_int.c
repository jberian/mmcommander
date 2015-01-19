/***********************************************************************************
  Filename:     hal_int.c

  Description:  HAL interrupt control

***********************************************************************************/

/***********************************************************************************
* INCLUDES
*/
#include "hal_types.h"
#include "hal_defs.h"
#include "hal_int.h"
#include "hal_board.h"

/***********************************************************************************
* GLOBAL FUNCTIONS
*/

/***********************************************************************************
* @fn      halIntOn
*
* @brief   Enable global interrupts.
*
* @param   none
*
* @return  none
*/
void halIntOn(void)
{
    HAL_INT_ON();
}


/***********************************************************************************
* @fn      halIntOff
*
* @brief   Turns global interrupts off.
*
* @param   none
*
* @return  none
*/
void halIntOff(void)
{
    HAL_INT_OFF();
}


/***********************************************************************************
* @fn      halIntLock
*
* @brief   Turns global interrupts off and returns current interrupt state.
*          Should always be used together with halIntUnlock().
*
* @param   none
*
* @return  uint16 - current interrupt state
*/
uint16 halIntLock(void)
{
    istate_t key;
    HAL_INT_LOCK(key);
    return(key);
}


/***********************************************************************************
* @fn      halIntUnlock
*
* @brief   Set interrupt state back to the state it had before calling halIntLock().
*          Should always be used together with halIntLock().
*
* @param   key
*
* @return  none
*/
void halIntUnlock(uint16 key)
{
    HAL_INT_UNLOCK(key);
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

