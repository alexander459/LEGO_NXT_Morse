#ifndef   SOUND
#define   SOUND

#include <stdconst.h>


extern ULONG *sound_pattern;
extern UBYTE sound_buf_len;
extern UBYTE sound_rate;
extern UWORD sound_duration;
extern UWORD sound_buf_index;

void SoundInit(void);
void SoundSync(ULONG *pattern, UBYTE length, UBYTE rate, UWORD duration);
void SoundExit(void);

void SoundEnable(void);
void SoundDisable(void);
void SoundInterruptDisable(void);
void SoundAsync(ULONG *pattern, UBYTE length, UBYTE rate, UWORD duration);
void SoundHandler(void);

#endif
