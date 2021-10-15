#ifndef CAMERA_H
#define CAMERA_H

#include "graphics_headers.h"
#include "config.h"

class Camera
{
  public:
    Camera();
    ~Camera();
    bool Initialize(int w, int h);
    glm::mat4 GetProjection();
    glm::mat4 GetView();

    bool ZoomIn();
    bool ZoomOut();
    bool LookUp();
    bool LookDown();
    bool MoveLeft();
    bool MoveRight();
    bool MoveUp();
    bool MoveDown();
    bool ResetCamera();
    void lockCamera(int index, float dist_mod, float speed_mod, unsigned int dt, float deg);

    void SpeedUp();
    void SpeedDown();
  
  private:
    glm::vec3 eyePosition;
    glm::vec3 focusPoint;
    glm::vec3 upVector;

    float speed;
    float vertiRadius;
    float horizRadius;
    float degree;

    glm::mat4 projection;
    glm::mat4 view;

    int offset[50] = {8, 30, 40, 0, 10, 500, 0, 0, 0, 0, 500, 0, 150, 150, 20};

    Configuration m_config;
};

#endif /* CAMERA_H */
