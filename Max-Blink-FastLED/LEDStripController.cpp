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

//LEDStripController::LEDStripController(CRGB *leds, uint16_t stripLength)
LEDStripController::LEDStripController( CRGB *leds, 
                                        uint16_t stripLength, 
                                        CRGBPalette16 colorPalette, 
                                        uint8_t invertStrip, 
                                        uint16_t stripStartIndex)
{

  _leds = &leds[stripStartIndex];
  _stripLength = stripLength;
  _colorPalette = colorPalette;
  _invertStrip = invertStrip;

  _activeAnimationType = ALL_OFF;
  
}


// *********************************************************************************
//      MAIN UPDATE FUNCTION
// *********************************************************************************

void LEDStripController::Update() {

  if( (millis() - _lastUpdateTime) > _updateInterval ){
    switch(_activeAnimationType) {
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
      case FADE_LOW_BPM:
        FadeLowBPM();
        break;
      case RAINBOW:
        Rainbow();
        break;
      case PALETTE:
        Palette();
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
    
    _lastUpdateTime = millis();
  }

}


// *********************************************************************************
//      ANIMATION TRIGGERING AND INITIALIZATION METHODS
//        This is how the external world tells our class to trigger an animation
//        Upon being triggered, the animation first sets its initial state
// *********************************************************************************

AnimationType LEDStripController::GetActiveAnimationType(){
     return _activeAnimationType;
}

// SET THE "AnimationType"
void LEDStripController::SetActiveAnimationType(AnimationType newAnimationState){

  if (_activeAnimationType != newAnimationState) {
    _activeAnimationType = newAnimationState;
  }

  // SET THE INITIAL STATE OF THE ANIMATION
  InitializeAnimation();
  
}

// IF THE ANIMATION IS TRIGGERED IT WILL INITIALIZE ITSELF BASED ON THESE SETTTINGS
void LEDStripController::InitializeAnimation() {

  switch(_activeAnimationType) {
    case ALL_OFF:
      _updateInterval = ALL_OFF_UPDATE_INTERVAL;
      break;
    case FADE_LOW:
      _updateInterval = FADE_UPDATE_INTERVAL;
      break;  
    case FADE_IN:
    case FADE_OUT:
    case FADE_IN_OUT:
      _updateInterval = FADE_UPDATE_INTERVAL;
      break;
    case FADE_OUT_BPM:  
      SetStripHSV(CHSV( _hue, _saturation, _brightness));
      _bsTimebase = millis();
      _showStrip = true;
      _updateInterval = FADE_UPDATE_INTERVAL;
      break;
    case FADE_LOW_BPM:
      SetStripHSV(CHSV( _hue, _saturation, _brightness));
      _bsTimebase = millis();
      _updateInterval = FADE_UPDATE_INTERVAL;
      break;          
    case RAINBOW:
    case PALETTE:
    case RAINBOW_W_GLITTER:
      _hue = 0;
      _updateInterval = RAINBOW_UPDATE_INTERVAL;
      break;
    case CONFETTI:
      _hue = 0;
      _updateInterval = DEFAULT_UPDATE_INTERVAL;
      break;
    case SINELON:  
      _hue = 0;
      _bsTimebase = millis();
      _updateInterval = DEFAULT_UPDATE_INTERVAL;
      break;      
    case SOLID_COLOR:
      SetStripHSV(CHSV( random8(), _saturation, _brightness));
      break;      
    case NONE:
      _updateInterval = DEFAULT_UPDATE_INTERVAL;
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

  fill_solid( _leds, _stripLength, newCHSV ); 

}

// set strip to color based on CRGB input
void LEDStripController::SetStripHSV(CRGB newCRGB) {

  fill_solid( _leds, _stripLength, newCRGB ); 

}



// *********************************************************************************
//      ANIMATION METHODS
//        This is where we write the logic for how our animations function
//        These functions will be called once per call to the Update method
// *********************************************************************************

// quickly turn off the strip
void LEDStripController::AllOff() {
  fadeToBlackBy( _leds, _stripLength, 80);
}

// more slowly fade the strip to black
void LEDStripController::FadeLow() {
  fadeToBlackBy( _leds, _stripLength, 10);
}


// this function fades out brightness similar to the "FadeLow()" function
// except this one does so over the course of one full beat at the speed of the global BPM
void LEDStripController::FadeOutBPM() {

// the beatsin8 function creates a sine wave at the given BPM
// between the values given for "lowest" and "highest"
// and the phase of the sine wave is set by the system's clock
// if we set the "timebase" argument, then whatever time we give it will set 
// the reference for a phase of 0
// since we are setting the "_bsTimebase" in our animation initialization function
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

  if(_showStrip){

    // we divide the global BPM by 2 because we are only using half of the saw wave (from 255 to 0)
    // set the low value to 0 and high to BRIGHTNESS_FULL
    // set _bsTimebase reference to now so that the wave reference always starts at 0
    // then shift it by 1/4 wavelength using sizeof(byte) / 4    
    uint8_t brightness = beatsin8( GLOBAL_BPM / 2 , 0, BRIGHTNESS_FULL, _bsTimebase, 256 / 4);
  
    SetStripHSV(CHSV( _hue, _saturation, brightness));

    // since we only want to trigger this animation as a one shot, 
    // we need to disable the animation once the brightnes drops below a certain threshold
    // as well as set the brightnes to 0 (or whatever value we want to stop it at)
    if(brightness <= 5){
      SetStripHSV(CHSV( _hue, _saturation, 0));
      _showStrip = false;      
    }
  
  }

}

// same as above except the fade stops at the lowBrightness level that is indicated
void LEDStripController::FadeLowBPM() {






}




// a simple function for creating an animated rainbow
void LEDStripController::Rainbow() {

  // increment the _hue position on every update so the colors "move"
  // if the strip is inverted "move" in the opposite direction 
  if(_invertStrip){
      _hue++;
  }
  else{
      _hue--;
  }

  // FastLED's built-in rainbow generator
  fill_rainbow( _leds, _stripLength, _hue, 7);
  
}


void LEDStripController::Palette()
{

  // increment the _hue position on every update so the colors "move"
  // if the strip is inverted "move" in the opposite direction 
  if(_invertStrip){
      _hue++;
  }
  else{
      _hue--;
  }
  
  fill_palette( _leds, _stripLength, _hue, 7, _colorPalette, _brightness, LINEARBLEND);
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
    _leds[ random16(_stripLength) ] += CRGB::White;
  }
}





// Pop and Fade. like confetti!!!
void LEDStripController::Confetti() {
  // increment the _hue position on every update so the rainbow moves
  _hue++;

  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( _leds, _stripLength, 10);
  int pos = random16(_stripLength);
  _leds[pos] += CHSV( _hue + random8(64), 200, 255);
}


// a colored dot sweeping back and forth, with fading trails
void LEDStripController::Sinelon(){
  // increment the _hue position on every update so the rainbow moves
  _hue++;

  fadeToBlackBy( _leds, _stripLength, 20);

  // divide the BPM by 2 so that half the saw wave will finish at the actual BPM
  // set the low value to 0 and high to one less than strip length
  // set timebase reference to now so that the wave reference always starts at 0
  // then shift it by 1/4 wavelength using sizeof(int) / 4
  int pos = beatsin16( GLOBAL_BPM / 2, 0, _stripLength - 1 , _bsTimebase, 65536 / 4);  
  
  _leds[pos] += CHSV( _hue, SATURATION_FULL, 192);
}