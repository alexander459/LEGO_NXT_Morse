#ifndef   _BUTTON_H_
#define   _BUTTON_H_
#include<stdconst.h>
enum button_t {
  BUTTON_NONE,
  BUTTON_LEFT,
  BUTTON_ENTER,
  BUTTON_RIGHT,
  BUTTON_EXIT 
};

void   ButtonInit(void);
void   ButtonExit(void);
enum button_t ButtonRead(void);

#endif
