#ifndef __MORSE_H_
#define __MORSE_H_

#include <stdconst.h>

void Morse2String(UBYTE* m_buf, UBYTE* s_buf);
void command2Morse(UBYTE* cmd_buf, UBYTE* morse_buf);
void morse2command(UBYTE* morse_buf, UBYTE* cmd_buf);
#endif
