/*  Simple example of Max->Teensy communication.   9/7/2020 ams
   The Teensy listens for incoming characters over the USB host serial port.
   When any letter available in the switch statement is received...
   the Teensy will flash the onboard LED for 1/10 of a second.

   Also, the Teensy sends a "K" back to Max when it receives a command. This can be ignored, or used as a way for Max to verify that the Teensy is alive and connected.

   The accompanying Max patch is setup to send B on the downbeat and b on every quarter note.

*/

/////// INCLUDES ///////
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

CRGBPalette16 colorPalette = HeatColors_p;

// Controller for each strip (to manage each strip array's state without blocking main thread)
LEDStripController ALedStripController(aLEDs, ALEN/2);
LEDStripController ALedStripController_2(aLEDs, ALEN/2, ForestColors_p, INVERT_STRIP, ALEN/2);
LEDStripController BLedStripController(bLEDs, BLEN, colorPalette);
LEDStripController CLedStripController(cLEDs, CLEN, colorPalette);  

const int NUM_SEGMENTS = 4;

// Array of all controllers (to make it more efficient to update all of them at once)
LEDStripController *LedStripControllerArray[NUM_SEGMENTS] = {  
                                                            &ALedStripController,
                                                            &ALedStripController_2, 
                                                            &BLedStripController,
                                                            &CLedStripController
                                                          };

// *********************************************************************************
//      SETUP
// *********************************************************************************
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

// *********************************************************************************
//      MAIN LOOP
// *********************************************************************************
void loop() {

  // SET THE STRIP'S ANIMATION BASED ON THE INPUT FROM MAX PATCH
  while(Serial.available()) {         // check for incoming bytes
    incomingByte = Serial.read();   // read incoming byte
    
    switch (incomingByte) {
      // trigger ALL_OFF animation
      case 'b':    
        triggerAnimationAllStrips(ALL_OFF);
        break;

      // trigger FADE_LOW animation
      case 'F':
        triggerAnimationAllStrips(FADE_LOW);
        break;
      
      // trigger FADE_IN_OUT animation
      case 'B':    
        triggerAnimationAllStrips(FADE_OUT_BPM);
        break;

      // trigger FADE_LOW animation
      case 'f':
        triggerAnimationAllStrips(FADE_LOW_BPM);
        break;

      // trigger RAINBOW animation
      case 'R':
        triggerAnimationAllStrips(RAINBOW);
        break;

      // trigger RAINBOW animation
      case 'P':
        triggerAnimationAllStrips(PALETTE);
        break;

      // trigger RAINBOW_W_GLITTER animation
      case 'G':
        triggerAnimationAllStrips(RAINBOW_W_GLITTER);
        break;

      // trigger CONFETTI animation
      case 'C':
        triggerAnimationAllStrips(CONFETTI);
        break;

      // trigger SINELON animation
      case 'S':
        triggerAnimationAllStrips(SINELON);
        break;

      // trigger SOLID_COLOR animation
      case 'L':
        triggerAnimationAllStrips(SOLID_COLOR);
        break;
    
    }  // end of switch
    
  }

  // UPDATE THE VISUAL REPRESENTATION OF OUR STRIPS IN EACH STRIP CONTROLLER OBJECT
  for(int i = 0; i < NUM_SEGMENTS; i++){
    LedStripControllerArray[i]->Update();
  }


  // PUSH OUT LATEST FRAME TO THE ACTUAL PHYSICAL LEDS
  // this physically displays the current state of leds in each strip controller object
  // we wrap it in a timer so that it only triggers at a modest frame rate
  if( (millis() - lastFastLEDShowTime) > (1000/FRAMES_PER_SECOND) ){
     FastLED.show();
     lastFastLEDShowTime = millis();
  }


}






// *********************************************************************************
//      HELPER FUNCTIONS
// *********************************************************************************
// blink the onboard LED
// update the active animation for each LEDStripController object
// based on the AnimationType Enum value sent into the function
void triggerAnimationAllStrips(AnimationType animationToSet){

  triggerLED(100);

  for(int i = 0; i < NUM_SEGMENTS; i++){
    LedStripControllerArray[i]->SetActiveAnimationType( animationToSet );
  }

}




// this function blinks the onboard LED. The argument sets how long is the blink.
void triggerLED(int blinkTime) {      
  digitalWrite(led, HIGH);            // turn the LED on (HIGH is the voltage level)
  delay(blinkTime);                   // wait for a second
  digitalWrite(led, LOW);             // turn the LED off by making the voltage LOW
}
