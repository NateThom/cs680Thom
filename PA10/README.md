# PA10 - Pinball

# Teammates
### Christopher Collum
### Alireza Kamran Pishhesari
primary repo: https://github.com/cwcollum/cs680Collum/tree/master/PA10

# Program Instructions
### Keyboard Controls
Up arrow key = fire plunger
Left arrow key = move left flipper
Right arrow key = move right flipper

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
### Notes
There is a known bug that causes the score to stop decreasing after it decrements once. There are also some board states which the ball can get stuck in.
