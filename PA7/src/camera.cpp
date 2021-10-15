#include "camera.h"

Camera::Camera()
{

}

Camera::~Camera()
{

}

bool Camera::Initialize(int w, int h)
{
  // Initialize vectors that will be changed during runtime
  eyePosition = glm::vec3(0.0, 800, -5000.0);
  focusPoint = glm::vec3(0.0, 0.0, 0.0);
  upVector = glm::vec3(0.0, 1.0, 0.0);

  speed = 50.0;
  vertiRadius = sqrt(pow((focusPoint.z - eyePosition.z), 2) + pow((focusPoint.y - eyePosition.y), 2));
  horizRadius = sqrt(pow((focusPoint.z - eyePosition.z), 2) + pow((focusPoint.x - eyePosition.x), 2));

  //--Init the view and projection matrices
  //  if you will be having a moving camera the view matrix will need to more dynamic
  //  ...Like you should update it before you render more dynamic 
  //  for this project having them static will be fine
  view = glm::lookAt( eyePosition, //Eye Position
                      focusPoint, //Focus point
                      upVector); //Positive Y is up

  projection = glm::perspective( 45.0f, //the FoV typically 90 degrees is good which is what this is set to
                                 float(w)/float(h), //Aspect Ratio, so Circles stay Circular
                                 1.0f, //Distance to the near plane, normally a small value like this
                                 100000.0f); //Distance to the far plane, 

  m_config.LoadConfigFile("../config.txt");
  degree = 0.0f;

  return true;
}

glm::mat4 Camera::GetProjection()
{
  return projection;
}

glm::mat4 Camera::GetView()
{
  return view;
}

/*
 * Moves the eye position in
 */
bool Camera::ZoomIn()
{
  // check if the user is too close to keep zooming in
  if( eyePosition.z == -1.0 )
  {
    return false;
  }

  float vertiIncrement = (focusPoint.y - eyePosition.y) / (focusPoint.z - eyePosition.z);

  eyePosition += glm::vec3(0.0, vertiIncrement, speed);
  focusPoint += glm::vec3(0.0, vertiIncrement, speed);

  view = glm::lookAt( eyePosition, //Eye Position
                      focusPoint, //Focus point
                      upVector); //Positive Y is up

  return true;
}

/*
 * Moves the eye position out
 */
bool Camera::ZoomOut()
{
  float vertiIncrement = (focusPoint.y - eyePosition.y) / (focusPoint.z - eyePosition.z);

  eyePosition += glm::vec3(0.0, -vertiIncrement, -speed);
  focusPoint += glm::vec3(0.0, -vertiIncrement, -speed);

  view = glm::lookAt( eyePosition, //Eye Position
                      focusPoint, //Focus point
                      upVector); //Positive Y is up

  return true;
}

/*
 * Adjusts the focus point up
 */
bool Camera::LookUp()
{
  focusPoint += glm::vec3(0.0, speed, 0);
  float z = sqrt(pow(vertiRadius, 2) - pow((focusPoint.y - eyePosition.y), 2));
  focusPoint += glm::vec3(0.0, 0.0, z - (focusPoint.z - eyePosition.z));

  view = glm::lookAt( eyePosition, //Eye Position
                      focusPoint, //Focus point
                      upVector); //Positive Y is up

  return true;
}

/*
 * Adjusts the focus point down
 */
bool Camera::LookDown()
{
  focusPoint += glm::vec3(0.0, -speed, 0.0);
  float z = sqrt(pow(vertiRadius, 2) - pow((focusPoint.y - eyePosition.y), 2));
  focusPoint += glm::vec3(0.0, 0.0, z - (focusPoint.z - eyePosition.z));

  view = glm::lookAt( eyePosition, //Eye Position
                      focusPoint, //Focus point
                      upVector); //Positive Y is up

  return true;
}

bool Camera::MoveLeft()
{
  eyePosition += glm::vec3(speed, 0.0, 0.0);
  focusPoint += glm::vec3(speed, 0.0, 0.0);

  view = glm::lookAt( eyePosition, //Eye Position
                      focusPoint, //Focus point
                      upVector); //Positive Y is up

  return true;
}

/*
 * Moves the eye position out
 */
bool Camera::MoveRight()
{
  eyePosition += glm::vec3(-speed, 0.0, 0.0);
  focusPoint += glm::vec3(-speed, 0.0, 0.0);

  view = glm::lookAt( eyePosition, //Eye Position
                      focusPoint, //Focus point
                      upVector); //Positive Y is up

  return true;
}

bool Camera::MoveUp()
{
  eyePosition += glm::vec3(0.0, speed, 0.0);
  focusPoint += glm::vec3(0.0, speed, 0.0);

  view = glm::lookAt( eyePosition, //Eye Position
                      focusPoint, //Focus point
                      upVector); //Positive Y is up

  return true;
}

bool Camera::MoveDown()
{
  eyePosition += glm::vec3(0.0, -speed, 0.0);
  focusPoint += glm::vec3(0.0, -speed, 0.0);

  view = glm::lookAt( eyePosition, //Eye Position
                      focusPoint, //Focus point
                      upVector); //Positive Y is up

  return true;
}

bool Camera::ResetCamera()
{
  eyePosition = glm::vec3(0.0, 800, -5000.0);
  focusPoint = glm::vec3(0.0, 0.0, 0.0);
  upVector = glm::vec3(0.0, 1.0, 0.0);

  view = glm::lookAt( eyePosition, //Eye Position
                      focusPoint, //Focus point
                      upVector); //Positive Y is up

  return true;
}

void Camera::lockCamera(int index, float dist_mod, float speed_mod, unsigned int dt, float deg)
{
  degree = deg;
  std::vector<StellarBody> stellarObjects = m_config.GetStellarList();

  glm::mat4 planet = glm::translate(glm::mat4(1.0f), glm::vec3(cos(degree)*((stellarObjects[index].aphelion*dist_mod)+offset[index-1]), 0.0f, sin(degree)*((stellarObjects[index].periphelion*dist_mod)+offset[index-1])));

  focusPoint = glm::vec3(0.0, 0.0, 0.0);
  eyePosition = glm::vec3(planet[3][0], planet[3][1], planet[3][2]);
  upVector = glm::vec3(0.0, 1.0, 0.0);

  view = glm::lookAt( eyePosition,
                      focusPoint,
                      upVector);
}

void Camera::SpeedUp()
{
  speed += 20;
}

void Camera::SpeedDown()
{
  if (speed >= 20){
    speed -= 20;
  }
  else{
    speed = 0;
  }
}