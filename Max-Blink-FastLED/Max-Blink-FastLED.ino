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

/////// GLOBAL CONSTANTS ///////
#define baudRate 9600   //this is a safe and common rate. Feel free to change it as desired. Justmake sure that Max and the Teensy are at the same setting.
char incomingByte;        // variable used for data from Max.

/////// GLOBAL MUTABLES ///////
uint32_t timeToCallFastLEDShow = 0; // time of last update of call to FastLED.show()

// teensy LED timer variables
uint32_t timeToTurnOffTeensyLED = 0;
bool teensyLEDIsOn = false;


// THESE STEPS SETUP THE VIRTUAL REPRESENTATION OF OUR LED STRIPS
// CRGB Array for each strip.
CRGB aLEDs[ALEN];
CRGB bLEDs[BLEN];
CRGB cLEDs[CLEN];

// Controller for each strip (to manage each strip array's state without blocking main thread)

// //Simple non-segmented version
//LEDStripController ALedStripController_1(aLEDs, ALEN);

// Segmented version
LEDStripController ALedStripController_1(aLEDs, 24, DEFAULT_PALETTE, !INVERT_STRIP, 0); // right side triangle
LEDStripController ALedStripController_2(aLEDs, 16, DEFAULT_PALETTE, !INVERT_STRIP, 24); // top big triangle
LEDStripController ALedStripController_3(aLEDs, 16, DEFAULT_PALETTE, INVERT_STRIP, 24+16); // top big triangle (inverted)
LEDStripController ALedStripController_4(aLEDs, 24, DEFAULT_PALETTE, INVERT_STRIP, 24+32);  // left side triangle (inverted)


LEDStripController BLedStripController_1(bLEDs, 24, DEFAULT_PALETTE, !INVERT_STRIP, 0); // right side triangle
LEDStripController BLedStripController_2(bLEDs, 16, DEFAULT_PALETTE, !INVERT_STRIP, 24); // top big triangle
LEDStripController BLedStripController_3(bLEDs, 16, DEFAULT_PALETTE, INVERT_STRIP, 24+16); // top big triangle (inverted)
LEDStripController BLedStripController_4(bLEDs, 24, DEFAULT_PALETTE, INVERT_STRIP, 24+32);  // left side triangle (inverted)


LEDStripController CLedStripController_1(cLEDs, 24, DEFAULT_PALETTE, !INVERT_STRIP, 0); // right side triangle
LEDStripController CLedStripController_2(cLEDs, 16, DEFAULT_PALETTE, !INVERT_STRIP, 24); // top big triangle
LEDStripController CLedStripController_3(cLEDs, 16, DEFAULT_PALETTE, INVERT_STRIP, 24+16); // top big triangle (inverted)
LEDStripController CLedStripController_4(cLEDs, 24, DEFAULT_PALETTE, INVERT_STRIP, 24+32);  // left side triangle (inverted)


const int NUM_SEGMENTS = 12;

// Array of all controllers (to make it more efficient to update all of them at once)
LEDStripController *LedStripControllerArray[NUM_SEGMENTS] = {  
                                                              &ALedStripController_1,
                                                              &ALedStripController_2, 
                                                              &ALedStripController_3,
                                                              &ALedStripController_4,
                                                              &BLedStripController_1,
                                                              &BLedStripController_2, 
                                                              &BLedStripController_3,
                                                              &BLedStripController_4,
                                                              &CLedStripController_1,
                                                              &CLedStripController_2, 
                                                              &CLedStripController_3,
                                                              &CLedStripController_4,
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
    
    // you now have control over these parameters for each strip
    uint8_t aHue = 90;              // the hue/color of the strip for all animations other than the palette controlled animations. 0 (red) - 255 (end spectrum red)
    uint8_t aBrightness = 255;      // the brightness of the strip for all animations INCLUDING palette controlled animations
    uint16_t aBPM = 85;             // the speed of the animation in BPM for any of the "Fade" animations
    uint8_t aBrightnessHigh = 255;  // the top level of brightness for any of the "Fade" animations
    uint8_t aBrightnessLow = 80;    // the bottom level of brightness for any of the "Fade" animations; colors below ~30 are very inaccurate

    // add this function into the switch statement below before triggering an animation
    //setAllStripParams(aHue, aBrightness, aBPM, aBrightnessHigh, aBrightnessLow);

    // for color palette animations, you can use this one as well
    CRGBPalette16 aColorPalette = RainbowColors_p; // the color palette that controls the colors for any of the palette controlled animations
    //setAllStripColorPalettes(newColorPalette);

    switch (incomingByte) {
      case 'o':
      {
        triggerAnimationAllStrips(ALL_OFF);
        break;
      }
      case 'O':
      {
        triggerAnimationAllStrips(FADE_OUT_BPM);
        break;
      }

      case 'b':
      {
        setAllStripParams(180, 255, aBPM, 255, 70);  //(aHue, aBrightness, aBPM, aBrightnessHigh, aBrightnessLow)
        triggerAnimationAllStrips(FADE_LOW_BPM);
        break;
      }
      case 'L':
      {      
        setAllStripParams(180, 255, aBPM*2, 255, 70);  //(aHue, aBrightness, aBPM, aBrightnessHigh, aBrightnessLow)
        triggerAnimationAllStrips(FADE_LOW_BPM);
        break;
      }
      case 'I':
      {
        triggerAnimationAllStrips(FADE_IN_OUT_BPM);
        break;
      }
      case 'p':
      {
        triggerAnimationAllStrips(PALETTE);
        break;
      }
      case 'g':
      {
        triggerAnimationAllStrips(PALETTE_W_GLITTER);
        break;
      }
      case 'P':
      {
        // for this animation, (PALETTE_FADE_LOW_BPM)
        // argument 1 does nothing so we set to 0 (normally hue)
        // argument 1 does nothing so we set to 0 (normally brightness)
        // argument 3 sets the speed of the fade in BPM
        // argument 4 sets the brightness the fade starts at
        // argument 5 sets the brightness the fade ends at  
        setAllStripParams(0, 0, aBPM, 255, 20);  //(aHue, aBrightness, aBPM, aBrightnessHigh, aBrightnessLow)
        triggerAnimationAllStrips(PALETTE_FADE_LOW_BPM);
        break;
      }      
      case 'G':
      {
        // for this animation, (PALETTE_W_GLITTER_FADE_LOW_BPM)
        // argument 1 does nothing so we set to 0 (normally hue)
        // argument 2 sets the brightness of the glitter pops
        // argument 3 sets the speed of the fade in BPM
        // argument 4 sets the brightness the fade starts at
        // argument 5 sets the brightness the fade ends at
        setAllStripParams(0, 125, aBPM*2, 255, 20);
        triggerAnimationAllStrips(PALETTE_W_GLITTER_FADE_LOW_BPM);
        break;
      }      
      case 'c':
      {
        triggerAnimationAllStrips(CONFETTI);
        break;
      }
      case 's':
      {
        triggerAnimationAllStrips(SINELON);
        break;
      }
      case 'S':
      {
        triggerAnimationAllStrips(SOLID_COLOR);
        break;
      }
      case 'y':
        {
          uint8_t paletteIndex = random8(0,7);
          CRGBPalette16 newColorPalette = ALL_COLOR_PALETTES[paletteIndex];

          Serial.println("************");
          Serial.print("Palette: ");
          Serial.println(paletteIndex);
          Serial.println("************");

          setAllStripColorPalettes(newColorPalette);
          break;
        }

      case 'z':
        {
          uint8_t newHue = random8();
          uint8_t newBrightness = random8(30, 220);
          uint8_t newBrightnessHigh = random8(150, 255);
          uint8_t newBrightnessLow = random8(30, 50); // min 30;  colors below ~30 are very inaccurate
          uint16_t newBPM = random16(60, 160);

          Serial.println("************");
          Serial.print("Hue: ");
          Serial.println(newHue);
          Serial.print("Brightness: ");
          Serial.println(newBrightness);
          Serial.print("bpm: ");
          Serial.println(newBPM);
          Serial.print("High: ");
          Serial.println(newBrightnessHigh);
          Serial.print("Low: ");
          Serial.println(newBrightnessLow);
          Serial.println("************");
          
          setAllStripParams(newHue, newBrightness, newBPM, newBrightnessHigh, newBrightnessLow);
          break;
        }
    }  // end of switch
    
  }

  static uint32_t currentTime;
  currentTime = millis();

  // update the teensy led (this makes it so the teensy LED doesn't block the main thread)
  updateTeensyLED(currentTime);


  // UPDATE THE VISUAL REPRESENTATION OF OUR STRIPS IN EACH STRIP CONTROLLER OBJECT
  for(int i = 0; i < NUM_SEGMENTS; i++){
    LedStripControllerArray[i]->Update(currentTime);
  } 

  // PUSH OUT LATEST FRAME TO THE ACTUAL PHYSICAL LEDS
  // this physically displays the current state of leds in each strip controller object
  // we wrap it in a timer so that it only triggers at our chosen frame rate
  if( currentTime > timeToCallFastLEDShow ){
     FastLED.show();
     timeToCallFastLEDShow = currentTime + (1000/FRAMES_PER_SECOND);
  }


}






// *********************************************************************************
//      HELPER FUNCTIONS
// *********************************************************************************
// blink the onboard LED
// update the active animation for each LEDStripController object
// based on the AnimationType Enum value sent into the function
void triggerAnimationAllStrips(AnimationType animationToSet){

  turnTeensyLEDOn();

  for(int i = 0; i < NUM_SEGMENTS; i++){
    LedStripControllerArray[i]->SetActiveAnimationType( animationToSet );
  }

}


// blink the onboard LED
// update all the important parameters for the solid color and BPM based animations
void setAllStripParams(uint8_t aHue, uint8_t aBrightness, uint16_t aBPM, uint8_t aBrightnessHigh, uint8_t aBrightnessLow){

  turnTeensyLEDOn();

  for(int i = 0; i < NUM_SEGMENTS; i++){
    LedStripControllerArray[i]->SetStripParams( aHue, aBrightness, aBPM, aBrightnessHigh, aBrightnessLow);
  }

}



// blink the onboard LED
void setAllStripColorPalettes(CRGBPalette16 newColorPalette){

  turnTeensyLEDOn();

  for(int i = 0; i < NUM_SEGMENTS; i++){
    LedStripControllerArray[i]->SetColorPalette( newColorPalette );
  }

}


// this turns on the Teensy LED and tells our program to turn it off in 500 ms
void turnTeensyLEDOn(){

  digitalWrite(led, HIGH); 

  teensyLEDIsOn = true;

  timeToTurnOffTeensyLED = millis() + 100;
}


// this function turns the LED off when it's time
void updateTeensyLED(uint32_t currentTime) {
  
  if(currentTime > timeToTurnOffTeensyLED && teensyLEDIsOn){
    digitalWrite(led, LOW);
    teensyLEDIsOn = false;
  }

}