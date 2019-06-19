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
	}

	setup() {

	}

	loop() {

	}

}