Set up
* Javascript Framework refrence (http://p5js.org/reference/);
* setting up local server to serve imgs (https://github.com/processing/p5.js/wiki/Local-server)

* public sharable link master branch (https://billdna.github.io/boots-and-cats-dj-booth/p5/empty-example/index.html) 

File hirearchy 
* file not realy oginized as of yet but notable files are
* Globals.js - Arduino Code uses globals more than scoped variables as memory can be limiting and having variables as globals helps make memory usage predictable
* sketch.js - this is what draw all the things and what takes user input


creating a new animation
1. new JS file in animations floder with class that extends Animation
2. add to to index.html load
3. add to dropdown menu in sketch.js preloadfunction
4. add you new animation to the changeAnimation() logic in sketch.js
5. write animation code :)


Sound Data Docs
1. https://p5js.org/reference/#/p5.FFT
2. https://p5js.org/reference/#/p5.FFT/getEnergy