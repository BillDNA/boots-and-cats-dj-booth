/**
 * WordWords is used to  
 */
class WordWords extends Animation{
	/**
	* WordWords is used to
	* @param json - a json object containing initial inputs
	*/
	constructor (json) {
		super(json);
		this.loadJSON(json);
	}

	/**
	* loads in information and updates the WordWords/
	* @param json - a json object containing more information about this WordWords
	*/
	loadJSON(json) {
		json = json === undefined ? {} : json;
	}
	//------------------------------------------------------------- Getters and Setters
	setUp() {
		if(DOM.wordSelection === undefined) {
			DOM.wordSelection = createInput('g');
			DOM.wordSelection.position(300,400);
  			//DOM.wordSelection.input(function(e) {});
		}
		frontPanel.clear();
		leftBoot.clear();
		rightBoot.clear();
	}

	loop() {
		let m = this.charaterMatrix(DOM.wordSelection.value());
		frontPanel.clear();
		if(m !== undefined) {
			for(let my = 0; my < m.length; my++) {
				for(let mx = 0; mx < m[my].length; mx++) {
					let i = frontPanel.GridToIndex(mx+(frame % 42),my);	
					if(m[my][mx] == 1) {
						frontPanel.setColor({r:255,g:0,b:0},i);
					} else {
						frontPanel.setColor({r:0,g:0,b:0},i);
					}
				}
			}
		}
		frame++;
		delay(100);
	}
	//------------------------------------------------------------- helpers

	charaterMatrix(character="*") {
		let m;
		if(character == "a" || character == "A") {
			m = [
			[0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0],
			[1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1],
			[1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1],
			[1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
			[1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1],
			[1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1]
			];
		} else if(character == "b" || character == "B") {
			m = [ 
			[1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0],
			[1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1],
			[1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0],
			[1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0],
			[1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1],
			[1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0]
			];
		} else if(character == "c" || character == "C") {
			m = [ 
			[0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0],
			[1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1],
			[1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0],
			[1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0],
			[1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1],
			[0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0]
			];
		} else if(character == "d" || character == "D") {
			m = [ 
			[1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0],
			[1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1],
			[1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1],
			[1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1],
			[1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1],
			[1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0]
			];
		} else if(character == "e" || character == "E") {
			m = [ 
			[1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
			[1, 1, 1, 1, 0, 0, 0, 0, 0, 0],
			[1, 1, 1, 1, 0, 1, 0, 1, 0, 1],
			[1, 1, 1, 1, 1, 0, 1, 0, 1, 0],
			[1, 1, 1, 1, 0, 0, 0, 0, 0, 0],
			[1, 1, 1, 1, 1, 1, 1, 1, 1, 1]
			];
		} else if(character == "f" || character == "F") {
			m = [ 
			[1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
			[1, 1, 1, 1, 0, 0, 0, 0, 0, 0],
			[1, 1, 1, 1, 0, 1, 0, 1, 0, 1],
			[1, 1, 1, 1, 1, 0, 1, 0, 1, 0],
			[1, 1, 1, 1, 0, 0, 0, 0, 0, 0],
			[1, 1, 1, 1, 0, 0, 0, 0, 0, 0]
			];
		} else if(character == "g" || character == "G") {
			m = [ 
			[0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0],
			[1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1],
			[1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 1],
			[1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1],
			[1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1],
			[0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0]
			];
		}

		return m;
	}
		
}