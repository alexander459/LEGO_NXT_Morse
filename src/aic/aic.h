#ifndef   AIC
#define   AIC


void      AICInit(void);
void      AICInterruptEnable(int which, int type, int priority, void (*handler)(void));
void      AICInterruptDisable(int which);
#endif
