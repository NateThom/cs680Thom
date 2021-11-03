#ifndef GRAPHICS_HEADERS_H
#define GRAPHICS_HEADERS_H

#include <iostream>

#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
#define GLM_ENABLE_EXPERIMENTAL 

#if defined(__APPLE__) || defined(MACOSX)
  #include <OpenGL/gl3.h>
  #include <OpenGL/GLU.h>
#else //linux as default
  #include <GL/glew.h>
  //#include <GL/glu.h>
#endif

// GLM for matricies
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <math.h>

#define INVALID_UNIFORM_LOCATION 0x7fffffff

struct Vertex
{
  glm::vec3 vertex;
  glm::vec3 color;
  glm::vec2 texture;

  Vertex(glm::vec3 v, glm::vec3 c, glm::vec2 t): vertex(v), color(c), texture(t) {}
};

struct Material
{
  std::string newmtl;
  glm::vec3 Ka;
  glm::vec3 Kd;
  glm::vec3 Ks;
  float Ns;
  float Ni;
  float d;
  int illum;
  std::string map_Kd;

  Material()
  {
    newmtl = "";
    Ka = glm::vec3(0, 0, 0);
    Kd = glm::vec3(0, 0, 0);
    Ks = glm::vec3(0, 0, 0);
    Ns = 0.0;
    Ni = 0.0;
    d = 0.0;
    illum = 0;
    map_Kd = "";
  }
};

#endif /* GRAPHICS_HEADERS_H */