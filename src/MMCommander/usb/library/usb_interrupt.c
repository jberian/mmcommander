/***********************************************************************************

    Filename:     usb_interrupt.c

    Description:  USB library interrupt initialisation and ISR.

***********************************************************************************/

/// \addtogroup module_usb_interrupt
/// @{
#define USBINTERRUPT_C ///< Modifies the behavior of "EXTERN" in usb_interrupt.h
#include "usb_firmware_library_headers.h"
#include "hal_board.h"
#include "clock.h"


/** \brief Initializes the \ref module_usb_interrupt module
 *
 * This function should be called after the \ref module_usb_framework module has been initialized.
 * Use interrupt group priority control (refer to the CC2511/CC1111 datasheet) to adjust the priority of the
 * USB interrupt relative to other interrupts.
 *
 * \param[in]       irqMask
 *     A bit mask containing USBIRQ_EVENT bits for all events that shall be reported
 */
void usbirqInit(uint16 irqMask)
{
   // Initialize variables
   usbirqData.eventMask = 0x0000;
   usbirqData.inSuspend = FALSE;
   usbirqData.irqMask = irqMask;

   // Select IRQ flags to handle
   USBCIE = irqMask;
   USBIIE = irqMask >> 4;
   USBOIE = (irqMask >> 9) & 0x3E;

   // Configure P0 for rising edge detection on P0[7:4], but keep the interrupt disabled until it is
   // needed.
   PICTL |= 0x10;
   HAL_USB_RESUME_INT_CLEAR();
   HAL_USB_INT_CLEAR();
   HAL_USB_INT_ENABLE();

} // usbirqInit




/** \brief USB interrupt handler
 *
 * Clears the P2 interrupt flag and converts all USB interrupt flags into events.
 * The interrupt also lets \ref usbsuspEnter() break from the suspend loop.
 */
#pragma vector=P2INT_VECTOR
__interrupt void usbirqHandler(void)
{
   uint8 usbcif;

   // First make sure that the crystal oscillator is stable
   while (!IS_XOSC_STABLE());

   // Special handling for reset interrupts
   usbcif = USBCIF;
   if (usbcif & USBCIF_RSTIF) {

       // All interrupts (except suspend) are by default enabled by hardware, so
       // re-initialize the enable bits to avoid unwanted interrupts
       USBCIE = usbirqData.irqMask;
       USBIIE = usbirqData.irqMask >> 4;
       USBOIE = (usbirqData.irqMask >> 9) & 0x3E;

       // Enable suspend mode when suspend signaling is detected on the bus
       USBPOW |= USBPOW_SUSPEND_EN;
   }

   // Record events (keeping existing)
   usbirqData.eventMask |= (uint16) usbcif;
   usbirqData.eventMask |= (uint16) USBIIF << 4;
   usbirqData.eventMask |= (uint16) USBOIF << 9;

   // If we get a suspend event, we should always enter suspend mode. We must,
   // however be sure that we exit the suspend loop upon resume or reset
   // signaling.
   if (usbcif & USBCIF_SUSPENDIF) {
      usbirqData.inSuspend = TRUE;
   }
   if (usbcif & (USBCIF_RSTIF | USBCIF_RESUMEIF)) {
      usbirqData.inSuspend = FALSE;
   }

   // Hand them over to the application
   usbirqHookProcessEvents();

   // Clear the P2 interrupt flag
   HAL_USB_INT_CLEAR();

} // usbirqHandler




/** \\brief USB resume interrupt handler
 *
 * This routine clears the USB resume interrupt flag, and makes sure that MCU does not return to power
 * mode 1 again until the the suspend loop has been exited.
 */
#pragma vector = P0INT_VECTOR
__interrupt void usbirqResumeHandler(void)
{
   uint8 flags;

   // First make sure that the crystal oscillator is stable
   while (!IS_XOSC_STABLE());

   // Clear and disable the interrupt flag
   flags = P0IFG;
   if (flags & 0x80) {
      // We have a USB_RESUME interrupt (which could also be a USB reset)
      HAL_USB_RESUME_INT_DISABLE();
      usbsuspStopPm1();
   }

   HAL_USB_RESUME_INT_CLEAR();

} // usbResumeHandler

/// @}

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
