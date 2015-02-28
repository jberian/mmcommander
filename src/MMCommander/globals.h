#ifndef _GLOBALS_H_

#define _GLOBALS_H_

#include "hal_types.h"
#include "constants.h"

extern char   __xdata uartRxBuffer[SIZE_OF_UART_RX_BUFFER];
extern char   __xdata uartTxBuffer[SIZE_OF_UART_RX_BUFFER];
extern int    __xdata uartTxLength;
extern int    __xdata uartTxIndex;
extern int    __xdata uartRxIndex;
extern char   __xdata txFilterEnabled;
extern int    __xdata txCalcCRC;
extern int    __xdata txCalcCRC16;
extern char   __xdata txLength;
extern int    __xdata txTimes;

#endif