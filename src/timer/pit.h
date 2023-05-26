#ifndef   PIT
#define   PIT
#include <stdconst.h>

extern ULONG secs;
extern ULONG mins;
extern ULONG hrs;
extern ULONG interrupt_counter;

void      PITEnable(void);
void      PITDisable(void);
ULONG     PITRead(void);
ULONG     PITReadReset(void);
void      PITInterruptEnable(ULONG period, void (*handler)(void));
void      PITInterruptDisable(void);
void      PITAckInterrupt(void);
UWORD	  PITTicks2ms(ULONG ticks);
UWORD	  PITTicks2s(ULONG ticks);
UWORD     ms2sec(UWORD ms);
void 	  spindelayms(ULONG ms);

void PITSetPIV(ULONG period);
#endif
