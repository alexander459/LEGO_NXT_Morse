#include "navigation.h"
#include <stdconst.h>

#define NUM_OF_STATES 11
UBYTE *root_text[] = {(UBYTE*)"SEND",  (UBYTE*)"RECEIVE", (UBYTE*)"COMMANDS", (UBYTE*)"EXIT"};
UBYTE *send_text[] = {(UBYTE*)"WRITE TEXT", (UBYTE*)"VIEW TEXT", (UBYTE*)"SEND SOUND", (UBYTE*)"SEND LED", (UBYTE*)"BACK"};
UBYTE *receive_text[] = {(UBYTE*)"RECEIVE MIC", (UBYTE*)"RECEIVE LIGHT", (UBYTE*)"VIEW TEXT", (UBYTE*)"BACK"};
UBYTE *commands_text[] = {(UBYTE*)"SET COMMANDS", (UBYTE*)"VIEW COMMANDS", (UBYTE*)"SEND COMMANDS", (UBYTE*)"RCVE COMMANDS", (UBYTE*)"BACK"};


UBYTE *write_txt_text[] = {(UBYTE*)"USE THE TOUCH"};
UBYTE *writing_txt_text[] = {(UBYTE*)"USE THE TOUCH"};

UBYTE *send_led_text[] = {(UBYTE*)"SENDING LED"};
UBYTE *sending_led_text[] = {(UBYTE*)"SENDING LED"};

UBYTE *send_sound_text[] = {(UBYTE*)"SEND SOUND"};
UBYTE *sending_sound_text[] = {(UBYTE*)"SEND SOUND"};

UBYTE *receive_mic_text[] = {(UBYTE*)"RECEIVING"};
UBYTE *receive_light_text[] = {(UBYTE*)"RECEIVING"};

UBYTE *set_commands_text[] = {(UBYTE*)"FORWARD 5", (UBYTE*)"BACKWARDS 5", (UBYTE*)"LEFT 90", (UBYTE*)"RIGHT 90", (UBYTE*)"BACK" };
UBYTE *receive_commands_text[] = {(UBYTE*)"RECEIVING"};

/* LEAF NODES */
struct nav_node back2main_node = {BACK, 0, NULL, 0, NULL, NULL};
struct nav_node back2send_node = {BACK, 0, NULL, 0, NULL, NULL};
struct nav_node back2receive_node = {BACK, 0, NULL, 0, NULL, NULL};
struct nav_node back2set_node = {BACK, 0, NULL, 0, NULL, NULL};
struct nav_node back2commands_node = {BACK, 0, NULL, 0, NULL, NULL};
struct nav_node exit_node = {EXIT, 0, NULL, 0, NULL, NULL};

/* SENDING NODES */
struct nav_node *writing_txt_children[] = {&back2send_node};
struct nav_node writing_txt_node = {WRITING_TXT, 1, writing_txt_children, 1, writing_txt_text, NULL};
struct nav_node *write_txt_children[] = {&writing_txt_node};
struct nav_node write_txt_node = {WRITE_TXT, 1, write_txt_children, 1, write_txt_text, NULL};

struct nav_node *view_txt_snd_children[] = {&back2send_node};
struct nav_node view_txt_snd_node = {VIEW_TXT, 1, view_txt_snd_children, 0, NULL, NULL};

struct nav_node *sending_led_children[] = {&back2send_node};
struct nav_node sending_led_node = {SENDING_LED, 1, sending_led_children, 1, sending_led_text, NULL};
struct nav_node *send_led_children[] = {&sending_led_node};
struct nav_node send_led_node = {SEND_LED, 1, send_led_children, 1, send_led_text, NULL};

struct nav_node *sending_sound_children[] = {&back2send_node};
struct nav_node sending_sound_node = {SENDING_SOUND, 1, sending_sound_children, 1, sending_sound_text, NULL};
struct nav_node *send_sound_children[] = {&sending_sound_node};
struct nav_node send_sound_node = {SEND_SOUND, 1, send_sound_children, 1, send_sound_text, NULL};

struct nav_node *send_children[] = {&write_txt_node, &view_txt_snd_node, &send_sound_node, &send_led_node, &back2main_node};
struct nav_node send_node = {SEND, 5, send_children, 5, send_text, NULL};


/* RECEIVE NODES */
struct nav_node *rcving_mic_children[] = {&back2receive_node};
struct nav_node rcving_mic_node = {RECEIVING_MIC, 1, rcving_mic_children, 1, receive_mic_text, NULL};
struct nav_node *rcv_mic_children[] = {&rcving_mic_node};
struct nav_node rcv_mic_node = {RECEIVE_MIC, 1, rcv_mic_children, 1, receive_mic_text, NULL};

struct nav_node *rcving_light_children[] = {&back2receive_node};
struct nav_node rcving_light_node = {RECEIVING_LIGHT, 1, rcving_light_children, 1, receive_mic_text, NULL};
struct nav_node *rcv_light_children[] = {&rcving_light_node};
struct nav_node rcv_light_node = {RECEIVE_LIGHT, 1, rcv_light_children, 1, receive_mic_text, NULL};

struct nav_node *view_txt_rcv_children[] = {&back2receive_node};
struct nav_node view_txt_rcv_node = {VIEW_TXT, 1, view_txt_rcv_children, 0, NULL, NULL};

struct nav_node *recv_children[] = {&rcv_mic_node, &rcv_light_node, &view_txt_rcv_node, &back2main_node};
struct nav_node rcv_node = {RECEIVE, 4, recv_children, 4, receive_text, NULL};


/* COMMAND NODES */
struct nav_node *view_cmd_children[] = {&back2commands_node};
struct nav_node view_cmd_node = {VIEW_CMD, 1, view_cmd_children, 0, NULL, NULL};

struct nav_node *forward_node_children[] = {&back2set_node};
struct nav_node forward_node = {FORWARD, 1, forward_node_children, 0, NULL, NULL};

struct nav_node *backwards_node_children[] = {&back2set_node};
struct nav_node backwards_node = {BACKWARDS, 1, backwards_node_children, 0, NULL, NULL};

struct nav_node *right_node_children[] = {&back2set_node};
struct nav_node right_node = {RIGHT, 1, right_node_children, 0, NULL, NULL};

struct nav_node *left_node_children[] = {&back2set_node};
struct nav_node left_node = {LEFT, 1, left_node_children, 0, NULL, NULL};

struct nav_node *set_commands_children[] = {&forward_node, &backwards_node, &left_node, &right_node, &back2commands_node};
struct nav_node set_commands_node = {SET_COMMANDS, 5, set_commands_children, 5, set_commands_text, NULL};

struct nav_node *send_commands_children[] = {&back2commands_node};
struct nav_node send_commands_node = {SEND_COMMANDS, 1, send_commands_children, 0, NULL, NULL};

struct nav_node *receiving_commands_children[] = {&back2commands_node};
struct nav_node receiving_commands_node = {RECEIVING_COMMANDS, 1, receiving_commands_children, 1, receive_commands_text, NULL};
struct nav_node *receive_commands_children[] = {&receiving_commands_node};
struct nav_node receive_commands_node = {RECEIVE_COMMANDS, 1, receive_commands_children, 1, receive_commands_text, NULL};

struct nav_node *commands_children[] = {&set_commands_node, &view_cmd_node, &send_commands_node, &receive_commands_node, &back2main_node};
struct nav_node commands_node = {COMMANDS, 5, commands_children, 5, commands_text, NULL};

/* ROOT NODE */
struct nav_node *root_children[] = {&send_node, &rcv_node, &commands_node, &exit_node};
struct nav_node root = {MAIN_MENU, 4, root_children, 4, root_text, NULL};



void NavTreeInit(void){
    back2send_node.back = &send_node;
    back2receive_node.back = &rcv_node;
    back2main_node.back = &root;
    back2set_node.back = &set_commands_node;
    back2commands_node.back = &commands_node;
    return; 
}
