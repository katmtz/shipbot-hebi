The file is in the folder projects/arm

It contains sub-folders. 'hebi' contains the api files, examples are the examples from api. 
Ignore testex and build, those are test/backup and build file for the example.

'armcontrol' is where my build file is

To re-make everthing. cd to armcontrol and cmake ../ using the cmakelist in /arm. 
If you change the name or location, cmakelist needs to be modified. Unless you are very confident, please don't do it. 

Then, make and run

You can edit armcontrol.cpp in greany for colored programming environment.