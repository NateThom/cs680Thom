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
#include <Magick++.h>

class Object
{
  public:
    // Object();
    Object(std::string name, 
            std::string o_path, 
            std::string f_path, 
            std::string type,
            float aphelion,
            float periphelion,
            float axial_tilt,
            float rotational_period,
            float orbital_speed);
    ~Object();
    void UpdateFromOrigin(unsigned int dt, 
                            float distance,
                            float speed);
    void UpdateFromModel(unsigned int dt, 
                            glm::mat4 planet_model, 
                            float dist_mod,
                            float speed);
    void Render();

    glm::mat4 GetModel();
    glm::mat4 GetOriginModel();

    float getOrbitSpeed(unsigned int dt, float o_speed, float rotational_period);
    float getSpinSpeed(unsigned int dt, float s_speed, float rotational_period);
    std::string getType();

	  bool loadOBJ(std::vector<Vertex> &out_vertices, std::vector<unsigned int> &out_indices, std::string object);
	  std::vector<std::string> split(std::string str);
    bool loadMaterialFile(std::string material);
    bool loadTexture();
    float getDegree();

  private:
    glm::mat4 model;
    glm::mat4 origin_model;
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;
    bool tex_avail;

    std::vector<unsigned int> indice_size;
    unsigned int num_meshes;

    float degree;
    float angle;

    std::string stellar_Type;

    std::vector<Material> Materials;
    GLuint textures[INT_MAX];

    std::string name;
    float aphel;
    float perih;
    float tilt;
    float rotation;
    float orbit;
};

#endif /* OBJECT_H */
