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
  const int BRIGHTNESS_FULL = 255;
  const int SATURATION_FULL = 255;
  const int WHITE_COLOR = 255;   
  const uint16_t GLOBAL_BPM = 85;


  // ******************************************************************************************
  //    HARDWARE DEFINITION SELECT - 
  //        uncomment this line if you want to enable hardware definitions
  //        contained withint #if statement for __TURNERS_TESTING_TEENSY__
  //        otherwise the settings after #else will be used
  // ******************************************************************************************
  // #define __TURNERS_TESTING_TEENSY__
//   #define __TURNERS_TESTING_UNO__

  
  // ******************************************************************************************
  //    HARDWARE DEFINITIONS -- Change these based on your hardware setup
  // ******************************************************************************************

  // hardware definition for Turner's Testing setup  

  
  // hardware definition for Turner's Testing setup  
  #if defined(__TURNERS_TESTING_UNO__) 
    
    const int led = LED_BUILTIN;

    const int APIN = 13;     //pin number for StationA LED strip
    const int ALEN = 30;     //pixel count for StationA LED strip
    
    const int BPIN = 9;     // don't actually use this
    const int BLEN = 30;     //pixel count for StationB LED strip

    //const int CPIN = 11;    // don't actually use this
    const int CLEN = 30;    //standin pixel count for StationC LED strip

    // BUTTON DEFINITIONS
    #define PRIMARY_BUTTON_PIN  11   // has PWM

    // this is the button sodered directly to the blinkytape
    #define ONBOARD_PRIMARY_BUTTON_PIN  10   // has PWM

    // this is the second button used mostly for color changes
    #define SECONDARY_BUTTON_PIN  7      // does NOT have PWM

  #elif defined(__TURNERS_TESTING_TEENSY__)
    
    const int led = LED_BUILTIN;

    const int APIN = 3;     //pin number for StationA LED strip
    const int BPIN = 4;     //pin number for StationB LED strip
    const int CPIN = 5;     //pin number for StationC LED strip
    
    const int ALEN = 30;     //pixel count for StationA LED strip
    const int BLEN = 174;     //pixel count for StationB LED strip
    const int CLEN = 174;     //pixel count for StationC LED strip

  // the default settings to always fall back on
  #else

    const int led = 13;      //pin number for onboard LED

    const int APIN = 18;     //pin number for StationA LED strip
    const int ALEN = 80;    //pixel count for StationA LED strip
    
    const int BPIN = 19;     //pin number for StationB LED strip
    const int BLEN = 80;    //pixel count for StationB LED strip
    
    const int CPIN = 22;     //pin number for StationC LED strip
    const int CLEN = 80;    //pixel count for StationC LED strip

  #endif



  // ******************************************************************************************
  //    ANIMATION TYPES ENUM -- This makes it easy to change animations
  // ******************************************************************************************
  // sets the frames per second. Change this depending on how fast your processor is
  #ifndef FRAMES_PER_SECOND
    #define FRAMES_PER_SECOND  60
  #endif


  // *******  Helper macro for calculating the length of an array ******* 
  // creates a macro that computes the length of an array (number of elements)
  // assuming all of the elements are the same size as the element in position 0
  #define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

  // *******  Default Color Palette to use when instantiating LED Strip Controllers ******* 
  const CRGBPalette16 DEFAULT_PALETTE = RainbowColors_p;

#endif
