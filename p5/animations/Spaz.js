/**
 * Spaz is used to make the colors completely random  
 */
class Spaz extends Animation {
	/**
	* @param json - a json object containing initial inputs
	*/
    constructor(json) {
        super(json);
        this.loadJSON(json);
    }

	/**
	* loads in information and updates the Spaz/
	* @param json - a json object containing more information about this Spaz
	*/
    loadJSON(json) {
        json = json === undefined ? {} : json;
    }
    //------------------------------------------------------------- Getters and Setters

    //------------------------------------------------------------- draw methods
    setUp() {
        frontPanel.clear();
        rightBoot.clear();
        leftBoot.clear();
        this.updateRGB();
    }

    loop() {
        let i; // light index

        for (let y = 0; y < frontPanel.height; y++) {
            for (let x = 0; x <= frontPanel.width - 1; x++) {
                i = frontPanel.GridToIndex(x, y);
                this.updateRGB();
                frontPanel.setColor({ r, g, b }, i);
            }
        }

        for (let i = rightBoot.length - 1; i >= 0; i--) {
            this.updateRGB();
            rightBoot.setColor({ r, g, b }, i);
        }
        for (let i = leftBoot.length - 1; i >= 0; i--) {
            this.updateRGB();
            leftBoot.setColor({ r, g, b }, i);
        }
        delay(50);
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