#include "ioCCxx10_bitdef.h"
#include "ioCC1110.h"
#include "medtronicRF.h"
#include "crc_4b6b.h"
#include "constants.h"
#include "globals.h"
#include "usb_uart.h"
#include "hal_uart.h"
#include "txFilter.h"
#include "configuration.h"
#include "interrupts.h"

// Globals
static char   __xdata rfMessage[512];
static unsigned int __xdata rfLength;
static int    __xdata txCalcCRC;
static int    __xdata txCalcCRC16;
static char   __xdata txLength;
static int    __xdata txTimes;
static char   __xdata lastData;

void sendMedtronicMessage (char *message, unsigned int length, int times)
{
  int i,j;
  
  encode4b6b (message, length, rfMessage, &rfLength);
  PKTLEN = rfLength;
  
  RFST = RFST_SIDLE;
  
  for (j=0; j<times; j++) {  
     RFST = RFST_STX;
     for (i=0; i<rfLength; i++) {
        while (!RFTXRXIF);
        TCON &= 0xFD;
        RFD = rfMessage[i];
     }   
     
     i=4096;
     /* Add NOP to avoid that the loop is optimized away */
     while(--i) asm (" NOP"); 
    }
  
  PKTLEN = 0xFF;
  RFST = RFST_SIDLE;
  RFST = RFST_SRX;    
}

char receiveMedtronicMessage (char *message, unsigned int *length)
{
  unsigned int i;
  char calcCRC;
  short calcCRC16;
  
  RFST = RFST_SIDLE;
  RFST = RFST_SRX;

  PKTLEN = 0xFF;
  lastData = 0xFF;
  for (i=0; i<500 && lastData != 0x00; i++) {
    while (!RFTXRXIF) {
        usbUartProcess();
        usbReceiveData();
    }
    rfMessage[i] = RFD;
    lastData = rfMessage[i];
    TCON &= ~0x02;
  }   
  rfLength = i-1;
  RFST = RFST_SIDLE;
  
  decode4b6b(rfMessage,rfLength,message,length);
  calcCRC = crc8(message,(*length)-1);
  
  if (calcCRC == message[(*length)-1]) {
    return (0);
  } 
  
  calcCRC16 = crc16(message,(*length)-2);
  if (((char)( calcCRC16       & 0x00FF) == message[(*length)-1]) && 
      ((char)((calcCRC16 >> 8) & 0x00FF) == message[(*length)-2])) {
    return (0);
  }
  
  calcCRC = crc8(message,(*length)-2);
  
  if (calcCRC == message[(*length)-2]) {
    (*length) = (*length)-1;
    return (0);
  } 
 
  calcCRC16 = crc16(message,(*length)-3);
  if (((char)( calcCRC16       & 0x00FF) == message[(*length)-2]) &&
      ((char)((calcCRC16 >> 8) & 0x00FF) == message[(*length)-3])) {
    (*length) = (*length)-1;
    return (0);
  }
 
  crc16Init();
  return(1);
}

void usbReceiveData (void)
{
  char  tempData[128];
  short tmpCRC16;
  unsigned int nBytes,readBytes;
  unsigned int i;
  
  nBytes = halUartGetNumRxBytes();
  for (i=0; i<nBytes; i=i+48) {
    if (nBytes-i > 48) readBytes = 48;
    else readBytes = nBytes-i;
    halUartRead((uint8 *)&tempData[i],readBytes);
    usbUartProcess();
  }
  
  for(i=0; i<nBytes; i++) { 

   // Read Rx buffer
    uartRxBuffer[uartRxIndex] = tempData[i];
  
    if (uartRxIndex == 0) {
      if (uartRxBuffer[0] == 0x01) {
        uartRxIndex++;
        txCalcCRC   = 0;
        txCalcCRC16 = 0;
        enableTimerInt();
      } else if (uartRxBuffer[0] == 0x81) {
        uartRxIndex++;
        txCalcCRC   = 1;
        txCalcCRC16 = 0;
        enableTimerInt();
      } else if (uartRxBuffer[0] == 0xC1) {
        uartRxIndex++;
        txCalcCRC   = 0;
        txCalcCRC16 = 1;
        enableTimerInt();
      } else if ((uartRxBuffer[0] == 0x03) ||
                 (uartRxBuffer[0] == 0x13)) {
        txFilterEnabled = 1;
        P1_1 = 0;
        uartRxBuffer[0] = 0x03;
        halUartWrite((uint8 const *)uartRxBuffer,1);
      } else if (uartRxBuffer[0] == 0x00) {
        uartRxBuffer[0] = _MMCOMMANDER_VERSION_ ;
        halUartWrite((uint8 const *)uartRxBuffer,1);
      }
    } else if (uartRxIndex == 1) {
      txLength = uartRxBuffer[1];
      uartRxIndex++;
      resetTimerCounter();
    } else if (uartRxIndex == 2) {
      txTimes = uartRxBuffer[2];
      uartRxIndex++;
      resetTimerCounter();
    } else if (uartRxIndex > 2) {
      resetTimerCounter();
      if (uartRxIndex == (txLength + 2)) {
        stopTimerInt();
        if (txCalcCRC == 1) {
          uartRxBuffer[++uartRxIndex] = crc8(&uartRxBuffer[3],(int)txLength);
          txLength++;
        }
        if (txCalcCRC16 == 1) {
          tmpCRC16 = crc16 (&uartRxBuffer[3],(int)txLength);
          uartRxBuffer[++uartRxIndex] = (char)((tmpCRC16 >> 8) & 0x00FF);
          uartRxBuffer[++uartRxIndex] = (char)(tmpCRC16 & 0x00FF);
          txLength += 2;
        }
      
        if (txFilter(&uartRxBuffer[3],txLength) == 0) {
          sendMedtronicMessage(&uartRxBuffer[3],txLength,txTimes);
          crc16Init();
          halUartWrite((uint8 const *)uartRxBuffer,3);
          uartRxIndex=0;
        } else {
          uartRxBuffer[1]=0x00;
          uartRxBuffer[2]=0x00;
          halUartWrite((uint8 const *)uartRxBuffer,3);
          uartRxIndex=0;
        }
      } else {
        uartRxIndex++;
      }
    }
  }
}
