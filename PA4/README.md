# PA4 - Blender and Model Loading

# Dependencies, Building, and Running

## Dependency Instructions
For both of the operating systems to run this project installation of these three programs are required [GLEW](http://glew.sourceforge.net/), [GLM](http://glm.g-truc.net/0.9.7/index.html), and [SDL2](https://wiki.libsdl.org/Tutorials).

### Ubuntu/Linux
```bash
sudo apt-get install libglew-dev libglm-dev libsdl2-dev
```

### Mac OSX
Installation of brew is suggested to easily install the libs. Ensure that the latest version of the Developer Tools is installed.
```bash
brew install glew glm sdl2
```

## Building and Running

### CMake Instructions
The building of the project is done using CMake, installation with apt-get or brew may be necessary. 

```bash
mkdir build
cd build
cmake ..
make
./P4 -v <path-to-vertex-shader> -f <path-to-fragment-shader> -m <path-to-model-obj-file>
```

Note that this program is designed to accept custom vertex and fragment shaders in addition to custom model files. To run program with custom shaders and model
please specify shader file paths with either -v for vertex shader, -f for fragment shader and -m for custom model .obj file. For example:

```bash
./PA4 -v <path-to-vertex-shader> -f <path-to-fragment-shader> -m <path-to-model-obj-file>
./PA4 -v ../shaders/vert1.glsl -f ../shaders/frag1.glsl -m ../models/ball_pit.obj
```

## Usage
While the program is running controls can be input via the keyboard as follows:

<ol>
  <li> Right Arrow Key: Increases planet rotation speed </li>
  <li> Left Arrow Key: Decreases planet rotation speed </li>
  <li> Up Arrow Key: Increases planet translation speed </li>
  <li> Down Arrow Key: Decreases planet translation speed </li>
</ol>
