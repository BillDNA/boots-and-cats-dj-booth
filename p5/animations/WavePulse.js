/**
 * WavePulse is used to  
 */
class WavePulse extends Animation{
	/**
	* WavePulse is used to
	* @param json - a json object containing initial inputs
	*/
	constructor (json) {
		super(json);
		this.loadJSON(json);
	}

	/**
	* loads in information and updates the WavePulse/
	* @param json - a json object containing more information about this WavePulse
	*/
	loadJSON(json) {
		json = json === undefined ? {} : json;
	}
	//------------------------------------------------------------- Getters and Setters

	//------------------------------------------------------------- draw methods
	setUp() {
		animationFrame = 0;
		frontPanel.clear();
		rightBoot.clear();
		leftBoot.clear();
		this.updateRGB();
	}
	loop() {
		let i;
		animationFrame = (animationFrame+1) % 22;
		if(animationFrame == 0) {
			this.updateRGB();
		}
		for(let y = 0; y < frontPanel.height;y++) {
			
			// Left of center
			for(let x = 0; x < 22; x++) {
				t = 256 - ((x+animationFrame) % 22) * 6;
				i = frontPanel.GridToIndex(x,y);
				frontPanel.setColor(
					{
						r:r*t,
						g:g*t,
						b:b*t
					},i);
			}
			// Right of center
			for(let x = frontPanel.width-1; x >= 22; x--) {
				t = (((x)+(22-animationFrame)) % 22) * 6 + 128;
				i = frontPanel.GridToIndex(x,y);
				frontPanel.setColor(
					{
						r:r*t,
						g:g*t,
						b:b*t
					},i);
			}
		}

		for(let i = rightBoot.length-1; i>=0; i--) {
			if(i > (22-animationFrame)/22 * rightBoot.length) {
				rightBoot.setColor({r:r*255,g:g*255,b:b*255},i);
			} else {
				rightBoot.setColor({r:0,g:0,b:0},i);
			}
		}
		for(let i = leftBoot.length-1; i>=0; i--) {
			if(i > (22-animationFrame)/22 * rightBoot.length) {
				leftBoot.setColor({r:r*255,g:g*255,b:b*255},i);
			} else {
				leftBoot.setColor({r:0,g:0,b:0},i);
			}
		}
		delay(50);
	}
	//------------------------------------------------------------- helpers
	updateRGB() {
		r = random([0,1]);
		g = random([0,1]);
		b = random([0,1]);
		while(r+g+b == 0) {
			r = random([0,1]);
			g = random([0,1]);
			b = random([0,1]);
		}
	}
}