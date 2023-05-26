#include "button.h"
#include <stdconst.h>
#include "../arm2avr.h"
#include "../lcd/display.h"
#include "../timer/pit.h"
#include "../i2c/i2c.h"


void ButtonInit(void){
    IoFromAvr.Buttons = 0;
    return;
}

void ButtonExit(void){
    IoFromAvr.Buttons = 0;
    return;
}


enum button_t ButtonRead(void){
    UWORD b = IoFromAvr.Buttons;
    if(b == 2047)
        return BUTTON_ENTER;
    else if(b >= 380 && b <= 406)
        return BUTTON_RIGHT;
    else if(b >= 90 && b <= 127)
        return BUTTON_LEFT;
    else if(b >= 750 && b <= 1023)
        return BUTTON_EXIT;
    else if(b == 0)
        return BUTTON_NONE;
}
