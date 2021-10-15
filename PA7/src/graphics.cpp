#include "graphics.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

// Graphics constructor. Takes boolean camera lock variable as input.
Graphics::Graphics(bool &Lock)
{
  m_config.LoadConfigFile("../config.txt");
  cameraIndex = 0;
  cameraLock = &Lock;
}

// Graphics constructor. Takes boolean camera lock variable and configuration filepath variables as input.
Graphics::Graphics(std::string config_filepath, bool &Lock)
{
  m_config.LoadConfigFile(config_filepath);
  cameraIndex = 0;
  cameraLock = &Lock;
}

Graphics::~Graphics()
{

}

bool Graphics::Initialize(int width, int height)
{
  // Used for the linux OS
  #if !defined(__APPLE__) && !defined(MACOSX)
    // cout << glewGetString(GLEW_VERSION) << endl;
    glewExperimental = GL_TRUE;

    auto status = glewInit();

    // This is here to grab the error that comes from glew init.
    // This error is an GL_INVALID_ENUM that has no effects on the performance
    glGetError();

    //Check for error
    if (status != GLEW_OK)
    {
      std::cerr << "GLEW Error: " << glewGetErrorString(status) << "\n";
      return false;
    }
  #endif

  // For OpenGL 3
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // Init Camera
  m_camera = new Camera();
  if(!m_camera->Initialize(m_config.GetWindowWidth(), m_config.GetWindowHeight()))
  {
    printf("Camera Failed to Initialize\n");
    return false;
  }

  // Set up the shaders
  m_shader = new Shader(m_config.GetVShaderFilePath(), m_config.GetFShaderFilePath());
  if(!m_shader->Initialize())
  {
    printf("Shader Failed to Initialize\n");
    return false;
  }

  // Add the vertex shader
  if(!m_shader->AddShader(GL_VERTEX_SHADER))
  {
    printf("Vertex Shader failed to Initialize\n");
    return false;
  }

  // Add the fragment shader
  if(!m_shader->AddShader(GL_FRAGMENT_SHADER))
  {
    printf("Fragment Shader failed to Initialize\n");
    return false;
  }

  // Connect the program
  if(!m_shader->Finalize())
  {
    printf("Program to Finalize\n");
    return false;
  }

  // Create/load the objects
  generateObjects();

  // Locate the projection matrix in the shader
  m_projectionMatrix = m_shader->GetUniformLocation("projectionMatrix");
  if (m_projectionMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_projectionMatrix not found\n");
    return false;
  }

  // Locate the view matrix in the shader
  m_viewMatrix = m_shader->GetUniformLocation("viewMatrix");
  if (m_viewMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_viewMatrix not found\n");
    return false;
  }

  //enable depth testing
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  dist_mod = 0.001f;
  speed_mod = 1.0f;

  return true;
}

void Graphics::Update(unsigned int dt)
{
  glm::mat4 planet_model;
  // Update the object
  for(int i = 0; i < m_config.GetStellarBodyCount(); i++){
    if(m_StellarBody[i]->getType() == "star"){
      m_StellarBody[i]->UpdateFromOrigin(dt, dist_mod, speed_mod);
    }
    else if(m_StellarBody[i]->getType() == "planet"){
      m_StellarBody[i]->UpdateFromOrigin(dt, dist_mod, speed_mod);
      planet_model = m_StellarBody[i]->GetOriginModel();
    }
    else if(m_StellarBody[i]->getType() == "moon"){
      m_StellarBody[i]->UpdateFromModel(dt, planet_model, dist_mod, speed_mod);
    }
  }
  if(*cameraLock == true){
    planetCamera(cameraIndex);
  }
}

void Graphics::Render(bool &running, unsigned int dt)
{
  //clear the screen
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Start the correct program
  m_shader->Enable();

  // Send in the projection and view to the shader
  glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection())); 
  glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView())); 

  // Render the objects
  for(int i = 0; i < m_config.GetStellarBodyCount(); i++){
    glUniformMatrix4fv(m_modelMatrix[i], 1, GL_FALSE, glm::value_ptr(m_StellarBody[i]->GetModel()));
    m_StellarBody[i]->Render(); 
  }

  RenderMenu(running, dt);

  // Get any errors from OpenGL
  auto error = glGetError();
  if ( error != GL_NO_ERROR )
  {
    string val = ErrorString( error );
    std::cout<< "Error initializing OpenGL! " << error << ", " << val << std::endl;
  }
}

std::string Graphics::ErrorString(GLenum error)
{
  if(error == GL_INVALID_ENUM)
  {
    return "GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument.";
  }

  else if(error == GL_INVALID_VALUE)
  {
    return "GL_INVALID_VALUE: A numeric argument is out of range.";
  }

  else if(error == GL_INVALID_OPERATION)
  {
    return "GL_INVALID_OPERATION: The specified operation is not allowed in the current state.";
  }

  else if(error == GL_INVALID_FRAMEBUFFER_OPERATION)
  {
    return "GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete.";
  }

  else if(error == GL_OUT_OF_MEMORY)
  {
    return "GL_OUT_OF_MEMORY: There is not enough memory left to execute the command.";
  }
  else
  {
    return "None";
  }
}

bool Graphics::generateObjects()
{
  StellarBody stellar;

  for(int i = 0; i < m_config.GetStellarBodyCount(); i++){
    stellar = m_config.GetStellarList()[i];
    m_body = new Object(stellar.name, 
                        stellar.path_to_obj, 
                        stellar.path_to_mtl, 
                        stellar.type,
                        stellar.aphelion,
                        stellar.periphelion,
                        stellar.axial_tilt,
                        stellar.rotational_period,
                        stellar.orbital_speed);
    m_StellarBody.push_back(m_body);
  }

  // Locate the model matrix in the shader
  for(int i = 0; i < m_config.GetStellarBodyCount(); i++){
    m_modelMatrix[i] = m_shader->GetUniformLocation("modelMatrix");
    if (m_modelMatrix[i] == INVALID_UNIFORM_LOCATION) 
    {
      printf("m_modelMatrix not found\n");
      return false;
    }
  }

  return true;
}

void Graphics::RenderMenu(bool &running, unsigned int dt)
{
  // Start the Dear ImGui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL2_NewFrame();
  ImGui::NewFrame();

  ImGui::Begin("Control Menu");

  ImGui::Text("Solar System Simulation:");
  ImGui::NewLine();

  ImGui::Text("Welcome to a scaled simulation of our solar system! A lot of effort has been");
  ImGui::Text("spent on accurately replicating the interactions of the celestial bodies.");
  ImGui::Text("Particularly, we would like to direct your attention to: ");
  ImGui::Text("The presence of the sun");
  ImGui::Text("The presence of all planets");
  ImGui::Text("The presence of Pluto");
  ImGui::Text("The presence of orbiting satellites including:");
  ImGui::Text("     Earth's Moon");
  ImGui::Text("     Jupiter's moons (Io, Europa, Ganymede, and Callisto");
  ImGui::Text("     Saturn's moon (Titan");
  ImGui::Text("     Pluto's moon (Charon");
  ImGui::Text("The accurate size of all stellar bodies");
  ImGui::Text("The accurate orbit paths of all stellar bodies");
  ImGui::Text("The accurate orbit speeds of all stellar bodies");
  ImGui::Text("The accurate rotation speeds of all stellar bodies");
  ImGui::Text("The accurate pitch of the planets");
  ImGui::NewLine();

  ImGui::Text("Scroll down in this menu to see a list of options and controls that will");
  ImGui::Text("enable your exploration!");
  ImGui::NewLine();

  ImGui::Text("Interactive Menu:");
  ImGui::NewLine();

  ImGui::Text("Distance Scale allows changing between an actual view and a scaled view.");
  ImGui::SliderFloat("Distance Scale", &dist_mod, 0.001f, 1.0f);
  ImGui::Text("Speed Scale allows changing the speed of the simulation.");
  ImGui::SliderFloat("Speed Scale", &speed_mod, 1.0f, 1000.0f);
  ImGui::NewLine();

  ImGui::Text("Keyboard Controls:");
  ImGui::NewLine();

  ImGui::Text("Left arrow key slows camera down");
  ImGui::Text("Right arrow key speeds camera up");
  ImGui::Text("Down arrow key looks down");
  ImGui::Text("Up arrow key looks up");
  ImGui::Text("W zooms in");
  ImGui::Text("S zooms out");
  ImGui::Text("A moves left");
  ImGui::Text("D moves right");
  ImGui::Text("Spacebar resets the camera view to default.");
  ImGui::Text("1-9 center the view on a planet. The camera will always be looking towards the sun.");
  ImGui::NewLine();

  ImGui::Text("'Jump To' buttons:");

  if (ImGui::Button("Mercury")) {
    *cameraLock = true;
    setCameraLock(1);
  }
  if (ImGui::Button("Venus")) {
    *cameraLock = true;
    setCameraLock(2);
  }
  if (ImGui::Button("Earth")) {
    *cameraLock = true;
    setCameraLock(3);
  }
  if (ImGui::Button("Mars")) {
    *cameraLock = true;
    setCameraLock(5);
  }
  if (ImGui::Button("Jupiter")) {
    *cameraLock = true;
    setCameraLock(6);
  }
  if (ImGui::Button("Saturn")) {
    *cameraLock = true;
    setCameraLock(11);
  }
  if (ImGui::Button("Uranus")) {
    *cameraLock = true;
    setCameraLock(13);
  }
  if (ImGui::Button("Neptune")) {
    *cameraLock = true;
    setCameraLock(14);
  }
  if (ImGui::Button("Pluto")) {
    *cameraLock = true;
    setCameraLock(15);
  }
  if (ImGui::Button("Reset Camera")) {
    ResetCamera();
    *cameraLock = false;
  }
  ImGui::NewLine();

  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
  ImGui::NewLine();

//  ImGui::NewLine();
  if (ImGui::Button("Quit Application"))
    running = false;
  ImGui::End();

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool Graphics::ZoomIn()
{
  return m_camera->ZoomIn();
}

bool Graphics::ZoomOut()
{
  return m_camera->ZoomOut();
}

bool Graphics::LookUp()
{
  return m_camera->LookUp();
}

bool Graphics::LookDown()
{
  return m_camera->LookDown();
}

void Graphics::CameraSpeedDown()
{
  m_camera->SpeedDown();
}

void Graphics::CameraSpeedUp()
{
  m_camera->SpeedUp();
}

bool Graphics::MoveUp()
{
  return m_camera->MoveUp();
}

bool Graphics::MoveDown()
{
  return m_camera->MoveDown();
}

bool Graphics::MoveLeft()
{
  return m_camera->MoveLeft();
}

bool Graphics::MoveRight()
{
  return m_camera->MoveRight();
}

bool Graphics::ResetCamera()
{
  return m_camera->ResetCamera();
}

void Graphics::setCameraLock(int index)
{
  cameraIndex = index;
}

void Graphics::planetCamera(int index)
{
  m_camera->lockCamera(cameraIndex, dist_mod, speed_mod, dt, m_StellarBody[index]->getDegree());
}

void Graphics::setDT(unsigned int dt_value)
{
  dt = dt_value;
}