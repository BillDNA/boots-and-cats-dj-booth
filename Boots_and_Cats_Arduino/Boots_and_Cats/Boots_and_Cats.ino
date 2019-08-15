bool UseAudio = false;

#include <arduinoFFT.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
#include <Adafruit_NeoPixel.h>
//
//----------------------------------------------------------------------------------- Globals
int r,g,b; //CURRENT RGB
int lr,lg,lb; //last used rgbs
int llr,llg,llb; //last last used rgbs
int dir = 1;
int currentAnimation = 1;
unsigned long currentAnimationTimeLimit = 10000;
unsigned long currentAnimationStartTime;
unsigned long animationFrame = 0;
//----------------------------------------------------------------------------------- Classes
//----------------------------------------------------------------------------------- Cat Eyes Start
#define EYE_RED 11
#define EYE_GREEN 10
#define EYE_BLUE 9
class CatEyes {
  private: 
	bool _r;
	bool _g;
	bool _b;

	void _update() {
	  digitalWrite(11, !this->_r);
	  digitalWrite(10, !this->_g);
	  digitalWrite(9, !this->_b);
	}
  public:
	CatEyes() {
	  this->_r = false;
	  this->_g = false;
	  this->_b = false;
	}
	void red() {
	  this->_r = true;
	  this->_g = false;
	  this->_b = false;
	  this->_update();
	}
	void green() {
	  this->_r = false;
	  this->_g = true;
	  this->_b = false;
	  this->_update();
	}
	void blue() {
	  this->_r = false;
	  this->_g = false;
	  this->_b = true;
	  this->_update();
	}
	void cyan() {
	  this->_r = false;
	  this->_g = true;
	  this->_b = true;
	  this->_update();
	}
	void yellow() {
	  this->_r = true;
	  this->_g = true;
	  this->_b = false;
	  this->_update();
	}
	void purple() {
	  this->_r = true;
	  this->_g = false;
	  this->_b = true;
	  this->_update();
	}
	void white() {
	  this->_r = true;
	  this->_g = true;
	  this->_b = true;
	  this->_update();
	}
	void black() {
	  this->_r = false;
	  this->_g = false;
	  this->_b = false;
	  this->_update();
	}
	void setEyeColor(int red, int green, int blue) {
	  this->_r = red!=0;
	  this->_g = green!=0;
	  this->_b = blue!=0;
	  this->_update();
	}
};
//----------------------------------------------------------------------------------- Cat Eyes End
//----------------------------------------------------------------------------------- LED Start
class LED {
	private:
		int _pin;
		int _zero;
		int _total;
		uint8_t* _red; 
		uint8_t* _green; 
		uint8_t* _blue;
		Adafruit_NeoPixel* _strip;
	public:
		LED(int pin, int zero, int total) {
			this->_pin = pin;
			this->_zero = zero;
			this->_total = total;
			this->_red = new uint8_t[total] {0};
			this->_green = new uint8_t[total] {0};
			this->_blue = new uint8_t[total] {0};
		}
	int total() {
	  return this->_total;
	}
	int zero() {
	  return this->_zero;
	}
	uint8_t getRed(int i) {
		return this->_red[i];
	}
	uint8_t getGreen(int i) {
		return this->_green[i];
	}
	uint8_t getBlue(int i) {
		return this->_blue[i];
	}

	void setIndex(int i, double r, double g, double b) {
		this->_red[i] = (int)round(r);
		this->_green[i] = (int)round(g);
		this->_blue[i] = (int)round(b);
		if(i < 0 || i > this->_total) {
			Serial.println("BROKEN INDEX");
		}
	}
	void push(double r, double g, double b) {
		for(int i = this->_total - 1; i > this->_zero; i--) {
			this->_red[i] = this->_red[i-1];
			this->_green[i] = this->_green[i-1];
			this->_blue[i] = this->_blue[i-1];
		}
		this->setIndex(this->_zero,r,g,b);
	}
	void pull(double r, double g, double b) {
		for(int i = 0; i < this->_total-1; i++) {
			this->_red[i] = this->_red[i+1];
			this->_green[i] = this->_green[i+1];
			this->_blue[i] = this->_blue[i+1];
		}
		this->setIndex(this->_total-1,r,g,b);
	}
	void flash(double r, double g, double b) {
		for(int i = 0; i < this->_total; i++) {
			this->setIndex(i,r,g,b);
		}
	}
	int gridToIndex(int x, int y) {
		int m[43] = {
			0, 42, 1, 41, 2, 40, 3, 39, 4, 38, 5, 37, 6, 36, 7, 35, 8, 34, 9, 33, 10, 32, 11, 31, 12, 30, 13, 29, 14, 28, 15, 27, 16, 26, 17, 25, 18, 24, 19, 23, 20, 22, 21
			};
		return m[x] + 43 * y;
	}
};
//----------------------------------------------------------------------------------- LED end



#define LED_FRONT 6
#define LED_RIGHT 4
#define LED_LEFT 5
#define RIGHT_ZERO 0
#define COUNT_RIGHT 104
#define RIGHT_END 104

#define LEFT_ZERO 0;
#define COUNT_LEFT 108
#define LEFT_END 108;

#define COUNT_FRONT 258
#define FRONT_HEIGHT 6
#define FRONT_WIDTH 43
#define FRONT_MIDDLE 22
#define FRONT_WIDTH_ODD 22
#define FRONT_WIDTH_EVEN 21
//Audio setup
#define SAMPLES 256             //Must be a power of 2
#define SAMPLING_FREQUENCY 4400 //Hz, must be less than 10000 due to ADC
arduinoFFT FFT = arduinoFFT(); 
unsigned int sampling_period_us = round(1000000*(1.0/SAMPLING_FREQUENCY));
unsigned long microseconds;
double vReal[SAMPLES];
double vImag[SAMPLES];
double peak;
int incomingAudio;

//delclare strip
Adafruit_NeoPixel lbs(108,5,NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel rbs(108,4,NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel fps(258, 6, NEO_GRB + NEO_KHZ800);

LED leftBoot = LED(5, 0, 108);
LED rightBoot = LED(4, 0, 108);
LED frontPanel = LED(6, 0, 258);

CatEyes eyes = CatEyes();

int debug;
//-------------------------------------------------------------------------------------------- Aduio sampling start
double audioBins[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
bool hasProcessedAudioAtLeastOnce = false;
void AudioSamplingLoop(){//Task* me) {
  unsigned long StartTime = millis();
  unsigned long CurrentTime = millis();
  unsigned long ElapsedTime = CurrentTime - StartTime;
  
	for(int i=0; i<SAMPLES; i++) {
		microseconds = micros();    //Overflows after around 70 minutes!
		vReal[i] = analogRead(A3);
		vImag[i] = 0;  
		while(micros() < (microseconds + sampling_period_us)){
		  /*
		  CurrentTime = millis();
		  ElapsedTime = CurrentTime - StartTime;
		  if(ElapsedTime > 100) {
			StartTime = millis();
			drawLoop();
		  }*/
		} //idle to make even sample
	}

	FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
	FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
	FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);
	peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);

	int bin = 0;
	double maxBin = 0;
	double tempAudioBins[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
	for(int i = 1; i < (SAMPLES/2); i++) {
		if(i / (SAMPLES/2.0)  >= bin / 11.0) { 
			bin++; 
		}
		tempAudioBins[bin] += vReal[i]; //transphers bins to somthing we can refger back to 
		maxBin = max(maxBin, tempAudioBins[bin]);
	}
	//now normalize the bins
	for(bin = 0; bin < 12; bin++) {
		audioBins[bin] = tempAudioBins[bin] / maxBin;
	 //Serial.println(audioBins[bin]);
	}
  	hasProcessedAudioAtLeastOnce = true;
}
//-------------------------------------------------------------------------------------------- Audio Sample Loop end
//-------------------------------------------------------------------------------------------- Draw Loop start
void drawLoop(){//Task* me) {  
	for(int i = 0; i < max(leftBoot.total(),max(rightBoot.total(), frontPanel.total())); i++ ) {
		uint32_t c;
		//left boot
		if(i < leftBoot.total()) {
		  if(i < leftBoot.zero()) {
			c = lbs.Color(0,0,0); //force the extra leds to black
		  } else {
			c = lbs.Color(
			  leftBoot.getRed(i),
			  leftBoot.getGreen(i),
			  leftBoot.getBlue(i)); //grab the correct color
		  }
		  lbs.setPixelColor(i,c); //set the color if we are in the bounds
		}
		//right boot
		if(i < rightBoot.total()) {
		  if(i < rightBoot.zero()) {
			c = rbs.Color(0,0,0); //force the extra leds to black
		  } else {
			c = rbs.Color(
			  rightBoot.getRed(i),
			  rightBoot.getGreen(i),
			  rightBoot.getBlue(i)); //grab the correct color
		  }
		  rbs.setPixelColor(i,c); //set the color if we are in the bounds
		}
		//front pannel
		if(i < frontPanel.total()) {
		  if(i < frontPanel.zero()) {
			c = fps.Color(0,0,0); //force the extra leds to black
		  } else {
			c = fps.Color(
			  frontPanel.getRed(i),
			  frontPanel.getGreen(i),
			  frontPanel.getBlue(i)); //grab the correct color
		  }
		  fps.setPixelColor(i,c); //set the color if we are in the bounds
		}
	}
	//now show the strips
	lbs.show();
	rbs.show();
	fps.show();
 
 AnimationLoop();
}
//-------------------------------------------------------------------------------------------- Draw Loop end
//-------------------------------------------------------------------------------------------- Animation Managment Loop Start
#define TOTAL_ANIMATIONS 5
unsigned long animationSettings[][4] = {
	//id number, run time short, runtime long, chance to happen
	{0, 10000, 15000, 33}, //Rainbow 
	{1, 60000, 120000, 60}, //pulse
	{2, 30000, 45000, 40}, //pong 
  {3, 30000, 45000, 40}, //Boots and cats 
  {4, 60000, 120000, 40}, //Cheveron
};
int forcedAnimation = -1;
void AnimationLoop(){//Task* me) {
	if(currentAnimation == 0) {
		RainbowAnimation();
	} else if(currentAnimation == 1) {
		PulseAnimation();
	} else if(currentAnimation == 2) {
		PongAnimation();
	} else if(currentAnimation == 3) {
		BootsAndCatsAnimation();
	} else if(currentAnimation == 4) {
		CheveronAnimation();
	}

	if(forcedAnimation == -1) {
		unsigned long CurrentTime = millis();
		unsigned long ElapsedTime = CurrentTime - currentAnimationStartTime;
		if(ElapsedTime > currentAnimationTimeLimit) {
			shuffleAnimations();
			int selected = 0;
			unsigned long animationSetting[4];
	    	memcpy(animationSetting,animationSettings[selected],sizeof(animationSettings[selected]));
			/*while(selected < TOTAL_ANIMATIONS && animationSetting[3] < (int)random(0, 100)) {
				selected++;
				memcpy(animationSetting,animationSettings[selected],sizeof(animationSettings[selected]));
			}*/
			currentAnimation = animationSetting[0];
			currentAnimationTimeLimit = 10000;//random(animationSetting[1], animationSetting[2]);
			currentAnimationStartTime = millis();
			animationFrame = 0;
		}
	} else {currentAnimation = forcedAnimation;}
}
//-------------------------------------------------------------------------------------------- Animation Managment Loop End
void CheveronAnimation() {
	int spacing = 7;
	if(animationFrame == 0) {
		frontPanel.flash(0,0,0);
		leftBoot.flash(0,0,0);
		rightBoot.flash(0,0,0);
		eyes.white();
	}
	r = 0; g = 0; b = 0;
	int frame = animationFrame % 21;
	if(frame == 0) {
    dir = dir * -1;
		r = 128; g = 0; b = 0;
	} else if(frame == 1) {
		r = 128; g = 64; b = 0;
	} else if(frame == 2) {
		r = 128; g = 128; b = 0;
	} else if(frame == 3) {
		r = 0; g = 128; b = 0;
	} else if(frame == 4) {
		r = 0; g = 0; b = 128;
	} else if(frame == 5) {
		r = 37; g = 0; b = 65;
	} else if(frame == 6) {
		r = 74; g = 0; b = 105;
	}
  if(animationFrame % 100 < 50 || animationFrame < 108) {
  	leftBoot.pull( r,g,b);
  	rightBoot.pull( r,g,b);
  } else {
    leftBoot.push( r,g,b);
    rightBoot.push( r,g,b);
  }
	frontPanel.pull( r,g,b);
	animationFrame++;
}
//-------------------------------------------------------------------------------------------- Boot And Cats Animation start
void BootsAndCatsAnimation() {
	int totalFramesInAnimation = 16;
	int animationStep = animationFrame % totalFramesInAnimation;
	if(animationStep == 0) {
		r = 0; g = 0; b = 0;
		while (r + g + b == 0 || r + g + b == 3) {
		  r = (int)random(0, 2);
		  g = (int)random(0, 2);
		  b = (int)random(0, 2);
		}
    dir = (dir + 1) % 3;
		eyes.setEyeColor(r,g,b);
	}
	int boots[6][43] = {
		{1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1},
		{0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0},
		{0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
		{0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0}
	};
	int n[6][43] = {
		{1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
		{0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
	};
	int cats[6][43] = {
		{1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1},
		{0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0}
	};
	double offR = abs(1-r) * (double)(animationStep / (double)totalFramesInAnimation) * 128;
	double offG = abs(1-g) * (double)(animationStep / (double)totalFramesInAnimation) * 128;
	double offB = abs(1-b) * (double)(animationStep / (double)totalFramesInAnimation) * 128;

	for(int y = 0; y < FRONT_HEIGHT; y++) {
		for(int x = 0; x < FRONT_WIDTH; x++) {
			int index = frontPanel.gridToIndex(x,y);
      		int temp = 0;
			if(dir == 0) {
        		temp = boots[y][x];       
			} else if(dir == 1) {
				temp = n[y][x];
			} else if(dir == 2) {
				temp = cats[y][x];
			}
     		if(temp == 1) {
      			frontPanel.setIndex(index,r*128,g*128,b*128);
     		} else {
     			frontPanel.setIndex(index,offR/8,offG/8,offB/8);
     		}
		}
	}

	leftBoot.flash(offR,offG,offB);
	rightBoot.flash(offR, offG, offB);
	animationFrame++;
}
//-------------------------------------------------------------------------------------------- Boot And Cats  Animation End
//-------------------------------------------------------------------------------------------- Rainbow Chase Animation start
void RainbowAnimation() {
	if(animationFrame % 8 == 0) {
		r = 0; g = 0; b =0;
		while (r + g + b == 0) {
		  r = (int)random(0, 2);
		  g = (int)random(0, 2);
		  b = (int)random(0, 2);
		}
		eyes.setEyeColor(r,g,b);
	}
	leftBoot.pull(r*128, g*128, b*128);
	rightBoot.pull(r*128, g*128, b*128);
	frontPanel.push(r*128, g*128, b*128);
	animationFrame++;
	
}
//-------------------------------------------------------------------------------------------- Rainbow Chase Animation End
//-------------------------------------------------------------------------------------------- Pulse Animation start
void PulseAnimation() {
	int totalFramesInAnimation = 22;
	int animationStep = animationFrame % totalFramesInAnimation;
	if(animationStep == 0) {
		lr = r; lg = g; lb = b;
		r = 0; g = 0; b =0;
		while (r + g + b == 0) {
		  r = (int)random(0, 2);
		  g = (int)random(0, 2);
		  b = (int)random(0, 2);
		}
		eyes.setEyeColor(r,g,b);
	}
	for(int y = 0; y < FRONT_HEIGHT; y++) { 
		int index = frontPanel.gridToIndex(FRONT_MIDDLE-1,y);

		frontPanel.setIndex(index, r*128,g*128,b*128);
	}
	for(int y = 0; y < FRONT_HEIGHT; y++) {
		for(int x = FRONT_MIDDLE;  x < FRONT_MIDDLE + animationStep; x++) {
			int index = frontPanel.gridToIndex(x,y);
			double t = 132.0 - (x + animationStep) * 6; 
			frontPanel.setIndex(index, t*r,t*g,t*b);
			index = frontPanel.gridToIndex(FRONT_WIDTH-1-x,y);
			frontPanel.setIndex(index, t*r,t*g,t*b);
		}
	}
	double t = animationStep / 22.0;
	leftBoot.flash(
		(r * t + lr * (1-t)) *128, 
		(g * t + lg * (1-t)) *128, 
		(b * t + lb * (1-t)) *128
		);
	rightBoot.flash(
		(r * t + lr * (1-t)) *128, 
		(g * t + lg * (1-t)) *128, 
		(b * t + lb * (1-t)) *128
		);
  animationFrame++;
	
}
//-------------------------------------------------------------------------------------------- Pulse Animation End
//-------------------------------------------------------------------------------------------- Pong Animation start
void PongAnimation() {
	int totalFramesInAnimation = FRONT_WIDTH-1;
	int animationStep = animationFrame % totalFramesInAnimation;
	if(animationStep == 0) {
    llr = lr; llg = lg; llb = lb;
		dir = -1 * dir;
		lr = r; lg = g; lb = b;
		r = 0; g = 0; b =0;
		while (
			(r + g + b == 0) ||
     (r == lr && g == lg && b == lb)||
      (r == llr && g == llg && b == llb)
			) {
		  r = (int)random(0, 2);
		  g = (int)random(0, 2);
		  b = (int)random(0, 2);
		}
		eyes.setEyeColor(r,g,b);
	}
	double transition = animationStep / (double)totalFramesInAnimation;
	double dropoff[8] = {0,32,56,74,87,98,105,111};
	for(int y = 0; y < FRONT_HEIGHT; y++) {
		for(int x = 0;  x < FRONT_WIDTH; x++) {
			int distance = abs(x - animationStep);
			if(dir == -1) { distance = abs(FRONT_WIDTH-x-animationStep);}
			double t = 128-dropoff[distance]; 			
			if(distance > 7) {t = 0;}
			int index = frontPanel.gridToIndex(x,y);
			frontPanel.setIndex(index, 
				t * (lr * transition + r * (1-transition)),
				t * (lg * transition + g * (1-transition)),
				t * (lb * transition + b * (1-transition))
				);
		}
	}
	double t = animationStep / totalFramesInAnimation;
	if(dir < 0) {
		leftBoot.flash(r*128,g*128,b*128);
		rightBoot.flash(lr*128,lg*128,lb*128);
	} else {
		rightBoot.flash(r*128,g*128,b*128);
		leftBoot.flash(lr*128,lg*128,lb*128);

	}
	animationFrame+=6;
	
}
//-------------------------------------------------------------------------------------------- Pong Animation End
//-------------------------------------------------------------------------------------------- Debug Animation start
void DegubAnimation() {
  
}

//-------------------------------------------------------------------------------------------- Setup Start
//Set up callbacks with soft timer
//Task audioThread(100, AudioSamplingLoop);
//Task drawingThread(0, drawLoop);
//Task AnimationThread(10, AnimationLoop);

void setup() {
	// put your setup code here, to run once:
	Serial.begin(115200);
	//eye pin modes
	pinMode(11, OUTPUT);
	pinMode(10, OUTPUT);
	pinMode(9, OUTPUT);
	//now shart the strip
	lbs.begin();
	rbs.begin();
	fps.begin();
	lbs.show();
	rbs.show();
	fps.show();

	//AUDIO Setup
	sampling_period_us = round(1000000*(1.0/SAMPLING_FREQUENCY));
	pinMode(A3, INPUT);


	currentAnimationStartTime = millis();
	//Animation Selection
	//currentAnimationStartTime = millis();
	//currentAnimation = 0;

	//Soft timers 'treads'
	//SoftTimer.add(&audioThread);
	//SoftTimer.add(&drawingThread);
	//SoftTimer.add(&AnimationThread);
}
//-------------------------------------------------------------------------------------------- Setup end
void loop() {
  if(UseAudio) {AudioSamplingLoop();}
  drawLoop();
}
//-------------------------------------------------------------------------------------------- Helpers
void shuffleAnimations() {
	unsigned long animationSetting[4];
	for(int i = 0; i < TOTAL_ANIMATIONS; i++) {
		int j = (int) random(0, TOTAL_ANIMATIONS);
		memcpy(animationSetting,animationSettings[i],sizeof(animationSettings[i]));
		memcpy(animationSettings[i],animationSettings[j],sizeof(animationSettings[i]));
		memcpy(animationSettings[j],animationSetting,sizeof(animationSettings[i]));
	}
}
//-------------------------------------------------------------------------------------------- Debug
void debug_rgb(double r, double g, double b) {
  Serial.print(r); Serial.print(" - "); Serial.print(g); Serial.print(" - "); Serial.println(b);
}
void debug_xyi(int xx, int yy, int ii) {
  Serial.print(xx); Serial.print(","); Serial.print(yy); Serial.print(" - "); Serial.println(ii);
}
