/***********************************************************************************
  Filename:     hal_types.h

  Description:  HAL type definitions

***********************************************************************************/

#ifndef HAL_TYPES_H
#define HAL_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif



/***********************************************************************************
 * TYPEDEFS
 */

typedef signed   char   int8;
typedef unsigned char   uint8;

typedef signed   short  int16;
typedef unsigned short  uint16;

typedef signed   long   int32;
typedef unsigned long   uint32;


typedef void (*ISR_FUNC_PTR)(void);
typedef void (*VFPTR)(void);


/***********************************************************************************
 * Compiler abstraction
 */

/*****************************************************
 * IAR MSP430
 */
#ifdef __IAR_SYSTEMS_ICC__

#define _PRAGMA(x) _Pragma(#x)

#if defined __ICC430__

#ifndef CODE
#define CODE
#endif
#ifndef XDATA
#define XDATA
#endif
#define FAR
#define NOP()  asm("NOP")

#define HAL_ISR_FUNC_DECLARATION(f,v)   \
    _PRAGMA(vector=v##_VECTOR) __interrupt void f(void)
#define HAL_ISR_FUNC_PROTOTYPE(f,v)     \
    _PRAGMA(vector=v##_VECTOR) __interrupt void f(void)
#define HAL_ISR_FUNCTION(f,v)           \
    HAL_ISR_FUNC_PROTOTYPE(f,v); HAL_ISR_FUNC_DECLARATION(f,v)


/*****************************************************
 * IAR 8051
 */
#elif defined __ICC8051__

#ifndef BSP_H
#define CODE   __code
#define XDATA  __xdata
#endif

#define FAR
#define NOP()  asm("NOP")

#define HAL_MCU_LITTLE_ENDIAN()   __LITTLE_ENDIAN__
#define HAL_ISR_FUNC_DECLARATION(f,v)   \
    _PRAGMA(vector=v) __near_func __interrupt void f(void)
#define HAL_ISR_FUNC_PROTOTYPE(f,v)     \
    _PRAGMA(vector=v) __near_func __interrupt void f(void)
#define HAL_ISR_FUNCTION(f,v)           \
    HAL_ISR_FUNC_PROTOTYPE(f,v); HAL_ISR_FUNC_DECLARATION(f,v)

#else
#error "Unsupported architecture"
#endif


/*****************************************************
 * KEIL 8051
 */
#elif defined __KEIL__
#include <intrins.h>
#define BIG_ENDIAN

#define CODE   code
#define XDATA  xdata
#define FAR
#define NOP()  _nop_()

#define HAL_ISR_FUNC_DECLARATION(f,v)   \
    void f(void) interrupt v
#define HAL_ISR_FUNC_PROTOTYPE(f,v)     \
    void f(void)
#define HAL_ISR_FUNCTION(f,v)           \
    HAL_ISR_FUNC_PROTOTYPE(f,v); HAL_ISR_FUNC_DECLARATION(f,v)

typedef unsigned short istate_t;

// Keil workaround
#define __code  code
#define __xdata xdata


/*****************************************************
 * WIN32
 */
#elif defined WIN32

#define CODE
#define XDATA
#include "windows.h"
#define FAR far
#pragma warning (disable :4761)


/*****************************************************
 * Code Composer Essential
 */

#elif __TI_COMPILER_VERSION__
#define CODE
#define XDATA
#define FAR

typedef unsigned short istate_t;


/*****************************************************
 * Other compilers
 */
#else
#error "Unsupported compiler"
#endif



#ifdef __cplusplus
}
#endif



/***********************************************************************************
  Copyright 2007-2008 Texas Instruments Incorporated. All rights reserved.

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
