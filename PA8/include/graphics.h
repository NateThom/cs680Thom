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
    // Graphics(std::string v_path, std::string f_path, std::string o_path, std::string m_path);
    ~Graphics();
    bool Initialize(int width, int height);
    void Update(unsigned int dt);
    void Render(bool &running, unsigned int dt);
    void RenderMenu(bool &running, unsigned int dt);
    bool generateObjects();
    void InitBullet();

    void cubeUp();
    void cubeDown();
    void cubeLeft();
    void cubeRight();

    void speedUp();
    void speedDown();

  private:
    std::string ErrorString(GLenum error);
    std::string m_VERTEX_FILEPATH;
    std::string m_FRAGMENT_FILEPATH;

    float speed;

    Camera *m_camera;
    Shader *m_shader;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix[100];

    std::vector<Object*> m_models;
    Object *m_body;
    int model_count;

    glm::mat4 planePos;
    glm::mat4 spherePos;
    glm::mat4 cylinderPos;
    glm::mat4 cubePos;
    glm::mat4 positions[4];

    btBroadphaseInterface *broadphase;
    btDefaultCollisionConfiguration *collisionConfiguration;
    btCollisionDispatcher *dispatcher;
    btSequentialImpulseConstraintSolver *solver;
    btDiscreteDynamicsWorld *dynamicsWorld;

    btCollisionShape *plane;
    btCollisionShape *sphere;
    btCollisionShape *cylinder;
    btCollisionShape *wall;
    btCollisionShape *cube;

    btDefaultMotionState *sphereMotionState;
    btDefaultMotionState *planeMotionState;
    btDefaultMotionState *cylinderMotionState;
    btDefaultMotionState *wallMotionState;
    btDefaultMotionState *cubeMotionState;

    std::vector<btRigidBody*> rigidBody;
};

#endif /* GRAPHICS_H */
