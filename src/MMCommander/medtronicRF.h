#ifndef _MEDTRONICRF_H_
#define _MEDTRONICRF_H_

void sendMedtronicMessage (char *message, unsigned int length, int times);
char receiveMedtronicMessage (char *message, unsigned int *length);
void usbReceiveData (void);

#endif