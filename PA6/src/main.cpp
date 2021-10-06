#include <iostream>

#include "engine.h"


int main(int argc, char **argv)
{
  if(argc != 5){
    printf("Please provide a vertex file, fragment file, object file, and material file\n");
    return 0;
  }

  std::string shader_folder = "shaders/";
  std::string object_folder = "objects/";
  std::string v_shader_filepath = argv[1];
  std::string f_shader_filepath = argv[2];
  std::string object_filepath = argv[3];
  std::string material_filepath = argv[4];
  v_shader_filepath = shader_folder + v_shader_filepath;
  f_shader_filepath = shader_folder + f_shader_filepath;
  object_filepath = object_folder + object_filepath;
  material_filepath = object_folder + material_filepath;

  // Start an engine and run it then cleanup after
  Engine *engine = new Engine("Tutorial Window Name", 1200, 720, v_shader_filepath, f_shader_filepath, object_filepath, material_filepath);
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
  return 0;
}
