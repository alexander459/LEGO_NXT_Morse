#include "morse.h"
#include <string.h>
#include <stdconst.h>
#include "../lcd/display.h"
#define MORSE_LEN_STR       26
#define CHARS_LEN           26
#define UNKNOWN_SYMBOL_STR  26
#define UNKNOWN_SYMBOL_CMD  5
#define MORSE_LEN_CMD       5


static char* Morse[] = {"._", "_...", "_._.", "_..", ".", ".._.", "__.", "....", "..", ".___", "_._", "._..", "__", "_.", "___", ".__.", "__._", "._.", "...", "_", ".._",\
                "..._", ".__", "_.._", "_.__", "__.."};
static char  Chars[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '*'};

static char* Morse_commands[] = {"_____", "____.", "___..", "__...", "_...."};
static char  Commands[] = {'Q', 'F', 'B', 'L', 'R', '*'};

static UBYTE findMorseStr(UBYTE* morse){
    UBYTE i;
    for(i=0; i<MORSE_LEN_STR; i++){
        if(!strcmp((char*)morse, (char*)Morse[i]))
            return i;
    }
    return UNKNOWN_SYMBOL_STR;
}


static UBYTE findMorseCmd(UBYTE* morse){
    UBYTE i;
    for(i=0; i<MORSE_LEN_CMD; i++){
        if(!strcmp((char*)morse, (char*)Morse_commands[i]))
            return i;
    }
    return UNKNOWN_SYMBOL_CMD;
}


void morse2command(UBYTE* morse_buf, UBYTE* cmd_buf){
    UWORD morse_buffer_index = 0;
    UWORD cmd_buffer_index = 0;
    UBYTE tmp_buffer_index = 0;
    UBYTE tmp_buf[6];

    while(morse_buf[morse_buffer_index] != '\0'){

        if(morse_buf[morse_buffer_index] != '?' && morse_buf[morse_buffer_index] != '*'){               // ? BETWEEN LETTERS
            if(tmp_buffer_index < 6){
                tmp_buf[tmp_buffer_index ++] = morse_buf[morse_buffer_index];
                tmp_buf[tmp_buffer_index] = '\0';
            }
        }else if(morse_buf[morse_buffer_index] == '?'){
            cmd_buf[cmd_buffer_index ++] = Commands[findMorseCmd(tmp_buf)];
            tmp_buffer_index = 0;
        }

        morse_buffer_index ++;
    }
    cmd_buf[cmd_buffer_index ++] = Commands[findMorseCmd(tmp_buf)];                      // LAST LETTER IS NOT SEPERATED WITH ?
    cmd_buf[cmd_buffer_index] = '\0';
    return;
}
 


void command2Morse(UBYTE* cmd_buf, UBYTE* morse_buf){
    UWORD cmd_buffer_index = 0;
    UWORD morse_buffer_index = 0;
    UWORD morse_cmd_index, i;

    while(cmd_buf[cmd_buffer_index] != '\0'){
        switch(cmd_buf[cmd_buffer_index]){
            case 'F':
                morse_cmd_index = 1;
                break;
            case 'B':
                morse_cmd_index = 2;
                break;
            case 'L':
                morse_cmd_index = 3;
                break;
            case 'R':
                morse_cmd_index = 4;
                break;
            default:
                break;
        }
        for(i=0; Morse_commands[morse_cmd_index][i] != '\0'; i++){
            morse_buf[morse_buffer_index++] = Morse_commands[morse_cmd_index][i];
            morse_buf[morse_buffer_index++] = '*';
        }
        morse_buf[--morse_buffer_index] = '?';
        morse_buffer_index++;
        cmd_buffer_index ++;
    }
    morse_buf[morse_buffer_index] = '\0';
    return;
}

void Morse2String(UBYTE* m_buf, UBYTE* s_buf){
    UWORD morse_buffer_index = 0;
    UWORD string_buffer_index = 0;
    UBYTE tmp_buffer_index = 0;
    UBYTE tmp_buf[5];
    while(m_buf[morse_buffer_index] != '\0'){

        if(m_buf[morse_buffer_index] != '?' && m_buf[morse_buffer_index] != ' ' && m_buf[morse_buffer_index] != '*'){               // ? BETWEEN LETTERS
            if(tmp_buffer_index < 5){
                tmp_buf[tmp_buffer_index ++] = m_buf[morse_buffer_index];
                tmp_buf[tmp_buffer_index] = '\0';
            }

        }else if(m_buf[morse_buffer_index] == '?'){
            s_buf[string_buffer_index ++] = Chars[findMorseStr(tmp_buf)];
            tmp_buffer_index = 0;

        }else if(m_buf[morse_buffer_index] == ' '){
            s_buf[string_buffer_index ++] = Chars[findMorseStr(tmp_buf)];
            s_buf[string_buffer_index ++] = ' ';
            tmp_buffer_index = 0;
        }
        morse_buffer_index ++;
    }
    s_buf[string_buffer_index ++] = Chars[findMorseStr(tmp_buf)];                      // LAST LETTER IS NOT SEPERATED WITH ?
    s_buf[string_buffer_index] = '\0';
    return;
}

