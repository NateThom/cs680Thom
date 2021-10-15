#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
using namespace std;

#include "graphics_headers.h"
#include "camera.h"
#include "shader.h"
#include "object.h"
#include "config.h"

class Graphics
{
  public:
    Graphics(bool &Lock);
    Graphics(std::string config_file, bool &Lock);
    ~Graphics();
    bool Initialize(int width, int height);
    void Update(unsigned int dt);
    void Render(bool &running, unsigned int dt);
    void RenderMenu(bool &running, unsigned int dt);
    bool generateObjects();

    // for updating the view
    bool ZoomIn();
    bool ZoomOut();
    bool LookUp();
    bool LookDown();

    void CameraSpeedDown();
    void CameraSpeedUp();

    bool MoveUp();
    bool MoveDown();
    bool MoveLeft();
    bool MoveRight();
    bool ResetCamera();

    void setCameraLock(int index);
    void planetCamera(int index);

    void setDT(unsigned int dt_value);

  private:
    std::string ErrorString(GLenum error);

    Camera *m_camera;
    Shader *m_shader;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix[100];

    std::vector<Object*> m_StellarBody;
    Object *m_body;

    float dist_mod;
    float speed_mod;

    int cameraIndex;
    bool *cameraLock;
    unsigned int dt;

    Configuration m_config;
};

#endif /* GRAPHICS_H */
