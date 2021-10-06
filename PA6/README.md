# PA6: Texture Loading

# Teammates
### Nathan Thom
### Alireza Kamran Pishhesari
primary repo: https://github.com/cwcollum/cs680Collum/tree/master/PA6

# Program Instructions
### Keyboard Controls
Left arrow key reverses orbit.

Right arrow key reverses spin.

Down arrow key pauses the orbit.  Press again to restart.  Will only restart if speed wasn't zero from slider when paused.

Up arrow key pauses the spin.  Press again to restart.  Will only restart if speed wasn't zero from slider when paused.

All arrow key inputs are taken aftert the key has been released.  Holding the key down does nothing.

### Mouse Controls
Only right click was implemented due to the menu

Right click pauses all motion.  Pressing again does nothing.

### Menu Controls
Six buttons are provided for the object.  Each button press only applies one effect.  
Pause sets either the orbit or speed to zero.  Pressing again will only set value once more to zero.
Reset sets orbit or spin to a slider value of 1.
Reverse sets orbit or spin to a slider value of -1.

Esc key closes program.

# Dependencies, Building, and Running

### Ubuntu/Linux
```bash
sudo apt-get install libglew-dev libglm-dev libsdl2-dev libassimp-dev libmagick++-dev
```

### CMake Instructions
The building of the project is done using CMake.  CMake automatically handles moving the shader and objects directory into the build folder and will automatically build all the imgui files.

```bash
mkdir build
cd build
cmake ..
make
./Tutorial v_shader.txt f_shader.txt buddha.obj buddha.mtl
```

For this project, only the buddha file renders completely correctly due to the only two textures being provided being relevant to that object.  Other objects can be loaded, but if they have one mesh only the granite will be drawn, and more than two will segfault.  The code will be generalized in the next version.  Any object provided that does not have texture vertices will crash the program.

Failure to provide all four filenames will currently cause the program to respond with an error and then exit.  Any new files can be added to the objects folder in the root directory, then rerun make.  If already made, they can also be put into the objects directory generated in the build folder.

Texture directories are currently hardcoded(will be generalized next), but the cmake in this project repo has the proper textures and places them in the correct location.
