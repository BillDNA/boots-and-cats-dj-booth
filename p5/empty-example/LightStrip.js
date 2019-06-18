/**
 * LightStrip is used to  
 */
class LightStrip {
	/**
	* LightStrip is used to hold data for drawing and manipulation of the lights
	* @param json - a json object containing initial inputs
	*/
	constructor (json) {
		this.loadJSON(json);
	}

	/**
	* loads in information and updates the LightStrip/
	* @param json - a json object containing more information about this LightStrip
	*/
	loadJSON(json) {
		json = json === undefined ? {} : json;
		length = json.length === undefined ? 100 : json.length;
		this.values = json.values === undefined ? this.createBlankValues(length) : json.values;
		this.ledSize = json.ledSize === undefined ? 5 : json.ledSize;
		this.loc = json.loc === undefined ? {x:0,y:0} : json.loc;
	}
	//------------------------------------------------------------- Getters and Setters
	get length() {
		return this.values.length;
	}
	set length(new_length) {
		console.log("WARNING: LightStrip length trying to set directly, use loadJSON()");	
	}
	//------------------------------------------------------------- helpers
	createBlankValues(length) {
		let v = [];
		for(let i = 0;  i < length; i++) {
			v.push({r:0,g:0,b:0});
		}
		return v;
	}
	push(newValue = {r:0,g:0,b:0},atEnd = false) {
		if(atEnd) {
			for(let i = 1; i < this.length; i++) {
				this.values[i-1] = this.values[i];
			}
			this.values[this.length-1] = newValue;
		} else {
			for(let i = this.length-1; i > 0; i--) {
				this.values[i] = this.values[i-1];
			}
			this.values[0] = newValue;
		}
	}


	

}