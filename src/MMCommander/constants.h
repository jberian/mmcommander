#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

/***********************************************************************************
* CONSTANTS
*/
// Bit masks to check CLKCON register
#define OSC_BIT           0x40  // bit mask used to select/check the system clock oscillator
#define TICKSPD_BITS      0x38  // bit mask used to check the timer ticks output setting
#define CLKSPD_BIT        0x03  // bit maks used to check the clock speed
#define MAIN_OSC_BITS     0x7F  // bit mask used to control the system clock oscillator
                                // e.g. ~MAIN_OSC_BITS can be used to start Crystal OSC

// Bit masks to check SLEEP register
#define XOSC_STABLE_BIT   0x40  // bit mask used to check the stability of XOSC
#define HFRC_STB_BIT      0x20  // bit maks used to check the stability of the High-frequency RC oscillator
#define OSC_PD_BIT        0x04  // bit maks used to power down system clock oscillators

// Macro for checking status of the crystal oscillator
#define XOSC_STABLE (SLEEP & XOSC_STABLE_BIT)

// RF States
#define RFST_SFSTXON      0x00
#define RFST_SCAL         0x01
#define RFST_SRX          0x02
#define RFST_STX          0x03
#define RFST_SIDLE        0x04

// Size of allocated UART RX/TX buffer (just an example)
#define SIZE_OF_UART_RX_BUFFER   128
#define SIZE_OF_UART_TX_BUFFER   SIZE_OF_UART_RX_BUFFER

// Baudrate = 57.6 kbps (U0BAUD.BAUD_M = 34, U0GCR.BAUD_E = 11)
#define UART_BAUD_M  34
#define UART_BAUD_E  11

#endif