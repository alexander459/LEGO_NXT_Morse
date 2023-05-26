#include "input.h"
#include <stdconst.h>
#include "../arm2avr.h"

#include "../lcd/display.h"
void InputInit(void){
    UBYTE i;
    for(i=0; i<NOS_OF_AVR_INPUTS; i++)
        IoFromAvr.AdValue[i] = 1023;
    IoFromAvr.Buttons = 0x00;
    IoFromAvr.Battery = 0x00;
    return;
}

void InputExit(void){
    UBYTE i;
    for(i=0; i<NOS_OF_AVR_INPUTS; i++)
        IoFromAvr.AdValue[i] = 0x00;
    IoFromAvr.Buttons = 0x00;
    IoFromAvr.Battery = 0x00;
    return;
}

void InputGetSensorValue(UWORD *value, UBYTE port){
    *value = IoFromAvr.AdValue[port-1];
    return;
}

void GetTouch(UWORD *value){
    InputGetSensorValue(value, TOUCH_S);
    if(*value > 512){
        //DisplayNum(SET, 0,0,*value);
        //DisplayUpdateSync();
        *value = FALSE;
    }else{
        //DisplayNum(SET, 0,10,*value);
       // DisplayUpdateSync();
        *value = TRUE;
    }
    return;
}

void GetLight(UWORD *value){
    InputGetSensorValue(value, LIGHT_S);
    *value = 100 - (float)*value/(float)10;
    return;
}

void GetSound(UWORD *value){
    InputGetSensorValue(value, SOUND_S);
    *value = 100 - (float)*value/(float)10;
    return;
}

void GetUltrasonic(UWORD *value){
    InputGetSensorValue(value, ULTRASONIC_S);
    return;
}
