#include <iostream>

#include "engine.h"


int main(int argc, char **argv)
{
  // Start an engine and run it then cleanup after
  Engine *engine = new Engine("../config.txt");

  // Set the engine to its initial state
  if(!engine->Initialize())
  {
    printf("The engine failed to start.\n");
    delete engine;
    engine = NULL;
    return 1;
  }

  // Run the engine
  engine->Run();
  delete engine;
  engine = NULL;
  return 0;
}
