#ifndef _CONFIGURATION_H_
#define _CONFIGURATION_H_

// The _USA_FREQUENCY_MODE_ must be set to 1 if you are planning to use 
// this interface in the US and must be set to 0 in Europe and Asia. When set
// to 1 the operation frequency will be 916.5 MHz and when 0 will be 868.35 
// MHz.
#define _USA_FREQUENCY_MODE_ 0

// The parameter _TX_ENABLE_ must be set to 1 if you want the interface
// to be capable of transmitting any signal to the air. If 0, the MMCommander
// will act as a sniffer only.
#define _TX_ENABLE_ 1

// The parameter _TX_FILTER_ENABLE_ must be set to 1 if you want the interface
// to filter certain types of messages from being sent. This mode will be 
// the default and can be disabled pressing the push button for a couple
// seconds until the led turns on. To re-enable it, press the push button 
// again until the led turns off. If the _TX_FILTER_ENABLE_ parameter is set 
// to 0, the push button and the led wont have any effect on the interface 
// and any message will be transmitted. This filter will prevent a possible 
// hacker from sending commands to your pump in case your smartphone or 
// computer gets hacked. USE THIS MODE WITH CAUTION.
#define _TX_FILTER_ENABLE_ 1

// The parameter _SUPPORT_SUSPEND_WHEN_FILTERED_ must be set to 1
// if you want the interface to allow SUSPEND commands in filtered mode.
// This will allow us to emulate the Medtronic's "Low Glucose Suspend".
#define _SUPPORT_SUSPEND_WHEN_FILTERED_ 1

// The parameter _REPEATED_COMMAND_ENABLED_ must be set to 1 if you
// want the MMCommander to send a 0x04 command each time the received 
// message is exactly the same as the previous one. If 0 you'll receive
// the whole message every time.
#define _REPEATED_COMMAND_ENABLED_ 1

// The version number is defined here for identification purposes.
#define _MMCOMMANDER_VERSION_ 89

#endif