// NEOPIXEL BEST PRACTICES for most reliable operation:
// - Add 1000 uF CAPACITOR between NeoPixel strip's + and - connections.
// - MINIMIZE WIRING LENGTH between microcontroller board and first pixel.
// - NeoPixel strip's DATA-IN should pass through a 300-500 OHM RESISTOR.
// - AVOID connecting NeoPixels on a LIVE CIRCUIT. If you must, ALWAYS
//   connect GROUND (-) first, then +, then data.
// - When using a 3.3V microcontroller with a 5V-powered NeoPixel strip,
//   a LOGIC-LEVEL CONVERTER on the data line is STRONGLY RECOMMENDED.
// (Skipping these may work OK on your workbench but can fail in the field)
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
#include "arduinoFFT.h"

//----------------------------------------------------------------------------------- define pins and const
//analog outputs
#define LED_FRONT 6
#define LED_RIGHT 4
#define LED_LEFT 5
#define EYE_RED 11
#define EYE_GREEN 10
#define EYE_BLUE 9
//number of leds per strip
#define RIGHT_ZERO 0
#define COUNT_RIGHT 104
#define RIGHT_END 104

#define LEFT_ZERO 0;
#define COUNT_LEFT 108
#define LEFT_END 108;

#define COUNT_FRONT 258
#define FRONT_HEIGHT 12
#define FRONT_WIDTH 43
#define FRONT_WIDTH_ODD 22
#define FRONT_WIDTH_EVEN 21
//FFT
#define SAMPLES 128             //Must be a power of 2
#define SAMPLING_FREQUENCY 1000 //Hz, must be less than 10000 due to ADC

arduinoFFT FFT = arduinoFFT();
//delclare strip
Adafruit_NeoPixel strip_front(COUNT_FRONT, LED_FRONT, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip_right(COUNT_RIGHT, LED_RIGHT, NEO_GRB + NEO_KHZ800);
#define rightOffset 4;
Adafruit_NeoPixel strip_left(COUNT_LEFT, LED_LEFT , NEO_GRB + NEO_KHZ800); //The 4 is cause there are some extra as the strip loops around boot and so needs to be off set during draw
//----------------------------------------------------------------------------------- variables
int index_front, index_right, index_left, selected_animation, animation_time;
uint8_t red_front[COUNT_FRONT], red_right[COUNT_LEFT], red_left[COUNT_RIGHT];
uint8_t green_front[COUNT_FRONT], green_left[COUNT_LEFT], green_right[COUNT_RIGHT];
uint8_t blue_front[COUNT_FRONT], blue_left[COUNT_LEFT], blue_right[COUNT_RIGHT];
int animation_front, animation_left[COUNT_LEFT], animation_right[COUNT_RIGHT];
uint8_t eye_red, eye_green, eye_blue;
int frame; // 0-center, 1 - right, 2 - left;
//FFT
unsigned int sampling_period_us;
unsigned long microseconds;
double vReal[SAMPLES];
double vImag[SAMPLES];
//----------------------------------------------------------------------------------- temp variables
int i, x, y, temp, aniimation_frame;
uint8_t r, g, b;
uint32_t color;
//----------------------------------------------------------------------------------- setup
void setup() {
  Serial.begin(9600);
  Serial.print("Hello Boots and Cats.");
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.
  //start the strips
  strip_front.begin();
  strip_front.show();
  strip_right.begin();
  strip_right.show();
  strip_left.begin();
  strip_left.show();
  //set up color arrays
  setup_0();
  //set up eye output
  pinMode(EYE_RED, OUTPUT);
  pinMode(EYE_GREEN, OUTPUT);
  pinMode(EYE_BLUE, OUTPUT);

  frame = 0;
  aniimation_frame = 0;
  selected_animation = 0;
  //FFT
  //Serial.begin(115200);
  //sampling_period_us = round(1000000*(1.0/SAMPLING_FREQUENCY));
}
void loop() {
  //Serial.println("update");
  frame = (frame + 1 ) % 3;
  //sample_sound();
  // set Eyes Color
  eye_red = 128;
  analogWrite(EYE_RED, eye_red * 1.0);
  analogWrite(EYE_GREEN, eye_green * 0);
  analogWrite(EYE_BLUE, eye_blue * 0);

  //set led strips
  for (i = 0; i < max(COUNT_LEFT, max(COUNT_RIGHT, COUNT_FRONT)); i++) {
    if ( i < COUNT_FRONT) {
      color = strip_front.Color(red_front[i] , green_front[i] , blue_front[i]  );
      strip_front.setPixelColor(i, color);
    }
    if ( i < COUNT_RIGHT) {
      color = strip_right.Color(red_right[i], green_right[i] , blue_right[i]);
      strip_right.setPixelColor(i, color);//The 4 is cause there are some extra as the strip loops around boot and so needs to be off set during draw
    }
    if ( i < COUNT_LEFT) {
      color = strip_left.Color(red_left[i], green_left[i], blue_left[i]);
      strip_left.setPixelColor(i, color);
    }
  }

  strip_front.show();
  strip_left.show();
  strip_right.show();

  //------------------- now animate
  if (selected_animation == 0 ) {
    animation_0();
  } else if (selected_animation == 1) {
    animation_1();
  }
  animation_time--;
  if (animation_time == 0) {
    selected_animation = random(0, 2);

    if (selected_animation == 0 ) {
      setup_0();
    } else if (selected_animation == 1) {
      setup_1();
    }
  }

  //Serial.println("done");
  //TODO - research how to grab time  so can compare and have animations be time sensitive
}
//-------------------------------------------------------------------------------------------- setup_0
void setup_0() {
  temp = 0;
  aniimation_frame = 0;
  animation_time = COUNT_FRONT * 2;
  while (r + g + b == 0) {
    r = (int)random(0, 2);
    g = (int)random(0, 2);
    b = (int)random(0, 2);
  }

  for (i = 0; i < COUNT_FRONT; i++) {
    if (r == 1) {
      red_front[i] = (i * 8) % 128;
    }
    if (g == 1) {
      green_front[i] = (i * 8) % 128;
    }
    if (b == 1) {
      blue_front[i] = (i * 8) % 128;
    }
    if (i % 8 == 0) {
      temp += 1;
      r = 0; g = 0; b = 0;

      if ( temp % 2 != 0) {
        while (r + g + b == 0) {
          r = (int)random(0, 2);
          g = (int)random(0, 2);
          b = (int)random(0, 2);
        }
      }
    }
    if ( temp % 2 == 0) {
      red_front[i] = 0;
      green_front[i] = 0;
      blue_front[i] = 0;
    }
  }
  for (i = 0; i < COUNT_RIGHT; i++) {
    if (r == 1) {
      red_right[i] = (i * 8) % 128;
    }
    if (g == 1) {
      green_right[i] = (i * 8) % 128;
    }
    if (b == 1) {
      blue_right[i] = (i * 8) % 128;
    }
    if (i % 8 == 0) {
      temp += 1;
      r = 0; g = 0; b = 0;

      if ( temp % 2 != 0) {
        while (r + g + b == 0) {
          r = (int)random(0, 2);
          g = (int)random(0, 2);
          b = (int)random(0, 2);
        }
      }
    }
    if ( temp % 2 == 0) {
      red_front[i] = 0;
      green_front[i] = 0;
      blue_front[i] = 0;
    }
  }
  for (i = 0; i < COUNT_LEFT; i++) {
    if (r == 1) {
      red_left[i] = (i * 8) % 128;
    }
    if (g == 1) {
      green_left[i] = (i * 8) % 128;
    }
    if (b == 1) {
      blue_left[i] = (i * 8) % 128;
    }
    if (i % 8 == 0) {
      temp += 1;
      r = 0; g = 0; b = 0;
      if ( temp % 2 != 0) {
        while (r + g + b == 0) {
          r = (int)random(0, 2);
          g = (int)random(0, 2);
          b = (int)random(0, 2);
        }
      }
    }
    if ( temp % 2 == 0) {
      red_front[i] = 0;
      green_front[i] = 0;
      blue_front[i] = 0;
    }
  }
}

//-------------------------------------------------------------------------------------------- setup_1
void setup_1() {
  animation_time = 22 * 24;
  aniimation_frame = 0;
}
//-------------------------------------------------------------------------------------------- Animation 1
void animation_1() {
  temp += 1;
  // set eye color
  if (temp > 22) {
    r = 0; g = 0; b = 0;
    Serial.println("jump");
    while (r + g + b == 0 && r + g + b != 3) {
      r = (int)random(0, 2);
      g = (int)random(0, 2);
      b = (int)random(0, 2);
    }
    temp = 0;
  }
  aniimation_frame = (aniimation_frame + 1) % 22;
  for (y = 0; y < FRONT_HEIGHT; y++) {
    for (x = 0; x < 22; x++) {
      int t = 132 - ((x + aniimation_frame) % 22) * 6;
      i = front_grid_to_index(x, y);
      set_front_color(i, t * r, t * g, t * b);
    }
    for (x = FRONT_WIDTH - 1; x >= 22; x--) {
      int t = ((x + (22 - aniimation_frame)) % 22) * 6;
      i = front_grid_to_index(x, y);
      set_front_color(i, t * r, t * g, t * b);
    }
  }
  int t = 128;// (float)aniimation_frame / 22.0 * 128;
  for (i = 0; i < max(COUNT_LEFT, max(COUNT_RIGHT, COUNT_FRONT)) - 1; i++) {
    if ( i < COUNT_RIGHT) {
      if ( i > ((float)(22 - aniimation_frame) / 22.0) * COUNT_RIGHT) {

        set_right_color(i, t * r, t * g, t * b);
      } else {
        set_right_color(i, 0, 0, 0);
      }
    }

    if ( i < COUNT_LEFT) {
      if ( i > ((float)(22 - aniimation_frame) / 22.0) * COUNT_LEFT) {

        set_left_color(i, t * r, t * g, t * b);
      } else {
        set_left_color(i, 0, 0, 0);
      }
    }
  }


  delay(15);
}
//-------------------------------------------------------------------------------------------- Animation 0
void animation_0() {
  temp += 1;
  // set eye color
  if (temp > 8) {
    r = 0; g = 0; b = 0;
    Serial.println("jump");
    while (r + g + b == 0) {
      r = (int)random(0, 2);
      g = (int)random(0, 2);
      b = (int)random(0, 2);
    }
    aniimation_frame++;
    temp = 0;
  }

  int t = (int)(((float)((float)temp / 8.0)) * 128.0);
  red_front[COUNT_FRONT - 1] = r * t;
  green_front[COUNT_FRONT - 1] = g * t;
  blue_front[COUNT_FRONT - 1] = b *  t;
  debug_front(COUNT_FRONT - 1);

  red_left[COUNT_LEFT - 1] = r * t;
  green_left[COUNT_LEFT - 1] = g * t;
  blue_left[COUNT_LEFT - 1] = b * t;

  red_right[COUNT_RIGHT - 1] = r * t;
  green_right[COUNT_RIGHT - 1] = g * t;
  blue_right[COUNT_RIGHT - 1] = b * t;

  // set strip colors

  for (i = 0; i < max(COUNT_LEFT, max(COUNT_RIGHT, COUNT_FRONT)) - 1; i++) {
    //for (i = max(COUNT_LEFT, max(COUNT_RIGHT, COUNT_FRONT)) - 2; i >= 0; i--) {
    if ( i < COUNT_FRONT) {
      set_front_color(i, red_front[i + 1], green_front[i + 1], blue_front[i + 1]);
    }
    if ( i < COUNT_RIGHT) {
      set_right_color(i, red_right[i + 1], green_right[i + 1], blue_right[i + 1]);
    }
    if ( i < COUNT_LEFT) {
      set_left_color(i, red_left[i + 1], green_left[i + 1], blue_left[i + 1]);
    }
  }
  delay(5);
}
//-------------------------------------------------------------------------------------------- Sound FFT
void sample_sound() {
  /*SAMPLING*/
  for (int i = 0; i < SAMPLES; i++)
  {
    microseconds = micros();    //Overflows after around 70 minutes!

    vReal[i] = analogRead(0);
    vImag[i] = 0;

    while (micros() < (microseconds + sampling_period_us)) {
    }
  }

  /*FFT*/
  FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
  FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);
  double peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);

  /*PRINT RESULTS*/
  //Serial.println(peak);     //Print out what frequency is the most dominant.

  for (int i = 0; i < (SAMPLES / 2); i++)
  {
    /*View all these three lines in serial terminal to see which frequencies has which amplitudes*/

    //Serial.print((i * 1.0 * SAMPLING_FREQUENCY) / SAMPLES, 1);
    //Serial.print(" ");
    Serial.println(vReal[i], 1);    //View only this line in serial plotter to visualize the bins
  }
}


//-------------------------------------------------------------------------------------------- Helpers
int wrap_color(int c, int add) {
  c = ((c + add) + 256) % 256;
  return c;
}
int front_grid_to_index(int x, int y) {
  int m[FRONT_WIDTH] = {
    0, 42, 1, 41, 2, 40, 3, 39, 4, 38, 5, 37, 6, 36, 7, 35, 8, 34, 9, 33, 10, 32, 11, 31, 12, 30, 13, 29, 14, 28, 15, 27, 16, 26, 17, 25, 18, 24, 19, 23, 20, 22, 21
  };

  return m[x] + 43 * y;

}
void set_front_color(int i, uint8_t red, uint8_t green, uint8_t blue) {
  if (i >= 0 && i < COUNT_FRONT) {
    red_front[i] = red;
    green_front[i] = green;
    blue_front[i] = blue;
  }
}
void set_left_color(int i, uint8_t red, uint8_t green, uint8_t blue) {
  if (i >= 0 && i < COUNT_LEFT) {
    red_left[i] = red;
    green_left[i] = green;
    blue_left[i] = blue;
  }
}
void set_right_color(int i, uint8_t red, uint8_t green, uint8_t blue) {
  if (i >= 0 && i < COUNT_RIGHT) {
    red_right[i] = red;
    green_right[i] = green;
    blue_right[i] = blue;
  }
}
//-------------------------------------------------------------------------------------------- Debug
void debug_rgb() {
  Serial.print(r); Serial.print(" - "); Serial.print(g); Serial.print(" - "); Serial.println(b);
}
void debug_xyi(int xx, int yy, int ii) {
  Serial.print(xx); Serial.print(","); Serial.print(yy); Serial.print(" - "); Serial.println(ii);
}
void debug_front(int index) {
  Serial.print(red_front[index]); Serial.print(" - "); Serial.print(green_front[index]); Serial.print(" - "); Serial.println(blue_front[index]);
}
