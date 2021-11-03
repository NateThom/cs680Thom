# PA8: Bullet

# Teammates
### Christopher Collum
### Alireza Kamran Pishhesari
primary repo: https://github.com/cwcollum/cs680Collum/tree/master/PA9

# Program Instructions
### Keyboard Controls
Up arrow key = move cube forward
Down arrow key = move cube backward
Left arrow key = move cube left
Right arrow key = move cube right

w key = speed movement steps up
s key = speed movement steps down

0.5 is maximum speed
0.0 is minimum speed

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
I was able to get the ball to clip through the walls in very very specific circumstances when the cube and the ball were directly lined up together.  The ball could clip through when it has nowhere else to bounce to.

The other thing to note is the cylinder is offset to allow a user to simply press up arrow key right at the beginning in order to test the bounce easily.
