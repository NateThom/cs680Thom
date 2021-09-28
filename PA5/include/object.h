#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "graphics_headers.h"
#include <iostream>
#include <fstream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/color4.h>

class Object
{
  public:
    Object();
    Object(std::string o_path, std::string f_path);
    ~Object();
    void UpdateFromOrigin(unsigned int dt,
                            float f_scale, 
                            float o_speed, 
                            float s_speed);
    void UpdateFromModel(unsigned int dt, 
                            glm::mat4 planet_model, 
                            float f_scale, 
                            float o_speed, 
                            float s_speed);
    void Render();

    glm::mat4 GetModel();
    glm::mat4 GetOriginModel();

    float getOrbitSpeed(unsigned int dt, float o_speed, float rotational_period);
    float getSpinSpeed(unsigned int dt, float s_speed, float rotational_period);

	  bool loadOBJ(std::vector<Vertex> &out_vertices, std::vector<unsigned int> &out_indices, std::string object, std::string material);
	  std::vector<std::string> split(std::string str);

  private:
    glm::mat4 model;
    glm::mat4 origin_model;
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;

    float degree;
    float angle;
};

#endif /* OBJECT_H */
