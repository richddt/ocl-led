/*  Simple example of Max->Teensy communication.   9/7/2020 ams
   The Teensy listens for incoming characters over the USB host serial port.
   When a "b" is received, the Teensy will flash the onboard LED for 1/100 of a second.
   When a "B" is received, it will flash the LED for 1/10 of a second.

   Also, the Teensy sends a "K" back to Max when it receives a command. This can be ignored, or used as a way for Max to verify that the Teensy is alive and connected.

   The accompanying Max patch is setup to send B on the downbeat and b on every quarter note.
   This example jist shows how to listen for different characters and respond.

   The LED flashing code is placed in a function, hopefully to make it clearer and easier to replace with code that does something cooler!

*/

/////// INCLUDES ///////
#include <FastLED.h>
#include "GlobalVariables.h"
#include "LEDStripController.h"

/////// CONSTANTS ///////
#define baudRate 9600   //this is a safe and common rate. Feel free to change it as desired. Justmake sure that Max and the Teensy are at the same setting.
char incomingByte;        // variable used for data from Max.
unsigned long lastFastLEDShowTime = 0; // time of last update of position
//uint16_t stripShowUpdateInterval = 1000/FRAMES_PER_SECOND;   // milliseconds between updates


// THESE STEPS SETUP THE VIRTUAL REPRESENTATION OF OUR LED STRIPS
// CRGB Array for each strip. 
CRGB aLEDs[ALEN];
CRGB bLEDs[BLEN];
CRGB cLEDs[CLEN];

// Controller for each strip (to manage each strip array's state without blocking main thread)
LEDStripController ALedStripController(&(aLEDs[0]), ALEN);
LEDStripController BLedStripController(&(bLEDs[0]), BLEN);
LEDStripController CLedStripController(&(cLEDs[0]), CLEN);  

// Array of all controllers (to make it more efficient to update all of them at once)
LEDStripController LedStripControllerArray[TOTAL_NUM_STRIPS] = {  
                                                                  ALedStripController, 
                                                                  BLedStripController,
                                                                  CLedStripController
                                                               };

/////// SETUP ///////
//setup runs once at startup:
void setup() {
  pinMode(led, OUTPUT);     // initialize the digital pin as an output.
  Serial.begin(baudRate);     //initialize the host USB port.


  // THIS STEP SETS UP THE PHYSICAL REPRESENTATION OF OUR LED STRIPS
  FastLED.addLeds<NEOPIXEL, APIN>(aLEDs, ALEN);
  FastLED.addLeds<NEOPIXEL, BPIN>(bLEDs, BLEN);
  FastLED.addLeds<NEOPIXEL, CPIN>(cLEDs, CLEN);


  // set master brightness control from our global variable
  FastLED.setBrightness(bright);


}

////// MAIN LOOP ///////
void loop() {

  // TRIGGER AN ANIMATION BASED ON THE INPUT FROM MAX PATCH
  if (Serial.available()) {         // check for incoming bytes
    incomingByte = Serial.read();   // read incoming byte

    switch (incomingByte) {
      // turn all animations off
      case 'b':
        triggerLED(100);
        triggerAnimationAllStrips(ALL_OFF);
        break;

      // trigger animation 1
      case 'U':
        triggerLED(100);
        triggerAnimationAllStrips(FADE_LOW);
        break;

      // trigger animation 2
      case 'H':
        triggerLED(100);
        triggerAnimationAllStrips(RAINBOW);
        break;

      // trigger animation 3
      case 'M':
        triggerLED(100);
        break;

      // trigger animation 4
      case 'L':
        triggerLED(100);
        break;
    
    }  // end of switch
    
  }

  // UPDATE THE STATE OF EACH STRIP CONTROLLER OBJECT
  for(int i = 0; i < TOTAL_NUM_STRIPS; i++){
    LedStripControllerArray[i].Update();
  }


  // PUSH OUT LATEST FRAME TO THE ACTUAL LEDS
  // this physically displays the current state of leds in each strip controller object
  // we wrap it in a timer so that it only triggers at a modest frame rate
  if( (millis() - lastFastLEDShowTime) > (1000/FRAMES_PER_SECOND) ){
     FastLED.show();
     lastFastLEDShowTime = millis();
  }


}    // end of loop()


///// FUNCTIONS //////
void triggerAnimationAllStrips(AnimationType animationToSet){

  for(int i = 0; i < TOTAL_NUM_STRIPS; i++){
    LedStripControllerArray[i].SetActiveAnimationType( animationToSet );
  }

}


// this function blinks the onboard LED. The argument sets how long is the blink.
void triggerLED(int blinkTime) {      
  digitalWrite(led, HIGH);            // turn the LED on (HIGH is the voltage level)
  delay(blinkTime);                   // wait for a second
  digitalWrite(led, LOW);             // turn the LED off by making the voltage LOW
}







/*
// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<Astrip.numPixels(); i++) {
    Astrip.setPixelColor(i, c);
    Astrip.show();
    delay(wait);
  }
}


void setPixel(int Pixel, byte red, byte green, byte blue) {
   // NeoPixel
   Astrip.setPixelColor(Pixel, Astrip.Color(red, green, blue));  //strip A,B,C
   Bstrip.setPixelColor(Pixel, Astrip.Color(red, green, blue));  //strip A,B,C   
   Cstrip.setPixelColor(Pixel, Astrip.Color(red, green, blue));  //strip A,B,C
}

void setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < ALEN; i++ ) {
    setPixel(i, red, green, blue);
  }
  showStrip();
}

// INTEGRATE CODE BELOW:
void FadeLow(byte red, byte green, byte blue, float FadeSteps, int FadeDelay, int LowLevel) {
    float r, g, b;
 
    for(float j = FadeSteps; j >= LowLevel; j--) {
      r = j*(red/FadeSteps);
      g = j*(green/FadeSteps);
      b = j*(blue/FadeSteps);
     
      setAll(r,g,b);
      delay(FadeDelay);
      }
//      delay(FadeDelay);     
      Astrip.show();
      Bstrip.show(); 
      Cstrip.show();     
//      delay(FadeDelay);
//      Serial.print(FadeDelay);
//      Serial.print('\n');
    }

void FadeInOut(byte red, byte green, byte blue){
  float r, g, b;
     
  for(int k = 0; k < 256; k=k+1) {
    r = (k/256.0)*red;
    g = (k/256.0)*green;
    b = (k/256.0)*blue;
    setAll(r,g,b);
    showStrip();
  }
     
  for(int k = 255; k >= 0; k=k-2) {
    r = (k/256.0)*red;
    g = (k/256.0)*green;
    b = (k/256.0)*blue;
    setAll(r,g,b);
    showStrip();
  }
}

void FadeOut(byte red, byte green, byte blue, int FadeSteps, int FadeDelay) {
    float r, g, b;
 
    for(int j = FadeSteps; j >= 0; j--) {
      r = j*(red/FadeSteps);
      g = j*(green/FadeSteps);
      b = j*(blue/FadeSteps);
     
      setAll(r,g,b);
      delay(FadeDelay);
      }
//      delay(FadeDelay);     
      Astrip.show();
//      delay(FadeDelay);
//      Serial.print(FadeDelay);
//      Serial.print('\n');
    }

void FadeIn(byte red, byte green, byte blue, int FadeSteps, int FadeDelay) {
    float r, g, b;
 
    for(int k = 0; k <= FadeSteps; k++) {
      r = k*(red/FadeSteps);
      g = k*(green/FadeSteps);
      b = k*(blue/FadeSteps);
     
      setAll(r,g,b);
      delay(FadeDelay);
      }
//      delay(FadeDelay);     
      Astrip.show();
//      delay(FadeDelay);
//      Serial.print(FadeDelay);
    }
*/
