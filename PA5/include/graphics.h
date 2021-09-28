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
    Graphics(std::string v_path, std::string f_path, std::string o_path, std::string m_path);
    ~Graphics();
    bool Initialize(int width, int height);
    void Update(unsigned int dt);
    void Render(bool &running, unsigned int dt);

    void setPlanetOrbitSpeed(float speed);
    void setPlanetSpinSpeed(float speed);

    float getPlanetOrbitSpeed();
    float getPlanetSpinSpeed();

  private:
    std::string ErrorString(GLenum error);
    std::string m_VERTEX_FILEPATH;
    std::string m_FRAGMENT_FILEPATH;
    std::string m_OBJECT_FILEPATH;
    std::string m_MATERIAL_FILEPATH;

    Camera *m_camera;
    Shader *m_shader;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;
    GLint m_modelMatrixMoon;

    Object *m_cube;
    Object *m_moon;

    float moon_scale;
    float planet_scale;

    float moon_orbit_speed;
    float moon_spin_speed;

    float planet_orbit_speed;
    float planet_spin_speed;
};

#endif /* GRAPHICS_H */
