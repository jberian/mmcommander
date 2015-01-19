/******************************************************************************
    Filename: clock.h

    This file defines interface for clock related functions for the
    CC1110/CC2510 family of RF system-on-chips from Texas Instruments.

    Copyright 2007 Texas Instruments, Inc.
******************************************************************************/
#ifndef _CLOCK_H
#define _CLOCK_H


/*******************************************************************************
 * INCLUDES
 */

#include "hal_types.h"

// Include chip specific IO definition file
#if (chip == 2510)
#include "ioCC2510.h"
#endif
#if (chip == 1110)
#include "ioCC1110.h"
#endif
#if (chip == 2511)
#include "ioCC2511.h"
#endif
#if (chip == 1111)
#include "ioCC1111.h"
#endif



/*******************************************************************************
 * CONSTANTS
 */

/* SEE DATA SHEET FOR DETAILS ABOUT THE FOLLOWING BIT MASKS */

/* Pre defined values for main clock source, used in cc2510ClockSetMainSrc() */
#define CLOCK_SRC_XOSC      0x00  /*  High speed Crystal Oscillator Control */
#define CLOCK_SRC_HFRC      0x01  /*  Low power RC Oscillator */

// Bit masks to check CLKCON register
#define CLKCON_OSC32K_BM    0x80  // bit mask, for the slow 32k clock oscillator
#define CLKCON_OSC_BM       0x40  // bit mask, for the system clock oscillator
#define CLKCON_TICKSPD_BM   0x38  // bit mask, for timer ticks output setting
#define CLKCON_CLKSPD_BM    0x07  // bit maks, for the clock speed

#define TICKSPD_DIV_1       (0x00 << 3)
#define TICKSPD_DIV_2       (0x01 << 3)
#define TICKSPD_DIV_4       (0x02 << 3)
#define TICKSPD_DIV_8       (0x03 << 3)
#define TICKSPD_DIV_16      (0x04 << 3)
#define TICKSPD_DIV_32      (0x05 << 3)
#define TICKSPD_DIV_64      (0x06 << 3)
#define TICKSPD_DIV_128     (0x07 << 3)

#define CLKSPD_DIV_1        (0x00)
#define CLKSPD_DIV_2        (0x01)
#define CLKSPD_DIV_4        (0x02)
#define CLKSPD_DIV_8        (0x03)
#define CLKSPD_DIV_16       (0x04)
#define CLKSPD_DIV_32       (0x05)
#define CLKSPD_DIV_64       (0x06)
#define CLKSPD_DIV_128      (0x07)

// Bit masks to check SLEEP register
#define SLEEP_XOSC_STB_BM   0x40  // bit mask, check the stability of XOSC
#define SLEEP_HFRC_STB_BM   0x20  // bit maks, check the stability of the High-frequency RC oscillator
#define SLEEP_OSC_PD_BM     0x04  // bit mask, power down system clock oscillator(s)

/*******************************************************************************
 * TYPEDEFS
 */

/*******************************************************************************
 * MACROS
 */

// Macro for checking status of the high frequency RC oscillator.
#define IS_HFRC_STABLE()    (SLEEP & SLEEP_HFRC_STB_BM)

// Macro for checking status of the crystal oscillator
#define IS_XOSC_STABLE()    (SLEEP & SLEEP_XOSC_STB_BM)

// Macro for getting the clock division factor
#define CLKSPD_GET()        (CLKCON & CLKCON_CLKSPD_BM)

// Macro for getting the timer tick division factor.
#define TICKSPD_GET()       ((CLKCON & CLKCON_TICKSPD_BM) >> 3)

// Macro for setting the timer division factor, x value from 0b000 to 0b111
#define TICKSPD_SET(x)      do { CLKCON = ((((x) << 3) & 0x38)  \
                                            | (CLKCON & 0xC7)); \
                            } while (0)

// Macro for setting the clock tick division factor, x value from 0b000 to 0b111
#define CLKSPD_SET(x)       do { CLKCON = (((x) & 0x07)         \
                                            | (CLKCON & 0xF8)); \
                            } while (0)


/*******************************************************************************
 * FUNCTIONS
 */

void clockSetMainSrc(uint8 source);


/*****************************************************************************/
#endif
