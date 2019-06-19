/**
 * RapidRainbow is used to  
 */
class RapidRainbow extends Animation {
	/**
	* RapidRainbow is used to
	* @param json - a json object containing initial inputs
	*/
	constructor (json) {
		super(json);
		this.loadJSON(json);
	}

	/**
	* loads in information and updates the RapidRainbow/
	* @param json - a json object containing more information about this RapidRainbow
	*/
	loadJSON(json) {
		json = json === undefined ? {} : json;
		this._name = json.name === undefined ? "Rapid Rainbow" : json.name;
	}
	//------------------------------------------------------------- Getters and Setters

	//------------------------------------------------------------- draw methods
	setUp() {
		this.updateRGB();
		for(let i = 0; i< Math.max(Math.max(leftBoot.length,frontPanel.length),rightBoot.length); i++) {
			if(i<frontPanel.length) {frontPanel.setColor({r:r,g:g,b:b},i);}
			if(i<rightBoot.length) {rightBoot.setColor({r:r,g:g,b:b},i);}
			if(i<leftBoot.length) {leftBoot.setColor({r:r,g:g,b:b},i);}
			if(i%8 == 0) {
				this.updateRGB();
			}
		}
	}
	loop() {
		if(frame%8 == 0) {
			this.updateRGB();
		}
		frontPanel.push({r:r,g:g,b:b},true);
		rightBoot.push({r:r,g:g,b:b},true);
		leftBoot.push({r:r,g:g,b:b},true);
		delay(15);
	}
	//------------------------------------------------------------- helpers
	updateRGB() {
		r = random([0,255]);
		g = random([0,255]);
		b = random([0,255]);
		while(r+g+b == 0) {
			r = random([0,255]);
			g = random([0,255]);
			b = random([0,255]);
		}
	}
}