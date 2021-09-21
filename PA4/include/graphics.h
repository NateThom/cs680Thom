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
    void Update(float planet_rotation_speed, float planet_translation_speed, float planet_scale,
               float moon_rotation_speed, float moon_translation_speed, float moon_scale);
    void Render(float red, float green, float blue, float alpha);

  private:
    std::string ErrorString(GLenum error);

    Camera *m_camera;
    Shader *m_shader;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_planetModelMatrix;
    GLint m_moonModelMatrix;

    Object *m_planet;
    Object *m_moon;
};

#endif /* GRAPHICS_H */
