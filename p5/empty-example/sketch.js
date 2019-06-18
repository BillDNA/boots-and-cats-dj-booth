let frontPanel;
let frame = 0;
function setup() {
	createCanvas(1024, 1024);
	background(50);
  // put setup code here
  frontPanel = new FrontPanel({loc:{x:200,y:50},ledSize:10});
}

function draw() {
	frame++;
	frontPanel.draw();
  	// put drawing code here
  	if(frame % 43 < 21) {
  		frontPanel.push({r:(frame%43)/21 * 256,g:255,b:frame % 256}, true);
	} else {
  		frontPanel.push({g:((frame-21)%43)/21 * 256,r:frame % 256,b:255}, true);
	}
}