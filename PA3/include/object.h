#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "graphics_headers.h"

class Object
{
  public:
    Object();
    ~Object();
    void UpdateFromOrigin(float rotate_speed, float translation_speed, float scale);
    void UpdateFromModel(glm::mat4 other_model, float rotate_speed, float translation_speed, float scale);
    void Render();

    glm::mat4 GetModel();
    glm::mat4 GetModelTranslation();

  private:
    glm::mat4 model;
    glm::mat4 model_translation;
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;

    float angle_rotate;
    float angle_translate;
};

#endif /* OBJECT_H */
