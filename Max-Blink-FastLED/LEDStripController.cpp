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

void LEDStripController::Update(uint32_t currentTime) { //keep this list sync'd with LEDStripController.h ANIMATION TYPES ENUM

  if( currentTime > _timeToUpdate ){
    switch(_activeAnimationType) {
      case ALL_OFF:
        AllOff();
        break;
      case SOLID_COLOR:
        SolidColor();
        break;
      case FADE_OUT_BPM:  
        FadeOutBPM();
        break;
      case FADE_LOW_BPM:
        FadeLowBPM();
        break;
      case FADE_IN_OUT_BPM:
        FadeInOutBPM();
        break;
      case PALETTE:
        Palette();
        break;
      case PALETTE_W_GLITTER:
        PaletteWithGlitter();
        break;
      case PALETTE_FADE_LOW_BPM:
        PaletteFadeLowBPM();
        break;
      case PALETTE_W_GLITTER_FADE_LOW_BPM:
        PaletteWithGlitterFadeLowBPM();
        break;
      case CONFETTI:
        Confetti();
        break;
      case SINELON:
        Sinelon();
        break;
      case SINEPULSE:
        Sinepulse();
        break;
      case DDT_EXPERIMENTAL:
        DDT_Experimental();
        break;
      case NONE:
        break;
      default:
        // do something for the default
        break;
    }
    
    _timeToUpdate = currentTime + _updateInterval;
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
    case SOLID_COLOR:
      _updateInterval = DEFAULT_UPDATE_INTERVAL;
      break;
    case FADE_OUT_BPM:
    case FADE_LOW_BPM:
      _showStrip = true;
    case FADE_IN_OUT_BPM:
      _bsTimebase = millis();
      _updateInterval = FADE_UPDATE_INTERVAL;
      break;
    case PALETTE:
    case PALETTE_W_GLITTER:
      //_paletteHue = 0;
      _updateInterval = PALETTE_UPDATE_INTERVAL;
      break;
    case DDT_EXPERIMENTAL:
    case PALETTE_FADE_LOW_BPM:
    case PALETTE_W_GLITTER_FADE_LOW_BPM:
      _showStrip = true;
      _bsTimebase = millis();
      _updateInterval = PALETTE_UPDATE_INTERVAL;
      break;
    case CONFETTI:
      //_paletteHue = 0;
      _updateInterval = CONFETTI_UPDATE_INTERVAL;
      break;
    case SINELON:
      //_paletteHue = 0;
      _bsTimebase = millis();
      _updateInterval = SINELON_UPDATE_INTERVAL;
      break;
    case SINEPULSE:  
      _paletteHue = 0;      
      _bsTimebase = millis();     
      _lastPos = -1;
      _updateInterval = SINEPULSE_UPDATE_INTERVAL;
      break;
    case NONE:
      _updateInterval = DEFAULT_UPDATE_INTERVAL;
      break;
    default:
      _updateInterval = DEFAULT_UPDATE_INTERVAL;
      break;
  }

}


void LEDStripController::SetStripParams(uint8_t hue, uint8_t brightness, uint16_t bpm, uint8_t brightnessHigh, uint8_t brightnessLow){

  _hue = hue;
  _brightness = brightness;
  _brightnessHigh = brightnessHigh;
  _brightnessLow = brightnessLow;
  _bpm = bpm;

}


void LEDStripController::SetColorPalette(CRGBPalette16 colorPalette){
  _colorPalette = colorPalette;
}



void LEDStripController::SetStripHueIndexBPM(uint16_t hueIndexBPM){
  _hueIndexBPM = hueIndexBPM;
}



void LEDStripController::ReverseStripHueIndexDirection(){
  _reverseHueIndexDirection = !_reverseHueIndexDirection;
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
  fadeToBlackBy( _leds, _stripLength, 20);
}

void LEDStripController::SolidColor(){
  SetStripHSV(CHSV( _hue, _saturation, _brightness));
}


// this function fades out brightness similar to the "FadeLow()" function
// except this one does so over the course of one full beat at the speed of the _bpm
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

    // we divide the _bpm by 2 because we are only using half of the saw wave (from 255 to 0)
    // set the low value to 0 and high to _brightnessHigh
    // set _bsTimebase reference to now so that the wave reference always starts at 0
    // then shift it by 1/4 wavelength using sizeof(byte) / 4    
    uint8_t brightness = beatsin8( _bpm / 2 , 0, _brightnessHigh, _bsTimebase, 256 / 4);
  
    // since we only want to trigger this animation as a one shot, 
    // we need to disable the animation once the brightnes drops below a certain threshold
    // as well as set the brightnes to 0 (or whatever value we want to stop it at)
    if(brightness > 5){ // MAGIC NUMBER ALERT!!!
      SetStripHSV(CHSV( _hue, _saturation, brightness));
    }
    else {
      SetStripHSV(CRGB::Black);
      _showStrip = false;      
    }
  
  }

}


void LEDStripController::FadeLowBPM() {

  if(_showStrip){

    // we divide the _bpm by 2 because we are only using half of the saw wave (from 255 to 0)
    // set the low value to 0 and high to _brightnessHigh
    // set _bsTimebase reference to now so that the wave reference always starts at 0
    // then shift it by 1/4 wavelength using sizeof(byte) / 4    
    uint8_t brightness = beatsin8( _bpm / 2 , 0, _brightnessHigh, _bsTimebase, 256 / 4);
  

    // since we only want to trigger this animation as a one shot, 
    // we need to disable the animation once the brightnes drops below a certain threshold
    // as well as set the brightnes to 0 (or whatever value we want to stop it at)
    if(brightness > _brightnessLow && brightness > 5){ // MAGIC NUMBER ALERT!!!
      SetStripHSV(CHSV( _hue, _saturation, brightness));
    }
    else {
      SetStripHSV(CHSV( _hue, _saturation, _brightnessLow));
      _showStrip = false;      
    }
  
  }

}



// same as above except fades in and out stopping at the low threshold
void LEDStripController::FadeInOutBPM() {

  // we divide the _bpm by 2 because we are only using half of the saw wave (from 255 to 0)
  // set the low value to 0 and high to _brightnessHigh
  // set _bsTimebase reference to now so that the wave reference always starts at 0
  // then shift it by 1/4 wavelength using sizeof(byte) / 4    
  uint8_t brightness = beatsin8( _bpm / 2 , 0, _brightnessHigh, _bsTimebase, 256 / 4);

  // since we only want to trigger this animation as a one shot, 
  // we need to disable the animation once the brightnes drops below a certain threshold
  // as well as set the brightnes to 0 (or whatever value we want to stop it at)
  if(brightness > _brightnessLow){
    SetStripHSV(CHSV( _hue, _saturation, brightness));
  }
  

}



// a simple function for creating an animated color palettes
void LEDStripController::Palette()
{
  
  fill_palette( _leds, _stripLength, getHueIndex( _hueIndexBPM ), (256 / _stripLength), _colorPalette, _brightness, LINEARBLEND);
}


// same as the above function except this also ads glitter
void LEDStripController::PaletteWithGlitter() {
  // reference our palette function, plus some random sparkly glitter
  Palette();
  AddGlitter(80); // MAGIC NUMBER ALERT!!!
}

// the glitter function, randomly selects a pixel and sets it to white
void LEDStripController::AddGlitter( fract8 chanceOfGlitter, uint8_t brightness) {
  if( random8() < chanceOfGlitter) {
    //_leds[ random16(_stripLength) ] += CRGB::White;
    _leds[ random16(_stripLength) ] = CHSV( 0, 0, brightness);
  }
}


// combining the FadeLowBPM and Palette functions
void LEDStripController::PaletteFadeLowBPM() {

  // we divide the _bpm by 2 because we are only using half of the saw wave (from 255 to 0)
  // set the low value to 0 and high to _brightnessHigh
  // set _bsTimebase reference to now so that the wave reference always starts at 0
  // then shift it by 1/4 wavelength using sizeof(byte) / 4    
  uint8_t brightness = beatsin8( _bpm / 2 , 0, _brightnessHigh, _bsTimebase, 256 / 4);


  // since we only want to trigger this animation as a one shot, 
  // we need to disable the animation once the brightnes drops below a certain threshold
  // as well as set the brightnes to 0 (or whatever value we want to stop it at)
  if(brightness > _brightnessLow && brightness > 5 && _showStrip){ // MAGIC NUMBER ALERT!!!
    fill_palette( _leds, _stripLength, getHueIndex( _hueIndexBPM ), (256 / _stripLength), _colorPalette, brightness, LINEARBLEND);
  }
  else {
    fill_palette( _leds, _stripLength, getHueIndex( _hueIndexBPM ), (256 / _stripLength), _colorPalette, _brightnessLow, LINEARBLEND);
    _showStrip = false;
  }
  

}

// same as the above function except this also ads glitter
void LEDStripController::PaletteWithGlitterFadeLowBPM() {
  // reference our palette function, plus some random sparkly glitter
  PaletteFadeLowBPM();
  AddGlitter(80, _brightness); // MAGIC NUMBER ALERT!!!
}






// Pop and Fade. like confetti!!!
void LEDStripController::Confetti() {
  
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( _leds, _stripLength, 10); // MAGIC NUMBER ALERT!!! 20 is the speed of the brightness fade, smaller = longer fade
  uint16_t pos = random16(_stripLength);
  //int pos = random16(_stripLength);
  
  _paletteHue++;
  // if you want to draw from a palette use this method
  //_leds[pos] += ColorFromPalette( _colorPalette, random8(), _brightness);
  if (random8()<_bpm){  // probability control for how many LED's pop simultaneously. Dependent on processor speed & LED count. Default = 180 out of 255
    _leds[pos] += ColorFromPalette( _colorPalette, _paletteHue + random8(64), _brightness);
  }
  // here's an alternate method
  //_paletteHue++;
  //_leds[pos] += CHSV( _paletteHue + random8(64), 200, 255);
  
}


// a colored dot sweeping back and forth, with fading trails
void LEDStripController::Sinelon(){

  // fade the entire strip by 20. This is what causes the animation to have a tail
  fadeToBlackBy( _leds, _stripLength, 20); // MAGIC NUMBER ALERT!!!

  // increment the _paletteHue position so the color moves through the palette
  _paletteHue++;

  // divide the BPM by 2 so that half the saw wave will finish at the actual BPM
  // set the low value to 0 and high to one less than strip length
  // set timebase reference to now so that the wave reference always starts at 0
  // then shift it by 1/4 wavelength using sizeof(int) / 4
  int pos = beatsin16( _bpm / 2, 0, _stripLength - 1 , _bsTimebase, 65536 / 4);  
  
  if(_invertStrip){
    pos = (_stripLength-1) - pos;
  }

  // add the palette color to the led at pos
  _leds[pos] += ColorFromPalette( _colorPalette, _paletteHue, _brightness);

  //_leds[pos] += CHSV( _paletteHue, SATURATION_FULL, _brightness);

}



// a colored dot sweeping from top to bottom
void LEDStripController::Sinepulse(){

  // fade the entire strip by 20. This is what causes the animation to have a tail
  fadeToBlackBy( _leds, _stripLength, 20); // MAGIC NUMBER ALERT!!!

  // increment the _paletteHue position so the color moves through the palette
  _paletteHue++;

  // divide the BPM by 2 so that half the saw wave will finish at the actual BPM
  // set the low value to 0 and high to one less than strip length
  // set timebase reference to now so that the wave reference always starts at 0
  // then shift it by 1/4 wavelength using sizeof(int) / 4
  int pos = beatsin16( _bpm / 2, 0, _stripLength - 1, _bsTimebase, 65536 / 4);

  if( (_lastPos == -1) && (pos == 0) ){
    // if we just triggered the animation (_lastPos == -1) and pos is greater than 0 then we should wait until it is 0 before doing anything useful
  }
  else{

    if((_lastPos != -1) && pos > _lastPos){
      SetActiveAnimationType(ALL_OFF);      
    }
    else{

       _lastPos = pos;
      
      if(_invertStrip){
        pos = (_stripLength-1) - pos;
      }
    
      // add the palette color to the led at pos
      _leds[pos] += ColorFromPalette( _colorPalette, _paletteHue, _brightness);    
    }
  }

  //_leds[pos] += CHSV( _paletteHue, SATURATION_FULL, _brightness);
}




// experiments with Waves animation (similar to Sinelon)
void LEDStripController::DDT_Experimental(){
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( _leds, _stripLength, 20); // MAGIC NUMBER ALERT!!!
  uint16_t pos = random16(_stripLength);  
  //int pos = random16(_stripLength);
  
  _paletteHue++;
  // if you want to draw from a palette use this method
  //_leds[pos] += ColorFromPalette( _colorPalette, random8(), _brightness);
  _leds[pos] += ColorFromPalette( _colorPalette, _paletteHue + random8(64), _brightness);
  
  // here's an alternate method
  //_paletteHue++;
  //_leds[pos] += CHSV( _paletteHue + random8(64), 200, 255);
}




// *********************************************************************************
//      CLASS HELPER FUNCTIONS
// *********************************************************************************

// get the next hue based on the bpm and if the strip is inverted or not
// uint8_t LEDStripController::getHueIndex(uint8_t hueIndexBPM, uint8_t reverseDirecton){
uint8_t LEDStripController::getHueIndex(uint8_t hueIndexBPM){

    // XOR logic
    if(_reverseHueIndexDirection != _invertStrip){
        return beat8(hueIndexBPM);  // leds appear to be moving reverse
    }
    else {
        return 255 - beat8(hueIndexBPM);  // leds appear to be moving forward
    }    
}
