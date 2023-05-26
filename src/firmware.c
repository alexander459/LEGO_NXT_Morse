#include <stdlib.h>
#include <string.h>
#include "AT91SAM7S256.h"
#include "hwinit/hwinit.h"
#include "aic/aic.h"
#include "lcd/display.h"
#include "sound/sound.h"
#include "i2c/i2c.h"
#include "input/input.h"
#include "button/button.h"
#include "output/output.h"
#include "led/led.h"
#include "timer/pit.h"
#include "timer/timer.h"
#include "morse/morse.h"
#include "navigation/navigation.h"

#define DOT_TIME          150
#define BETWEEN_TIME	    2 * DOT_TIME
#define DASH_TIME         3 * DOT_TIME
#define NEW_LETTER_TIME   5 * DOT_TIME
#define NEW_WORD_TIME     7 * DOT_TIME
#define MIC_SENSITIVITY   18

/* MORSE */
#define DOT               '.'
#define DASH              '_'
#define NEW_LETTER        '?'
#define NEW_WORD          ' '
#define STOP              '\0'
#define BETWEEN		        '*'

/* COMMANDS */
#define CMD_F             'F'
#define CMD_B             'B'
#define CMD_L             'L'
#define CMD_R             'R'
#define CMD_REQ           'Q'

#define MAIN_MENU_LEN     4
#define SEND_MENU_LEN     5
#define RECEIVE_MENU_LEN  3

#define MORSE_BUF_LEN     512
#define MORSE_CMD_BUF_LEN 512
#define STR_BUF_LEN       512
#define CMD_BUF_LEN       128


void exit_all(void);
void WellcomeSound(ULONG *pattern);
void PlayDot(ULONG *pattern);
void PlayDash(ULONG *pattern);

void ExecuteCommands(UBYTE *commands){
  UWORD i=0;
  while(commands[i]!='\0'){
    Sleep(1000);
    switch(commands[i]){
      case 'F':
        OutputSetSpeed (2,80);
        OutputSetSpeed (3,80);
        Sleep(2000);
        OutputSetSpeed (2,0);
        OutputSetSpeed (3,0);
        OutputSetSpeed (2,-70);
        OutputSetSpeed (3,-70);
        Sleep(60);
        OutputSetSpeed (2,0);
        OutputSetSpeed (3,0);
        break;
      case 'B':
        OutputSetSpeed (2,-80);
        OutputSetSpeed (3,-80);
        Sleep(2000);
        OutputSetSpeed (2,0);
        OutputSetSpeed (3,0);
        OutputSetSpeed (2,70);
        OutputSetSpeed (3,70);
        Sleep(60);
        OutputSetSpeed (2,0);
        OutputSetSpeed (3,0);
        break;
      case 'L':
        OutputSetSpeed (2,70);
        OutputSetSpeed (3,-70);
        Sleep(860);
        OutputSetSpeed (2,0);
        OutputSetSpeed (3,0);
        OutputSetSpeed (2,-70);
        OutputSetSpeed (3,70);
        Sleep(60);
        OutputSetSpeed (2,0);
        OutputSetSpeed (3,0);
        break;
      case 'R':
        OutputSetSpeed (2,-70);
        OutputSetSpeed (3,70);
        Sleep(860);
        OutputSetSpeed (2,0);
        OutputSetSpeed (3,0);
        OutputSetSpeed (2,70);
        OutputSetSpeed (3,-70);
        Sleep(60);
        OutputSetSpeed (2,0);
        OutputSetSpeed (3,0);
        break;
      default:
        break;
    }
    i++;
  }
  OutputSetSpeed (1,100);
  Sleep(2000);
  OutputSetSpeed (1,-100);
  Sleep(2000);
  OutputSetSpeed (1,0);
}

int main(void) {
  HardwareInit(); // need this to init PIOA clock
  AICInit();
  DisplayInit(); 
  PITEnable();
  I2CInit();

  InputInit();
  ButtonInit();
  SoundInit();
  NavTreeInit();
  StartTimer();
  LedSwitchOff(LIGHT_S);
  
  // WELCOME MESSAGE
  ULONG pattern[] = {0xFFFFFFFF};
  DisplayWelcomeFrame(SET, 25, 5, 30, 50);
  DisplayWelcomeLines(SET, 25, 5, 30, 50);
  DisplayUpdateSync();

  Sleep(1000);
  WellcomeSound(pattern);
  Sleep(3000);

  DisplayWelcomeFrame(CLEAR, 25, 5, 30, 50);
  DisplayWelcomeLines(CLEAR, 25, 5, 30, 50);
  DisplayLogo(LOGO_T, CLEAR, 8, 40);
  DisplayLogo(LOGO_D, CLEAR, 35, 40);
  DisplayLogo(LOGO_A, CLEAR, 70, 40);
  DisplayUpdateSync();
  

  UBYTE released = FALSE;
  UBYTE pressed = TRUE;
  ULONG ms_p = 0, ms_r = 0;
  ULONG time_released, time_pressed, idle_time = 0;

  UBYTE str_buf[STR_BUF_LEN];
  
  UBYTE morse_buf[MORSE_BUF_LEN] = {DOT, BETWEEN, DOT, BETWEEN, DOT, BETWEEN, DOT, NEW_LETTER, DOT, NEW_LETTER, DOT, BETWEEN, DASH, BETWEEN, DOT, BETWEEN, DOT, NEW_LETTER,  DOT, BETWEEN, DASH, BETWEEN, DOT, BETWEEN, DOT, NEW_LETTER, DASH, BETWEEN, DASH, BETWEEN, DASH, NEW_WORD, DASH, BETWEEN, DASH, BETWEEN, DOT, NEW_LETTER, DOT, BETWEEN, DOT, NEW_LETTER, DOT, BETWEEN, DASH, NEW_WORD, DASH, BETWEEN, DOT, BETWEEN, DASH, BETWEEN, DOT, NEW_LETTER, DOT, BETWEEN, DOT, BETWEEN, DOT, NEW_LETTER, DOT, BETWEEN, DASH, BETWEEN, DASH, BETWEEN, DOT};
  UWORD morse_buf_index = 0;

  UBYTE command_buf[CMD_BUF_LEN];     /* commands as strings */
  UWORD command_buf_index = 0;

  UBYTE morse_cmd_buf[MORSE_CMD_BUF_LEN];   /* COMMANDS AS MORSE */
  UWORD morse_cmd_index = 0;

  UBYTE first_time = TRUE;
  UBYTE first_time_set = TRUE;
  UBYTE btn_is_pressed = FALSE;
  UWORD t = FALSE;
  UWORD s;

  navigation_states current_nav_state = MAIN_MENU;
  navigation_node *current_node = &root;
  UBYTE index_of_children = 0;
  command_buf[0] = '\0';
  str_buf[0] = '\0';
  
  DisplayUpdateSync();
  while(1) {
    I2CTransfer();
    GetTouch(&t);
    GetSound(&s);
    
    switch(current_nav_state){
      case NOP:

        break;

      case MAIN_MENU:
        DisplayMenu(SET, current_node->text, current_node->text_len);
        DisplaySelectWord(SET, current_node->text, current_node->text_len, index_of_children);
        current_nav_state = NOP;
        break;

      case COMMANDS:
        first_time_set = TRUE;
        morse_cmd_index = 0;
        DisplayMenu(SET, current_node->text, current_node->text_len);
        DisplaySelectWord(SET, current_node->text, current_node->text_len, index_of_children);
        current_nav_state = NOP;
        break;
        /*DisplayString(SET, 0,0,"imhere");
        released = FALSE;
        pressed = TRUE;
        ms_p = 0;
        ms_r = 0;*/

      case SET_COMMANDS:
        if(first_time_set == TRUE){
          memset((UBYTE*)command_buf, 0, CMD_BUF_LEN);
          first_time_set = FALSE;
          command_buf_index = 0;
        }
        DisplayMenu(SET, current_node->text, current_node->text_len);
        DisplaySelectWord(SET, current_node->text, current_node->text_len, index_of_children);
        current_nav_state = NOP;
        break;

      case FORWARD:
        command_buf[command_buf_index++] = CMD_F;
        current_nav_state = current_node->children[index_of_children]->state;
        current_node = current_node->children[index_of_children];
        break;

      case BACKWARDS:
        command_buf[command_buf_index++] = CMD_B;
        current_nav_state = current_node->children[index_of_children]->state;
        current_node = current_node->children[index_of_children];
        break;

      case LEFT:
        command_buf[command_buf_index++] = CMD_L;
        current_nav_state = current_node->children[index_of_children]->state;
        current_node = current_node->children[index_of_children];
        break;

      case RIGHT:
        command_buf[command_buf_index++] = CMD_R;
        current_nav_state = current_node->children[index_of_children]->state;
        current_node = current_node->children[index_of_children];
        break;

      case VIEW_CMD:
        DisplayString(SET, 0, 0, command_buf);
        current_nav_state = NOP;
        break;

      case SEND_COMMANDS:
        command2Morse(command_buf, morse_cmd_buf);
        Sleep(500);
        DisplayMenu(SET, current_node->text, current_node->text_len);
        DisplayUpdateSync();
        for(morse_cmd_index=0; morse_cmd_buf[morse_cmd_index]!='\0'; morse_cmd_index++){
          switch(morse_cmd_buf[morse_cmd_index]){
            case DOT:
              PlayDot(pattern);
              break;
            case DASH:
              PlayDash(pattern);
              break;
            case BETWEEN:
              Sleep(BETWEEN_TIME);
              break;
            case NEW_LETTER:
              Sleep(NEW_LETTER_TIME);
              break;
            default:
              break;
          }
        }
        Sleep(500);
        current_nav_state = current_node->children[index_of_children]->state;
        DisplayMenu(CLEAR, current_node->text, current_node->text_len);
        current_node = current_node->children[index_of_children];
        break;

      case RECEIVE_COMMANDS:
        Sleep(350);
        DisplayMenu(SET, current_node->text, current_node->text_len);
        current_nav_state = current_node->children[index_of_children] -> state;
        current_node = current_node->children[index_of_children];
        released = FALSE;
        pressed = TRUE;
        first_time = TRUE;
        ms_p = 0;
        ms_r = 0;
        morse_buf_index = 0;
        memset((UBYTE*)morse_cmd_buf, 0, MORSE_CMD_BUF_LEN);
        memset((UBYTE*)command_buf, 0, CMD_BUF_LEN);
        idle_time = GetMs();
        break;

      case RECEIVING_COMMANDS:
        if((GetMs() - idle_time) > (NEW_WORD_TIME + 1000)){
          morse2command(morse_cmd_buf, command_buf);
          current_nav_state = current_node->children[index_of_children]->state;
          DisplayMenu(CLEAR, current_node->text, current_node->text_len);
          current_node = current_node->children[index_of_children];
          UBYTE *tmp[] = {"EXECUTING..."};
          DisplayUpdateSync();
          DisplayMenu(SET,tmp,1);
          ExecuteCommands(command_buf);
          break;
        }

      	if(s > MIC_SENSITIVITY){
          idle_time = GetMs();
          if(pressed == TRUE){
            ms_p = GetMs();
            time_released = ms_p - ms_r;

            if(first_time){
              time_released = 0;
            }

            first_time = FALSE;
            pressed = FALSE;
            released = TRUE;

            if(time_released >= NEW_LETTER_TIME - 50 && time_released < NEW_LETTER_TIME + 50){
              morse_cmd_buf[--morse_cmd_index]=NEW_LETTER;
              morse_cmd_index++;
            }else if(time_released >= NEW_WORD_TIME - 50){
              morse_cmd_buf[--morse_cmd_index]=NEW_WORD;
              morse_cmd_index++;
            }
          }
        }else{
          if(released == TRUE){
            ms_r = GetMs();
            time_pressed = ms_r - ms_p;
            released = FALSE;
            pressed = TRUE;
                  
            if(time_pressed < DOT_TIME + 50){
              morse_cmd_buf[morse_cmd_index++]= DOT;
              morse_cmd_buf[morse_cmd_index++]= BETWEEN;
            }else{
              morse_cmd_buf[morse_cmd_index++]=DASH;
              morse_cmd_buf[morse_cmd_index++]= BETWEEN;
            }
          }
        }
        break;


      case SEND:
      case RECEIVE:
        DisplayMenu(SET, current_node->text, current_node->text_len);
        DisplaySelectWord(SET, current_node->text, current_node->text_len, index_of_children);
        current_nav_state = NOP;
        break;

      case BACK:
        DisplayString(CLEAR, 0, 0, str_buf);
        DisplayString(CLEAR, 0, 0, command_buf);
        current_node = current_node->back;
        current_nav_state = current_node->state;
        break;

      case WRITE_TXT:
        DisplayMenu(SET, current_node->text, current_node->text_len);
        DisplaySelectWord(SET, current_node->text, current_node->text_len, index_of_children);
        current_nav_state = WRITING_TXT;
        current_node = current_node->children[index_of_children];
        released = FALSE;
        pressed = TRUE;
        first_time = TRUE;
        ms_p = 0;
        ms_r = 0;
        morse_buf_index = 0;
        memset((UBYTE*)morse_buf, 0, MORSE_BUF_LEN);
        memset((UBYTE*)str_buf, 0, STR_BUF_LEN);
        break;

      case WRITING_TXT:
        if(t == TRUE){
          if(pressed == TRUE){
            ms_p = GetMs();
            time_released = ms_p - ms_r;

            if(first_time){
              time_released = 0;
            }
            first_time = FALSE;
            pressed = FALSE;
            released = TRUE;

            if(time_released >= 500 && time_released < 1000){
              morse_buf[--morse_buf_index]=NEW_LETTER;
              morse_buf_index++;
            }else if(time_released >= 1000){
              morse_buf[--morse_buf_index]=NEW_WORD;
              morse_buf_index++;
            }
          }
        }else if(t == FALSE){
          if(released == TRUE){
            ms_r = GetMs();
            time_pressed = ms_r - ms_p;
            released = FALSE;
            pressed = TRUE;
                  
            if(time_pressed < 200){
              morse_buf[morse_buf_index++]= DOT;
              morse_buf[morse_buf_index++]= BETWEEN;
            }else{
              morse_buf[morse_buf_index++]=DASH;
              morse_buf[morse_buf_index++]= BETWEEN;
            }
          }
        }
        break;

      case VIEW_TXT:
        Morse2String(morse_buf, str_buf);
        DisplayString(SET, 0, 0, str_buf);
        current_nav_state = NOP;
        break;
              
      case SEND_SOUND:
        DisplayMenu(SET, current_node->text, current_node->text_len);
        DisplaySelectWord(SET, current_node->text, current_node->text_len, index_of_children);
        current_nav_state = current_node->children[index_of_children] -> state;
        current_node = current_node->children[index_of_children];
      	break;
      	
      case RECEIVE_MIC:
      	Sleep(350);
        DisplayMenu(SET, current_node->text, current_node->text_len);
        DisplaySelectWord(SET, current_node->text, current_node->text_len, index_of_children);
        current_nav_state = current_node->children[index_of_children] -> state;
        current_node = current_node->children[index_of_children];
        released = FALSE;
        pressed = TRUE;
        first_time = TRUE;
        ms_p = 0;
        ms_r = 0;
        morse_buf_index = 0;
        memset((UBYTE*)morse_buf, 0, MORSE_BUF_LEN);
        memset((UBYTE*)str_buf, 0, STR_BUF_LEN);
        idle_time = GetMs();
        break;

      case SENDING_SOUND:
        Sleep(500);
        for(morse_buf_index=0; morse_buf[morse_buf_index]!='\0'; morse_buf_index++){
          switch(morse_buf[morse_buf_index]){
            case DOT:
              PlayDot(pattern);
              break;
            case DASH:
              PlayDash(pattern);
              break;
            case BETWEEN:
              Sleep(BETWEEN_TIME);
              break;
            case NEW_LETTER:
              Sleep(NEW_LETTER_TIME);
              break;
            case NEW_WORD:
              Sleep(NEW_WORD_TIME);
              break;
            default:
              break;
          }
        }
        Sleep(500);
        current_nav_state = current_node->children[index_of_children]->state;
        DisplayMenu(CLEAR, current_node->text, current_node->text_len);
        current_node = current_node->children[index_of_children];
        break;

      case RECEIVING_MIC:
        if((GetMs() - idle_time) > (NEW_WORD_TIME + 1000)){
          current_nav_state = current_node->children[index_of_children]->state;
          DisplayMenu(CLEAR, current_node->text, current_node->text_len);
          current_node = current_node->children[index_of_children];
          break;
        }

      	if(s > MIC_SENSITIVITY){
          idle_time = GetMs();
          if(pressed == TRUE){
            ms_p = GetMs();
            time_released = ms_p - ms_r;

            if(first_time){
              time_released = 0;
            }

            first_time = FALSE;
            pressed = FALSE;
            released = TRUE;

            if(time_released >= NEW_LETTER_TIME - 50 && time_released < NEW_LETTER_TIME + 50){
              morse_buf[--morse_buf_index]=NEW_LETTER;
              morse_buf_index++;
            }else if(time_released >= NEW_WORD_TIME - 50){
              morse_buf[--morse_buf_index]=NEW_WORD;
              morse_buf_index++;
            }
          }
        }else{
          if(released == TRUE){
            ms_r = GetMs();
            time_pressed = ms_r - ms_p;
            released = FALSE;
            pressed = TRUE;
                  
            if(time_pressed < DOT_TIME + 50){
              morse_buf[morse_buf_index++]= DOT;
              morse_buf[morse_buf_index++]= BETWEEN;
            }else{
              morse_buf[morse_buf_index++]=DASH;
              morse_buf[morse_buf_index++]= BETWEEN;
            }
          }
        }
        break;
      
      case RECEIVING_LIGHT:
        break;
      case SENDING_LED:
        break;
      case RECEIVE_LIGHT:
        break;
      case SEND_LED:
        break;

      case EXIT:
        DisplayNum(SET,20,0,33);
        DisplayUpdateSync();
        I2CCtrl(POWERDOWN);
        break;
    }

    
    switch(ButtonRead()){
    case BUTTON_LEFT:               // MOVE TO THE PREV NAVIGATION OPTION
      if(btn_is_pressed)            // MAKE SURE THAT THE BUTTON IS TRIGGERED ONLY ONCE AFTER PRESSED
        break;
      btn_is_pressed = TRUE;
      DisplaySelectWord(CLEAR, current_node->text, current_node->text_len, index_of_children);    // CLEAR THE OLD ARROWS
      if(index_of_children == 0)
        index_of_children = current_node->num_of_children - 1;
      else
        index_of_children--;
      DisplaySelectWord(SET, current_node->text, current_node->text_len, index_of_children);      // SET NEW ARROWS
      break;

    case BUTTON_RIGHT:              // MOVE TO THE NEXT NAVIGATION OPTION
      if(btn_is_pressed)
        break;
      btn_is_pressed = TRUE;
      DisplaySelectWord(CLEAR, current_node->text, current_node->text_len, index_of_children);    // CLEAR THE OLD ARROWS
      index_of_children ++;
      index_of_children = index_of_children % current_node->num_of_children;
      DisplaySelectWord(SET, current_node->text, current_node->text_len, index_of_children);    // CLEAR THE OLD ARROWS
      break;

    case BUTTON_ENTER:
      if(btn_is_pressed)
        break;
      btn_is_pressed = TRUE;
      
      current_nav_state = current_node->children[index_of_children]->state;
      DisplayMenu(CLEAR, current_node->text, current_node->text_len);
      DisplaySelectWord(CLEAR, current_node->text, current_node->text_len, index_of_children);
      current_node = current_node->children[index_of_children];
      index_of_children = 0;
      break;

    case BUTTON_EXIT:
      if(btn_is_pressed)
        break;
      btn_is_pressed = TRUE;
      I2CCtrl(REPROGRAM);
      break;

    case BUTTON_NONE:
      btn_is_pressed = FALSE;
      break;

    default:
      break;
    }
    DisplayUpdateSync();
  }
  
  I2CExit();
  PITInterruptDisable();
  PITDisable();
  return 0;
}


void exit_all(void){
  ButtonExit();
  InputExit();
  OutputExit();
  SoundExit();
  DisplayExit();
  return;
}


void PlayDot(ULONG *pattern){
  ULONG _ms = GetMs();
  while(GetMs() - _ms < DOT_TIME) {
    I2CTransfer();
    SoundSync(pattern, sizeof(pattern), 250, 1);
  }
  return;
}

void PlayDash(ULONG *pattern){
  ULONG _ms = GetMs();
  while(GetMs() - _ms < DASH_TIME) {
    I2CTransfer();
    SoundSync(pattern, sizeof(pattern), 250, 1);
  }
  return;
}


void WellcomeSound(ULONG *pattern){
  UBYTE welcome_morse[] = {DASH, NEW_LETTER, DASH, DOT, DOT, NEW_LETTER, DOT, DASH, STOP};
  UBYTE current_morse;
  UBYTE i = 0;
  UBYTE letter = 0;
  // use array with trhee letters in order to print logos
  current_morse = welcome_morse[i++];
  while(current_morse != STOP){
    switch(current_morse){
      case DOT:
        PlayDot(pattern);
        Sleep(80);
        break;
      case DASH:
        PlayDash(pattern);
        Sleep(80);
        break;
      case NEW_LETTER:
        letter ++;
        if(letter == 1){
          DisplayLogo(LOGO_T, SET, 8, 40);
          DisplayUpdateSync();
        }else if(letter == 2){
          DisplayLogo(LOGO_D, SET, 35, 40);
          DisplayUpdateSync();
        }
        Sleep(NEW_LETTER_TIME);
        break;
    }
    current_morse = welcome_morse[i++];
  }
  DisplayLogo(LOGO_A, SET, 70, 40);
  DisplayUpdateSync();
  return;
}



