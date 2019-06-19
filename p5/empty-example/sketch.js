let frontPanel, rightBoot, leftBoot;
let frame = 0;
function setup() {
	createCanvas(1024, 1024);
	background(50);
  // put setup code here
  frontPanel = new FrontPanel({loc:{x:200,y:50}, ledSize:10});
  rightBoot = new Boot({loc:{x:100,y:50}, ledSize: 10});
  leftBoot = new Boot({loc:{x:600,y:50}, ledSize:10});
}

function draw() {
	frame++;
	frontPanel.draw();
	rightBoot.draw();
	leftBoot.draw();
  	// put drawing code here
  	for(let i = 0; i < leftBoot.length; i++) {
  		leftBoot.setColor(
  			{
  				r:255,
  				g:0,
  				b:0
  			},
  			i
  			);
  	}

  	for(let x = 0; x < frontPanel.width;x++) {
  		for(let y = 0; y < frontPanel.height;y++) {
  			frontPanel.setColor(
  				{
  					r:((x+frame)%frontPanel.width)/frontPanel.width * 256, 
  					g:((y+frame/10)%frontPanel.height)/frontPanel.height * 256,
  					b:0},
  				frontPanel.GridToIndex(x,y)
  				);
  		}
  	}
}