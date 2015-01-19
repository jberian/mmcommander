/***********************************************************************************

    Filename:     usb_uart.h

    Description:  USB Virtual UART interface.

***********************************************************************************/


#ifndef USB_UART_H
#define USB_UART_H


#ifdef __cplusplus
extern "C" {
#endif

#define USB_MAX_PACKET_SIZE             64 // As set in USB endpoint descriptor
#define UART_SET_RTS_LIMIT              200
#define UART_RELEASE_RTS_LIMIT          100

#define UART_FLOW_CTRL_STOP             1
#define UART_FLOW_CTRL_GO               0

#define UART_TX_BUFFER_EMPTY            0x01
#define UART_TX_STOPED_BY_FLOW_CTRL     0x02


typedef struct {
   uint32 dteRate;
   uint8 charFormat;
   uint8 parityType;
   uint8 dataBits;
} __xdata CDC_LINE_CODING_STRUCTURE;


void usbUartInit(uint32 baudrate);
void usbUartProcess(void);


#ifdef __cplusplus
}
#endif


#endif
