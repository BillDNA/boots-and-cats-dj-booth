
#include <PciManager.h>

#include <SoftTimer.h>

#include <ArduinoTrace.h>


#include <arduinoFFT.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
#include <Adafruit_NeoPixel.h>

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
    //Serial.print(r); Serial.print(" vs "); Serial.print((int)round(r)); Serial.print(" vs "); Serial.println(this->_red[i] );
	}
	void push(double r, double g, double b) {
		for(int i = this->_total - 1; i > this->_zero; i--) {
			this->_red[i] = this->_red[i-1];
			this->_green[i] = this->_green[i-1];
			this->_blue[i] = this->_blue[i-1];
		}
   this->setIndex(this->_zero,r,g,b);
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
double audioBins[12];
bool hasProcessedAudioAtLeastOnce = false;
void AudioSamplingLoop(Task* me) {
	for(int i=0; i<SAMPLES; i++) {
        microseconds = micros();    //Overflows after around 70 minutes!
        vReal[i] = analogRead(A3);
        vImag[i] = 0;  
        while(micros() < (microseconds + sampling_period_us)){ } //idle to make even sample
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
void drawLoop(Task* me) {  
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
int currentAnimation = 0;
unsigned long currentAnimationStartTime;
void AnimationLoop(){//Task* me) {
  if(hasProcessedAudioAtLeastOnce) {
    
    if(currentAnimation == 0) {
      RainbowAnimation();
    }
  }
}
//-------------------------------------------------------------------------------------------- Animation Managment Loop End


//-------------------------------------------------------------------------------------------- Rainbow Chase Animation start
void RainbowAnimation() {
  int x = -1;
  if(peak / 1000 > 0.66) {
  	eyes.blue();
  } else if(peak / 1000 > 0.33) {
  	eyes.green();
  } else {
  	eyes.red();
  }
  rightBoot.push(128,0,0);
  
  	leftBoot.push(
  		((audioBins[0] + audioBins[1] + audioBins[2 ] + audioBins[3 ]) / 4.0) * 128,
  		((audioBins[4] + audioBins[5] + audioBins[6 ] + audioBins[7 ]) / 4.0) * 128,
  		((audioBins[8] + audioBins[9] + audioBins[10] + audioBins[11]) / 4.0) * 128
  		);

  	for(int x = FRONT_WIDTH-1;  x > 1; x--) {
  		for(int y = 0; y < FRONT_HEIGHT; y++) {
  			int index = frontPanel.gridToIndex(x,y);
  			int index2 = frontPanel.gridToIndex(x-1,y);
  			frontPanel.setIndex(index,
  				frontPanel.getRed(index2),
  				frontPanel.getGreen(index2),
  				frontPanel.getBlue(index2));
  		}
  	}
	int index = frontPanel.gridToIndex(0,0);
	frontPanel.setIndex(index,0,0,((audioBins[10] + audioBins[11])/2) * 128);
	index = frontPanel.gridToIndex(0,1);
	frontPanel.setIndex(index,0,0,((audioBins[8] + audioBins[9])/2) * 128);

	index = frontPanel.gridToIndex(0,2);
	frontPanel.setIndex(index,0,((audioBins[6] + audioBins[7])/2) * 128,0);
	index = frontPanel.gridToIndex(0,3);
	frontPanel.setIndex(index,0,((audioBins[4] + audioBins[5])/2) * 128,0);

	index = frontPanel.gridToIndex(0,4);
	frontPanel.setIndex(index,((audioBins[2] + audioBins[3])/2) * 128,0,0);
	index = frontPanel.gridToIndex(0,5);
	frontPanel.setIndex(index,((audioBins[0] + audioBins[1])/2) * 128,0,0);
  
}

//-------------------------------------------------------------------------------------------- Setup Start
//Set up callbacks with soft timer
Task audioThread(100, AudioSamplingLoop);
Task drawingThread(0, drawLoop);
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

	//Animation Selection
	//currentAnimationStartTime = millis();
	currentAnimation = 0;

	//Soft timers 'treads'
	SoftTimer.add(&audioThread);
	SoftTimer.add(&drawingThread);
	//SoftTimer.add(&AnimationThread);


}
//-------------------------------------------------------------------------------------------- Setup end
//-------------------------------------------------------------------------------------------- Debug
void debug_rgb(double r, double g, double b) {
  Serial.print(r); Serial.print(" - "); Serial.print(g); Serial.print(" - "); Serial.println(b);
}
void debug_xyi(int xx, int yy, int ii) {
  Serial.print(xx); Serial.print(","); Serial.print(yy); Serial.print(" - "); Serial.println(ii);
}
