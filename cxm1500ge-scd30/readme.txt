This is an example C++ library for Arduino ATmega2650, based on one created by Yusron Muttaqin for firmware IC CXM1500GE

Installation
--------------------------------------------------------------------------------

To install this library, just place this entire folder as a subfolder in your
Arduino/lib/targets/libraries folder.

When installed, this library should look like:

Arduino/lib/targets/libraries/cxm1500ge              (this library's folder)
Arduino/lib/targets/libraries/cxm1500ge/atmega2650/cxm1500ge.a     (the library implementation file)
Arduino/lib/targets/libraries/cxm1500ge/cxm1500ge.h       (the library description file)
Arduino/lib/targets/libraries/cxm1500ge/keywords.txt (the syntax coloring file)
Arduino/lib/targets/libraries/cxm1500ge/examples     (the examples in the "open" menu)
Arduino/lib/targets/libraries/cxm1500ge/readme.txt   (this file)

Building
--------------------------------------------------------------------------------

After this library is installed, you just have to start the Arduino application.
You may see a few warning messages as it's built.

To use this library in a sketch, go to the Sketch | Import Library menu and
select cxm1500ge.  This will add a corresponding line to the top of your sketch:
#include <cxm1500ge.h>

To stop using this library, delete that line from your sketch.

Geeky information:
After a successful build of this library, a new file named "cxm1500ge.o" will appear
in "Arduino/lib/targets/libraries/cxm1500ge". This file is the built/compiled library
code.

If you choose to modify the code for this library (i.e. "cxm1500ge.a" or "cxm1500ge.h"),
then you must first 'unbuild' this library by deleting the "cxm1500ge.o" file. The
new "cxm1500ge.o" with your code will appear after the next press of "verify"

