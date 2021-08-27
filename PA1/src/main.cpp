#include <iostream>

#include "engine.h"


int main(int argc, char **argv)
{
  // Collect command line arguments for fragment and vertex shader files
  std::string vertexShaderFilePath, fragmentShaderFilePath;
  if(argc > 2){
    vertexShaderFilePath = argv[1];
    fragmentShaderFilePath = argv[2];

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
