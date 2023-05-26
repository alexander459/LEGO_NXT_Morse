#ifndef   _OUTPUT_H_
#define   _OUTPUT_H_
#include <stdconst.h>

#define	  MAX_SPEED_FW (100)
#define	  MAX_SPEED_RW (-MAX_SPEED_FW)

#define A 1
#define B 2
#define C 3

void      OutputInit(void);
void      OutputExit(void);
void OutputSetSpeed (UBYTE MotorNr, SBYTE Speed);
SBYTE OutputGetSpeed(UBYTE MotorNr);
#endif
