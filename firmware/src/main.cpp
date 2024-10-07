#include <Arduino.h>
#include <Adafruit_NeoPixel.h>


#include "pins_cfg.h"

#define PIN        WS2812_LED // On Trinket or Gemma, suggest changing this to 1

#define NUMPIXELS 10 

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels

void setup() {

  SERIAL_OUT.begin(115200);
  SERIAL_OUT.println("8 Button Board");
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)

  pinMode(COL0_RED,OUTPUT);
  pinMode(COL1_BLAC_L,OUTPUT);
  pinMode(ROW0_YELLOW,INPUT_PULLDOWN);
  pinMode(ROW1_ORANGE,INPUT_PULLDOWN);
  pinMode(ROW2_BLUE,INPUT_PULLDOWN);
  pinMode(ROW3_BLACK,INPUT_PULLDOWN);
    

}

u_int8_t col0;
u_int8_t col1;

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
  col0=btn_readRows();

  digitalWrite(COL0_RED,0);
  digitalWrite(COL1_BLAC_L,1);
  col1=btn_readRows();


}

void btn_print(){
  SERIAL_OUT.printf("COL0: %02x\n",col0);
  SERIAL_OUT.printf("COL1: %02x\n",col1);
}

void loop() {
  pixels.clear(); // Set all pixel colors to 'off'

  // The first NeoPixel in a strand is #0, second is 1, all the way up
  // to the count of pixels minus one.
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
    btn_scan();
    btn_print();
    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    pixels.setPixelColor(i, pixels.Color(0, 150, 0));

    pixels.show();   // Send the updated pixel colors to the hardware.

    delay(DELAYVAL); // Pause before next pass through loop
  }
}