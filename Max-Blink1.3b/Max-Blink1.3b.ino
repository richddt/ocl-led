/*  Simple example of Max->Teensy communication.   9/7/2020 ams
   The Teensy listens for incoming characters over the USB host serial port.
   When a "b" is received, the Teensy will flash the onboard LED for 1/100 of a second.
   When a "B" is received, it will flash the LED for 1/10 of a second.

   Also, the Teensy sends a "K" back to Max when it receives a command. This can be ignored, or used as a way for Max to verify that the Teensy is alive and connected.

   The accompanying Max patch is setup to send B on the downbeat and b on every quarter note.
   This example jist shows how to listen for different characters and respond.

   The LED flashing code is placed in a function, hopefully to make it clearer and easier to replace with code that does something cooler!

*/

/////// INCLUDES ///////
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

/////// CONSTANTS ///////
//configuration constant variables to change:
#define baudRate 9600   //this is a safe and common rate. Feel free to change it as desired. Justmake sure that Max and the Teensy are at the same setting.

const int led = 13;             //pin number for onboard LED

const int APIN = 18;     //pin number for StationA LED strip
const int ALEN = 125;     //pixel count for StationA LED strip

const int BPIN = 19;     //pin number for StationB LED strip
const int BLEN = 125;     //pixel count for StationB LED strip

const int CPIN = 22;     //pin number for StationC LED strip
const int CLEN = 125;     //pixel count for StationC LED strip

const int bright = 255;     //set between 0 (off) to 255

//const int FadeSteps = 50; // how many steps of brightness to fade in/out (default = 20)
//const int FadeDelay = 5; // delay betw each step of the fade (default = 10)

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel Astrip = Adafruit_NeoPixel(ALEN, APIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel Bstrip = Adafruit_NeoPixel(BLEN, BPIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel Cstrip = Adafruit_NeoPixel(CLEN, CPIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

char incomingByte;        // variable used for data from Max.

/////// SETUP ///////
//setup runs once at startup:
void setup() {
  pinMode(led, OUTPUT);     // initialize the digital pin as an output.

  Serial.begin(baudRate);     //initialize the host USB port.
  Astrip.begin();
  Astrip.setBrightness(bright);    
  Astrip.show(); // Initialize all pixels to 'off'
}

////// MAIN LOOP ///////
//loop runs over and over "forever":
void loop() {

  if (Serial.available()) {           // check for incoming bytes
    incomingByte = Serial.read();       // read incoming byte

    switch (incomingByte) {           // check the incomingByte for a match to a valid command
      case 'b':                       // b = every 2nd & 3rd 1/4 Note (170BPM): every 0.35 sec - max event time is 350ms - 300 to be safe
        triggerLED(10);               // act on the command 'b'
//        Serial.print ("Beat");           // acknowledge the command
        setAll(0,0,0); // Off        
        Astrip.show();
        break;

      case 'U':                       // B = 1st 1/4 Note aka "on the One" (170BPM): every 1.06 sec - max event time is 1060ms - 1000 to be safe
        triggerLED(1);              // act on the command 'B'  Alexnote 9/20: this function is "blocking", meaning the following code will not execute until after the 100 msec blink caused by calling this. Might help to make this shorter or rearrange order?
//        Serial.print ("170BPM Bar+Beat");           // acknowledge the command
//        setAll(255,0,0);            // Red pulse
//        delay(1500);
//        setAll(0,0,0);      
//       170bpm pulse:  FadeLow(255,0,255,90,7,25)
        FadeLow(255,0,200,90,3,25);          // Fade: (R, G, B, Steps, Delay, LowLevel). R/G/B/Steps/Low = 0-255. Steps*Delay = duration of Fade (max event time)
        break;

      case 'H':                       // B = 1st 1/4 Note aka "on the One" (170BPM): every 1.06 sec - max event time is 1060ms - 1000 to be safe
        triggerLED(1);              // act on the command 'B'  Alexnote 9/20: this function is "blocking", meaning the following code will not execute until after the 100 msec blink caused by calling this. Might help to make this shorter or rearrange order?
//        Serial.print ("170BPM Bar");           // acknowledge the command
//        setAll(255,0,0);            // Red pulse
//        delay(1500);
//        setAll(0,0,0);      
//       170bpm pulse:  FadeLow(255,0,255,90,7,25)
        FadeLow(255,0,200,90,7,25);          // Fade: (R, G, B, Steps, Delay, LowLevel). R/G/B/Steps/Low = 0-255. Steps*Delay = duration of Fade (max event time)
        break;

      case 'M':                       // B = 1st 1/4 Note aka "on the One" (170BPM): every 1.06 sec - max event time is 1060ms - 1000 to be safe
        triggerLED(1);              // act on the command 'B'  Alexnote 9/20: this function is "blocking", meaning the following code will not execute until after the 100 msec blink caused by calling this. Might help to make this shorter or rearrange order?
//        Serial.print ("85BPM Bar");           // acknowledge the command
//        setAll(255,0,0);            // Red pulse
//        delay(1500);
//        setAll(0,0,0);      
//        FadeOut(255,0,255,80,7);          // Fade: (R, G, B, Steps, Delay). R/G/B/Steps = 0-255. Steps*Delay = duration of Fade (max event time)
//       170bpm pulse:  FadeLow(255,0,255,90,7,25)
        FadeLow(255,0,200,90,7,25);          // Fade: (R, G, B, Steps, Delay, LowLevel). R/G/B/Steps/Low = 0-255. Steps*Delay = duration of Fade (max event time)
//        delay(20);                      // ?? Do I need to calculate time window betw/ bars and match that here?
//        FadeIn(255,0,255,50,1);          // Fade: (R, G, B, Steps, Delay). R/G/B/Steps = 0-255. Steps*Delay = duration of Fade (max event time)
//        delay(500);
//        Astrip.show();
        break;
//      default:
//        Serial.print ("?");           // no valid command recognized, reply with "?"

      case 'L':                       // B = 1st 1/4 Note aka "on the One" (170BPM): every 1.06 sec - max event time is 1060ms - 1000 to be safe
        triggerLED(1);              // act on the command 'B'  Alexnote 9/20: this function is "blocking", meaning the following code will not execute until after the 100 msec blink caused by calling this. Might help to make this shorter or rearrange order?
//        Serial.print ("85BPM 2xBar");           // acknowledge the command
//        setAll(255,0,0);            // Red pulse
//        delay(1500);
//        setAll(0,0,0);      
//       170bpm pulse:  FadeLow(255,0,255,90,7,25)
        FadeLow(0,180,180,80,7,50);          // Fade: (R, G, B, Steps, Delay, LowLevel). LowLevel has to be < Steps. R/G/B/Steps/Low = 0-255. Steps*Delay = duration of Fade (max event time)
//         FadeLow(10,255,255,90,7,25);
        break;


    
    
    }  // end of switch
    
  }
  
}    // end of loop()


///// FUNCTIONS //////

void triggerLED(int blinkTime) {      // this function blinks the onboard LED. The argument sets how long is the blink.
  digitalWrite(led, HIGH);            // turn the LED on (HIGH is the voltage level)
  delay(blinkTime);                   // wait for a second
  digitalWrite(led, LOW);             // turn the LED off by making the voltage LOW
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<Astrip.numPixels(); i++) {
    Astrip.setPixelColor(i, c);
    Astrip.show();
    delay(wait);
  }
}

void showStrip() {
   // NeoPixel
   Astrip.show();
}

void setPixel(int Pixel, byte red, byte green, byte blue) {
   // NeoPixel
   Astrip.setPixelColor(Pixel, Astrip.Color(red, green, blue));  //strip A,B,C
   Bstrip.setPixelColor(Pixel, Astrip.Color(red, green, blue));  //strip A,B,C   
   Cstrip.setPixelColor(Pixel, Astrip.Color(red, green, blue));  //strip A,B,C
}

void setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < ALEN; i++ ) {
    setPixel(i, red, green, blue);
  }
  showStrip();
}

// INTEGRATE CODE BELOW:
void FadeLow(byte red, byte green, byte blue, float FadeSteps, int FadeDelay, int LowLevel) {
    float r, g, b;
 
    for(float j = FadeSteps; j >= LowLevel; j--) {
      r = j*(red/FadeSteps);
      g = j*(green/FadeSteps);
      b = j*(blue/FadeSteps);
     
      setAll(r,g,b);
      delay(FadeDelay);
      }
//      delay(FadeDelay);     
      Astrip.show();
      Bstrip.show(); 
      Cstrip.show();     
//      delay(FadeDelay);
//      Serial.print(FadeDelay);
//      Serial.print('\n');
    }

void FadeInOut(byte red, byte green, byte blue){
  float r, g, b;
     
  for(int k = 0; k < 256; k=k+1) {
    r = (k/256.0)*red;
    g = (k/256.0)*green;
    b = (k/256.0)*blue;
    setAll(r,g,b);
    showStrip();
  }
     
  for(int k = 255; k >= 0; k=k-2) {
    r = (k/256.0)*red;
    g = (k/256.0)*green;
    b = (k/256.0)*blue;
    setAll(r,g,b);
    showStrip();
  }
}

void FadeOut(byte red, byte green, byte blue, int FadeSteps, int FadeDelay) {
    float r, g, b;
 
    for(int j = FadeSteps; j >= 0; j--) {
      r = j*(red/FadeSteps);
      g = j*(green/FadeSteps);
      b = j*(blue/FadeSteps);
     
      setAll(r,g,b);
      delay(FadeDelay);
      }
//      delay(FadeDelay);     
      Astrip.show();
//      delay(FadeDelay);
//      Serial.print(FadeDelay);
//      Serial.print('\n');
    }

void FadeIn(byte red, byte green, byte blue, int FadeSteps, int FadeDelay) {
    float r, g, b;
 
    for(int k = 0; k <= FadeSteps; k++) {
      r = k*(red/FadeSteps);
      g = k*(green/FadeSteps);
      b = k*(blue/FadeSteps);
     
      setAll(r,g,b);
      delay(FadeDelay);
      }
//      delay(FadeDelay);     
      Astrip.show();
//      delay(FadeDelay);
//      Serial.print(FadeDelay);
    }
