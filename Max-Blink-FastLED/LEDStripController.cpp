/*
  LEDStripController.cpp   - Class that provides specific controlls of a defined LED Strip using the FastLED Library
                                Originally intended to play nice with MIDI controllers
  Created by Turner E. Kirk, January 20, 2019.
  Released into the public domain.
*/


// ******************************************************************
//            Includes
// ******************************************************************

#include "LEDStripController.h"


// *****************************************************************************************************
//      CONSTRUCTOR
// *****************************************************************************************************

LEDStripController::LEDStripController(CRGB *leds, uint16_t _numPixelsInStrip){
  ledStrip = leds;
  
  numPixelsInStrip = _numPixelsInStrip;

  activeAnimationType = ALL_OFF;
  
}


// *****************************************************************************************************
//      MAIN UPDATE FUNCTION
// *****************************************************************************************************

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
      case SOLID_COLOR:
        // don't do anything because we set the color in the Initialization funciton
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



// ******************** ANIMATION TRIGGERING AND INITIALIZATION METHODS ******************************
//      THIS IS WHERE WE GET AND SET OUR ANIMATION STATES
// ***************************************************************************************************

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
    case FADE_IN:
    case FADE_OUT:
    case FADE_IN_OUT:
      break;
    case FADE_OUT_BPM:  
      SetStripHSV(CHSV( hue, saturation, brightness));
      beatStartTime = millis();
      showStrip = true;
      updateInterval = FADE_UPDATE_INTERVAL;
      break;
    case RAINBOW:
    case RAINBOW_W_GLITTER:
      updateInterval = RAINBOW_UPDATE_INTERVAL;
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


// set strip to color based on CHSV input
void LEDStripController::SetStripHSV(CHSV newCHSV) {

  fill_solid( &(ledStrip[ledStripStartIndex]), numPixelsInStrip, newCHSV ); 

}

// set strip to color based on CRGB input
void LEDStripController::SetStripHSV(CRGB newCRGB) {

  fill_solid( &(ledStrip[ledStripStartIndex]), numPixelsInStrip, newCRGB ); 

}



// ************************** ANIMATION METHODS ************************************
//  THIS IS THE FUN PART WHERE WE ACTUALLY WRITE THE ANIMATIONS THAT WILL RUN
// *********************************************************************************
void LEDStripController::AllOff() {
  fadeToBlackBy( &(ledStrip[ledStripStartIndex]), numPixelsInStrip, 80);
}


void LEDStripController::FadeLow() {
  fadeToBlackBy( &(ledStrip[ledStripStartIndex]), numPixelsInStrip, 10);
}


// this function fades out brightness from full brightness to off
// synchronized to one full beat at the speed of the global BPM
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

  uint8_t _brightness = beatsin8( GLOBAL_BPM / 2 , 0, full_brightness, beatStartTime, 90);
  
  // since we only want to trigger this animation as a one shot, we need to disable it once the brightnes drops below a certain threshold
  if(_brightness <= 5){
    showStrip = false;
    AllOff();
  }
  if(showStrip){
    SetStripHSV(CHSV( hue, saturation, _brightness));
  }

}




void LEDStripController::Rainbow() {

  // increment the hue position on every update
  hue++;

  // FastLED's built-in rainbow generator
  fill_rainbow( &(ledStrip[ledStripStartIndex]), numPixelsInStrip, hue, 7);
  
}


void LEDStripController::RainbowWithGlitter() {
  // reference our rainbow function, plus some random sparkly glitter
  Rainbow();
  AddGlitter(80);
}


void LEDStripController::AddGlitter( fract8 chanceOfGlitter) {
  if( random8() < chanceOfGlitter) {
    ledStrip[ ledStripStartIndex + random16(numPixelsInStrip - 1) ] += CRGB::White;
  }
}
