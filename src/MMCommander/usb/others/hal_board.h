/******************************************************************************
    Filename: hal_board.h

    Description: HAL board peripherals header file for the
                 CCxx11 USB MCU on the USB dongle.

    Copyright 2007 Texas Instruments, Inc.
******************************************************************************/
#ifndef HAL_BOARD_H
#define HAL_BOARD_H


/******************************************************************************
 * INCLUDES
 */

#include "hal_types.h"
#include "hal_defs.h"

#if chip==2511
#include <ioCC2511.h>
#elif chip==1111
#include <ioCC1111.h>
#else
#error "chip not defined!"
#endif
#include "hal_cc8051.h"

#ifdef USB_UART
#include "usb_uart.h"
#endif

/******************************************************************************
 * CONSTANTS
 */

// Board properties
#define BOARD_NAME                     "RFUSBXX11"
#define NUM_LEDS                        1
#define NUM_BUTTONS                     1
#define NUM_JSTKS                       0
#define NUM_POTS                        0


//-----------------------------------------------------------------------------
//  Define ports and pins used by CCxx11 on USB dongle
//-----------------------------------------------------------------------------
// USB
#define HAL_BOARD_IO_USB_ENABLE_PORT   1
#define HAL_BOARD_IO_USB_ENABLE_PIN    0

// LED
#define HAL_BOARD_IO_LED_PORT          1
#define HAL_BOARD_IO_LED_PIN           1

// Button
#define HAL_BOARD_IO_BTN_PORT          1
#define HAL_BOARD_IO_BTN_PIN           2


/******************************************************************************
 * MACROS
 */

//-----------------------------------------------------------------------------
//  Some simple macros
//-----------------------------------------------------------------------------
// LED
#define HAL_LED_SET()              MCU_IO_SET_HIGH(HAL_BOARD_IO_LED_PORT, HAL_BOARD_IO_LED_PIN)
#define HAL_LED_CLR()              MCU_IO_SET_LOW(HAL_BOARD_IO_LED_PORT, HAL_BOARD_IO_LED_PIN)
#define HAL_LED_TGL()              MCU_IO_TGL(HAL_BOARD_IO_LED_PORT, HAL_BOARD_IO_LED_PIN)

// Push button
#define HAL_BUTTON_ACTIVATE()      MCU_IO_INPUT(HAL_BOARD_IO_BTN_PORT, HAL_BOARD_IO_BTN_PIN, MCU_IO_PULLUP)
#define HAL_BUTTON_PUSHED()        (!MCU_IO_GET(HAL_BOARD_IO_BTN_PORT, HAL_BOARD_IO_BTN_PIN))

#define HAL_DEBOUNCE(expr)         { int i; for (i=0; i<500; i++) { if (!(expr)) i = 0; } }


// USB
#define HAL_USB_ENABLE()           ( SLEEP |= SLEEP_USB_EN )

#define HAL_USB_PULLUP_ENABLE()    MCU_IO_OUTPUT(HAL_BOARD_IO_USB_ENABLE_PORT, HAL_BOARD_IO_USB_ENABLE_PIN, 1)
#define HAL_USB_PULLUP_DISABLE()   MCU_IO_SET_LOW(HAL_BOARD_IO_USB_ENABLE_PORT, HAL_BOARD_IO_USB_ENABLE_PIN)

#define HAL_USB_INT_ENABLE()       st( IEN2|= 0x02; )
#define HAL_USB_INT_DISABLE()      st( IEN2&= ~0x02; )
#define HAL_USB_INT_CLEAR()        st( P2IFG= 0; P2IF= 0; )

#define HAL_USB_RESUME_INT_ENABLE() st( P0IE= 1;)
#define HAL_USB_RESUME_INT_DISABLE() st( P0IE= 0;)
#define HAL_USB_RESUME_INT_CLEAR()  st(P0IFG= 0; P0IF= 0; )


#define LCD_NOT_SUPPORTED

#ifdef __cplusplus
extern "C" {
#endif

#ifdef USB_UART
#define HAL_PROCESS()              usbUartProcess()
#else
#define HAL_PROCESS()
#endif


/******************************************************************************
 * FUNCTIONS
 */

void halBoardInit(void);
void halButtonInit(void);


#ifdef __cplusplus
}
#endif

/*****************************************************************************/
#endif
