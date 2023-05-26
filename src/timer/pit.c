#include <stdconst.h>
#include "AT91SAM7S256.h"
#include "pit.h"
#include "../aic/aic.h"


void PITEnable(void){
    *AT91C_PITC_PIMR |= AT91C_PITC_PITEN;
    return;
}

void PITDisable(void){
    *AT91C_PITC_PIMR &= ~AT91C_PITC_PITEN;
    *AT91C_PITC_PIMR &= ~AT91C_PITC_PITIEN;
    return;
}

ULONG PITRead(void){
    return *AT91C_PITC_PIIR;
}

ULONG PITReadReset(void){
    return *AT91C_PITC_PIVR;
}

void PITSetPIV(ULONG period){
    *AT91C_PITC_PIMR &= ~AT91C_PITC_PIV;
    *AT91C_PITC_PIMR |= period;
    return;
}

/* MAKE SURE THAT YOU HAVE CALLED AIC INIT FIRST */
void PITInterruptEnable(ULONG period, void (*handler)(void)){
    PITSetPIV(period);
    *AT91C_PITC_PIMR |= AT91C_PITC_PITIEN;
    AICInterruptEnable(AT91C_ID_SYS, AT91C_AIC_SRCTYPE_INT_EDGE_TRIGGERED, AT91C_AIC_PRIOR_LOWEST, handler);
    return;
}

void PITInterruptDisable(void){
    AICInterruptDisable(AT91C_ID_SYS);
    return;    
}

UWORD PITTicks2ms(ULONG ticks){
    return ticks/3000;
}

UWORD PITTicks2s(ULONG ticks){
    return (PITTicks2ms(ticks)/3000);
}

UWORD ms2sec(UWORD ms){
    return ms/1000;
}

void spindelayms(ULONG ms){
    ULONG current_int;              /* CURRENT INTERVAL VALUE */
    ULONG prev_int;                 /* PREVIOUS INTERVAL VALUE */
    ULONG time_elapsed;             /* THE TIME ELAPSED IN MS */

    current_int = 0;
    prev_int = 0;
    time_elapsed = 0;

    PITSetPIV(2900);
    
    PITReadReset();                 /* RESET ONCE BEFORE START */
    
    while(time_elapsed <= ms){
        current_int = PITRead() >> 20;
        if(current_int != prev_int){
            time_elapsed++;
        }
        prev_int = current_int;
    }

    return;
}


void PITAckInterrupt(void){
    *AT91C_AIC_EOICR = 0xBABE;
    PITReadReset();
}
