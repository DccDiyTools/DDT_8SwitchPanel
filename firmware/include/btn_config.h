#ifndef DDT_BTN_CFG
#define DDT_BTN_CFG

#include <Arduino.h>

/*
  This file defines all the configuration protocol for the buttons

  A line defines a state (mode) of a button (id field), the position (Row, Col) and the led position (on the ws8212C strip)
  Each configuration Line is 16bits in 6 fields

  bits:
  MM NNNN CC RR E LLLLL
  
  fields:
  MM: Mode see Mode table
  NNNN: Button Number (0-15)
  CC: Column where is located
  RR: Row where is located
  E: Extended (reserved future)
  LLLLL: Led (0-32)
 */


struct s_btn_cfg{
    u_int16_t led:5;
    u_int16_t ext:1;
    u_int16_t row:2;
    u_int16_t col:2;
    u_int16_t id:4;
    u_int16_t mode:2;
};

typedef struct s_btn_cfg t_btn_cfg;

union u_btn_config{
    u_int16_t num;
    t_btn_cfg cfg;
};

typedef union u_btn_config tu_btn_config;

/*
 Mode Table

 This device supports Simple buttons (on - off state) and 2 ways switchs (on_1 - off? - on_2)

 Simple Buttons:
  * This is a button than only has 2 states, closed (on) or open (off)
  * Needs two lines, the first one must be one with mode equal to 0
  * If a line starts with mode 0, it means that the button "id" is simple, 
    row and col indicates the matrix pins (must be 1 to activate),
    and led the number to iluminate in green (TODO)
  * A future line with mode 1, will have the same "id" number.
    row and col will be ignored, the estate will check from the Button Defintion
    the led number will be the position on the strip that will be red.

  example button number 3 in col 0 row 3 and led 9
      MM NNNN CC RR E LLLLL
  L1: 00 0003 00 03 0 00009 
  L2: 01 0003 00 00 0 00009

 2 way switches:
  * A switch that can close one circuit or another.
  * May have 2 states (closed 1 or closed 2) or three (closed 1, closed 2 or none)
  * Only one may be activea at any moment (one or none)
  * The board needs 3 configuration lines, the first line must be with mode 2 o 3
    All three lines
  * If a line starts with 2 or 3 it means that the button "id" is a switch
  * If a line starts with 2 the line configures the state 1
    The row and col will the matrix position that is closed
    The led will be the led that will be green when the swith is in position 1
  * If a line starts with 3 the line configures the state 2
    The row and col will the matrix position that is closed
    The led will be the led that will be green when the swith is in position 1
  * The last line will have the mode as 1. Indicating the led to iluminate in red
    when both states positions are inactive.
    row and col will be ignored.

  Example swith "0" that connects row 2 to Col0 in state 1 and Col1 in state 2
  Leds 0,1 & 2, for (on1, off, on2) each
      MM NNNN CC RR E LLLLL
  L1: 02 0000 00 02 0 00000
  L2: 03 0000 01 02 0 00002
  L3: 01 0000 00 00 0 00001

*/

#define BTN_CFG_MODE_SIMPLE 0
#define BTN_CFG_MODE_LED_OFF 1
#define BTN_CFG_MODE_SWITCH_ON_1 2
#define BTN_CFG_MODE_SWITCH_ON_2 3

typedef enum e_button_status{
    bs_off=0,
    bs_on,
    bs_on_1,
    bs_on_2
} t_button_status;

typedef enum e_button_type{
    bt_unknown=0,
    bt_simple,
    bt_switch
} t_button_type;

struct s_button{
    u_int8_t id;
    t_button_status status; 
    t_button_type type;
    union {
        struct{
            tu_btn_config cfg;
            u_int8_t off_led;
            u_int8_t on_led;
        } simple;
        struct{
            u_btn_config cfg1;
            u_btn_config cfg2;
            u_int8_t off_led;
            u_int8_t on1_led;
            u_int8_t on2_led;
         } sw_btn;
    } btn;
    
};

typedef struct s_button t_button;

#endif