#ifndef   DISPLAY
#define   DISPLAY

#include <stdconst.h>

#define SET 1
#define CLEAR 0

#define RIGHT_ARROW 2
#define LEFT_ARROW  1

#define LOGO_T      0
#define LOGO_D      1
#define LOGO_A      2

void      DisplayInit(void);
void      DisplayExit(void);
void      DisplayErase(void);
UBYTE     DisplayWrite(UBYTE type, UBYTE *buf, UWORD len);
void      DisplayUpdateSync(void);
void      DisplayChar(UBYTE cmd, UBYTE X,UBYTE Y,UBYTE Char);
void      DisplayNum(UBYTE cmd, UBYTE X,UBYTE Y,ULONG Num);
void      DisplayString(UBYTE cmd, UBYTE X,UBYTE Y,UBYTE *pString);
void      DisplaySetPixel(UBYTE cmd, UBYTE X,UBYTE Y);
void      DisplayLineX(UBYTE cmd, UBYTE X, UBYTE Y1, UBYTE Y2);
void      DisplayLineY(UBYTE cmd, UBYTE X, UBYTE Y1, UBYTE Y2);
void      draw_line(int cmd, int weight, int _x0, int _y0, int _x1, int _y1);

void      DisplayLogo(UBYTE letter, UBYTE cmd, UBYTE X, UBYTE Y);
void      DisplayMenu(UBYTE cmd, UBYTE** words, UBYTE len);
void      DisplaySelectWord(UBYTE cmd, UBYTE** words, UBYTE len, UBYTE selected_word_index);
void      DisplayWelcomeLines(UBYTE cmd, UBYTE X, UBYTE Y, UBYTE height, UBYTE width);
void      DisplayWelcomeFrame(UBYTE cmd, UBYTE X, UBYTE Y, UBYTE height, UBYTE width);
#endif
