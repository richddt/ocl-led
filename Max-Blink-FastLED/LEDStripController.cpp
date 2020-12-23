/*
  LEDStripController.cpp   - Class that provides specific controlls of a defined LED Strip using the FastLED Library
*/


// ******************************************************************
//      INCLUDES
// ******************************************************************
#include "LEDStripController.h"


// *********************************************************************************
//      CONSTRUCTOR
// *********************************************************************************

LEDStripController::LEDStripController(CRGB *leds, uint16_t _numPixelsInStrip){
  ledStrip = leds;
  
  numPixelsInStrip = _numPixelsInStrip;

  activeAnimationType = ALL_OFF;
  
}


// *********************************************************************************
//      MAIN UPDATE FUNCTION
// *********************************************************************************

void LEDStripController::Update() {

  if( (millis() - lastUpdateTime) > updateInterval ){
    switch(activeAnimationType) {
      case ALL_OFF:
        AllOff();
        break;
      case FADE_LOW:
        FadeLow();
        break;
      case FADE_IN:
      case FADE_OUT:
      case FADE_IN_OUT:  
        // do something for these animations
        break;
      case FADE_OUT_BPM:  
        FadeOutBPM();
        break;
      case RAINBOW:
        Rainbow();
        break;
      case RAINBOW_W_GLITTER:
        RainbowWithGlitter();
        break;
      case CONFETTI:
        Confetti();
        break;
      case SINELON:
        Sinelon();
        break;
      case SOLID_COLOR:
        // don't need to do anything for this one 
        // because we set the color in the Initialization funciton
        // and it isn't actually an animation
        break;
      case NONE:
        break;
      default:
        // do something for the default
        break;
    }
    
    lastUpdateTime = millis();
  }

}


// *********************************************************************************
//      ANIMATION TRIGGERING AND INITIALIZATION METHODS
//        This is how the external world tells our class to trigger an animation
//        Upon being triggered, the animation first sets its initial state
// *********************************************************************************

AnimationType LEDStripController::GetActiveAnimationType(){
     return activeAnimationType;
}

// SET THE "AnimationType"
void LEDStripController::SetActiveAnimationType(AnimationType newAnimationState){

  if (activeAnimationType != newAnimationState) {
    activeAnimationType = newAnimationState;
  }

  // SET THE INITIAL STATE OF THE ANIMATION
  InitializeAnimation();
  
}

// IF THE ANIMATION IS TRIGGERED IT WILL INITIALIZE ITSELF BASED ON THESE SETTTINGS
void LEDStripController::InitializeAnimation() {

  switch(activeAnimationType) {
    case ALL_OFF:
      updateInterval = ALL_OFF_UPDATE_INTERVAL;
      break;
    case FADE_LOW:
      updateInterval = FADE_UPDATE_INTERVAL;
      break;
    case FADE_IN:
    case FADE_OUT:
    case FADE_IN_OUT:
      updateInterval = FADE_UPDATE_INTERVAL;
      break;
    case FADE_OUT_BPM:  
      SetStripHSV(CHSV( hue, saturation, brightness));
      beatStartTime = millis();
      showStrip = true;
      updateInterval = FADE_UPDATE_INTERVAL;
      break;
    case RAINBOW:
    case RAINBOW_W_GLITTER:
      hue = 0;
      updateInterval = RAINBOW_UPDATE_INTERVAL;
      break;
    case CONFETTI:
      hue = 0;
      updateInterval = DEFAULT_UPDATE_INTERVAL;
      break;
    case SINELON:  
      hue = 0;
      beatStartTime = millis();
      updateInterval = DEFAULT_UPDATE_INTERVAL;
      break;      
    case SOLID_COLOR:
      SetStripHSV(CHSV( random8(), saturation, brightness));
      break;      
    case NONE:
      updateInterval = DEFAULT_UPDATE_INTERVAL;
      break;
    default:
      // do something for the default
      break;
  }

}



// *********************************************************************************
//      BASIC FUNCTIONS FOR SETTING THE STRIP TO A SOLID COLOR
// *********************************************************************************
// set strip to color based on CHSV input
void LEDStripController::SetStripHSV(CHSV newCHSV) {

  fill_solid( &(ledStrip[ledStripStartIndex]), numPixelsInStrip, newCHSV ); 

}

// set strip to color based on CRGB input
void LEDStripController::SetStripHSV(CRGB newCRGB) {

  fill_solid( &(ledStrip[ledStripStartIndex]), numPixelsInStrip, newCRGB ); 

}



// *********************************************************************************
//      ANIMATION METHODS
//        This is where we write the logic for how our animations function
//        These functions will be called once per call to the Update method
// *********************************************************************************

// quickly turn off the strip
void LEDStripController::AllOff() {
  fadeToBlackBy( &(ledStrip[ledStripStartIndex]), numPixelsInStrip, 80);
}

// more slowly fade the strip to black
void LEDStripController::FadeLow() {
  fadeToBlackBy( &(ledStrip[ledStripStartIndex]), numPixelsInStrip, 10);
}


// this function fades out brightness similar to the "FadeLow()" function
// except this one does so over the course of one full beat at the speed of the global BPM
void LEDStripController::FadeOutBPM() {

// the beatsin8 function creates a sine wave at the given BPM
// between the values given for "lowest" and "highest"
// and the phase of the sine wave is set by the system's clock
// if we set the "timebase" argument, then whatever time we give it will set 
// the reference for a phase of 0
// since we are setting the "beatStartTime" in our animation initialization function
// then we always start with phase set to 0
// we can then offset the phase by 90 degrees by using the "phase_offset" argument
/*
  uint8_t beatsin8	(	accum88 	beats_per_minute,
                      uint8_t 	lowest = 0,
                      uint8_t 	highest = 255,
                      uint32_t 	timebase = 0,
                      uint8_t 	phase_offset = 0 
                    )	
*/

  if(showStrip){

    // we divide the global BPM by 2 because we are only using half of the saw wave (from 255 to 0)
    // set the low value to 0 and high to full_brightness
    // set timebase reference to now so that the wave reference always starts at 0
    // then shift it by 1/4 wavelength using sizeof(byte) / 4    
    uint8_t _brightness = beatsin8( GLOBAL_BPM / 2 , 0, full_brightness, beatStartTime, 256 / 4);
  
    SetStripHSV(CHSV( hue, saturation, _brightness));

    // since we only want to trigger this animation as a one shot, 
    // we need to disable the animation once the brightnes drops below a certain threshold
    // as well as set the brightnes to 0 (or whatever value we want to stop it at)
    if(_brightness <= 5){
      SetStripHSV(CHSV( hue, saturation, 0));
      showStrip = false;      
    }
  
  }

}



// a simple function for creating an animated rainbow
void LEDStripController::Rainbow() {

  // increment the hue position on every update so the rainbow moves
  hue++;

  // FastLED's built-in rainbow generator
  fill_rainbow( &(ledStrip[ledStripStartIndex]), numPixelsInStrip, hue, 7);
  
}


// same as the above function except this also ads glitter
void LEDStripController::RainbowWithGlitter() {
  // reference our rainbow function, plus some random sparkly glitter
  Rainbow();
  AddGlitter(80);
}


// the glitter function, randomly selects a pixel and sets it to white
void LEDStripController::AddGlitter( fract8 chanceOfGlitter) {
  if( random8() < chanceOfGlitter) {
    ledStrip[ ledStripStartIndex + random16(numPixelsInStrip) ] += CRGB::White;
  }
}





// Pop and Fade. like confetti!!!
void LEDStripController::Confetti() {
  // increment the hue position on every update so the rainbow moves
  hue++;

  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( &(ledStrip[ledStripStartIndex]), numPixelsInStrip, 10);
  int pos = random16(numPixelsInStrip);
  ledStrip[pos] += CHSV( hue + random8(64), 200, 255);
}


// a colored dot sweeping back and forth, with fading trails
void LEDStripController::Sinelon(){
  // increment the hue position on every update so the rainbow moves
  hue++;

  fadeToBlackBy( &(ledStrip[ledStripStartIndex]), numPixelsInStrip, 20);

  // divide the BPM by 2 so that half the saw wave will finish at the actual BPM
  // set the low value to 0 and high to one less than strip length
  // set timebase reference to now so that the wave reference always starts at 0
  // then shift it by 1/4 wavelength using sizeof(int) / 4
  int pos = beatsin16( GLOBAL_BPM / 2, 0, numPixelsInStrip - 1 , beatStartTime, 65536 / 4);  
  
  ledStrip[pos] += CHSV( hue, full_saturation, 192);
}