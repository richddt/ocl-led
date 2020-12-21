/*
  LEDStripController.h - Class that provides specific controlls of a defined LED Strip using the FastLED Library
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
    
    // our virtual strip representations
    CRGB *ledStrip;
    uint16_t ledStripStartIndex = 0;
    uint16_t numPixelsInStrip;

    // a globally defined Enum that makes it easy to update animations with human readable names
    AnimationType activeAnimationType;
    
    // mutable variables that save the state of the strip's hue, saturation and brightness
    uint8_t hue = 92;
    uint8_t saturation = full_saturation;
    uint8_t brightness = full_brightness;
  
    // mutable variables that help manage the state of parameters used in specific animations
    uint32_t beatStartTime = 0;
    bool showStrip = true;

    //General timing variables used in our Update() method
    unsigned long lastUpdateTime = 0; // time of last update of position
    uint16_t updateInterval = DEFAULT_UPDATE_INTERVAL;   // milliseconds between updates. Likely needs to be 5

    //INITIALIZATION AND STATIC STRIP COLOR METHODS
    void InitializeAnimation();
    void SetStripHSV(CHSV newCHSV);
    void SetStripHSV(CRGB newCRGB);
    
    //ANIMATION METHODS
    void AllOff();
    void FadeLow();
    void FadeInOut();
    void FadeOutBPM();
    void Rainbow();
    void RainbowWithGlitter();
    void AddGlitter( fract8 chanceOfGlitter);
    void Confetti();
    void Sinelon();

};



#endif
