HEBI C++ API
------------

The 'hebi' folder contains a C++ wrapper of the HEBI C API.  The only
dependencies are the include files and the library file itself.

A set of example programs is included, as well as a CMake configuration that
compiles the examples (just type 'cmake <path to this directory>' in the directory you
wish to build the examples in, and then use the created build system to build
the 'examples' project).

To use this in your own program, the examples give a basic pattern that can be
followed.
1) Add the 'include', and 'src' directories to your project.
2) Ensure the files in 'src' are compiled in.
3) Link with the files in the 'lib' directory.
4) Use the C++ data structures from your code as needed. The various examples
   and the documentation can serve as a reference here.
5) Use the CMakeLists.txt file as an example to guide you how to compile the C++
   library into your project.
6) Note that you can change the names of the added directories, as long as you
   update the references in the build system.  No code should have to change.
