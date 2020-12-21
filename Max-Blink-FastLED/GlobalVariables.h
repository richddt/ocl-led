/*
  GlobalVariables.h  - Globally defined variables to be used throughout the sketch
                    -- Hardware definitions for various testing setups
*/

#ifndef GlobalVariables_h
#define GlobalVariables_h
  // ******************************************************************************************
  //    SHARED GLOBAL CONSTAND VARIABLES
  // ******************************************************************************************
  const int fastLEDGlobalBrightness = 255;     //set between 0 (off) to 255
  const int full_brightness = 255;
  const int full_saturation = 255;
  const int white_color = 255;   
  const uint16_t GLOBAL_BPM = 85;

  // ******************************************************************************************
  //    HARDWARE DEFINITION SELECT - selects hardware definitions below
  // ******************************************************************************************
  //#define __PRODUCTION_VERSION__
  #define __TK_TESTING__

  
  // ******************************************************************************************
  //    HARDWARE DEFINITIONS -- Change these based on your hardware setup
  // ******************************************************************************************

  #if defined(__PRODUCTION_VERSION__)

    const int led = 13;      //pin number for onboard LED
  
    const int NUM_STRIPS = 3;

    const int APIN = 18;     //pin number for StationA LED strip
    const int ALEN = 125;    //pixel count for StationA LED strip
    
    const int BPIN = 19;     //pin number for StationB LED strip
    const int BLEN = 125;    //pixel count for StationB LED strip
    
    const int CPIN = 22;     //pin number for StationC LED strip
    const int CLEN = 125;    //pixel count for StationC LED strip
    
  
  #elif defined(__TK_TESTING__) //|| defined(__TK_TESTING_2__)
    
    const int led = LED_BUILTIN;

    const int NUM_STRIPS = 3;

    const int APIN = 3;     //pin number for StationA LED strip
    const int BPIN = 4;     //pin number for StationB LED strip
    const int CPIN = 5;     //pin number for StationC LED strip
    
    const int ALEN = 30;     //pixel count for StationA LED strip
    const int BLEN = 125;     //pixel count for StationB LED strip
    const int CLEN = 125;     //pixel count for StationC LED strip
  
  #endif
  

  // ******************************************************************************************
  //    ANIMATION TYPES ENUM -- This makes it easy to change animations
  // ******************************************************************************************
  enum AnimationType {
    ALL_OFF,
    FADE_LOW,
    FADE_IN,
    FADE_OUT,
    FADE_IN_OUT,
    FADE_OUT_BPM,
    RAINBOW,
    RAINBOW_W_GLITTER,
    CONFETTI,
    SINELON,
    SOLID_COLOR,
    NONE
  };

  #define DEFAULT_UPDATE_INTERVAL 10
  
  #define ALL_OFF_UPDATE_INTERVAL 10
  #define FADE_UPDATE_INTERVAL 10
  #define RAINBOW_UPDATE_INTERVAL 10


  // ******************************************************************************************
  //    ANIMATION TYPES ENUM -- This makes it easy to change animations
  // ******************************************************************************************
  // sets the frames per second. Change this depending on how fast your processor is
  #ifndef FRAMES_PER_SECOND
    #define FRAMES_PER_SECOND  60
  #endif


#endif
