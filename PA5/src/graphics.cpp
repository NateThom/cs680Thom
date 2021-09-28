#include "graphics.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

Graphics::Graphics()
{

}

Graphics::Graphics(std::string v_path, std::string f_path, std::string o_path, std::string m_path)
{
  m_VERTEX_FILEPATH = v_path;
  m_FRAGMENT_FILEPATH = f_path;
  m_OBJECT_FILEPATH = o_path;
  m_MATERIAL_FILEPATH = m_path;
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
  if(!m_camera->Initialize(width, height))
  {
    printf("Camera Failed to Initialize\n");
    return false;
  }

  // Create the object
  m_cube = new Object(m_OBJECT_FILEPATH, m_MATERIAL_FILEPATH);
  m_moon = new Object();

  // Set up the shaders
  m_shader = new Shader(m_VERTEX_FILEPATH, m_FRAGMENT_FILEPATH);
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

  // Locate the model matrix in the shader
  m_modelMatrix = m_shader->GetUniformLocation("modelMatrix");
  if (m_modelMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_modelMatrix not found\n");
    return false;
  }

  m_modelMatrixMoon = m_shader->GetUniformLocation("modelMatrix");
  if (m_modelMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_modelMatrix not found\n");
    return false;
  }

  //enable depth testing
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  moon_scale = 1.0f;
  planet_scale = 1.0f;
  moon_orbit_speed = 1.0f;
  planet_orbit_speed = 1.0f;
  moon_spin_speed = 1.0f;
  planet_spin_speed = 1.0f;

  return true;
}

void Graphics::Update(unsigned int dt)
{
  // Update the object
  m_cube->UpdateFromOrigin(dt, planet_scale, planet_orbit_speed, planet_spin_speed);
  glm::mat4 planet_model = m_cube->GetOriginModel();
  m_moon->UpdateFromModel(dt, planet_model, moon_scale, moon_orbit_speed, moon_spin_speed);
}

void Graphics::Render(bool &running, unsigned int dt)
{
  //clear the screen
  glClearColor(0.0, 0.0, 0.2, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Start the correct program
  m_shader->Enable();

  // Send in the projection and view to the shader
  glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection())); 
  glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView())); 

  // Render the object
  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_cube->GetModel()));
  m_cube->Render();

  glUniformMatrix4fv(m_modelMatrixMoon, 1, GL_FALSE, glm::value_ptr(m_moon->GetModel()));
  m_moon->Render();

  // Start the Dear ImGui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL2_NewFrame();
  ImGui::NewFrame();

  ImGui::Begin("Control Menu");       // Create a window called "Hello, world!" and append into it.

  ImGui::Text("Keyboard Controls:");
  ImGui::Text("Left Arrow: Reverse Planet Orbit");
  ImGui::Text("Right Arrow: Reverse Planet Spin");
  ImGui::Text("Down Arrow: Pause/Restart Planet Orbit");
  ImGui::Text("Up Arrow: Pause/Restart Planet Spin"); 
  ImGui::Text("Right Mouse Click: Pause Planet");
  ImGui::NewLine();

  ImGui::Text("Planet Controls");
  if (ImGui::Button("Pause P Orbit"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
    planet_orbit_speed = 0.0f;
  ImGui::SameLine();
  if (ImGui::Button("Reset P Orbit"))
    planet_orbit_speed = 1.0f;
  ImGui::SameLine();
  if (ImGui::Button("Reverse P Orbit"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
    planet_orbit_speed = -1.0f;
  if (ImGui::Button("Pause P Spin"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
    planet_spin_speed = 0.0f;
  ImGui::SameLine();
  if (ImGui::Button("Reset P Spin"))
    planet_spin_speed = 1.0f;
  ImGui::SameLine();
  if (ImGui::Button("Reverse P Spin"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
    planet_spin_speed = -1.0f;
  ImGui::SliderFloat("Planet Scale", &planet_scale, 1.0f, 2.0f);
  ImGui::SliderFloat("Planet Orbit Speed", &planet_orbit_speed, -10.0f, 10.0f);
  ImGui::SliderFloat("Planet Rotation Speed", &planet_spin_speed, -10.0f, 10.0f);
  ImGui::Text("Orbital Direction: ");
  ImGui::SameLine();
  if (planet_orbit_speed > 0) {
    ImGui::Text("Clockwise");
  }
  else if (planet_orbit_speed < 0) {
    ImGui::Text("Counter Clockwise");
  }
  else {
    ImGui::Text("No Orbit");
  }
  ImGui::Text("Rotational Direction: ");
  ImGui::SameLine();
  if (planet_spin_speed > 0) {
    ImGui::Text("Counter Clockwise");
  }
  else if (planet_spin_speed < 0) {
    ImGui::Text("Clockwise");
  }
  else {
    ImGui::Text("No Rotation");
  }
  ImGui::Text("Orbital Speed: %f degrees/update call", m_cube->getOrbitSpeed(dt, planet_orbit_speed, M_PI/10000));
  ImGui::Text("Rotational Speed: %f degrees/update call", m_cube->getSpinSpeed(dt, planet_spin_speed, M_PI/5000));
  ImGui::NewLine();

  ImGui::Text("Moon Controls");
  if (ImGui::Button("Pause M Orbit"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
    moon_orbit_speed = 0.0f;
  ImGui::SameLine();
  if (ImGui::Button("Reset M Orbit"))
    moon_orbit_speed = 1.0f;
  ImGui::SameLine();
  if (ImGui::Button("Reverse M Orbit"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
    moon_orbit_speed = -1.0f;
  if (ImGui::Button("Pause M Spin"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
    moon_spin_speed = 0.0f;
  ImGui::SameLine();
  if (ImGui::Button("Reset M Spin"))
    moon_spin_speed = 1.0f;
  ImGui::SameLine();
  if (ImGui::Button("Reverse M Spin"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
    moon_spin_speed = -1.0f;
  ImGui::SliderFloat("Moon Scale", &moon_scale, 1.0f, 2.0f);
  ImGui::SliderFloat("Moon Orbit Speed", &moon_orbit_speed, -10.0f, 10.0f);
  ImGui::SliderFloat("Moon Rotation Speed", &moon_spin_speed, -10.0f, 10.0f);
  ImGui::Text("Orbital Direction: ");
  ImGui::SameLine();
  if (moon_orbit_speed > 0) {
    ImGui::Text("Clockwise");
  }
  else if (moon_orbit_speed < 0) {
    ImGui::Text("Counter Clockwise");
  }
  else {
    ImGui::Text("No Orbit");
  }
  ImGui::Text("Rotational Direction: ");
  ImGui::SameLine();
  if (moon_spin_speed > 0) {
    ImGui::Text("Counter Clockwise");
  }
  else if (moon_spin_speed < 0) {
    ImGui::Text("Clockwise");
  }
  else {
    ImGui::Text("No Rotation");
  }
  ImGui::Text("Orbital Speed: %f degrees/update call", m_cube->getOrbitSpeed(dt, moon_orbit_speed, M_PI/5000));
  ImGui::Text("Rotational Speed: %f degrees/update call", m_cube->getSpinSpeed(dt, moon_spin_speed, M_PI/2000));
  ImGui::NewLine();

  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

  ImGui::NewLine();
  if (ImGui::Button("Quit Application"))
    running = false;
  ImGui::End();

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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

void Graphics::setPlanetOrbitSpeed(float speed)
{
  planet_orbit_speed = speed;
}

void Graphics::setPlanetSpinSpeed(float speed)
{
  planet_spin_speed = speed;
}

float Graphics::getPlanetOrbitSpeed()
{
  return planet_orbit_speed;
}

float Graphics::getPlanetSpinSpeed()
{
  return planet_spin_speed;
}
