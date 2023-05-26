#include <stdconst.h>
#include "../AT91SAM7S256.h"
#include "../sound/sound.h"
#include "../aic/aic.h"

ULONG *sound_pattern;
UBYTE sound_buf_len;
UBYTE sound_rate;
UWORD sound_duration;
UWORD sound_buf_index;


void SoundEnable(void){
  *AT91C_PIOA_PER = AT91C_PA17_TD;
  return;
}

void SoundDisable(void){
  *AT91C_PIOA_PDR = AT91C_PA17_TD;
  return;
}

void SoundInterruptDisable(void){
  AICInterruptDisable(AT91C_ID_PIOA);
  *AT91C_SSC_IDR = 0xFFFFFFFF;
  *AT91C_PIOA_IDR = AT91C_PA17_TD;
  return;
}

void SoundInterruptEnable(void (*handler)(void)){
  *AT91C_SSC_IDR = 0xFFFFFFFF;
  *AT91C_SSC_IER = 0x00000001;
  *AT91C_PIOA_IER = AT91C_PA17_TD;
  AICInterruptEnable(AT91C_ID_PIOA, AT91C_AIC_SRCTYPE_INT_EDGE_TRIGGERED, AT91C_AIC_PRIOR_LOWEST, handler);
  return;
}

void SoundInit(void){
  SoundInterruptDisable();
  SoundDisable();
  *AT91C_PMC_PCER   = (1L << AT91C_ID_SSC); /* Enable MCK clock */
  *AT91C_PIOA_ODR   = AT91C_PA17_TD;
  *AT91C_PIOA_OWDR  = AT91C_PA17_TD;
  *AT91C_PIOA_MDDR  = AT91C_PA17_TD;
  *AT91C_PIOA_PPUDR = AT91C_PA17_TD;
  *AT91C_PIOA_IFDR  = AT91C_PA17_TD;
  *AT91C_PIOA_CODR  = AT91C_PA17_TD;
  *AT91C_PIOA_IDR   = AT91C_PA17_TD;
  *AT91C_SSC_CR   = AT91C_SSC_SWRST;
  *AT91C_SSC_TCMR = AT91C_SSC_CKS_DIV + 
                    AT91C_SSC_CKO_CONTINOUS + AT91C_SSC_START_CONTINOUS;
  *AT91C_SSC_TFMR = (-1)+(((sizeof(ULONG)*8) & 0xF) << 8) + AT91C_SSC_MSBF;
  *AT91C_SSC_CR   = AT91C_SSC_TXEN;         /* TX enable */
}

void SoundSync(ULONG *pattern, UBYTE length, UBYTE rate, UWORD duration) {
  /* sound frequency will be (OSC/CMR*2)
     with CMR divisor values of 12 bits we can get a frequency range of 6KHz to 24MHz
     use rate values 0..255 to represent the range of 6KHz..24KHz, so div values 1K..4K
     for rate=0 => div=4K => freq ~= 6KHz and 
     for rate=255 => div=1K => freq ~= 24KHz
     *AT91C_SSC_CMR   = ((OSC / (2*sizeof(ULONG)*8)) / rate) + 1; 
   */
  UBYTE i=0;
  SoundEnable();
  *AT91C_SSC_CMR = (4095 - 12 * rate);
  while(duration-- > 0){
    for(i=0; i<length; i++){
      while(!(*AT91C_SSC_SR & AT91C_SSC_TXRDY)){/* wait */;};
      *AT91C_SSC_THR = pattern[i];
    }
  }
  SoundDisable();
  return;
}

void SoundAsync(ULONG *pattern, UBYTE length, UBYTE rate, UWORD duration){
  sound_pattern = pattern;
  sound_buf_len = length;
  sound_rate = rate;
  sound_duration = duration;
  sound_buf_index = 0;
  SoundEnable();
  sound_duration--;
  *AT91C_SSC_THR = pattern[sound_buf_index];
  sound_buf_index++;
  SoundSync(pattern, length, rate, duration);
  SoundInterruptEnable(SoundHandler);
  return;
}
//ULONG i = 0;
void SoundHandler(void){
  /* HANDLER HAS BEEN CALLED */
  /* THIS MEANS THAT AN INTERRUPD HAS BBEN TRIGGERES --> THR IS EMPTY*/
  /* FILL IT AGAIN IF THERE IS ANY DURATION LEFT! */
  /* IF NOT, DISABLE THE SOUND AND ITS INTERRUPTS */
  //ULONG i=0;
  if(sound_buf_index > sound_buf_len)
    sound_buf_index = 0;
  if(sound_duration>0){
    sound_duration--;
    *AT91C_SSC_THR = sound_pattern[sound_buf_index];
    sound_buf_index++;
  }else{
    SoundDisable();
    SoundInterruptDisable();
  }
  return;
}


void SoundExit(void){
  SoundDisable();
  SoundInterruptDisable();
}
