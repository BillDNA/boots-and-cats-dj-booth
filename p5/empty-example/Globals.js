//on an arduino memory space is limited so declaring globals rather than in methods helps keep memory usage predictable


let frontPanel, rightBoot, leftBoot; //LightStip objects for drawing and holding the pictures
let currentAnimation; //the active animation
let frame = 0; //incremets over each frame
let t,r,g,b;  //personaly i use these to help readablity 
let i,animationFrame;


//here are some global helpers available in the arduino code
function delay(millis) {
    var date = new Date();
    var curDate = null;
    do { curDate = new Date(); }
    while(curDate-date < millis);
}