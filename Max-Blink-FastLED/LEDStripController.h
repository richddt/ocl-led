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

// FASTLED_USING_NAMESPACE

// #if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
// #warning "Requires FastLED 3.1 or later; check github for latest code."
// #endif


// ******************************************************************
//    ANIMATION TYPES ENUM -- This makes it easy to change animations
// ******************************************************************
enum AnimationType {
  ALL_OFF,
  SOLID_COLOR,
  FADE_OUT_BPM,
  FADE_LOW_BPM,
  FADE_IN_OUT_BPM,
  PALETTE,
  PALETTE_W_GLITTER,
  CONFETTI,
  SINELON,  
  NONE
};


const CRGBPalette16 DEFAULT_PALETTE = RainbowColors_p;
const CRGBPalette16 ALL_COLOR_PALETTES[8] = {
                                                RainbowColors_p,
                                                ForestColors_p,
                                                CloudColors_p,
                                                LavaColors_p,
                                                OceanColors_p,
                                                PartyColors_p,
                                                HeatColors_p, // fire
                                                RainbowStripeColors_p,
                                              };

// ******************************************************************
//    UPDATE INTERVALS - the time in milliseconds between updates
// ******************************************************************
#define DEFAULT_UPDATE_INTERVAL 10
#define FADE_UPDATE_INTERVAL 5
#define PALETTE_UPDATE_INTERVAL 5
#define CONFETTI_UPDATE_INTERVAL 10
#define SINELON_UPDATE_INTERVAL 10


// this will set whether or not the strip is inverted
// meaning the beginning is the end and the end is the beginning
#define INVERT_STRIP true


// ******************************************************************
//            LEDStripController class definitions
// ******************************************************************
class LEDStripController
{

  //********** PUBLIC MEMBER VARIABLES AND FUNCTIONS **********
  public:
    //LEDStripController(CRGB *leds, uint16_t stripLength); // Constructor needs to be fully defined
    LEDStripController( CRGB *leds, 
                        uint16_t stripLength,
                        CRGBPalette16 colorPalette = DEFAULT_PALETTE,                        
                        uint8_t invertStrip = 0,
                        uint16_t stripStartIndex = 0 );
    void Update();

    AnimationType GetActiveAnimationType();
    void SetActiveAnimationType(AnimationType newAnimationState);
    void SetStripParams(uint8_t hue, uint8_t brightness, uint16_t bpm, uint8_t brightnessHigh, uint8_t brightnessLow);
    void SetColorPalette(CRGBPalette16 colorPalette);
    
    
    
  //********** PRIVATE MEMBER VARIABLES AND FUNCTIONS **********
  private:
    
    // our virtual strip representations
    CRGB *_leds;
    uint16_t _stripLength;
    CRGBPalette16 _colorPalette;    // the color palette to use in certain animations
    uint8_t _invertStrip;          // whether the strip is regular orientation (0) or reversed (1)


    // a globally defined Enum that makes it easy to update animations with human readable names
    AnimationType _activeAnimationType;
    
    // mutable variables that save the state of the strip's _hue, _saturation and _brightness
    uint8_t _hue = 92;
    uint8_t _saturation = SATURATION_FULL;
    uint8_t _brightness = BRIGHTNESS_FULL;
    uint8_t _brightnessHigh = BRIGHTNESS_FULL;
    uint8_t _brightnessLow = 40;
    uint16_t _bpm = GLOBAL_BPM;
  
    // mutable variables that help manage the state of parameters used in specific animations
    uint32_t _bsTimebase = 0;
    bool _showStrip = true;

    //General timing variables used in our Update() method
    unsigned long _lastUpdateTime = 0; // time of last update of position
    uint16_t _updateInterval = DEFAULT_UPDATE_INTERVAL;   // milliseconds between updates. Likely needs to be 5

    //INITIALIZATION AND STATIC STRIP COLOR METHODS
    void InitializeAnimation();
    void SetStripHSV(CHSV newCHSV);
    void SetStripHSV(CRGB newCRGB);
    
    //ANIMATION METHODS
    void AllOff();
    void SolidColor();
    void FadeOutBPM();
    void FadeLowBPM();
    void FadeInOutBPM();
    void Palette();
    void PaletteWithGlitter();
    void AddGlitter( fract8 chanceOfGlitter);
    void Confetti();
    void Sinelon();

};



#endif
