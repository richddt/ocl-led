/*-------------------------------------------------------------------------------------------------------*
 *                          GRADIENT PALETTES                                                            *
 * Below are a handful of Gradient Palettes that make it easier to apply a range of colors               *
 *        to an animation without changing the animation itself                                          *
 * A great resource on creating color palettes here: https://www.youtube.com/watch?v=Ukq0tH2Tnkc         *
 * Here's a great website for creating palettes (multiply position by 2.55): https://cssgradient.io/     *
 * Another great site for creating palettes: https://coolors.co/gradient-palette/f44369-3e3b92?number=7  *
 * Or find a pallete someone else has made here: http://soliton.vm.bytemark.co.uk/pub/cpt-city/          *
 * In order to use CPT-City you'll need to use the PaletteKnife Bookmarklet                              *
 * Get the Bookmarklet here: http://fastled.io/tools/paletteknife/                                       *
 *-------------------------------------------------------------------------------------------------------*/


#ifndef GradientPalettes_h
#define GradientPalettes_h

// **********************************************************
//      BUILT-IN PALETTES (converted to gradient palettes)
// **********************************************************
DEFINE_GRADIENT_PALETTE( tk_Rainbow_gp ) {
      0,  255,    0,    0, // Red
     32,  171,   85,    0, // Orange
     64,  171,  171,    0, // Yellow
     96,    0,  255,    0, // Green
    128,    0,  171,   85, // Aqua
    160,    0,    0,  255, // Blue
    192,   85,    0,  171, // Purple
    224,  171,    0,   85, // Pink
    255,  255,    0,    0  // and back to Red
};



DEFINE_GRADIENT_PALETTE( tk_Forest_gp ) {
      0,    0,  100,    0, // DarkGreen       -- 0x006400
     16,    0,  100,    0, // DarkGreen       -- 0x006400
     32,   85,  107,   47, // DarkOliveGreen  -- 0x556B2F
     48,    0,  100,    0, // DarkGreen       -- 0x006400

     64,    0,  128,    0, // Green           -- 0x008000
     80,   34,  139,   34, // ForestGreen     -- 0x228B22
     96,  107,  142,   35, // OliveDrab       -- 0x6B8E23
    112,    0,  128,    0, // Green           -- 0x008000
    
    128,   46,  139,   87, // SeaGreen        -- 0x2E8B57
    144,  102,  205,  170, // MediumAquamarine-- 0x66CDAA
    160,   50,  205,   50, // LimeGreen       -- 0x32CD32
    176,  154,  205,   50, // YellowGreen     -- 0x9ACD32
    
    192,  144,  238,  144, // LightGreen      -- 0x90EE90
    208,  124,  252,    0, // LawnGreen       -- 0x7CFC00
    224,  102,  205,  170, // MediumAquamarine-- 0x66CDAA
    240,   34,  139,   34, // ForestGreen    -- 0x228B22
    255,    0,  100,    0, // and back to the beginning
};



DEFINE_GRADIENT_PALETTE( tk_Party_gp ) {
      0,     85,    0,  171, // -- 0x 5500AB
     16,    132,    0,  124, // -- 0x 84007C
     32,    181,    0,   75, //  -- 0x B5004B
     48,    229,    0,   27, // -- 0x E5001B

     64,    232,   23,    0, // -- 0x E81700
     80,    184,   71,    0, // -- 0x B84700
     96,    171,  119,    0, // -- 0x AB7700
    112,    171,  171,    0, // -- 0x ABAB00
    
    128,    171,   85,    0, // -- 0x AB5500
    144,    221,   34,    0, // -- 0x DD2200
    160,    242,   14,    0, // -- 0x F2000E
    176,    194,    0,   62, // -- 0x C2003E
    
    192,    143,    0,  113, // -- 0x 8F0071
    208,     95,    0,  161, // -- 0x 5F00A1
    224,     47,    0,  208, // -- 0x 2F00D0
    240,      0,    7,  249, // -- 0x 0007F9
    255,     85,    0,  171  // and back to the beginning
};

// REMAKE of the builtin library's "HeatColors_p"
DEFINE_GRADIENT_PALETTE( tk_Fire_Red_gp ) {
    0,   0,     0,   0,   // black
   64,  204,    0,   0,   // red
   80,  255,    51,   0,  // red + little green
  144, 255,  255,    0,   // yellow
  255, 255,  255,  255    // white
};


// **********************************************************
//      TKS ORIGINAL GRADIENT PALETTES
// **********************************************************
DEFINE_GRADIENT_PALETTE( tk_Peacock_Colors_gp ) {
    0,  146,    0,  110,
   80,   40,    0,  215,
  112,    0,    6,  250,
  160,    0,  134,  122,
  208,    0,  215,   40,
  240,    0,  255,    0,
  255,  146,    0,  110
};


// **********************************************************
//      CPT-CITY COLOR PALETTES
// **********************************************************
// Gradient palette "Analogous_1_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/nd/red/tn/Analogous_1.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 20 bytes of program space.
DEFINE_GRADIENT_PALETTE( Analogous_1_gp ) {
    0,   3,  0,255,
   63,  23,  0,255,
  127,  67,  0,255,
  191, 142,  0, 45,
  255, 255,  0,  0};


// RENAME OF Gradient palette "fire_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/neota/elem/tn/fire.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 28 bytes of program space.
DEFINE_GRADIENT_PALETTE( tk_Fire_Candle_gp ) {
    0,   1,  1,  0,
   76,  32,  5,  0,
  146, 192, 24,  0,
  197, 220,105,  5,
  240, 252,255, 31,
  250, 252,255,111,
  255, 255,255,255};


// Gradient palette "Sunset_Real_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/nd/atmospheric/tn/Sunset_Real.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 28 bytes of program space.
DEFINE_GRADIENT_PALETTE( Sunset_Real_gp ) {
    0, 120,  0,  0,
   22, 179, 22,  0,
   51, 255,104,  0,
   85, 167, 22, 18,
  135, 100,  0,103,
  198,  16,  0,130,
  255,   0,  0,160};


// **********************************************************
//      OCL COLOR PALETTES
// **********************************************************
// Gradient palettes designed by Rich & Shade with the tool https://cssgradient.io/
DEFINE_GRADIENT_PALETTE( Shade4 ) {
0,  252,176,69,
26,  253,29, 29,
64,  187,45,111,
128, 131,58,180,
191, 200,42,94,
230, 253,29,29,
255, 252,176,69,};

DEFINE_GRADIENT_PALETTE( Shade5 ) {
0, 247,232,113,
51, 253,182,26,
102, 159,10,16,
153, 184,9,45,
242, 253,182,26,
255, 247,232,113};

DEFINE_GRADIENT_PALETTE( Shade6 ) {
  0,  27,25,52,
  26,  0,80,125,
  51,  0,150,200,
  77,  86,197,184,
  110, 172,245,168,
  145, 172,245,168,
  179, 86,197,184,
  204, 0,150,200,
  230, 0,80,125,
  255, 27,25,52};

DEFINE_GRADIENT_PALETTE( Shade7 ) {
0,  244,161,111,
26,  217,80,83,
51,  209,70,86,
77,  150,33,99,
102, 110,25,95,
128, 75,21,80,
153, 110,25,95,
179, 150,33,99,
204, 209,70,86,
230, 217,80,83,
255, 244,161,111};

DEFINE_GRADIENT_PALETTE( Shade8 ) {
0,  121,61,99,
51,  39,44,108,
89,  152,61,31,
128, 253,182,26,
166, 152,61,31,
204, 39,44,108,
255, 121,61,99};

DEFINE_GRADIENT_PALETTE( Shade9 ) {
0,  39,44,108,
51,  0,196,179,
102, 39,44,108,
128, 39,44,108,
153, 0,196,179,
255, 39,44,108};


#endif
