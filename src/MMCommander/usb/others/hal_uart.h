/***********************************************************************************
  Filename:     hal_uart.h

  Description:  hal UART library header file

***********************************************************************************/

#ifndef HAL_UART_H
#define HAL_UART_H

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************************
* INCLUDES
*/
#include "hal_types.h"

/***********************************************************************************
 * CONSTANTS AND DEFINES
 */
/* Serial Port Baudrate Settings */
#define HAL_UART_BAUDRATE_4800        0x01
#define HAL_UART_BAUDRATE_9600        0x02
#define HAL_UART_BAUDRATE_19200       0x03
#define HAL_UART_BAUDRATE_38400       0x04
#define HAL_UART_BAUDRATE_57600       0x05
#define HAL_UART_BAUDRATE_115200      0x06

/* Stop Bits */
#define HAL_UART_ONE_STOP_BIT         0x01
#define HAL_UART_TWO_STOP_BITS        0x02

/* Parity settings */
#define HAL_UART_NO_PARITY            0x04
#define HAL_UART_EVEN_PARITY          0x08
#define HAL_UART_ODD_PARITY           0x10

/* Number of bits in data field */
#define HAL_UART_7_BIT_DATA           0x20
#define HAL_UART_8_BIT_DATA           0x40

/***********************************************************************************
 * GLOBAL FUNCTIONS
 */
void   halUartInit(uint8 baudrate, uint8 options);
uint16 halUartWrite(const uint8* buf, uint16 length);
uint16 halUartRead(uint8* buf, uint16 length);
uint16 halUartGetNumRxBytes(void);
void   halUartEnableRxFlow(uint8 enable);


#ifdef  __cplusplus
}
#endif

/**********************************************************************************/
#endif


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

