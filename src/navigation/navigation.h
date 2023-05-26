#ifndef _NAVIGATION_
#define _NAVIGATION_
#include <stdconst.h>
enum navigation_states_t{MAIN_MENU, SEND, RECEIVE, BACK, EXIT, WRITE_TXT, VIEW_TXT, SEND_SOUND, SEND_LED, RECEIVE_MIC, RECEIVE_LIGHT, NOP,\
WRITING_TXT, SENDING_LED, SENDING_SOUND, RECEIVING_MIC, RECEIVING_LIGHT, \
FORWARD, BACKWARDS, LEFT, RIGHT, COMMANDS, SET_COMMANDS, SEND_COMMANDS, RECEIVE_COMMANDS, VIEW_CMD, RECEIVING_COMMANDS};

typedef enum navigation_states_t navigation_states;
extern struct nav_node root;

typedef struct nav_node{
    enum navigation_states_t state;
    int num_of_children;
    struct nav_node **children;
    int text_len;
    UBYTE **text;
    struct nav_node *back;
}navigation_node;


void NavTreeInit(void);

#endif
