
/***********************************************************************************
  Filename:     MMCommander.c

  Description:  Medtronic-enabled USB interface

  Version:      1
  Revision:     86
  Date:         Dec 24th, 2014

***********************************************************************************/

/***********************************************************************************
* INCLUDE
*/
#include "crc_4b6b.h"
#include "init.h"
#include "constants.h"
#include "interrupts.h"
#include "medtronicRF.h"
#include "hal_board.h"
#include "hal_uart.h"
#include "usb_uart.h"
#include "configuration.h"

/******************************************************************************
* GLOBAL VARIABLES
*/

char   __xdata uartRxBuffer[SIZE_OF_UART_RX_BUFFER];
char   __xdata uartTxBuffer[SIZE_OF_UART_TX_BUFFER];
int    __xdata uartTxLength;
int    __xdata uartTxIndex;
int    __xdata uartRxIndex;

/******************************************************************************
* MAIN FUNCTION
*/

int main(void)
{
  char dataPacket[256];
  char repPacket[3];
  char dataErr;
  unsigned int dataLength;
  char i;
  char repeatedMessage;

    /* Configure system */
    initGlobals();
    configureIO();
    configureOsc();
    crc16Init();
    configureMedtronicRFMode();
    enablePushButtonInt();
    halUartInit(HAL_UART_BAUDRATE_57600, 0);
    
    /* Reception loop */
    while (1) {
      dataErr = receiveMedtronicMessage(dataPacket, &dataLength); 
      if (dataLength > 0) {
        repeatedMessage = 0;
        if ( (_REPEATED_COMMAND_ENABLED_ == 1) && 
            (dataErr == ((uartTxBuffer[0]>>7) & 0x01)) && 
            (dataLength == (uartTxLength-2))
           ) {
          repeatedMessage = 1;
          for (i=0; i<dataLength; i++) {
            if (dataPacket[i] != uartTxBuffer[i+2]) {
              repeatedMessage = 0;
              break;
            }
          }
        }
        
        if (repeatedMessage == 1) {
          repPacket[0] = 0x04;
          halUartWrite((uint8 const *)repPacket,1);
          usbUartProcess();
          usbReceiveData();
        } else {
          if (dataErr == 0) {
            uartTxBuffer[0] = 0x02;
          } else {
            uartTxBuffer[0] = 0x82;
          }
          uartTxBuffer[1] = dataLength;
          for (i=0; i<dataLength; i++) uartTxBuffer[i+2] = dataPacket[i];
          uartTxLength = dataLength+2;
          for (i=0; i<uartTxLength; i=i+48) {
            if (uartTxLength-i > 48) {
              halUartWrite((uint8 const *)&uartTxBuffer[i],48);
              usbUartProcess();
              usbReceiveData();
            } else {
              halUartWrite((uint8 const *)&uartTxBuffer[i],uartTxLength-i);
            }
          }
        } 
      }
    }    
}

