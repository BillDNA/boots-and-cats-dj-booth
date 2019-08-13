/*
  CatEyes.h - Library for flashing the cats eyes.
*/

#ifndef CatEyes_h
#define CatEyes_h

#include "Arduino.h"

#define RED_PIN 11
#define GREEN_PIN 10
#define BLUE_PIN 9

class CatEyes {
  public:
    CateEyes();
    void red();
    void green();
    void blue();
    void cyan();
    void yellow();
    void purple();
    void black();
    void white();
  private:
    bool _g;
    bool _b;
    bool _r;
    void _update();
};


#endif
