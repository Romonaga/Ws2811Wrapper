# Ws2811Wrapper
This is nothing more than a wrapper around https://github.com/jgarff/rpi_ws281x  A fantastic easy to use lib for the rasberry pi.

It will require the library listed above, and expects the headers from that lib to be in /usr/local/include, and he lib itself in /usr/local/lib.  
As this uses scons, and I have little understanding of it, I simply copied the file to the proper location.

# *** UPDATE ***


Well, I no longer consider this just a "simple" wrapper around rpi_ws281x. It now provides some of the same features you see in higher level languages.

<b>Soon I will be renaming this Lib as it no longer reflects what it does. 
Now the Ws2811 Wrapper supports https://github.com/hzeller/rpi-rgb-led-matrix , thats correct, this lib will properly control most of the popular matrix boards 
on the market.  This is exciting as the ws2811 based LEDS are expensive.</b>

While the rpi_ws281x is  a fantastic lib on its own, it does lack a bit of lets say user-friendliness. It has and always will be a base lib for others to build on.  When I was looking to use this in my projects, I could not find a decent c++ lib that provided some of the same functionality I found in the Arduino world.
As a result, several years ago I developed this for my own projects. Recently I have posted it here, hoping others will find it useful.

While most people it seems do not develop in c or c++, a few of us still prefer it and find it more comfortable. So what does this "wrapper" do?

 * The most important feature is the Matrix support that is built in.  I found when testing different matrix's that they all had different start spots.  This wrapper, will overcome this.  Please review the initStrip.  Row 0, column 0, can be truly be achived. 
 * The underlying lib rpi_ws281x from that group of developers, has support for 2 channel's.   This wrapper will take advantage of this, and manage the 2 channels programmatic. 
 * Many of the color conversion's you will need.


Outside of these 2 features, yea it is a wrapper around a fun lib to play with.
