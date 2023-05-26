#ifndef _TIMER_H_
#define _TIMER_H_
#include <stdconst.h>


void StartTimer(void);
void StopTimer(void);
void ResetTimer(void);
ULONG GetMs(void);
void Sleep(ULONG ms_to_wait);
void I2CWait(void);

#endif
