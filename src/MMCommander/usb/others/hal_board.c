/***********************************************************************************
    Filename: hal_board.c

    Copyright 2007 Texas Instruments, Inc.
***********************************************************************************/


/******************************************************************************
 * INCLUDES
 */

#include "hal_types.h"
#include "hal_mcu.h"
#include "hal_board.h"



/*******************************************************************************
 * GLOBAL FUNCTIONS
 */


/******************************************************************************
 * @fn          halBoardInit
 *
 * @brief       Sets up board. Initialize MCU with HS XOSC, configure I/O pins
 *              and user interface.
 *
 * @param       none
 *
 * @return      none
 *****************************************************************************/
void halBoardInit(void)
{

    // Enable HS XOSC and set maximum clock speed
    halMcuInit();

    // LED
    //MCU_IO_OUTPUT(HAL_BOARD_IO_LED_PORT, HAL_BOARD_IO_LED_PIN, 0);  // Turn off

}









