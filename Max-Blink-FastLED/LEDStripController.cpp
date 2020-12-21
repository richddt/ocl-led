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
        // do something for this animation
        break;
      case RAINBOW:
        Rainbow();
        break;
      case NONE:
        // do something for this case
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
      updateInterval = FADE_LOW_UPDATE_INTERVAL;
      SetStripHSV(hue, saturation, brightness);
      break;
    case FADE_IN:
    case FADE_OUT:
    case FADE_IN_OUT:  
      updateInterval = DEFAULT_UPDATE_INTERVAL;
      break;
    case RAINBOW:
      updateInterval = DEFAULT_UPDATE_INTERVAL;
      break;
    case NONE:
      updateInterval = DEFAULT_UPDATE_INTERVAL;
      break;
    default:
      // do something for the default
      break;
  }


  //Re-Initialize the entire strip based on settings above (if it's already running, it will now start over)


}


// set strip to color based on HSV inputs
void LEDStripController::SetStripHSV(uint8_t _hue, uint8_t _saturation, uint8_t _brightness) {
  
  CHSV newHSV = CHSV( _hue, _saturation, _brightness);

  fill_solid( &(ledStrip[ledStripStartIndex]), numPixelsInStrip, newHSV ); 

}





// ************************** ANIMATION METHODS ************************************
//  THIS IS THE FUN PART WHERE WE ACTUALLY WRITE THE ANIMATIONS THAT WILL RUN
// *********************************************************************************
void LEDStripController::AllOff() {
  fadeToBlackBy( &(ledStrip[ledStripStartIndex]), numPixelsInStrip, 50);
}


void LEDStripController::FadeLow() {
  fadeToBlackBy( &(ledStrip[ledStripStartIndex]), numPixelsInStrip, 10);
}


void LEDStripController::Rainbow() {

  hue++;
  // FastLED's built-in rainbow generator
  fill_rainbow( &(ledStrip[ledStripStartIndex]), numPixelsInStrip, hue, 7);
  
}


void LEDStripController::RainbowWithGlitter() {
  // built-in FastLED rainbow, plus some random sparkly glitter
  Rainbow();
  AddGlitter(80);
}


void LEDStripController::AddGlitter( fract8 chanceOfGlitter) {
  if( random8() < chanceOfGlitter) {
    ledStrip[ ledStripStartIndex + random16(numPixelsInStrip) ] += CRGB::White;
  }
}
