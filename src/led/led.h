#ifndef _LED_H_
#define _LED_H_

/* use LED(port, 0)/LED(port, 1) to turn led off/on at port */
void LED(UBYTE port, UBYTE state);
void LedSwitchOn(UBYTE port);
void LedSwitchOff(UBYTE port);

#endif
