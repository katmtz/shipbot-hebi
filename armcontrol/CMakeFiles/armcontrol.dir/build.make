# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.8

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/pi/projects/arm

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/pi/projects/arm/armcontrol

# Include any dependencies generated for this target.
include CMakeFiles/armcontrol.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/armcontrol.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/armcontrol.dir/flags.make

CMakeFiles/armcontrol.dir/armcontrol.cpp.o: CMakeFiles/armcontrol.dir/flags.make
CMakeFiles/armcontrol.dir/armcontrol.cpp.o: armcontrol.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/projects/arm/armcontrol/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/armcontrol.dir/armcontrol.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/armcontrol.dir/armcontrol.cpp.o -c /home/pi/projects/arm/armcontrol/armcontrol.cpp

CMakeFiles/armcontrol.dir/armcontrol.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/armcontrol.dir/armcontrol.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pi/projects/arm/armcontrol/armcontrol.cpp > CMakeFiles/armcontrol.dir/armcontrol.cpp.i

CMakeFiles/armcontrol.dir/armcontrol.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/armcontrol.dir/armcontrol.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pi/projects/arm/armcontrol/armcontrol.cpp -o CMakeFiles/armcontrol.dir/armcontrol.cpp.s

CMakeFiles/armcontrol.dir/armcontrol.cpp.o.requires:

.PHONY : CMakeFiles/armcontrol.dir/armcontrol.cpp.o.requires

CMakeFiles/armcontrol.dir/armcontrol.cpp.o.provides: CMakeFiles/armcontrol.dir/armcontrol.cpp.o.requires
	$(MAKE) -f CMakeFiles/armcontrol.dir/build.make CMakeFiles/armcontrol.dir/armcontrol.cpp.o.provides.build
.PHONY : CMakeFiles/armcontrol.dir/armcontrol.cpp.o.provides

CMakeFiles/armcontrol.dir/armcontrol.cpp.o.provides.build: CMakeFiles/armcontrol.dir/armcontrol.cpp.o


# Object files for target armcontrol
armcontrol_OBJECTS = \
"CMakeFiles/armcontrol.dir/armcontrol.cpp.o"

# External object files for target armcontrol
armcontrol_EXTERNAL_OBJECTS = \
"/home/pi/projects/arm/armcontrol/CMakeFiles/hebi_cpp_files.dir/hebi/src/mac_address.cpp.o" \
"/home/pi/projects/arm/armcontrol/CMakeFiles/hebi_cpp_files.dir/hebi/src/command.cpp.o" \
"/home/pi/projects/arm/armcontrol/CMakeFiles/hebi_cpp_files.dir/hebi/src/feedback.cpp.o" \
"/home/pi/projects/arm/armcontrol/CMakeFiles/hebi_cpp_files.dir/hebi/src/info.cpp.o" \
"/home/pi/projects/arm/armcontrol/CMakeFiles/hebi_cpp_files.dir/hebi/src/group_command.cpp.o" \
"/home/pi/projects/arm/armcontrol/CMakeFiles/hebi_cpp_files.dir/hebi/src/group_feedback.cpp.o" \
"/home/pi/projects/arm/armcontrol/CMakeFiles/hebi_cpp_files.dir/hebi/src/group_info.cpp.o" \
"/home/pi/projects/arm/armcontrol/CMakeFiles/hebi_cpp_files.dir/hebi/src/module.cpp.o" \
"/home/pi/projects/arm/armcontrol/CMakeFiles/hebi_cpp_files.dir/hebi/src/group.cpp.o" \
"/home/pi/projects/arm/armcontrol/CMakeFiles/hebi_cpp_files.dir/hebi/src/lookup.cpp.o"

armcontrol: CMakeFiles/armcontrol.dir/armcontrol.cpp.o
armcontrol: CMakeFiles/hebi_cpp_files.dir/hebi/src/mac_address.cpp.o
armcontrol: CMakeFiles/hebi_cpp_files.dir/hebi/src/command.cpp.o
armcontrol: CMakeFiles/hebi_cpp_files.dir/hebi/src/feedback.cpp.o
armcontrol: CMakeFiles/hebi_cpp_files.dir/hebi/src/info.cpp.o
armcontrol: CMakeFiles/hebi_cpp_files.dir/hebi/src/group_command.cpp.o
armcontrol: CMakeFiles/hebi_cpp_files.dir/hebi/src/group_feedback.cpp.o
armcontrol: CMakeFiles/hebi_cpp_files.dir/hebi/src/group_info.cpp.o
armcontrol: CMakeFiles/hebi_cpp_files.dir/hebi/src/module.cpp.o
armcontrol: CMakeFiles/hebi_cpp_files.dir/hebi/src/group.cpp.o
armcontrol: CMakeFiles/hebi_cpp_files.dir/hebi/src/lookup.cpp.o
armcontrol: CMakeFiles/armcontrol.dir/build.make
armcontrol: CMakeFiles/armcontrol.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/pi/projects/arm/armcontrol/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable armcontrol"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/armcontrol.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/armcontrol.dir/build: armcontrol

.PHONY : CMakeFiles/armcontrol.dir/build

CMakeFiles/armcontrol.dir/requires: CMakeFiles/armcontrol.dir/armcontrol.cpp.o.requires

.PHONY : CMakeFiles/armcontrol.dir/requires

CMakeFiles/armcontrol.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/armcontrol.dir/cmake_clean.cmake
.PHONY : CMakeFiles/armcontrol.dir/clean

CMakeFiles/armcontrol.dir/depend:
	cd /home/pi/projects/arm/armcontrol && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pi/projects/arm /home/pi/projects/arm /home/pi/projects/arm/armcontrol /home/pi/projects/arm/armcontrol /home/pi/projects/arm/armcontrol/CMakeFiles/armcontrol.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/armcontrol.dir/depend

