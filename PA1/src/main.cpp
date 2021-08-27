#include <iostream>

#include "engine.h"
#include "argumentparser.h"

int main(int argc, char **argv){
  ArgumentParser input(argc, argv);

  // Collect command line arguments for fragment and vertex shader files
  std::string vertexShaderFilePath, fragmentShaderFilePath;
  if(input.cmdOptionExists("-v") && input.cmdOptionExists("-f")) {
    vertexShaderFilePath = input.getCmdOption("-v");
    fragmentShaderFilePath = input.getCmdOption("-f");

    // Start an engine and run it then cleanup after
    Engine *engine = new Engine("PA1", 800, 600);
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
  }
  else{
    std::cout<<"***Command line arguments not detected, running with default vertex and fragment shaders***"<<std::endl;
    std::cout<<"***To run program with custom shaders please specify shader file paths***"<<std::endl;
    std::cout<<"***(i.e. './PA1 -v <path-to-vertex-shader> -f <path-to-fragment-shader>'***"<<std::endl;

    // Start an engine and run it then cleanup after
    Engine *engine = new Engine("PA1", 800, 600);
    if(!engine->Initialize())
    {
      printf("The engine failed to start.\n");
      delete engine;
      engine = NULL;
      return 1;
    }
    engine->Run();
    delete engine;
    engine = NULL;
  }

  return 0;
}
