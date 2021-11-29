#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
using namespace std;

#include "graphics_headers.h"
#include "camera.h"
#include "shader.h"
#include "object.h"

struct bulletObject{
    int id;
    int score;
    int lives;
    btRigidBody* body;
    bulletObject(btRigidBody* b,int i, int s, int l) : body(b),id(i),score(s),lives(l) {}
};

class Graphics
{
  public:
    Graphics();
    // Graphics(std::string v_path, std::string f_path, std::string o_path, std::string m_path);
    ~Graphics();
    bool Initialize(int width, int height);
    void Update(unsigned int dt);
    void Render(bool &running, unsigned int dt);
    void RenderMenu(bool &running, unsigned int dt, int currentScore, int currentLives);
    bool generateObjects();
    void InitBullet();

    void createPlanes();
    void createPlunger();
    void createBall();
    void createFlippers();
    void createWalls();
    void createBumpers();

    bool InitPerVertShader();
    bool InitPerFragShader();
    void renderPerVertShader();
    void renderPerFragShader();
    bool createPerVertObject(int index);
    bool createPerFragObject(int index);

    bool static callbackFunc(btManifoldPoint& cp,const btCollisionObjectWrapper* obj1,int id1,int index1,
                      const btCollisionObjectWrapper* obj2,int id2,int index2);

    void plungerUp();
    void plungerDown();
    void lFlipperUp();
    void rFlipperUp();

    void cameraUp();
    void cameraDown();

  private:
    std::string ErrorString(GLenum error);
    std::string m_VERTEX_FILEPATH;
    std::string m_FRAGMENT_FILEPATH;
    std::string f_VERTEX_FILEPATH;
    std::string f_FRAGMENT_FILEPATH;

    int starting_lives;
    int current_lives;

    Camera *m_camera;
    Shader *m_vertShader;
    Shader *m_fragShader;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix[100];

    GLint m_globalAmbient;
    GLint m_positionalLight[2][4];
    GLint m_material[5][4];
    GLint m_normalMatrix[100];

    GLint f_projectionMatrix;
    GLint f_viewMatrix;
    GLint f_modelMatrix[100];

    GLint f_globalAmbient;
    GLint f_positionalLight[2][4];
    GLint f_material[5][4];
    GLint f_normalMatrix[100];

    GLint m_cutoffAngle;
    GLint f_cutoffAngle;
    GLint m_lightCount;
    GLint f_lightCount;

    std::vector<Object*> m_models;
    Object *m_body;
    int model_count;

    glm::mat4 planePos;
    glm::mat4 spherePos;
    glm::mat4 cylinderPos;
    glm::mat4 plungerPos;
    glm::mat4 flipperRPos;
    glm::mat4 flipperLPos;
    glm::mat4 plane2Pos;
    glm::mat4 cylinder2Pos;
    glm::mat4 cylinder3Pos;
    glm::mat4 bustPos;
    glm::mat4 positions[10];

    btBroadphaseInterface *broadphase;
    btDefaultCollisionConfiguration *collisionConfiguration;
    btCollisionDispatcher *dispatcher;
    btSequentialImpulseConstraintSolver *solver;
    btDiscreteDynamicsWorld *dynamicsWorld;

    btCollisionShape *plane;
    btCollisionShape *plane2;
    btCollisionShape *sphere;
    btCollisionShape *cylinder;
    btCollisionShape *wall;
    btCollisionShape *plunger;
    btCollisionShape *flipperR;
    btCollisionShape *flipperL;

    btDefaultMotionState *sphereMotionState;
    btDefaultMotionState *planeMotionState;
    btDefaultMotionState *plane2MotionState;
    btDefaultMotionState *cylinderMotionState;
    btDefaultMotionState *wallMotionState;
    btDefaultMotionState *plungerMotionState;
    btDefaultMotionState *flipperRMotionState;
    btDefaultMotionState *flipperLMotionState;

    std::vector<bulletObject*> rigidBody;

    Light posLight;

    float ambient;
    float lightAmbient;
    float lightSpecular;
    float lightDiffuse;
    int spotAngle;
    int lightCount;
    float lightIntensity;
    float offset[5] = {0};

    float plungerMod;
    bool plungerCharge;
    bool flipperLMove;
    bool flipperRMove;

    bool shader_switch;

    btVector3 plungerOrigPos;
    btQuaternion flipperLOrigPos;
    btQuaternion flipperROrigPos;

    string language;
};

#endif /* GRAPHICS_H */
