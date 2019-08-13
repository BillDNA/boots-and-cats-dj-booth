/*
  CatEyes.cpp - Library for flashing the cats eyes.
*/

#include "Arduino.h"
#include "Arduino.h"

CatEyes::CatEyes() {
	_r = false;
	_g = false;
	_b = false;
}
void CatEyes::_update() {
	digitalWrite(RED_PIN, !_r);
	digitalWrite(GREEN_PIN, !_g);
	digitalWrite(BLUE_PIN, !_b);
}
void CatEyes::red() {
	_r = true;
	_g = false;
	_b = false;
}
void CatEyes::green() {
	_r = false;
	_g = true;
	_b = false;
}
void CatEyes::blue() {
	_r = false;
	_g = false;
	_b = true;
}
void CatEyes::cyan() {
	_r = false;
	_g = true;
	_b = true;
}
void CatEyes::yellow() {
	_r = true;
	_g = true;
	_b = false;
}
void CatEyes::purple() {
	_r = true;
	_g = false;
	_b = true;
}
void CatEyes::white() {
	_r = true;
	_g = true;
	_b = true;
}
void CatEyes::black() {
	_r = false;
	_g = false;
	_b = false;
}