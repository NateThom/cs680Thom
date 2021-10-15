# PA7: Solar System

# Teammates
### Alireza Kamran Pishhesari
### Christopher Collum
primary repo: https://github.com/cwcollum/cs680Collum/tree/master/PA7

# Program Instructions
### Keyboard Controls
Left arrow key slows camera down

Right arrow key speeds camera up

Down arrow key looks down

Up arrow key looks up

W zooms in

S zooms out

A moves left

D moves right

1-9 center the view on a planet.  The camera will always be looking towards the sun.

### Menu Controls
Two sliders are used.

One controls the distance everything is from the sun, but sizes do not change

The other controls the speed of the simulation

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
./Tutorial
```

There is a config file provided.  The top of the config file controls different window initialization options.  For example, if you want the window size to be changed you can supply different resolutions.

Stellar object information can be changed but is not recommended due to being accurate to the real solar system.
