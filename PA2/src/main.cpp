#include <iostream>

#include "engine.h"
#include "argumentparser.h"

int main(int argc, char **argv){
  ArgumentParser input(argc, argv);

  // Collect command line arguments for fragment and vertex shader files
  std::string vertexShaderFilePath, fragmentShaderFilePath;

  if(input.cmdOptionExists("-v")) {
    vertexShaderFilePath = input.getCmdOption("-v");
  }
  if(input.cmdOptionExists("-f")) {
    fragmentShaderFilePath = input.getCmdOption("-f");
  }

  // Start an engine and run it then cleanup after
  Engine *engine = new Engine("PA2", 1000, 800);
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
