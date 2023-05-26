#include "timer.h"
#include "pit.h"
#include <stdconst.h>
#include "../lcd/display.h"
#include "../i2c/i2c.h"
#define _1MS_TICKS 3000

ULONG volatile ms;

void time_handler(void){
    ms++;
    PITAckInterrupt();
    return;
}

void StartTimer(void){
    ms = 0;
    PITEnable();
    PITInterruptEnable(_1MS_TICKS, time_handler);
    return;
}

void StopTimer(void){
    PITInterruptDisable();
    PITDisable();
    return;
}

void ResetTimer(void){
    StopTimer();
    StartTimer();
    return;
}

ULONG GetMs(void){
    return ms;
}

void Sleep(ULONG ms_to_wait){
    ULONG x = ms + ms_to_wait;
    while(ms <= x)
        I2CTransfer();
    return;
}

void I2CWait(void){
    ULONG x = ms + 2;   // wait 2 ms
    while(ms <= x);
    return;
}
