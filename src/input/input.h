#ifndef   _INPUT_H
#define   _INPUT_H
#include <stdconst.h>

#define TOUCH_S         1
#define LIGHT_S         2
#define SOUND_S         3
#define ULTRASONIC_S    4

void InputInit(void);
void InputExit(void);
void InputGetSensorValue(UWORD *value, UBYTE port);

void GetTouch(UWORD *value);
void GetLight(UWORD *value);
void GetSound(UWORD *value);
void GetUltrasonic(UWORD *value);
#endif
