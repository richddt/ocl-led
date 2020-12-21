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
#include "GlobalVariables.h"
#include <FastLED.h>
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
LEDStripController LedStripControllerArray[NUM_STRIPS] = {  
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
  FastLED.setBrightness(fastLEDGlobalBrightness);


}

////// MAIN LOOP ///////
void loop() {

  // TRIGGER AN ANIMATION BASED ON THE INPUT FROM MAX PATCH
  while(Serial.available()) {         // check for incoming bytes
    incomingByte = Serial.read();   // read incoming byte
    
    switch (incomingByte) {
      // trigger ALL_OFF animation
      case 'b':    
        triggerAnimationAllStrips(ALL_OFF);
        break;

      // trigger FADE_LOW animation
      case 'U':
        triggerAnimationAllStrips(FADE_LOW);
        break;
      
      // trigger FADE_IN_OUT animation
      case 'D':    
        triggerAnimationAllStrips(FADE_OUT_BPM);
        break;

      // trigger RAINBOW animation
      case 'H':
        triggerAnimationAllStrips(RAINBOW);
        break;

      // trigger RAINBOW_W_GLITTER animation
      case 'C':
        triggerAnimationAllStrips(RAINBOW_W_GLITTER);
        break;

      // trigger SOLID_COLOR animation
      case 'L':
        triggerAnimationAllStrips(SOLID_COLOR);
        break;
    
    }  // end of switch
    
  }

  // UPDATE THE STATE OF EACH STRIP CONTROLLER OBJECT
  for(int i = 0; i < NUM_STRIPS; i++){
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

  triggerLED(100);

  for(int i = 0; i < NUM_STRIPS; i++){
    LedStripControllerArray[i].SetActiveAnimationType( animationToSet );
  }

}




// this function blinks the onboard LED. The argument sets how long is the blink.
void triggerLED(int blinkTime) {      
  digitalWrite(led, HIGH);            // turn the LED on (HIGH is the voltage level)
  delay(blinkTime);                   // wait for a second
  digitalWrite(led, LOW);             // turn the LED off by making the voltage LOW
}
