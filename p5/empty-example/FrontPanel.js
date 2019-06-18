/**
 * FrontPanel is used to  
 */
class FrontPanel extends LightStrip {
	/**
	* FrontPanel is used to
	* @param json - a json object containing initial inputs
	*/
	constructor (json) {
		super(json);
		this.loadJSON(json);
	}

	/**
	* loads in information and updates the FrontPanel/
	* @param json - a json object containing more information about this FrontPanel
	*/
	loadJSON(json) {
		json = json === undefined ? {} : json;
		super.loadJSON(json);
		this._evenRowLength = json.evenRowLength === undefined ? 22 : json.evenRowLength;
		this._oddRowLength = json.oddRowLength === undefined ? 21 : json.oddRowLength;
		this._height = json.height === undefined ? 6 : json.height;
		if(this.length != this.width*this.height) {
			this.values = this.createBlankValues(this.width*this.height);
		}
	}

	//---------------------------------------------------------------------- Getters and setters
	get width() {
	return this._evenRowLength + this._oddRowLength;
	}
	set width(new_width) {
		console.log("WARNING: front panel trying to set width directly, use loadJSON()");
	}
	get height() {
		return this._height;
	}
	set height(new_height) {
		console.log("WARNING: front panel trying to set height directly, use loadJSON()");
	}
	//---------------------------------------------------------------------- Draw Methods
	draw() {
		let x = this.loc.x;
		let y = this.loc.y;
		let c;
		let i = 0;
		for(let yi = 0; yi < this.height; yi++) {
			x = this.loc.x;
			for(let xi = 0; xi < this._evenRowLength; xi++) {
				c = this.values[i];
				fill(c.r,c.g,c.b);
				rect(x, y, this.ledSize, this.ledSize);
				x += this.ledSize*2;
				i++;
			}		
			y += this.ledSize * 6/4;
			x -= this.ledSize * 6 / 2;
			for(let xi = this._oddRowLength-1; xi >= 0; xi--) {
				c = this.values[i];
				fill(c.r,c.g,c.b);
				rect(x,y, this.ledSize, this.ledSize);
				x -= this.ledSize*2;
				i++;
			}
			y += this.ledSize * 6/4;
		}
	}
	//---------------------------------------------------------------------- Helpers
	


}