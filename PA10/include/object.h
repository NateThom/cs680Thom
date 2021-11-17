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
    Object(std::string o_path, std::string f_path);
    ~Object();
    void Update(glm::mat4 position);
    void Render();

    glm::mat4 GetModel();
    glm::mat4 GetOriginModel();
    Material GetMaterial();

    bool loadOBJ(std::vector<Vertex> &out_vertices, std::vector<unsigned int> &out_indices, std::string object);
    bool loadMaterialFile(std::string material);
    std::vector<std::string> split(std::string str);
    bool loadTexture();

  private:
    glm::mat4 model;
    glm::mat4 origin_model;
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;
    GLuint textures[100];

    std::vector<unsigned int> indice_size;
    unsigned int num_meshes;

    float degree;
    float angle;

    std::vector<Material> Materials;
};

#endif /* OBJECT_H */
