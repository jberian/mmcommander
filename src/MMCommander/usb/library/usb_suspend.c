/***********************************************************************************

    Filename:     usb_framework.c

    Description:  USB library common functionality.

***********************************************************************************/

/// \addtogroup module_usb_suspend
/// @{
#include "usb_firmware_library_headers.h"
#include "hal_int.h"
#include "hal_mcu.h"
#include "hal_board.h"

static __xdata uint8 usbsuspEnterXdata[14];
static __xdata VFPTR pUsbsuspEnterXdata = NULL;

__xdata VFPTR pFnSuspendEnterHook=  NULL;
__xdata VFPTR pFnSuspendExitHook=   NULL;



/** \brief Puts the chip into power mode 1 during USB suspend.
 *
 * This function must be called from main (i.e. not from interrupt context) upon the reception of a
 * \ref USBIRQ_EVENT_SUSPEND event. To comply with the USB specification, this must happen within 10 ms
 * after the event occurs. The chip will stay in power mode 1 until a USB resume or USB reset is detected
 * on the USB bus, or remote wakeup is used. During this period, the MCU can only run code from
 * interrupt context.
 */
void usbsuspEnter(void)
{
   HAL_LED_CLR();

   if (pFnSuspendEnterHook!=NULL)
       pFnSuspendEnterHook();

   // SLEEP &= ~0x03;     / ANL SLEEP, #0xFC
   usbsuspEnterXdata[0] = 0x53;
   usbsuspEnterXdata[1] = 0xBE;
   usbsuspEnterXdata[2] = 0xFC;

   // SLEEP |= 0x01;      / ORL SLEEP, #0x01
   usbsuspEnterXdata[3] = 0x43;
   usbsuspEnterXdata[4] = 0xBE;
   usbsuspEnterXdata[5] = 0x01;

   // PCON = 0x01;        / MOV PCON, #0x01
   usbsuspEnterXdata[6] = 0x75;
   usbsuspEnterXdata[7] = 0x87;
   usbsuspEnterXdata[8] = 0x01;

   // The CPU will be halted here while suspended (in PM1).

   // while (inSuspend);  / MOV A, inSuspend
   usbsuspEnterXdata[9] = 0xE5;
   usbsuspEnterXdata[10] = (uint8) ((__data uint8 *) &usbirqData.inSuspend);

   // ... Restart ...     / JNZ -12
   usbsuspEnterXdata[11] = 0x70;
   usbsuspEnterXdata[12] = 0xF3;

   // return;             / RET
   usbsuspEnterXdata[13] = 0x22;

   // Enable the resume and reset detection interrupt
   HAL_USB_RESUME_INT_CLEAR();
   HAL_USB_RESUME_INT_ENABLE();

   // Call the suspend loop routine from XDATA
   // The routine is in XDATA so that usbsuspExit() wipe out the power-down instruction in the loop (by
   // replacing "MOV PCON, #0x01" with three NOP instructions).
   pUsbsuspEnterXdata = (VFPTR) ((uint16) usbsuspEnterXdata);
   pUsbsuspEnterXdata();

   if (pFnSuspendExitHook!=NULL)
       pFnSuspendExitHook();

} // usbsuspEnter




/** \brief Attempts USB remote wakeup.
 *
 * This function can be called from interrupt context while the USB device is suspend mode. If the device
 * is privileged to do so (see \c usbfwData.remoteWakeup and the \ref USBSR_EVENT_REMOTE_WAKEUP_ENABLED
 * and \ref USBSR_EVENT_REMOTE_WAKEUP_DISABLED events), remote wakeup will be performed. Note that this
 * function will block for 10 ms while the resume signal is set on the bus. Note: This function can only
 * be called when the 48 MHz XOSC is stable.
 *
 * \return
 *     \c TRUE if the remote wakeup was performed (the privilege had been granted), otherwise \c FALSE
 *     (the device is still in suspend mode).
 */
uint8 usbsuspDoRemoteWakeup(void)
{

   // Make sure that it's OK
   if (!usbfwData.remoteWakeup) return FALSE;

   halIntOff();

   // Make sure that the suspend loop does not power down the chip again
   usbsuspStopPm1();
   usbirqData.inSuspend = FALSE;

   // Perform remote wakeup by holding the USB resume signal for 10 ms
   USBPOW |= 0x04;
   halMcuWaitMs(10); // ms
   USBPOW &= ~0x04;

   // Clear and disable the interrupt flag
   HAL_USB_RESUME_INT_DISABLE();
   HAL_USB_RESUME_INT_CLEAR();

   halIntOn();

   return TRUE;

} // usbsuspDoRemoteWakeup




/** \brief Internal function, do not call from application..
 *
 * This internal function is called by the P0 "USB resume and reset" detection interrupt, and by
 * \ref usbsuspDoRemoteWakeup() to prevent \ref usbsuspEnter() from putting the device back in power
 * mode 1. \ref usbsuspEnter() will, however, not break out of the suspend loop until the resume event
 * has been received.
 */
void usbsuspStopPm1(void)
{

   // Disable the "ORL SLEEP, #0x01" instruction to make sure we avoid having
   // SLEEP.MODE != 0 after MCU has resumed to active mode. Otherwise all port
   // and sleep timer interrupts will be blocked.
   usbsuspEnterXdata[5] = 0x00; // Change it to: "ORL SLEEP, #0x00" (no effect)

   // Remove the "MOV PCON, #0x01" instruction (which puts MCU into power mode 1) from the suspend
   // loop
   usbsuspEnterXdata[6] = 0x00; // NOP
   usbsuspEnterXdata[7] = 0x00; // NOP
   usbsuspEnterXdata[8] = 0x00; // NOP

} // usbsuspStopPm1

//@}
/*
+------------------------------------------------------------------------------
|  Copyright 2004-2007 Texas Instruments Incorporated. All rights reserved.
|
|  IMPORTANT: Your use of this Software is limited to those specific rights
|  granted under the terms of a software license agreement between the user who
|  downloaded the software, his/her employer (which must be your employer) and
|  Texas Instruments Incorporated (the "License"). You may not use this Software
|  unless you agree to abide by the terms of the License. The License limits
|  your use, and you acknowledge, that the Software may not be modified, copied
|  or distributed unless embedded on a Texas Instruments microcontroller or used
|  solely and exclusively in conjunction with a Texas Instruments radio
|  frequency transceiver, which is integrated into your product. Other than for
|  the foregoing purpose, you may not use, reproduce, copy, prepare derivative
|  works of, modify, distribute, perform, display or sell this Software and/or
|  its documentation for any purpose.
|
|  YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE
|  PROVIDED “AS IS” WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
|  INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE,
|  NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL
|  TEXAS INSTRUMENTS OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT,
|  NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER
|  LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES INCLUDING
|  BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
|  CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
|  SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
|  (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
|
|  Should you have any questions regarding your right to use this Software,
|  contact Texas Instruments Incorporated at www.TI.com.
|
+------------------------------------------------------------------------------
*/
