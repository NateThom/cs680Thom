#include <iostream>

#include "engine.h"
#include "argumentparser.h"

int main(int argc, char **argv){
  ArgumentParser input(argc, argv);

  // Collect command line arguments for fragment and vertex shader files
  std::string vertexShaderFilePath, fragmentShaderFilePath;

  if(input.cmdOptionExists("-v") and input.cmdOptionExists("-f")) {
    vertexShaderFilePath = input.getCmdOption("-v");
    fragmentShaderFilePath = input.getCmdOption("-f");
  }
  else{
    std::cerr<<"Improper usage! You must specify the location of the vertex and fragment shaders as follows: \n"
               "./PA2 -v <path-to-vertex-shader> -f <path-to-fragment-shader>";
  }

  // Start an engine and run it then cleanup after
  auto *engine = new Engine("PA2", 1000, 800);
  if(!engine->Initialize(vertexShaderFilePath, fragmentShaderFilePath))
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
