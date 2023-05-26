#include "output.h"
#include "../arm2avr.h"
#include <stdconst.h>

void OutputInit(void){
    UBYTE i;
    IoToAvr.Power = 0x0;            /* NORMAL OPERATION */
    IoToAvr.PwmFreq = 8;            /* INIT MOTORS */
    for(i=0; i<4; i++)
        IoToAvr.PwmValue[i] = 0;    /* SET MOTORS SPEED TO 0 */
    IoToAvr.OutputMode = BRAKE;     /* BRAKE THE MOTORS */
    return;
}

void OutputExit(void){
    UBYTE i;
    for(i=0; i<4; i++)
        IoToAvr.PwmValue[i] = 0;    /* SET MOTORS SPEED TO 0 */
    IoToAvr.OutputMode = BRAKE;     /* BRAKE THE MOTORS */
    return;
}

void OutputSetSpeed (UBYTE MotorNr, SBYTE Speed){
    if(MotorNr < 0 || MotorNr > 3)
        return;

    if(Speed > MAX_SPEED_FW)
        Speed = MAX_SPEED_FW;

    if(Speed < MAX_SPEED_RW)
        Speed = MAX_SPEED_RW;

    if(Speed == 0)
        IoToAvr.OutputMode = BRAKE;
    else
        IoToAvr.OutputMode = FLOAT;
        
    IoToAvr.PwmValue[MotorNr-1] = Speed;

    return;
}

SBYTE OutputGetSpeed(UBYTE MotorNr){
    return IoToAvr.PwmValue[MotorNr-1];
}

