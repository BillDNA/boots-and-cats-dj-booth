/**
 * Boot is used to  
 */
class Boot extends LightStrip{
	/**
	* Boot is used to
	* @param json - a json object containing initial inputs
	*/
	constructor (json) {
		super(json);
		this.loadJSON(json);
	}

	/**
	* loads in information and updates the Boot/
	* @param json - a json object containing more information about this Boot
	*/
	loadJSON(json) {
		json = json === undefined ? {} : json;
		super.loadJSON(json);
	}


	//---------------------------------------------------------------------- Draw Methods
	draw() {
		let x = this.loc.x;
		let y = this.loc.y;
		let c;
		let i = 0;
		for(let yi = 0; yi < this.length; yi++) {
				c = this.values[i];
				fill(c.r,c.g,c.b);
				rect(x, y, this.ledSize, this.ledSize);
				y += this.ledSize*1.5;
				i++;	
		}
	}
}