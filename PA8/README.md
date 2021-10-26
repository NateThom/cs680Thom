# PA8: Bullet

# Teammates
### Nathan Thom
### Alireza Kamran Pishhesari
primary repo: https://github.com/cwcollum/cs680Collum/tree/master/PA8

# Program Instructions
### Keyboard Controls

### Menu Controls

Esc key closes program.

# Dependencies, Building, and Running

### Ubuntu/Linux
```bash
sudo apt-get install libglew-dev libglm-dev libsdl2-dev libassimp-dev libbullet-dev
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
