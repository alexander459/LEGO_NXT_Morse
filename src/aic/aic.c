#include "aic.h"
#include "AT91SAM7S256.h"
#include <stdconst.h>

void AICInit(void){
    /* DISABLING ALL INTERRUPT SOURCES 0-31*/
    *AT91C_AIC_IDCR = 0xFFFFFFFF;
    return;
}

void AICInterruptEnable(int which, int type, int priority, void (*handler)(void)){
    
    /* ENABLE SOURCE */
    *AT91C_AIC_IECR |= 0x1 << which;

    /* SET SMR SOURCE TYPE TO INTERNAL POSITIVE EDGED */
    AT91C_BASE_AIC->AIC_SMR[which] &= ~AT91C_AIC_SRCTYPE;
    AT91C_BASE_AIC->AIC_SMR[which] |= type;

    /* SET SMR OF which PRIORITY TO 0 */
    AT91C_BASE_AIC->AIC_SMR[which] &= ~AT91C_AIC_PRIOR;
    AT91C_BASE_AIC->AIC_SMR[which] |= priority;

    /* SET THE HANDLER ADDRESS IN CORRESPONDING SVR */
    AT91C_BASE_AIC->AIC_SVR[which] = handler;
    return;
}

void AICInterruptDisable(int which){
    *AT91C_AIC_IDCR |= 0x1 << which;
    *AT91C_AIC_ICCR |= 0x1 << which;
    return;
}
