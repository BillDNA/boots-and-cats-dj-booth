function preload() {
  boot_img = loadImage('assets/boots_cats_final-01.jpg');
}
function setup() {
	createCanvas(1024, 1024);
	background(50);
  // put setup code here
  frontPanel = new FrontPanel({loc:{x:250,y:100}, ledSize:10});
  rightBoot = new Boot({loc:{x:50,y:50}, ledSize: 10});
  leftBoot = new Boot({loc:{x:720,y:50}, ledSize:10, isLeft:true});

  currentAnimation = new RapidRainbow();
  currentAnimation.setUp();

  rightBoot.debug = true;
}

function draw() {
  currentAnimation.loop();
	frame++;
	frontPanel.draw();
	rightBoot.draw();
	leftBoot.draw();
  
}