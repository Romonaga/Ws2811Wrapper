# Ws2811Wrapper
This is nothing more than a wrapper around https://github.com/jgarff/rpi_ws281x  A fantastic easy to use lib for the rasberry pi.

I needed a c++ interface and attempted to make the interface appear like many of the Arduino libs.  So if you need a c++ interface, please feel free to use.

It will require the library listed above, and expects the headers from that lib to be in /usr/local/include, and he lib itself in /usr/local/lib.  
As this uses scons, and I have little understanding of it, I simply coppied the file to the proper location.
