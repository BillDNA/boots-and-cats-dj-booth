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
		this.isLeft = json.isLeft === undefined ? false : json.isLeft;
		super.loadJSON(json);
	}

	//---------------------------------------------------------------------- Draw Methods
	draw() {
		let bridge = 10;
		let x = this.loc.x;
		let y = this.loc.y + (this.length/2 - bridge/2) * this.ledSize*1.5;
		let c;
		let i = 0;
		for(let yi = 0; yi < this.length/2 - bridge/2; yi++) {
				c = this.values[i];
				x = this.loc.x;
				if(this.isLeft) {
					x = this.loc.x + this.ledSize*(bridge)*1.5;
				}
				fill(c.r,c.g,c.b);
				if(this.debug) {text(i, x-this.ledSize*1.5, y+10);}
				rect(x, y, this.ledSize, this.ledSize);
				
				x =this.loc.x + this.ledSize*(bridge-1)*1.5;
				if(this.isLeft) {
					x = this.loc.x + this.ledSize*1.5;
				}	
				let  ii = this.length -1 - i;
				c = this.values[ii];
				fill(c.r,c.g,c.b);
				if(this.debug){text(ii, x+ this.ledSize*1.5, y+10);}
				rect(x, y, this.ledSize, this.ledSize);
				y -= this.ledSize*1.5;
				i++;
		}
		y = this.loc.y;
		x = this.loc.x;
		if(this.isLeft) {
			x =	this.loc.x + this.ledSize*bridge*1.5;
		}
		for(let ii =0; ii < bridge; ii++) {
			c = this.values[i];
			fill(c.r,c.g,c.b);
			if(this.debug) {text(i, x, y);}
			rect(x, y, this.ledSize, this.ledSize);
			if(this.isLeft) {
				x -= this.ledSize*1.5;
			} else {
				x += this.ledSize*1.5;	
			}
			
			i++;
		}
		image(boot_img, this.loc.x, this.loc.y, 50, 50);

	}
}