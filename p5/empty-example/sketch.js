let DOM = {};
function preload() {
  boot_img = loadImage('assets/boots_cats_final-01.jpg');
  DOM.animationSelection = createSelect();
  DOM.animationSelection.position(450,314);
  DOM.animationSelection.option('Rapid Rainbow');
  DOM.animationSelection.option('Wave Pulse');
  DOM.animationSelection.option('Spaz');
  DOM.animationSelection.option('Words Words');
  DOM.animationSelection.changed(changeAnimation);
  DOM.lbls = {};
  DOM.lbls.animationSelection = createP("Animation Selection:");
  DOM.lbls.animationSelection.style('color', '#00ff00');
  DOM.lbls.animationSelection.position(300,300);
}
function setup() {
	createCanvas(1024, 1024);
	background(50);
  // put setup code here
  frontPanel = new FrontPanel({loc:{x:250,y:100}, ledSize:10});
  rightBoot = new Boot({loc:{x:50,y:50}, ledSize: 10});
  leftBoot = new Boot({loc:{x:720,y:50}, ledSize:10, isLeft:true});

  currentAnimation = new WordWords();
  currentAnimation.setUp();

  rightBoot.debug = true;
  leftBoot.debug = true;
  //sound stuff
  mic = new p5.AudioIn();
  mic.start();    
  fft = new p5.FFT();
  fft.setInput(mic);
}

function draw() {
  background(50); //clears the screen
  fft.analyze(); //updates the sound data

  currentAnimation.loop();
	frame++;
	frontPanel.draw();
	rightBoot.draw();
	leftBoot.draw();
  
}

function changeAnimation() {
  frame = 0;
  let item = DOM.animationSelection.value();
  if(item === 'Rapid Rainbow') {
    currentAnimation = new RapidRainbow();
    currentAnimation.setUp();
  } else if(item === 'Wave Pulse') {
    currentAnimation = new WavePulse();
    currentAnimation.setUp();
  } else if(item === 'Spaz') {
    currentAnimation = new Spaz();
    currentAnimation.setUp();
  } else if(item === 'Words Words') {
    currentAnimation = new WordWords();
    currentAnimation.setUp();
  }
}