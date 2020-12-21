/*
  LEDStripController.h     - Class that provides specific controlls of a defined LED Strip using the FastLED Library
                                Originally intended to play nice with MIDI controllers
  Created by Turner E. Kirk, January 20, 2019.
  Released into the public domain.
*/

#ifndef LEDStripController_h
#define LEDStripController_h

// ******************************************************************
//            Includes and Defines
// ******************************************************************
#include <FastLED.h>
#include "GlobalVariables.h"


// ******************************************************************
//            LEDStripController class definitions
// ******************************************************************
class LEDStripController
{

  //********** PUBLIC MEMBER VARIABLES AND FUNCTIONS **********
  public:
    LEDStripController(CRGB *leds, uint16_t _numPixelsInStrip); // Constructor needs to be fully defined
    void Update();

    AnimationType GetActiveAnimationType();
    void SetActiveAnimationType(AnimationType newAnimationState);
    
    
    
  //********** PRIVATE MEMBER VARIABLES AND FUNCTIONS **********
  private:
    
    CRGB *ledStrip;
    uint16_t ledStripStartIndex = 0;
    uint16_t numPixelsInStrip;

    AnimationType activeAnimationType;
    
    uint8_t hue = 92;
    uint8_t saturation = full_saturation;
    uint8_t brightness = bright;
   
    CHSV stripColorHSV = CHSV( hue, saturation, brightness);

    // INITIAL SETTING FOR COLOR OF STRIP  // new setting as of April 15th 2019
    CRGB rgbPlaceholder = CRGB::Black; //CRGB(0, 0, 60) <-- using this will set strip to a dim blue 

    //GENERAL UPDATE TIME INTERVALS
    unsigned long lastUpdateTime = 0; // time of last update of position
    uint16_t updateInterval = DEFAULT_UPDATE_INTERVAL;   // milliseconds between updates. Likely needs to be 5

    void InitializeAnimation();
    void SetStripHSV(uint8_t _hue, uint8_t _saturation, uint8_t _brightness);
    void AllOff();
    void FadeLow();
    void Rainbow();
    void RainbowWithGlitter();
    void AddGlitter( fract8 chanceOfGlitter);

};



#endif
