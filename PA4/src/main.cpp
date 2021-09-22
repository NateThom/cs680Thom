#include <iostream>

#include "engine.h"
#include "argumentparser.h"

int main(int argc, char **argv){
  ArgumentParser input(argc, argv);

  // Collect command line arguments for fragment and vertex shader files
  std::string vertexShaderFilePath, fragmentShaderFilePath, modelFilePath;

  if(input.cmdOptionExists("-v") and input.cmdOptionExists("-f") and input.cmdOptionExists("-m")) {
    vertexShaderFilePath = input.getCmdOption("-v");
    fragmentShaderFilePath = input.getCmdOption("-f");
    modelFilePath = input.getCmdOption("-m");
  }
  else{
    std::cerr<<"Improper usage! You must specify the location of the vertex and fragment shaders as well as"
               "the model's .obj file as follows: \n"
               "./PA2 -v <path-to-vertex-shader> -f <path-to-fragment-shader> -m <path-to-obj-file>\n";
  }

  // Start an engine and run it then cleanup after
  auto *engine = new Engine("PA4", 1080, 720);
  if(!engine->Initialize(vertexShaderFilePath, fragmentShaderFilePath, modelFilePath))
  {
    printf("The engine failed to start.\n");
    delete engine;
    engine = NULL;
    return 1;
  }
  engine->Run();
  delete engine;
  engine = NULL;

  return 0;
}
