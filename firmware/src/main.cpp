#include <Arduino.h>
#include <Adafruit_NeoPixel.h>


#include "pins_cfg.h"
#include "btn_config.h"

#define PIN        WS2812_LED // On Trinket or Gemma, suggest changing this to 1

#define NUMPIXELS 10 

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels

//|8 1000|9 1001 |A 1010|B 1011|C 1100|D 1101|E 1110|F 1111|
/*
SWT 0 on row 2 cols 0,1 
    M N C R E L     MM NN|NN CC| RR E L|LLLL
L1: 2 0 0 2 0 0  -> 10 00|00 00| 10 0 0|0000 0x8080
L2: 3 0 1 2 0 2  -> 11 00|00 01| 10 0 0|0010 0xC182
L3: 1 0 0 0 0 1  -> 01 00|00 00| 00 0 0|0001 0x4001  

SWT 1 on row 1 cols 0,1 
    M N C R E L     MM NN|NN CC| RR E L|LLLL
L1: 2 1 0 1 0 3  -> 10 00|01 00| 01 0 0|0011 0x8463
L2: 3 1 1 1 0 5  -> 11 00|01 01| 01 0 0|0101 0xC545
L3: 1 1 0 0 0 4  -> 01 00|01 00| 00 0 0|0100 0x4404  

SWT 2 on row 0 cols 0,1 
    M N C R E L     MM NN|NN CC| RR E L|LLLL
L1: 2 2 0 0 0 6  -> 10 00|10 00| 00 0 0|0101 0x8806
L2: 3 2 1 0 0 8  -> 11 00|10 01| 00 0 0|1000 0xC908
L3: 1 2 0 0 0 7  -> 01 00|10 00| 00 0 0|0111 0x4807  

btn 3 on row 0 col 3
    M N C R E L     MM NN|NN CC| RR E L|LLLL
L1: 0 3 0 3 0 9  -> 00 00|11 00| 11 0 0|1001 0x0CC9
L2: 1 3 0 0 0 9  -> 01 00|11 00| 00 0 0|1001 0x4C09
*/

#define config_size 11
#define n_buttons 4
uint16_t config[]={
  //Btn 0
    0x8080, //L1
    0xC182, //L2
    0x4001, //L3
  // Btn 1
    0x8463, //L4
    0xC545, //L5
    0x4404, //L6
  // Btn 2
    0x8806, //L7
    0xC908, //L8
    0x4807, //L9
  // Btn 3
    0x0CC9, //L10
    0x4C09 //L11
};

t_button buttons[n_buttons];

void configure_simple(int line, tu_btn_config cfg){
  int btn = cfg.cfg.id;
  if (buttons[btn].type != bt_unknown){
    //Warn and don't set
    SERIAL_OUT.println("WARNING: Trying to reconfigure button\n");
  }else{
    //set leds to
    buttons[btn].btn.simple.on_led=255;
    buttons[btn].btn.simple.off_led=255;
  }
  buttons[btn].type=bt_simple;
  buttons[btn].id=cfg.cfg.id;
  buttons[btn].status=bs_off;

  buttons[btn].btn.simple.cfg=cfg;
  buttons[btn].btn.simple.on_led=cfg.cfg.led;

}

void configure_off(int line, tu_btn_config cfg){
  int btn = cfg.cfg.id;
   if (buttons[btn].type == bt_unknown){
    SERIAL_OUT.println("ERROR: Off Configuration before set type\n");
   }else if(buttons[btn].type == bt_simple) {
      buttons[btn].btn.simple.off_led=cfg.cfg.led;
   }else if (buttons[btn].type == bt_switch){
      buttons[btn].btn.sw_btn.off_led=cfg.cfg.led;
   }
}

void configure_sw_1(int line, tu_btn_config cfg){
  int btn = cfg.cfg.id;
  if (buttons[btn].type == bt_unknown){
    //Init 
    buttons[btn].type=bt_switch;
    buttons[btn].id=cfg.cfg.id;
    buttons[btn].status=bs_off;
    
    buttons[btn].btn.sw_btn.on1_led=255;
    buttons[btn].btn.sw_btn.on2_led=255;
    buttons[btn].btn.sw_btn.off_led=255;
  }else{
    if(buttons[btn].id != cfg.cfg.id){
      SERIAL_OUT.println("ERROR: Id Mistach\n");
    }
  }
  buttons[btn].btn.sw_btn.on1_led=cfg.cfg.led;
  buttons[btn].btn.sw_btn.cfg1=cfg;

}

void configure_sw_2(int line, tu_btn_config cfg){
  int btn = cfg.cfg.id;
  if (buttons[btn].type == bt_unknown){
    //Init 
    buttons[btn].type=bt_switch;
    buttons[btn].id=cfg.cfg.id;
    buttons[btn].status=bs_off;
    
    buttons[btn].btn.sw_btn.on1_led=255;
    buttons[btn].btn.sw_btn.on2_led=255;
    buttons[btn].btn.sw_btn.off_led=255;
  }else{
    if(buttons[btn].id != cfg.cfg.id){
      SERIAL_OUT.println("ERROR: Id Mistach\n");
    }
  }
  buttons[btn].btn.sw_btn.on2_led=cfg.cfg.led;
  buttons[btn].btn.sw_btn.cfg2=cfg;
}

void configure(){
  SERIAL_OUT.printf("Sizes t_btn_cfg: %u tu_btn_cfg: %u \n", sizeof(t_btn_cfg), sizeof(tu_btn_config));
  for (int i=0;i<n_buttons;i++){
    buttons[i].id=255;
    buttons[i].type=bt_unknown;
  }
  tu_btn_config test;
  for(int i=0;i<config_size;i++){
    test.num=config[i];
    SERIAL_OUT.printf("  %02u -> %x ", i, config[i]);
    SERIAL_OUT.printf(" -> %u %u %u %u %u %u \n", test.cfg.mode,test.cfg.id,test.cfg.row,test.cfg.col,test.cfg.ext,test.cfg.led);
    switch (test.cfg.mode)
    {
    case BTN_CFG_MODE_SIMPLE: configure_simple(i,test); break;
    case BTN_CFG_MODE_LED_OFF: configure_off(i,test); break;
    case BTN_CFG_MODE_SWITCH_ON_1: configure_sw_1(i,test); break;
    case BTN_CFG_MODE_SWITCH_ON_2: configure_sw_2(i,test); break;
    default:
      break;
    }
  }

}

void setup() {

  SERIAL_OUT.begin(115200);
  SERIAL_OUT.println("8 Button Board");
  configure();


  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)

  pinMode(COL0_RED,OUTPUT);
  pinMode(COL1_BLAC_L,OUTPUT);
  pinMode(ROW0_YELLOW,INPUT_PULLDOWN);
  pinMode(ROW1_ORANGE,INPUT_PULLDOWN);
  pinMode(ROW2_BLUE,INPUT_PULLDOWN);
  pinMode(ROW3_BLACK,INPUT_PULLDOWN);
    

}

volatile u_int8_t col[2];

u_int8_t btn_readRows(){
  u_int8_t ret =0;
  if(digitalRead(ROW0_YELLOW)) ret|=1;
  if(digitalRead(ROW1_ORANGE)) ret|=2;
  if(digitalRead(ROW2_BLUE)) ret|=4;
  if(digitalRead(ROW3_BLACK)) ret|=8;
  return ret;
}

void btn_scan(){
  digitalWrite(COL0_RED,1);
  digitalWrite(COL1_BLAC_L,0);
  col[0]=btn_readRows();

  digitalWrite(COL0_RED,0);
  digitalWrite(COL1_BLAC_L,1);
  col[1]=btn_readRows();


}

boolean readBtn(t_btn_cfg cfg){
   u_int8_t colVal=col[cfg.col];
   u_int8_t mask = 1 << cfg.row;
   u_int8_t res = colVal & mask;
   if (res ==0){
      return false;
   }else{
      return true;
   }
}
/*
boolean readBtnLog(t_btn_cfg cfg){
   
   u_int8_t colVal=col[cfg.col];
   u_int8_t mask = 1 << cfg.row;

  SERIAL_OUT.printf("  cfg.col= %u  col Val= %02x cfg.row= %u  mask= %02x\n",cfg.col,colVal, cfg.row, mask);
   u_int8_t res = colVal & mask;
   SERIAL_OUT.printf("  colVal & mask = %02x \n",res);
   if ( res ==0){
      return false;
   }else{
      return true;
   }
}*/


void btn_read(){
  for (int i=0;i< n_buttons;i++){
    if(buttons[i].type==bt_simple){
      SERIAL_OUT.printf("Testing %u \n",i);
      boolean val = readBtn(buttons[i].btn.simple.cfg.cfg);
      if(val) {
        
        buttons[i].status=bs_on;
      }else{
        buttons[i].status=bs_off;
      }
    }else{
      boolean val1 = readBtn(buttons[i].btn.sw_btn.cfg1.cfg);
      boolean val2 = readBtn(buttons[i].btn.sw_btn.cfg2.cfg);
      if(val1 && !val2){
        buttons[i].status=bs_on_1;
      }else if(!val1 && val2){
        buttons[i].status=bs_on_2;
      }else {
        buttons[i].status=bs_off;
      }
    }

  }
}

void btn_print(){
  SERIAL_OUT.printf("COL0: %02x ",col[0]);
  SERIAL_OUT.printf("COL1: %02x\n",col[1]);
}



void loop() {
  btn_scan();
  btn_print();
  btn_read();

  pixels.clear(); // Set all pixel colors to 'off'

  for(int i=0;i<n_buttons;i++){
    switch (buttons[i].status)
    {
    case bs_off:
      if(buttons[i].type==bt_simple){
        pixels.setPixelColor(buttons[i].btn.simple.off_led, pixels.Color(150, 0, 0));
      }else{
        pixels.setPixelColor(buttons[i].btn.sw_btn.off_led, pixels.Color(150, 0, 0));
      }
      break;
    case bs_on:
      pixels.setPixelColor(buttons[i].btn.simple.on_led, pixels.Color(0, 150, 0));
      break;
    case bs_on_1:
      pixels.setPixelColor(buttons[i].btn.sw_btn.on1_led, pixels.Color(0, 150, 0));
      break;
    case bs_on_2:
      pixels.setPixelColor(buttons[i].btn.sw_btn.on2_led, pixels.Color(0, 150, 0));
      break;
    default:
      break;
    }
  }

  pixels.show(); 
  delay(DELAYVAL); // Pause before next pass through loop
  
}