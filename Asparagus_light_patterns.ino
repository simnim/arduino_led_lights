/*
 * Nick Hahner 2016
 * 
 *  Add new led pattern function definitions somewhere in the section 
 *  titled "DEFINE YOUR FUNCTIONS HERE", then add the function
 *  to led_patterns.
 *  
 *  Make sure your led pattern functions are infinite loops.
 *  They'll run for about 5 mins until the device reboots.
 * 
 * 
 */

// Optionally print out various useless messages about program state.
#define DEBUG

// This is necessary for the esp chips so that interrupts 
// don't fire while the led strip is being refreshed.
// Must be defined before the fastled import
#define FASTLED_ALLOW_INTERRUPTS 0


#include <EEPROM.h>
#include "FastLED.h"

#define EEPROM_CYCLE_ADDRESS 0

#define NUM_LEDS 45
#define DATA_PIN 13

#define VSHORT_DELAY 10
#define SHORT_DELAY 20
#define MED_DELAY 50
#define LONGISH_DELAY 100

#define NUM_HUES 256

// Let's have a green breathing pattern for when the strip first turns on.
#define MILLISECONDS_OF_GREEN_BREATHING (1000 * 60)
// Instead of turning off, let's keep them at least at this level
#define BREATHING_MIN_BRIGHTNESS 40

// For a 2A power supply this should be max 70
// Anywhere 5-50 is a great range for testing.
// Some of my functions were using just a straight value for the RGB setting.
// This is not using the FastLED brightness setting
#define RGB_INTENSITY 5
// Define this if you want FastLED to dim things globally
//#define FASTLED_BRIGHTNESS 50

// < GLOBALS >
// Our leds represented as an array.
CRGB leds[NUM_LEDS];
// For ease of use and since it does not matter I only use one byte of precision. #LAZY
uint8_t cycle_count;
// </ GLOBALS >


// < SOME UTIL FUNCTIONS >
// < SOME UTIL FUNCTIONS >
// < SOME UTIL FUNCTIONS >
// < SOME UTIL FUNCTIONS >
// < SOME UTIL FUNCTIONS >
// < SOME UTIL FUNCTIONS >
// < SOME UTIL FUNCTIONS >
// < SOME UTIL FUNCTIONS >


// Keep count of how many power cycles have passed. If you 
// want to zero it out and start over, use the eeprom_clear sketch.
void fetch_cycle_count_iterate_using_eeprom() {
  // Get the number of times the chip has been turned on and off from the eeprom
  cycle_count = EEPROM.read(EEPROM_CYCLE_ADDRESS);
  // The value might be maxed out (8 bits of 1's) on a fresh chip so we change that to 0
  if (cycle_count == 255) { cycle_count = 0; }
  #ifdef DEBUG
    Serial.print("Cycle count: ");
    Serial.println(cycle_count);
  #endif
  // Increment the count and write it back to eeprom
  EEPROM.write(EEPROM_CYCLE_ADDRESS, cycle_count + 1);
  EEPROM.commit();
}
  

// rotate all the leds by one per DELAY in an infinite loop
void rotate_leds(unsigned int DELAY) {
    while(1) {
      // Rotate the array one by one until we hit the end.
      CRGB temp;
      for (unsigned int i = 0; i < NUM_LEDS-1; i++) {
        temp = leds[i+1];
        leds[i+1] = leds[i];
        leds[i] = temp;
      }
      FastLED.show();
      FastLED.delay(DELAY);
  }
}


// A useful way to display a number on the strip.
// Uses include displaying a cycle count or device id on boot.
void show_counter_unary(int num_to_show) {
  for (unsigned int i = 0; i < NUM_LEDS; i++) {leds[i] = CRGB::Black;}
  for (unsigned int i = 0; i < num_to_show; i++) {
    if ((i+1) % 10 == 0) {
      leds[i] = CRGB(RGB_INTENSITY,0,0);
    } else if ((i+1) % 5) {
      leds[i] = CRGB(0,RGB_INTENSITY,0);
    } else {
      leds[i] = CRGB(0,0,RGB_INTENSITY);
    }
  }
  #ifdef DEBUG
     Serial.print("show_counter_unary: ");
     Serial.println(num_to_show);
  #endif
  FastLED.delay(1);  // Forgot why I added this, not sure it's useful, but it's not harmful.
  FastLED.show();
}

// </ SOME UTIL FUNCTIONS >
// </ SOME UTIL FUNCTIONS >
// </ SOME UTIL FUNCTIONS >
// </ SOME UTIL FUNCTIONS >
// </ SOME UTIL FUNCTIONS >
// </ SOME UTIL FUNCTIONS >





// < DEFINE YOUR FUNCTIONS HERE >
// < DEFINE YOUR FUNCTIONS HERE >
// < DEFINE YOUR FUNCTIONS HERE >
// < DEFINE YOUR FUNCTIONS HERE >
// < DEFINE YOUR FUNCTIONS HERE >
// < DEFINE YOUR FUNCTIONS HERE >
// < DEFINE YOUR FUNCTIONS HERE >



void addGlitter( fract8 chanceOfGlitter) {
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void rainbow_with_glitter() {
 static uint8_t hue = 0;
  while (1){
    fill_rainbow( leds, NUM_LEDS, hue++, 7);
    addGlitter(80);
    FastLED.show();
    FastLED.delay(VSHORT_DELAY);
  }
}

void confetti() {
  static uint8_t hue = 0;
  while (1){
    // random colored speckles that blink in and fade smoothly
    fadeToBlackBy( leds, NUM_LEDS, 10);
    int pos = random16(NUM_LEDS);
    leds[pos] += CHSV( hue++ + random8(64), 200, 255);
    FastLED.show();
    FastLED.delay(VSHORT_DELAY);
  }
}

// Rotate as many hues as possible in a loop.
void rainbow_wave() {
  static uint8_t hue = 0;
  while (1){
    fill_rainbow( leds, NUM_LEDS, hue++, 7);
    FastLED.show();
    FastLED.delay(VSHORT_DELAY);
  }
}


// Taste the rainbow
void rainbow_strip() { 
  static uint8_t hue = 0;
  while (1){
    FastLED.showColor(CHSV(hue++, 255, 60)); 
    FastLED.delay(VSHORT_DELAY);
  }
}



void sinelon() {
  static uint8_t hue = 0;
  while(1) {
    // a colored dot sweeping back and forth, with fading trails
    fadeToBlackBy( leds, NUM_LEDS, 20);
    int pos = beatsin16(13,0,NUM_LEDS);
    leds[pos] += CHSV( hue++, 255, 192);
    FastLED.show();
    FastLED.delay(VSHORT_DELAY);
  }
}



void bpm() {
   static uint8_t hue = 0;
   while (1) {
      // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
      uint8_t BeatsPerMinute = 62;
      CRGBPalette16 palette = PartyColors_p;
      uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
      for( int i = 0; i < NUM_LEDS; i++) { //9948
        leds[i] = ColorFromPalette(palette, hue+(i*2), beat-hue+(i*10));
      }
      FastLED.show();
      FastLED.delay(VSHORT_DELAY);
      hue++;
    }
}

void juggle() {
   static uint8_t hue = 0;
   while(1) {
      // eight colored dots, weaving in and out of sync with each other
      fadeToBlackBy( leds, NUM_LEDS, 20);
      byte dothue = 0;
      for( int i = 0; i < 8; i++) {
        leds[beatsin16(i+7,0,NUM_LEDS)] |= CHSV(dothue, 200, 255);
        dothue += 32;
      }
      FastLED.show();
      FastLED.delay(VSHORT_DELAY);
   }
}


// Mildly interesting pattern of rotating red, green, and blue leds
void red_green_blue_rotation() {
  // Set the initial array
  for (unsigned int i = 0; i < NUM_LEDS; i++) {
    if ((i+1) % 10 == 0) {
      leds[i] = CRGB(RGB_INTENSITY,0,0);
    } else if ((i+1) % 5) {
      leds[i] = CRGB(0,RGB_INTENSITY,0);
    } else {
      leds[i] = CRGB(0,0,RGB_INTENSITY);
    }
  }
  FastLED.show();
  rotate_leds(LONGISH_DELAY);
}



// Fun pattern that'll run for a bit on boot then change to the interesting pattern.
// I this guy is a fun way to show off the arduinos AND still have some green time.
void green_breathing_inner_loop() {
  // This code is inspired by http://sean.voisen.org/blog/2011/10/breathing-led-with-arduino/
  float green_bval = (exp(cos((millis()-2000)/2000.0*PI)) - 0.36787944)*108.0;
  // Actually, I don't want this to dim to nothing. I'll keep it at 
  // at least BREATHING_MIN_BRIGHTNESS by remapping from [0,255] to [_,255]
  green_bval = green_bval*(255-BREATHING_MIN_BRIGHTNESS)/255 + BREATHING_MIN_BRIGHTNESS;
  FastLED.showColor(CRGB(0,green_bval,0));
  FastLED.delay(VSHORT_DELAY);
}

// </ DEFINE YOUR FUNCTIONS HERE >
// </ DEFINE YOUR FUNCTIONS HERE >
// </ DEFINE YOUR FUNCTIONS HERE >
// </ DEFINE YOUR FUNCTIONS HERE >
// </ DEFINE YOUR FUNCTIONS HERE >




// Here's where we define the list of patterns to use in our loop.
typedef void (*pattern_list[])();
// #NOTE: Add your patterns to this list to have them randomly called.
pattern_list led_patterns = {  
  rainbow_strip, 
  rainbow_wave, 
  red_green_blue_rotation,
  rainbow_with_glitter,
  confetti,
  bpm,
  juggle
};
unsigned int num_patterns = sizeof(led_patterns) / sizeof(led_patterns[0]);


// Boring setup stuff run only once on boot.
void setup() {
  Serial.begin(115200);
  EEPROM.begin(512);
  // Put the cycle count in global memory
  fetch_cycle_count_iterate_using_eeprom();
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS ).setCorrection(TypicalLEDStrip);
  #ifdef FASTLED_BRIGHTNESS
     FastLED.setBrightness(FASTLED_BRIGHTNESS);
  #endif
  // Change the seed to the cycle count
  randomSeed(cycle_count);
}


// Do the green_breathing_inner_loop until enough time has passed then pick one at random.
void loop() {
  if (millis() < MILLISECONDS_OF_GREEN_BREATHING) {
    green_breathing_inner_loop();
  } else { 
    uint16_t pattern_index = random(num_patterns);
    #ifdef DEBUG
      Serial.print("Pattern index: ");
      Serial.println(pattern_index);
    #endif
    led_patterns[pattern_index]();
  }
}


