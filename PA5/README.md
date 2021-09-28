# PA5: Model Loading

# Teammates
### Alireza Kamran Pishhesari
### Christopher Collum
primary repo: https://github.com/cwcollum/cs680Collum/tree/master/PA5

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
Six buttons are provided for each cube object.  Each button press only applies one effect.  
Pause sets either the orbit or speed to zero.  Pressing again will only set value once more to zero.
Reset sets orbit or spin to a slider value of 1.
Reverse sets orbit or spin to a slider value of -1.

Esc key closes program.

# Dependencies, Building, and Running

## Dependency Instructions
For both of the operating systems to run this project installation of these three programs are required [GLEW](http://glew.sourceforge.net/), [GLM](http://glm.g-truc.net/0.9.7/index.html), and [SDL2](https://wiki.libsdl.org/Tutorials).

These programs should already be installed on the ECC computers.

### Ubuntu/Linux
```bash
sudo apt-get install libglew-dev libglm-dev libsdl2-dev libassimp-dev
```

### CMake Instructions
The building of the project is done using CMake.  CMake automatically handles moving the shader and objects directory into the build folder and will automatically build all the imgui files.

```bash
mkdir build
cd build
cmake ..
make
./Tutorial v_shader.txt f_shader.txt table.obj table.mtl
```

In the directory a sphere mesh with over 7000 vertices is also provided for testing purposes.  The command line call for that mesh is instead:
```
./Tutorial v_shader.txt f_shader.txt sphere.obj sphere.mtl
```
Any further object files provided must also include their corresponding material file and can be read in the following format:
```
./Tutorial v_shader.txt f_shader.txt <filename>.obj <filename>.mtl
```
Failure to provide all four filenames will currently cause the program to respond with an error and then exit.  Any new files can be added to the objects folder in the root directory, then rerun make.  If the already made, they can also be put into the objects directory generated in the build folder.

One of the models has been given the model loader arguments, the other was provided the default constructor to test when there is a default case.

For PA5 the dragon model and material file have been added to the objects folder.  Both are still required in the command line to run.
Color is also loaded in from the material file.  The provided table/sphere object and material files showcase the material color loading.

## Netboot Ubuntu\Debian
The project should run on the network install of Debian in the ECC. To boot into this OS, restart the computer and use the arrow keys on boot to select Ubuntu\Debian from the startup list (currently its listed under CS135).
