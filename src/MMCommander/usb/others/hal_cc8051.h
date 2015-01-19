/******************************************************************************
    Filename: hal_cc8051.h

    Copyright 2007 Texas Instruments, Inc.
******************************************************************************/

#ifndef HAL_CC8051_H
#define HAL_CC8051_H

/******************************************************************************
 * INCLUDES
 */

#include <hal_types.h>
#include <hal_defs.h>


/******************************************************************************
 * CONSTANTS
 */
#define BIT0              0x01
#define BIT1              0x02
#define BIT2              0x04
#define BIT3              0x08
#define BIT4              0x10
#define BIT5              0x20
#define BIT6              0x40
#define BIT7              0x80


/******************************************************************************
 * MACROS
 */

#define MCU_IO_TRISTATE   1             // Used as "func" for the macros below
#define MCU_IO_PULLUP     2
#define MCU_IO_PULLDOWN   3


//-----------------------------------------------------------------------------
//  Macros for simple configuration of IO pins on TI LPW SoCs
//-----------------------------------------------------------------------------
#define MCU_IO_PERIPHERAL(port, pin)   MCU_IO_PERIPHERAL_PREP(port, pin)
#define MCU_IO_INPUT(port, pin, func)  MCU_IO_INPUT_PREP(port, pin, func)
#define MCU_IO_OUTPUT(port, pin, val)  MCU_IO_OUTPUT_PREP(port, pin, val)
#define MCU_IO_SET(port, pin, val)     MCU_IO_SET_PREP(port, pin, val)
#define MCU_IO_SET_HIGH(port, pin)     MCU_IO_SET_HIGH_PREP(port, pin)
#define MCU_IO_SET_LOW(port, pin)      MCU_IO_SET_LOW_PREP(port, pin)
#define MCU_IO_TGL(port, pin)          MCU_IO_TGL_PREP(port, pin)
#define MCU_IO_GET(port, pin)          MCU_IO_GET_PREP(port, pin)

#define MCU_IO_DIR_INPUT(port, pin)    MCU_IO_DIR_INPUT_PREP(port, pin)
#define MCU_IO_DIR_OUTPUT(port, pin)   MCU_IO_DIR_OUTPUT_PREP(port, pin)


//----------------------------------------------------------------------------------
//  Macros for internal use (the macros above need a new round in the preprocessor)
//----------------------------------------------------------------------------------
#define MCU_IO_PERIPHERAL_PREP(port, pin)   st( P##port##SEL |= BM(pin); )

#define MCU_IO_INPUT_PREP(port, pin, func)  st( P##port##SEL &= ~BM(pin); \
                                                P##port##DIR &= ~BM(pin); \
                                                switch (func) { \
                                                case MCU_IO_PULLUP: \
                                                    P##port##INP &= ~BM(pin); \
                                                    P2INP &= ~BM(port + 5); \
                                                    break; \
                                                case MCU_IO_PULLDOWN: \
                                                    P##port##INP &= ~BM(pin); \
                                                    P2INP |= BM(port + 5); \
                                                    break; \
                                                default: \
                                                    P##port##INP |= BM(pin); \
                                                    break; } )

#define MCU_IO_OUTPUT_PREP(port, pin, val)  st( P##port##SEL &= ~BM(pin); \
                                                P##port##_##pin## = val; \
                                                P##port##DIR |= BM(pin); )

#define MCU_IO_SET_HIGH_PREP(port, pin)     st( P##port##_##pin## = 1; )
#define MCU_IO_SET_LOW_PREP(port, pin)      st( P##port##_##pin## = 0; )

#define MCU_IO_SET_PREP(port, pin, val)     st( P##port##_##pin## = val; )
#define MCU_IO_TGL_PREP(port, pin)          st( P##port##_##pin## ^= 1; )
#define MCU_IO_GET_PREP(port, pin)          (P##port## & BM(pin))

#define MCU_IO_DIR_INPUT_PREP(port, pin)    st( P##port##DIR &= ~BM(pin); )
#define MCU_IO_DIR_OUTPUT_PREP(port, pin)   st( P##port##DIR |= BM(pin); )


/****************************************************************************************
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
#endif
