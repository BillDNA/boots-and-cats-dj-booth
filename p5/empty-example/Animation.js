/**
 * Animation 
 */
class Animation {
	/**
	* Animation
	* @param json - a json object containing initial inputs
	*/
	constructor (json) {
		this.loadJSON(json);
	}

	/**
	* loads in information and updates the Animation/
	* @param json - a json object containing more information about this Animation
	*/
	loadJSON(json) {
		json = json === undefined ? {} : json;
		this._name = json.name === undefined ? "blank animation" : json.name;
	}

	//------------------------------------------------------------- Getters and Setters
	get name() {
		return this._name;
	}
	set name(new_name) {
		this._name = new_name;
	}

	setUp() {

	}
	loop() {

	}

	//------------------------------------------------------------- helpers
}