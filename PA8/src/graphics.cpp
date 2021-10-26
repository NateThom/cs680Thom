#include "graphics.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

Graphics::Graphics()
{
  model_count = 4;
  m_VERTEX_FILEPATH = "shaders/v_shader.glsl";
  m_FRAGMENT_FILEPATH = "shaders/f_shader.glsl";
  speed = 0.1;
}

// Graphics::Graphics(std::string v_path, std::string f_path, std::string o_path, std::string m_path)
// {
//   m_VERTEX_FILEPATH = v_path;
//   m_FRAGMENT_FILEPATH = f_path;
//   m_OBJECT_FILEPATH = o_path;
//   m_MATERIAL_FILEPATH = m_path;
// }

Graphics::~Graphics()
{
  broadphase = NULL;
  collisionConfiguration = NULL;
  dispatcher = NULL;
  solver = NULL;
  dynamicsWorld = NULL;

  plane = NULL;
  sphere = NULL;
  sphereMotionState = NULL;
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

  spherePos = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 8.0f, 0.0f));
  planePos = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
  cylinderPos = glm::translate(glm::mat4(1.0f), glm::vec3(0.75f, 1.5f, 10.0f));
  cubePos = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.5f, -5));
  positions[0] = planePos;
  positions[1] = spherePos;
  positions[2] = cylinderPos;
  positions[3] = cubePos;

  //initialize bullet physics engine
  InitBullet();

  return true;
}

void Graphics::Update(unsigned int dt)
{
  btTransform trans;
  btTransform cubeTrans;
  btScalar m[16];
  btScalar c[16];
  dynamicsWorld->stepSimulation(dt, 3);

  rigidBody[1]->getMotionState()->getWorldTransform(trans);
  trans.getOpenGLMatrix(m);

  positions[1] = glm::make_mat4(m);

  rigidBody[7]->getMotionState()->getWorldTransform(cubeTrans);
  cubeTrans.getOpenGLMatrix(c);

  positions[3] = glm::make_mat4(c);

  m_models[0]->Update(planePos);
  m_models[1]->Update(positions[1]);
  m_models[2]->Update(cylinderPos);
  m_models[3]->Update(positions[3]);

  // for(int i = 0; i < model_count; i++){
  //   m_models[i]->Update(model);
  // }
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
  for(int i = 0; i < model_count; i++){
    glUniformMatrix4fv(m_modelMatrix[i], 1, GL_FALSE, glm::value_ptr(m_models[i]->GetModel()));
    m_models[i]->Render(); 
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
  std::string object_path[] = {"objects/board.obj", "objects/sphere.obj", "objects/cylinder.obj", "objects/cube.obj"};
  std::string material_path[] = {"objects/board.mtl", "objects/sphere.mtl", "objects/cylinder.mtl", "objects/cube.mtl"};

  for(int i = 0; i < model_count; i++){
    m_body = new Object(object_path[i], material_path[i]);
    m_models.push_back(m_body);
  }

  // Locate the model matrix in the shader
  for(int i = 0; i < model_count; i++){
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

  ImGui::Begin("Menu");
  ImGui::Text("Controls:");
  ImGui::Text("Arrow Keys: Cube Direction");
  ImGui::Text("W: Increase speed");
  ImGui::Text("S: Decrease speed");

  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
  ImGui::NewLine();
  ImGui::Text("Current Speed %.1f", speed);

  if (ImGui::Button("Quit Application"))
    running = false;
  ImGui::End();

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Graphics::InitBullet()
{
  // Generate and initialize physics engine pointers
  broadphase = new btDbvtBroadphase();
  collisionConfiguration = new btDefaultCollisionConfiguration();
  dispatcher = new btCollisionDispatcher(collisionConfiguration);
  solver = new btSequentialImpulseConstraintSolver;
  dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
  dynamicsWorld->setGravity(btVector3(0.0, -9.81, 0.0));

  plane = new btStaticPlaneShape(btVector3(0.0f, 1.0f, 0.0f), 0.0f);
  sphere = new btSphereShape(1.0f);
  cylinder = new btCylinderShape(btVector3(1.5f, 1.5f, 1.5f));
  wall = new btBoxShape(btVector3(20, 2.5, 0));
  cube = new btBoxShape(btVector3(1, 1, 1));

  // Create rigid body information for floor plane
  planeMotionState = NULL;
  planeMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
  btVector3 planeInertia(0, 0, 0);

  btRigidBody::btRigidBodyConstructionInfo planeRigidBodyCI(0, planeMotionState, plane, btVector3(0, 0, 0));
  btRigidBody* planeRigidBody = new btRigidBody(planeRigidBodyCI);
  planeRigidBody->setRestitution(1);
  rigidBody.push_back(planeRigidBody);
  dynamicsWorld->addRigidBody(rigidBody[0]);

  // Create rigid body information for sphere
  sphereMotionState = NULL;
  sphereMotionState = new btDefaultMotionState(btTransform(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f), btVector3(0.0f, 8.0f, 0.0f)));
  btScalar mass(1);
  btVector3 inertia(1.0f, 1.0f, 1.0f);
  sphere->calculateLocalInertia(mass, inertia);

  btRigidBody::btRigidBodyConstructionInfo sphereRigidBodyCI(mass, sphereMotionState, sphere, inertia);
  btRigidBody* sphereRigidBody = new btRigidBody(sphereRigidBodyCI);
  sphereRigidBody->setRestitution(0.5);
  rigidBody.push_back(sphereRigidBody);
  dynamicsWorld->addRigidBody(rigidBody[1]);

  // Create rigid body information for static cylinder
  cylinderMotionState = NULL;
  cylinderMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0.75, 1.5, 10)));
  btVector3 cylinderIntertia(0, 0, 0);

  btRigidBody::btRigidBodyConstructionInfo cylinderRigidBodyCI(0, cylinderMotionState, cylinder, btVector3(0, 0, 0));
  btRigidBody* cylinderRigidBody = new btRigidBody(cylinderRigidBodyCI);
  cylinderRigidBody->setRestitution(1);
  rigidBody.push_back(cylinderRigidBody);
  dynamicsWorld->addRigidBody(rigidBody[2]);

  // Create rigid body information for static walls
  wallMotionState = NULL;
  wallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 2.5, 20)));
  btVector3 wallInertia(0, 0, 0);

  btRigidBody::btRigidBodyConstructionInfo wallRigidBodyCI(0, wallMotionState, wall, btVector3(0, 0, 0));
  btRigidBody* wallRigidBody = new btRigidBody(wallRigidBodyCI);
  wallRigidBody->setRestitution(1);
  rigidBody.push_back(wallRigidBody);
  dynamicsWorld->addRigidBody(rigidBody[3]);

  wallMotionState = NULL;
  wallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 1, 0, 1), btVector3(-20, 2.5, 0)));

  wallRigidBody = NULL;
  btRigidBody::btRigidBodyConstructionInfo wall2RigidBodyCI(0, wallMotionState, wall, btVector3(0, 0, 0));
  wallRigidBody = new btRigidBody(wall2RigidBodyCI);
  wallRigidBody->setRestitution(1);
  rigidBody.push_back(wallRigidBody);
  dynamicsWorld->addRigidBody(rigidBody[4]);

  wallMotionState = NULL;
  wallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 2.5, -20)));

  wallRigidBody = NULL;
  btRigidBody::btRigidBodyConstructionInfo wall3RigidBodyCI(0, wallMotionState, wall, btVector3(0, 0, 0));
  wallRigidBody = new btRigidBody(wall3RigidBodyCI);
  wallRigidBody->setRestitution(1);
  rigidBody.push_back(wallRigidBody);
  dynamicsWorld->addRigidBody(rigidBody[5]);

  wallMotionState = NULL;
  wallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 1, 0, 1), btVector3(20, 2.5, 0)));

  wallRigidBody = NULL;
  btRigidBody::btRigidBodyConstructionInfo wall4RigidBodyCI(0, wallMotionState, wall, btVector3(0, 0, 0));
  wallRigidBody = new btRigidBody(wall4RigidBodyCI);
  wallRigidBody->setRestitution(1);
  rigidBody.push_back(wallRigidBody);
  dynamicsWorld->addRigidBody(rigidBody[6]);

  // Create kinematic cube object
  cubeMotionState = NULL;
  cubeMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0.5, -5)));
  btVector3 cubeInertia(0, 0, 0);

  btRigidBody::btRigidBodyConstructionInfo cubeRigidBodyCI(0, cubeMotionState, cube, btVector3(0, 0, 0));
  btRigidBody* cubeRigidBody = new btRigidBody(cubeRigidBodyCI);
  cubeRigidBody->setRestitution(1);
  cubeRigidBody->setCollisionFlags(cubeRigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
  cubeRigidBody->setActivationState(DISABLE_DEACTIVATION);
  rigidBody.push_back(cubeRigidBody);
  dynamicsWorld->addRigidBody(rigidBody[7]);
}

void Graphics::cubeUp()
{
  btTransform trans;
  rigidBody[7]->getMotionState()->getWorldTransform(trans);
  btVector3 origin = trans.getOrigin();
  origin += btVector3(0.0, 0.0, speed);
  trans.setOrigin(origin);
  rigidBody[7]->getMotionState()->setWorldTransform(trans);
}

void Graphics::cubeDown()
{
  btTransform trans;
  rigidBody[7]->getMotionState()->getWorldTransform(trans);
  btVector3 origin = trans.getOrigin();
  origin += btVector3(0.0, 0.0, -speed);
  trans.setOrigin(origin);
  rigidBody[7]->getMotionState()->setWorldTransform(trans);
}

void Graphics::cubeLeft()
{
  btTransform trans;
  rigidBody[7]->getMotionState()->getWorldTransform(trans);
  btVector3 origin = trans.getOrigin();
  origin += btVector3(speed, 0.0, 0.0);
  trans.setOrigin(origin);
  rigidBody[7]->getMotionState()->setWorldTransform(trans);
}

void Graphics::cubeRight()
{
  btTransform trans;
  rigidBody[7]->getMotionState()->getWorldTransform(trans);
  btVector3 origin = trans.getOrigin();
  origin += btVector3(-speed, 0.0, 0.0);
  trans.setOrigin(origin);
  rigidBody[7]->getMotionState()->setWorldTransform(trans);
}

void Graphics::speedUp()
{
  if (speed >= 0.5){
    speed = 0.5;
  }
  else {
    speed += 0.1;
  }
}

void Graphics::speedDown()
{
  if (speed <= 0){
    speed = 0;
  }
  else {
    speed -= 0.1;
  }
}