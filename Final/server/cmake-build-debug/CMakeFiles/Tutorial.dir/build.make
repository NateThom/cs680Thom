# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.20

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /home/nthom/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/212.5457.51/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/nthom/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/212.5457.51/bin/cmake/linux/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/nthom/Documents/cs680Thom/Final

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/nthom/Documents/cs680Thom/Final/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/Tutorial.dir/depend.make
# Include the progress variables for this target.
include CMakeFiles/Tutorial.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Tutorial.dir/flags.make

CMakeFiles/Tutorial.dir/src/SDLnet.cpp.o: CMakeFiles/Tutorial.dir/flags.make
CMakeFiles/Tutorial.dir/src/SDLnet.cpp.o: ../src/SDLnet.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nthom/Documents/cs680Thom/Final/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Tutorial.dir/src/SDLnet.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Tutorial.dir/src/SDLnet.cpp.o -c /home/nthom/Documents/cs680Thom/Final/src/SDLnet.cpp

CMakeFiles/Tutorial.dir/src/SDLnet.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Tutorial.dir/src/SDLnet.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nthom/Documents/cs680Thom/Final/src/SDLnet.cpp > CMakeFiles/Tutorial.dir/src/SDLnet.cpp.i

CMakeFiles/Tutorial.dir/src/SDLnet.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Tutorial.dir/src/SDLnet.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nthom/Documents/cs680Thom/Final/src/SDLnet.cpp -o CMakeFiles/Tutorial.dir/src/SDLnet.cpp.s

CMakeFiles/Tutorial.dir/src/SDLnetTCP.cpp.o: CMakeFiles/Tutorial.dir/flags.make
CMakeFiles/Tutorial.dir/src/SDLnetTCP.cpp.o: ../src/SDLnetTCP.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nthom/Documents/cs680Thom/Final/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Tutorial.dir/src/SDLnetTCP.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Tutorial.dir/src/SDLnetTCP.cpp.o -c /home/nthom/Documents/cs680Thom/Final/src/SDLnetTCP.cpp

CMakeFiles/Tutorial.dir/src/SDLnetTCP.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Tutorial.dir/src/SDLnetTCP.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nthom/Documents/cs680Thom/Final/src/SDLnetTCP.cpp > CMakeFiles/Tutorial.dir/src/SDLnetTCP.cpp.i

CMakeFiles/Tutorial.dir/src/SDLnetTCP.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Tutorial.dir/src/SDLnetTCP.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nthom/Documents/cs680Thom/Final/src/SDLnetTCP.cpp -o CMakeFiles/Tutorial.dir/src/SDLnetTCP.cpp.s

CMakeFiles/Tutorial.dir/src/SDLnetUDP.cpp.o: CMakeFiles/Tutorial.dir/flags.make
CMakeFiles/Tutorial.dir/src/SDLnetUDP.cpp.o: ../src/SDLnetUDP.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nthom/Documents/cs680Thom/Final/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/Tutorial.dir/src/SDLnetUDP.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Tutorial.dir/src/SDLnetUDP.cpp.o -c /home/nthom/Documents/cs680Thom/Final/src/SDLnetUDP.cpp

CMakeFiles/Tutorial.dir/src/SDLnetUDP.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Tutorial.dir/src/SDLnetUDP.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nthom/Documents/cs680Thom/Final/src/SDLnetUDP.cpp > CMakeFiles/Tutorial.dir/src/SDLnetUDP.cpp.i

CMakeFiles/Tutorial.dir/src/SDLnetUDP.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Tutorial.dir/src/SDLnetUDP.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nthom/Documents/cs680Thom/Final/src/SDLnetUDP.cpp -o CMakeFiles/Tutorial.dir/src/SDLnetUDP.cpp.s

CMakeFiles/Tutorial.dir/src/SDLnetselect.cpp.o: CMakeFiles/Tutorial.dir/flags.make
CMakeFiles/Tutorial.dir/src/SDLnetselect.cpp.o: ../src/SDLnetselect.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nthom/Documents/cs680Thom/Final/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/Tutorial.dir/src/SDLnetselect.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Tutorial.dir/src/SDLnetselect.cpp.o -c /home/nthom/Documents/cs680Thom/Final/src/SDLnetselect.cpp

CMakeFiles/Tutorial.dir/src/SDLnetselect.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Tutorial.dir/src/SDLnetselect.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nthom/Documents/cs680Thom/Final/src/SDLnetselect.cpp > CMakeFiles/Tutorial.dir/src/SDLnetselect.cpp.i

CMakeFiles/Tutorial.dir/src/SDLnetselect.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Tutorial.dir/src/SDLnetselect.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nthom/Documents/cs680Thom/Final/src/SDLnetselect.cpp -o CMakeFiles/Tutorial.dir/src/SDLnetselect.cpp.s

CMakeFiles/Tutorial.dir/src/main.cpp.o: CMakeFiles/Tutorial.dir/flags.make
CMakeFiles/Tutorial.dir/src/main.cpp.o: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nthom/Documents/cs680Thom/Final/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/Tutorial.dir/src/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Tutorial.dir/src/main.cpp.o -c /home/nthom/Documents/cs680Thom/Final/src/main.cpp

CMakeFiles/Tutorial.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Tutorial.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nthom/Documents/cs680Thom/Final/src/main.cpp > CMakeFiles/Tutorial.dir/src/main.cpp.i

CMakeFiles/Tutorial.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Tutorial.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nthom/Documents/cs680Thom/Final/src/main.cpp -o CMakeFiles/Tutorial.dir/src/main.cpp.s

# Object files for target Tutorial
Tutorial_OBJECTS = \
"CMakeFiles/Tutorial.dir/src/SDLnet.cpp.o" \
"CMakeFiles/Tutorial.dir/src/SDLnetTCP.cpp.o" \
"CMakeFiles/Tutorial.dir/src/SDLnetUDP.cpp.o" \
"CMakeFiles/Tutorial.dir/src/SDLnetselect.cpp.o" \
"CMakeFiles/Tutorial.dir/src/main.cpp.o"

# External object files for target Tutorial
Tutorial_EXTERNAL_OBJECTS =

Tutorial: CMakeFiles/Tutorial.dir/src/SDLnet.cpp.o
Tutorial: CMakeFiles/Tutorial.dir/src/SDLnetTCP.cpp.o
Tutorial: CMakeFiles/Tutorial.dir/src/SDLnetUDP.cpp.o
Tutorial: CMakeFiles/Tutorial.dir/src/SDLnetselect.cpp.o
Tutorial: CMakeFiles/Tutorial.dir/src/main.cpp.o
Tutorial: CMakeFiles/Tutorial.dir/build.make
Tutorial: /usr/lib/x86_64-linux-gnu/libGLEW.so
Tutorial: /usr/lib/x86_64-linux-gnu/libBulletDynamics.so
Tutorial: /usr/lib/x86_64-linux-gnu/libBulletCollision.so
Tutorial: /usr/lib/x86_64-linux-gnu/libLinearMath.so
Tutorial: /usr/lib/x86_64-linux-gnu/libBulletSoftBody.so
Tutorial: /usr/lib/x86_64-linux-gnu/libGL.so
Tutorial: /usr/lib/x86_64-linux-gnu/libGLU.so
Tutorial: /usr/lib/x86_64-linux-gnu/libSDL2main.a
Tutorial: /usr/lib/x86_64-linux-gnu/libSDL2.so
Tutorial: /usr/lib/x86_64-linux-gnu/libMagick++-6.Q16.so
Tutorial: /usr/lib/x86_64-linux-gnu/libMagickCore-6.Q16.so
Tutorial: CMakeFiles/Tutorial.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/nthom/Documents/cs680Thom/Final/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX executable Tutorial"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Tutorial.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Tutorial.dir/build: Tutorial
.PHONY : CMakeFiles/Tutorial.dir/build

CMakeFiles/Tutorial.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Tutorial.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Tutorial.dir/clean

CMakeFiles/Tutorial.dir/depend:
	cd /home/nthom/Documents/cs680Thom/Final/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/nthom/Documents/cs680Thom/Final /home/nthom/Documents/cs680Thom/Final /home/nthom/Documents/cs680Thom/Final/cmake-build-debug /home/nthom/Documents/cs680Thom/Final/cmake-build-debug /home/nthom/Documents/cs680Thom/Final/cmake-build-debug/CMakeFiles/Tutorial.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Tutorial.dir/depend
