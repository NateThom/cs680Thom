#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
using namespace std;

#include "graphics_headers.h"
#include "camera.h"
#include "shader.h"
#include "object.h"

class Graphics
{
  public:
    Graphics();
    ~Graphics();
    bool Initialize(int width, int height, const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);
    void Update(unsigned int dt, bool rotate_flag, bool rotate_reverse_flag, bool translate_flag,
                bool translate_reverse_flag);
    void Render(float red, float green, float blue, float alpha);

  private:
    std::string ErrorString(GLenum error);

    Camera *m_camera;
    Shader *m_shader;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;

    Object *m_planet;
    Object *m_moon;
};

#endif /* GRAPHICS_H */
