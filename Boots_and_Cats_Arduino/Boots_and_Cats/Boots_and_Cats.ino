
#include <PciManager.h>

#include <SoftTimer.h>



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

	void setIndex(int i, int r, int g, int b) {
		this->_red[i] = r;
		this->_green[i] = g;
		this->_blue[i] = b;
	}
	void push(int r, int g, int b) {
		for(int i = this->_total - 1; i > this->_zero; i--) {
			this->_red[i] = this->_red[i-1];
			this->_green[i] = this->_green[i-1];
			this->_blue[i] = this->_blue[i-1];
		}
		this->_red[this->_zero] = r;
		this->_green[this->_zero] = g;
		this->_blue[this->_zero] = b;
	}
	int gridToIndex(int x, int y) {
		int m[43] = {
			0, 42, 1, 41, 2, 40, 3, 39, 4, 38, 5, 37, 6, 36, 7, 35, 8, 34, 9, 33, 10, 32, 11, 31, 12, 30, 13, 29, 14, 28, 15, 27, 16, 26, 17, 25, 18, 24, 19, 23, 20, 22, 21
			};
		return m[x] + 43 * y;
	}
};




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
unsigned int sampling_period_us;
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
void setup() {
	// put your setup code here, to run once:
	Serial.begin(115200);
	//eye pin modes
	pinMode(11, OUTPUT);
	pinMode(10, OUTPUT);
	pinMode(9, OUTPUT);
	//now shart the strips
	lbs.begin();
	rbs.begin();
	fps.begin();
	lbs.show();
	rbs.show();
	fps.show();

	//AUDIO Setup
	sampling_period_us = round(1000000*(1.0/SAMPLING_FREQUENCY));
	pinMode(A3, INPUT);


}

void loopz() {
	//Audio Sampling
	 for(int i=0; i<SAMPLES; i++) {
        microseconds = micros();    //Overflows after around 70 minutes!
        vReal[i] = analogRead(A3);
        vImag[i] = 0;  


        while(micros() < (microseconds + sampling_period_us)){ } //idle to make even sample
    }
	//FFT
	FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
    FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);
    peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);

    //Serial.println(debug);     //Print out what frequency is the most dominant.

	// put your main code here, to run repeatedly:
	debug = (peak / 2400.0) * 128;
	//leftBoot.push(debug,0,0);
	//rightBoot.push(0,debug,0);
	eyes.purple();
	int x = -1;
	for(int i=1; i<(SAMPLES)/2; i++) {

        /*View all these three lines in serial terminal to see which frequencies has which amplitudes*/
        //Serial.print(peak);
        //Serial.println((i * 1.0 * SAMPLING_FREQUENCY) / SAMPLES, 1);
        //Serial.print(" ");
        Serial.print(peak);     //Print out what frequency is the most dominant.
        Serial.print("\t");
        Serial.println(min(1000,vReal[i]));    //View only this line in serial plotter to visualize the bins

        x++;
        if(x < FRONT_WIDTH-1) {
        	for(int y = FRONT_HEIGHT-1; y >= 0; y--) {
    			int index = frontPanel.gridToIndex(x,FRONT_HEIGHT-y-1);
        		if(min(1000,vReal[i])/1000.0 >= (double)y / (FRONT_HEIGHT-1)) {
        			if(x > 2./3. * FRONT_WIDTH) {
    					frontPanel.setIndex(index,0,0,128);
        			} else if(x > 1./3. * FRONT_WIDTH) {
    					frontPanel.setIndex(index,0,128,0);
        			} else {
    					frontPanel.setIndex(index,128,0,0);
        			}
        		} else {
    				frontPanel.setIndex(index,0,0,0);
        		}
    		}
        }
        //Serial.println("\t");
    }


	// update LED Strips 
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
		//right boot
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
}
int r, g, b;
//-------------------------------------------------------------------------------------------- Debug
void debug_rgb() {
  Serial.print(r); Serial.print(" - "); Serial.print(g); Serial.print(" - "); Serial.println(b);
}
void debug_xyi(int xx, int yy, int ii) {
  Serial.print(xx); Serial.print(","); Serial.print(yy); Serial.print(" - "); Serial.println(ii);
}
