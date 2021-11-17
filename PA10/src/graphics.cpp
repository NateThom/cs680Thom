#include "graphics.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

Graphics::Graphics()
{
  model_count = 9;
  shader_switch = false;
  m_VERTEX_FILEPATH = "shaders/v_shader_per_vert.glsl";
  m_FRAGMENT_FILEPATH = "shaders/f_shader_per_vert.glsl";
  f_VERTEX_FILEPATH = "shaders/v_shader_per_frag.glsl";
  f_FRAGMENT_FILEPATH = "shaders/f_shader_per_frag.glsl";

  starting_lives = 4;
  current_lives = starting_lives;
  plungerMod = 0;
  plungerCharge = false;
  flipperLMove = false;
  flipperRMove = false;

  lightAmbient = 1.0f;
  lightDiffuse = 1.0f;
  lightSpecular = 1.0f;
  spotAngle = 150;
  lightCount = 1;
  lightIntensity = 1.0f;

  language = "english";
}

Graphics::~Graphics()
{
  broadphase = NULL;
  collisionConfiguration = NULL;
  dispatcher = NULL;
  solver = NULL;
  dynamicsWorld = NULL;

  plane = NULL;
  plane2 = NULL;
  sphere = NULL;
  cylinder = NULL;
  wall = NULL;
  plunger = NULL;
  flipperR = NULL;
  flipperL = NULL;

  sphereMotionState = NULL;
  planeMotionState = NULL;
  plane2MotionState = NULL;
  wallMotionState = NULL;
  cylinderMotionState = NULL;
  plungerMotionState = NULL;
  flipperRMotionState = NULL;
  flipperLMotionState = NULL;

  for(int i = 0; i < rigidBody.size(); i++){
    rigidBody[i] = NULL;
  }
  for(int i = 0; i < m_models.size(); i++){
    m_models[i] = NULL;
  }
  m_body = NULL;
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

  //Create shaders
  if(!InitPerVertShader()){
    printf("Per Vertex Shader Failed to Initialize\n");
    return false;
  }
  if(!InitPerFragShader()){
    printf("Per Fragment Shader Failed to Initialize\n");
    return false;
  }

  // Create/load the objects
  generateObjects();

  //enable depth testing
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  spherePos = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 8.0f, 0.0f));
  planePos = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
  plane2Pos = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 4.0f, 0.0f));
  cylinderPos = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.5f, 15.0f));
  plungerPos = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.5f, -5));
  plungerPos = glm::rotate(plungerPos, 0.785f, glm::vec3(1.0f, 0.0f, 0.0f));
  flipperRPos = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
  flipperLPos = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
  cylinder2Pos = glm::translate(glm::mat4(1.0f), glm::vec3(8.0f, 1.5f, 2.0f));
  cylinder3Pos = glm::translate(glm::mat4(1.0f), glm::vec3(-8.0f, 1.5f, -5.0f));
  bustPos = glm::translate(glm::mat4(1.0f), glm::vec3(1.5f, 4.0f, 28.0f));
  bustPos = glm::rotate(bustPos, 3.14f, glm::vec3(0.0f, 1.0f, 0.0f));
  bustPos = glm::scale(bustPos, glm::vec3(8.0f, 8.0f, 8.0f));

  positions[0] = planePos;
  positions[1] = spherePos;
  positions[2] = cylinderPos;
  positions[3] = plungerPos;
  positions[4] = flipperRPos;
  positions[5] = flipperLPos;
  positions[6] = plane2Pos;
  positions[7] = cylinder2Pos;
  positions[8] = cylinder3Pos;
  positions[9] = bustPos;

  //initialize bullet physics engine
  InitBullet();

  ambient = 0.1f;

  return true;
}

void Graphics::Update(unsigned int dt)
{
  btTransform ballTrans;
  btTransform plungerTrans;
  btTransform flipperRTrans;
  btTransform flipperLTrans;
  btScalar m[16];
  btScalar p[16];
  btScalar fl[16];
  btScalar fr[16];

  dynamicsWorld->stepSimulation(dt, 3);
  rigidBody[2]->body->getMotionState()->getWorldTransform(ballTrans);

  gContactAddedCallback = callbackFunc;

  ballTrans.getOpenGLMatrix(m);
  positions[1] = glm::make_mat4(m);

  posLight.position.x = positions[1][3][0];
  posLight.position.z = positions[1][3][2];
  posLight.ambient = glm::vec4(lightAmbient, lightAmbient, lightAmbient, 1.0f);
  posLight.diffuse = glm::vec4(lightDiffuse, lightDiffuse, lightDiffuse, 1.0f);
  posLight.specular = glm::vec4(lightSpecular, lightSpecular, lightSpecular, 1.0f);

  rigidBody[3]->body->getMotionState()->getWorldTransform(plungerTrans);
  plungerTrans.getOpenGLMatrix(p);
  positions[3] = glm::make_mat4(p);

  rigidBody[4]->body->getMotionState()->getWorldTransform(flipperRTrans);
  flipperRTrans.getOpenGLMatrix(fl);
  positions[4] = glm::make_mat4(fl);

  rigidBody[5]->body->getMotionState()->getWorldTransform(flipperLTrans);
  flipperLTrans.getOpenGLMatrix(fr);
  positions[5] = glm::make_mat4(fr);

  if(plungerCharge == true){
    btTransform trans;
    rigidBody[3]->body->getMotionState()->getWorldTransform(trans);
    btVector3 origin = trans.getOrigin();
    if(plungerOrigPos.z() <= origin.z()){
      plungerCharge = false;
      plungerMod = 0.0f;
    }
    else{
      origin += btVector3(0.0, 0.0, 0.1*plungerMod);
      trans.setOrigin(origin);
      rigidBody[3]->body->getMotionState()->setWorldTransform(trans);
    }
  }

  if(flipperLMove == true){
    btTransform trans;
    rigidBody[5]->body->getMotionState()->getWorldTransform(trans);
    btQuaternion rot = trans.getRotation();
    if(rot.y() <= -0.30){
      rot += btQuaternion(0.0, 0.2, 0.0, 0.0);
      trans.setRotation(rot);
      rigidBody[5]->body->getMotionState()->setWorldTransform(trans);
    }
    else{
      flipperLMove = false;
      trans.setRotation(flipperLOrigPos);
      rigidBody[5]->body->getMotionState()->setWorldTransform(trans);
    }
  }

  if(flipperRMove == true){
    btTransform trans;
    rigidBody[4]->body->getMotionState()->getWorldTransform(trans);
    btQuaternion rot = trans.getRotation();
    if(rot.y() >= 0.30){
      rot -= btQuaternion(0.0, 0.2, 0.0, 0.0);
      trans.setRotation(rot);
      rigidBody[4]->body->getMotionState()->setWorldTransform(trans);
    }
    else{
      flipperRMove = false;
      trans.setRotation(flipperROrigPos);
      rigidBody[4]->body->getMotionState()->setWorldTransform(trans);
    }
  }

  if(rigidBody[2]->lives != current_lives){
    btTransform transform;
    transform.setIdentity();

    transform.setOrigin(btVector3(-16.4f, 1.0f, -10.0f));
    transform.setRotation(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f));

    rigidBody[2]->body->setWorldTransform(transform);
    rigidBody[2]->body->getMotionState()->setWorldTransform(transform);

    rigidBody[2]->body->setLinearVelocity(btVector3(0.0, -9.81, -2.0));

    transform.setOrigin(btVector3(-16.4, 1, -13));
    transform.setRotation(btQuaternion(1, 0, 0, 1));

    rigidBody[3]->body->setWorldTransform(transform);
    rigidBody[3]->body->getMotionState()->setWorldTransform(transform);

    current_lives = rigidBody[2]->lives;
   }

  m_models[0]->Update(planePos);
  m_models[1]->Update(positions[1]);
  m_models[2]->Update(cylinderPos);
  m_models[3]->Update(positions[3]);
  m_models[4]->Update(positions[4]);
  m_models[5]->Update(positions[5]);
  m_models[6]->Update(positions[7]);
  m_models[7]->Update(positions[8]);
  m_models[8]->Update(positions[9]);
}

void Graphics::Render(bool &running, unsigned int dt)
{
  if(current_lives <= 0){
    cout << "Final Score: " << rigidBody[2]->score << endl;
    cout << "Thank you for playing!" << endl;
    running=false;
  }

  //clear the screen
  glClearColor(0.0, 0.0, 0.2, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Start the correct program
  if (!shader_switch){
    renderPerVertShader();
  }
  else{
    renderPerFragShader();
  }

  RenderMenu(running, dt, rigidBody[2]->score, rigidBody[2]->lives);

  // Get any errors from OpenGL
  auto error = glGetError();
  if ( error != GL_NO_ERROR )
  {
    string val = ErrorString( error );
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
  std::string object_path[] = {"objects/boardbase.obj",
                               "objects/ball.obj",
                               "objects/bumpers.obj",
                               "objects/plunger.obj",
                               "objects/flipper.obj",
                               "objects/flipper.obj",
                               "objects/bumpers.obj",
                               "objects/bumpers.obj",
                               "objects/goldBust.obj"};
  std::string material_path[] = {"objects/boardbase.mtl",
                                 "objects/ball.mtl",
                                 "objects/bumpers.mtl",
                                 "objects/plunger.mtl",
                                 "objects/flipper.mtl",
                                 "objects/flipper.mtl",
                                 "objects/bumpers.mtl",
                                 "objects/bumpers.mtl",
                                 "objects/goldBust.mtl"};

  for(int i = 0; i < model_count; i++){
    m_body = new Object(object_path[i], material_path[i]);
    m_models.push_back(m_body);
  }

  // Locate the model matrix in the shader
  for(int i = 0; i < model_count; i++){
    if(!createPerVertObject(i)){
      return false;
    };
    if(!createPerFragObject(i)){
      return false;
    }
  }

  return true;
}

void Graphics::RenderMenu(bool &running, unsigned int dt, int currentScore, int currentLives)
{
  // Start the Dear ImGui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL2_NewFrame();
  ImGui::NewFrame();

  if(language == "english"){
    ImGui::Begin("Menu");

    std::string displayScoreString = "Score: " + std::to_string(currentScore);
    ImGui::Text(displayScoreString.c_str());

    std::string displayLivesString = "Lives: " + std::to_string(currentLives - 1);
    ImGui::Text(displayLivesString.c_str());

    ImGui::Text("Controls:");
    ImGui::Text("Down Arrow: Charge plunger");
    ImGui::Text("Up Arrow: Fire plunger");
    ImGui::Text("Left Arrow: Actuate left flipper");
    ImGui::Text("Right Arrow: Actuate right flipper");

    if(ImGui::Button("Switch Shader"))
      if(shader_switch == false){shader_switch = true;}
      else{shader_switch = false;}
    ImGui::NewLine();

    ImGui::Text("Spotlight Controls");
    ImGui::SliderFloat("Ambient", &lightAmbient, 0.0f, 1.0f);
    ImGui::SliderFloat("Diffuse", &lightDiffuse, 0.0f, 1.0f);
    ImGui::SliderFloat("Specular", &lightSpecular, 0.0f, 1.0f);
    ImGui::SliderInt("Spotlight Cutoff", &spotAngle, 180, 90);
    ImGui::NewLine();

    ImGui::Text("Other Light Controls");
    ImGui::SliderFloat("Intensity", &lightIntensity, 0.0f, 2.0f);
    ImGui::NewLine();

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    if (ImGui::Button("English"))
      language = "english";
    if (ImGui::Button("Portuguese"))
      language = "portuguese";
    if (ImGui::Button("Turkish"))
      language = "turkish";

    if (ImGui::Button("Quit Application"))
      running = false;
  }
  else if(language == "portuguese"){
    ImGui::Begin("Cardápio");

    std::string displayScoreString = "Pontuação: " + std::to_string(currentScore);
    ImGui::Text(displayScoreString.c_str());

    std::string displayLivesString = "Vidas: " + std::to_string(currentLives - 1);
    ImGui::Text(displayLivesString.c_str());

    ImGui::Text("Controles:");
    ImGui::Text("Seta para cima: êmbolo de fogo");
    ImGui::Text("Seta para a esquerda: acionar o flipper esquerdo");
    ImGui::Text("Seta para a direita: acionar o flipper direito");

    if(ImGui::Button("Trocar Shader"))
      if(shader_switch == false){shader_switch = true;}
      else{shader_switch = false;}
    ImGui::NewLine();

    ImGui::Text("Controles Spotlight");
    ImGui::SliderFloat("Ambiente", &lightAmbient, 0.0f, 1.0f);
    ImGui::SliderFloat("Difuso", &lightDiffuse, 0.0f, 1.0f);
    ImGui::SliderFloat("Especular", &lightSpecular, 0.0f, 1.0f);
    ImGui::SliderInt("Limite Spotlight", &spotAngle, 180, 90);
    ImGui::NewLine();

    ImGui::Text("Outros controles de luz");
    ImGui::SliderFloat("Intensidade", &lightIntensity, 0.0f, 2.0f);
    ImGui::NewLine();

    ImGui::Text("Média de aplicação %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    if (ImGui::Button("Português"))
      language = "portuguese";
    if (ImGui::Button("Inglês"))
      language = "english";
    if (ImGui::Button("Turco"))
      language = "turkish";

    if (ImGui::Button("Sair do aplicativo"))
      running = false;
  }
  else if(language == "turkish"){
    ImGui::Begin("Menü");

    std::string displayScoreString = "Puan: " + std::to_string(currentScore);
    ImGui::Text(displayScoreString.c_str());

    std::string displayLivesString = "Hayatları: " + std::to_string(currentLives - 1);
    ImGui::Text(displayLivesString.c_str());

    ImGui::Text("Kontroller:");
    ImGui::Text("Yukarı Ok: Yangın pistonu");
    ImGui::Text("Sol Ok: Sol kanatçığı etkinleştir");
    ImGui::Text("Sağ Ok: Sağ kanatçığı etkinleştir");

    if(ImGui::Button("Değiştirmek Shader"))
      if(shader_switch == false){shader_switch = true;}
      else{shader_switch = false;}
    ImGui::NewLine();

    ImGui::Text("Spot Kontrolleri");
    ImGui::SliderFloat("Ortam", &lightAmbient, 0.0f, 1.0f);
    ImGui::SliderFloat("Dağınık", &lightDiffuse, 0.0f, 1.0f);
    ImGui::SliderFloat("Aynasal", &lightSpecular, 0.0f, 1.0f);
    ImGui::SliderInt("Spot Işığı Kesimi", &spotAngle, 180, 90);
    ImGui::NewLine();

    ImGui::Text("Diğer Işık Kontrolleri");
    ImGui::SliderFloat("Yoğunluk", &lightIntensity, 0.0f, 2.0f);
    ImGui::NewLine();

    ImGui::Text("Uygulama ortalaması %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    if (ImGui::Button("Türk"))
      language = "turkish";
    if (ImGui::Button("İngilizce"))
      language = "english";
    if (ImGui::Button("Portekizce"))
      language = "portuguese";

    if (ImGui::Button("Uygulamadan Çık"))
      running = false;
  }

  ImGui::End();

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool Graphics::callbackFunc(btManifoldPoint& cp,const btCollisionObjectWrapper* obj1,int id1,int index1,const btCollisionObjectWrapper* obj2,int id2,int index2)
{
  if(((bulletObject*)obj1->getCollisionObject()->getUserPointer())->id == 0 &&
      ((bulletObject*)obj2->getCollisionObject()->getUserPointer())->id == 1){
    
    cout << "Current Score: " << ((bulletObject*)obj1->getCollisionObject()->getUserPointer())->score << endl;
    ((bulletObject*)obj1->getCollisionObject()->getUserPointer())->score += 1;
  }

  if(((bulletObject*)obj1->getCollisionObject()->getUserPointer())->id == 0 &&
     ((bulletObject*)obj2->getCollisionObject()->getUserPointer())->id == 2){

    ((bulletObject*)obj1->getCollisionObject()->getUserPointer())->lives -= 1;
  }

  return false;
}

void Graphics::InitBullet()
{
  // Generate and initialize physics engine pointers
  broadphase = new btDbvtBroadphase();
  collisionConfiguration = new btDefaultCollisionConfiguration();
  dispatcher = new btCollisionDispatcher(collisionConfiguration);
  solver = new btSequentialImpulseConstraintSolver;
  dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
  dynamicsWorld->setGravity(btVector3(0.0, -9.81, -2.0));

  createPlanes();
  createBall();
  createPlunger();
  createFlippers();
  createWalls();
  createBumpers();
}

void Graphics::plungerUp()
{
  plungerCharge = true;
}

void Graphics::plungerDown()
{
  btTransform trans;
  rigidBody[3]->body->getMotionState()->getWorldTransform(trans);
  btVector3 origin = trans.getOrigin();
  if(origin.z() >= -15.0f){
    plungerMod += 0.5;
    origin += btVector3(0.0, 0.0, -0.1);
    trans.setOrigin(origin);
    rigidBody[3]->body->getMotionState()->setWorldTransform(trans);
  }
}

void Graphics::lFlipperUp()
{
  flipperLMove = true;
}

void Graphics::rFlipperUp()
{
  flipperRMove = true;
}

void Graphics::cameraUp()
{
  m_camera->moveCameraUp();
}

void Graphics::cameraDown()
{
  m_camera->moveCameraDown();
}

bool Graphics::InitPerVertShader()
{
  // Set up the shaders
  m_vertShader = new Shader(m_VERTEX_FILEPATH, m_FRAGMENT_FILEPATH);
  if(!m_vertShader->Initialize())
  {
    printf("Shader Failed to Initialize\n");
    return false;
  }

  // Add the vertex shader
  if(!m_vertShader->AddShader(GL_VERTEX_SHADER))
  {
    printf("Vertex Shader failed to Initialize\n");
    return false;
  }

  // Add the fragment shader
  if(!m_vertShader->AddShader(GL_FRAGMENT_SHADER))
  {
    printf("Fragment Shader failed to Initialize\n");
    return false;
  }

  // Connect the program
  if(!m_vertShader->Finalize())
  {
    printf("Program to Finalize\n");
    return false;
  }

  // Locate the projection matrix in the shader
  m_projectionMatrix = m_vertShader->GetUniformLocation("projectionMatrix");
  if (m_projectionMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_projectionMatrix not found\n");
    return false;
  }

  // Locate the view matrix in the shader
  m_viewMatrix = m_vertShader->GetUniformLocation("viewMatrix");
  if (m_viewMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_viewMatrix not found\n");
    return false;
  }

  //Locate globalambient value
  m_globalAmbient = m_vertShader->GetUniformLocation("globalAmbient");
  if (m_globalAmbient == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_globalAmbient not found\n");
    return false;
  }

  //Locate lighting values
  m_positionalLight[0][0] = m_vertShader->GetUniformLocation("spot.ambient");
  m_positionalLight[0][1] = m_vertShader->GetUniformLocation("spot.diffuse");
  m_positionalLight[0][2] = m_vertShader->GetUniformLocation("spot.specular");
  m_positionalLight[0][3] = m_vertShader->GetUniformLocation("spot.position");
  if (m_positionalLight[0][0] == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_positionalLight not found\n");
    return false;
  }

  m_positionalLight[1][0] = m_vertShader->GetUniformLocation("light[0].ambient");
  m_positionalLight[1][1] = m_vertShader->GetUniformLocation("light[0].diffuse");
  m_positionalLight[1][2] = m_vertShader->GetUniformLocation("light[0].specular");
  m_positionalLight[1][3] = m_vertShader->GetUniformLocation("light[0].position");
  m_cutoffAngle = m_vertShader->GetUniformLocation("cutoffAngle");
  m_lightCount = m_vertShader->GetUniformLocation("lightCount");

  return true;
}

bool Graphics::InitPerFragShader()
{
  //Set up the shader
  m_fragShader = new Shader(f_VERTEX_FILEPATH, f_FRAGMENT_FILEPATH);
  if(!m_fragShader->Initialize())
  {
    printf("Shader Failed to Initialize\n");
    return false;
  }

  //Add the vertex shader
  if(!m_fragShader->AddShader(GL_VERTEX_SHADER))
  {
    printf("Vertex Shader failed to Initialize\n");
    return false;
  }

  // Add the fragment shader
  if(!m_fragShader->AddShader(GL_FRAGMENT_SHADER))
  {
    printf("Fragment Shader failed to Initialize\n");
    return false;
  }

  // Connect the program
  if(!m_fragShader->Finalize())
  {
    printf("Program to Finalize\n");
    return false;
  }

  // Locate the projection matrix in the shader
  f_projectionMatrix = m_fragShader->GetUniformLocation("projectionMatrix");
  if (f_projectionMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_projectionMatrix not found\n");
    return false;
  }

  // Locate the view matrix in the shader
  f_viewMatrix = m_fragShader->GetUniformLocation("viewMatrix");
  if (f_viewMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_viewMatrix not found\n");
    return false;
  }

  //locate globalambient value
  f_globalAmbient = m_fragShader->GetUniformLocation("globalAmbient");
  if (f_globalAmbient == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_globalAmbient not found\n");
    return false;
  }

  //locate lighting values
  f_positionalLight[0][0] = m_fragShader->GetUniformLocation("spot.ambient");
  f_positionalLight[0][1] = m_fragShader->GetUniformLocation("spot.diffuse");
  f_positionalLight[0][2] = m_fragShader->GetUniformLocation("spot.specular");
  f_positionalLight[0][3] = m_fragShader->GetUniformLocation("spot.position");
  if (f_positionalLight[0][0] == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_positionalLight not found\n");
    return false;
  }

  f_positionalLight[1][0] = m_fragShader->GetUniformLocation("light[0].ambient");
  f_positionalLight[1][1] = m_fragShader->GetUniformLocation("light[0].diffuse");
  f_positionalLight[1][2] = m_fragShader->GetUniformLocation("light[0].specular");
  f_positionalLight[1][3] = m_fragShader->GetUniformLocation("light[0].position");
  f_cutoffAngle = m_fragShader->GetUniformLocation("cutoffAngle");
  f_lightCount = m_fragShader->GetUniformLocation("lightCount");

  return true;
}

void Graphics::renderPerVertShader()
{
  m_vertShader->Enable();
  // Send in the projection and view to the shader
  glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection())); 
  glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView())); 

  glUniform4f(m_globalAmbient, ambient, ambient, ambient, 1.0);
  glUniform1i(m_cutoffAngle, spotAngle);
  glUniform1i(m_lightCount, lightCount);

  glUniform4f(m_positionalLight[0][0], posLight.ambient.x, posLight.ambient.y, posLight.ambient.z, 1.0f);
  glUniform4f(m_positionalLight[0][1], posLight.diffuse.x, posLight.diffuse.y, posLight.diffuse.z, 1.0f);
  glUniform4f(m_positionalLight[0][2], posLight.specular.x, posLight.specular.y, posLight.specular.z, 1.0f);
  glUniform3f(m_positionalLight[0][3], posLight.position.x, 15.0f, posLight.position.z);

  glUniform4f(m_positionalLight[1][0], lightIntensity*0.2f, lightIntensity*0.2f, lightIntensity*0.6f, 1.0f);
  glUniform4f(m_positionalLight[1][1], lightIntensity*0.4f, lightIntensity*0.4f, lightIntensity*0.8f, 1.0f);
  glUniform4f(m_positionalLight[1][2], lightIntensity*0.8f, lightIntensity*0.8f, lightIntensity*1.0f, 1.0f);
  glUniform3f(m_positionalLight[1][3], 20.0, 10.0f, 20.0f);

  // Render the object
  for(int i = 0; i < model_count; i++){
    Material currMaterial = m_models[i]->GetMaterial();
    glUniformMatrix4fv(m_normalMatrix[i], 1, GL_TRUE, glm::value_ptr(glm::inverse(m_models[i]->GetModel())));
    glUniformMatrix4fv(m_modelMatrix[i], 1, GL_FALSE, glm::value_ptr(m_models[i]->GetModel()));
    glUniform4f(m_material[i][0], 0.1, 0.1, 0.1, 1.0);
    glUniform4f(m_material[i][1], currMaterial.Kd.x, currMaterial.Kd.y, currMaterial.Kd.z, 1.0);
    glUniform4f(m_material[i][2], currMaterial.Ks.x, currMaterial.Ks.y, currMaterial.Ks.z, 1.0);
    glUniform1f(m_material[i][3], currMaterial.Ns);
    m_models[i]->Render();
  }
}

void Graphics::renderPerFragShader()
{
  m_fragShader->Enable();
  // Send in the projection and view to the shader
  glUniformMatrix4fv(f_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection())); 
  glUniformMatrix4fv(f_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView())); 

  glUniform4f(f_globalAmbient, ambient, ambient, ambient, 1.0);
  glUniform1i(f_cutoffAngle, spotAngle);
  glUniform1i(f_lightCount, lightCount);

  glUniform4f(f_positionalLight[0][0], posLight.ambient.x, posLight.ambient.y, posLight.ambient.z, 1.0f);
  glUniform4f(f_positionalLight[0][1], posLight.diffuse.x, posLight.diffuse.y, posLight.diffuse.z, 1.0f);
  glUniform4f(f_positionalLight[0][2], posLight.specular.x, posLight.specular.y, posLight.specular.z, 1.0f);
  glUniform3f(f_positionalLight[0][3], posLight.position.x, 15.0f, posLight.position.z);

  glUniform4f(f_positionalLight[1][0], lightIntensity*0.2f, lightIntensity*0.2f, lightIntensity*0.6f, 1.0f);
  glUniform4f(f_positionalLight[1][1], lightIntensity*0.4f, lightIntensity*0.4f, lightIntensity*0.8f, 1.0f);
  glUniform4f(f_positionalLight[1][2], lightIntensity*0.8f, lightIntensity*0.8f, lightIntensity*1.0f, 1.0f);
  glUniform3f(f_positionalLight[1][3], 20.0, 10.0f, 20.0f);

  // Render the object
  for(int i = 0; i < model_count; i++){
    Material currMaterial = m_models[i]->GetMaterial();
    glUniformMatrix4fv(f_normalMatrix[i], 1, GL_TRUE, glm::value_ptr(glm::inverse(m_models[i]->GetModel())));
    glUniformMatrix4fv(f_modelMatrix[i], 1, GL_FALSE, glm::value_ptr(m_models[i]->GetModel()));
    glUniform4f(f_material[i][0], 0.1, 0.1, 0.1, 1.0);
    glUniform4f(f_material[i][1], currMaterial.Kd.x, currMaterial.Kd.y, currMaterial.Kd.z, 1.0);
    glUniform4f(f_material[i][2], currMaterial.Ks.x, currMaterial.Ks.y, currMaterial.Ks.z, 1.0);
    glUniform1f(f_material[i][3], currMaterial.Ns);
    m_models[i]->Render(); 
  }
}

bool Graphics::createPerVertObject(int index)
{
  m_modelMatrix[index] = m_vertShader->GetUniformLocation("modelMatrix");
  if (m_modelMatrix[index] == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_modelMatrix not found\n");
    return false;
  }

  m_normalMatrix[index] = m_vertShader->GetUniformLocation("norm_matrix");
  if (m_modelMatrix[index] == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_normalMatrix not found\n");
    return false;
  }

  m_material[index][0] = m_vertShader->GetUniformLocation("material.ambient");
  m_material[index][1] = m_vertShader->GetUniformLocation("material.diffuse");
  m_material[index][2] = m_vertShader->GetUniformLocation("material.specular");
  m_material[index][3] = m_vertShader->GetUniformLocation("material.shininess");
  if (m_material[index][0] == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_material not found\n");
    return false;
  }

  return true;
}

bool Graphics::createPerFragObject(int index)
{
  f_modelMatrix[index] = m_fragShader->GetUniformLocation("modelMatrix");
  if (f_modelMatrix[index] == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_modelMatrix not found\n");
    return false;
  }

  f_normalMatrix[index] = m_fragShader->GetUniformLocation("norm_matrix");
  if (f_normalMatrix[index] == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_normalMatrix not found\n");
    return false;
  }

  f_material[index][0] = m_fragShader->GetUniformLocation("material.ambient");
  f_material[index][1] = m_fragShader->GetUniformLocation("material.diffuse");
  f_material[index][2] = m_fragShader->GetUniformLocation("material.specular");
  f_material[index][3] = m_fragShader->GetUniformLocation("material.shininess");
  if (f_material[index][0] == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_material not found\n");
    return false;
  }

  return true;
}

void Graphics::createPlanes()
{
  plane = new btStaticPlaneShape(btVector3(0.0f, 1.0f, 0.0f), 0.0f);
  plane2 = new btStaticPlaneShape(btVector3(0.0f, -1.0f, 0.0f), 0.0f);

  // Create rigid body information for floor plane
  planeMotionState = NULL;
  planeMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
  btVector3 planeInertia(0, 0, 0);

  btRigidBody::btRigidBodyConstructionInfo planeRigidBodyCI(0, planeMotionState, plane, btVector3(0, 0, 0));
  btRigidBody* planeRigidBody = new btRigidBody(planeRigidBodyCI);
  planeRigidBody->setRestitution(1);
  rigidBody.push_back(new struct bulletObject(planeRigidBody, 4, 0, 0));
  planeRigidBody->setUserPointer(rigidBody[rigidBody.size() - 1]);
  dynamicsWorld->addRigidBody(rigidBody[rigidBody.size() - 1]->body);

  // Create rigid body information for ceiling plane
  plane2MotionState = NULL;
  plane2MotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 4, 0)));
  btVector3 plane2Inertia(0, 0, 0);

  btRigidBody::btRigidBodyConstructionInfo plane2RigidBodyCI(0, plane2MotionState, plane2, btVector3(0, 0, 0));
  btRigidBody* plane2RigidBody = new btRigidBody(plane2RigidBodyCI);
  plane2RigidBody->setRestitution(1);
  rigidBody.push_back(new struct bulletObject(plane2RigidBody, 4, 0, 0));
  plane2RigidBody->setUserPointer(rigidBody[rigidBody.size() - 1]);
  dynamicsWorld->addRigidBody(rigidBody[rigidBody.size() - 1]->body);
}

void Graphics::createPlunger()
{
  plunger = new btCylinderShape(btVector3(1, 2, 1));

  // Create kinematic plunger object
  plungerMotionState = NULL;
  plungerMotionState = new btDefaultMotionState(btTransform(btQuaternion(1, 0, 0, 1), btVector3(-16.4, 1, -13)));
  btVector3 plungerInertia(0, 0, 0);

  btRigidBody::btRigidBodyConstructionInfo plungerRigidBodyCI(0, plungerMotionState, plunger, btVector3(0, 0, 0));
  btRigidBody* plungerRigidBody = new btRigidBody(plungerRigidBodyCI);
  plungerRigidBody->setRestitution(1);
  plungerRigidBody->setCollisionFlags(plungerRigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
  plungerRigidBody->setCollisionFlags(plungerRigidBody->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
  plungerRigidBody->setActivationState(DISABLE_DEACTIVATION);
  rigidBody.push_back(new struct bulletObject(plungerRigidBody, 6, 0, 0));
  plungerRigidBody->setUserPointer(rigidBody[rigidBody.size() - 1]);
  dynamicsWorld->addRigidBody(rigidBody[rigidBody.size() - 1]->body);

  btTransform trans;
  rigidBody[3]->body->getMotionState()->getWorldTransform(trans);
  plungerOrigPos = trans.getOrigin();
}

void Graphics::createBall()
{
  sphere = new btSphereShape(1.0f);

  // Create rigid body information for sphere
  sphereMotionState = NULL;
  sphereMotionState = new btDefaultMotionState(btTransform(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f), btVector3(-16.4f, 1.0f, -10.0f)));
  btScalar mass(1);
  btVector3 inertia(1.0f, 1.0f, 1.0f);
  sphere->calculateLocalInertia(mass, inertia);

  btRigidBody::btRigidBodyConstructionInfo sphereRigidBodyCI(mass, sphereMotionState, sphere, inertia);
  btRigidBody* sphereRigidBody = new btRigidBody(sphereRigidBodyCI);
  sphereRigidBody->setRestitution(0.5);
  rigidBody.push_back(new struct bulletObject(sphereRigidBody, 0, 0, starting_lives));
  sphereRigidBody->setUserPointer(rigidBody[rigidBody.size() - 1]);
  dynamicsWorld->addRigidBody(rigidBody[rigidBody.size() - 1]->body);
}

void Graphics::createFlippers()
{
  flipperR = new btBoxShape(btVector3(0.9, 2, 5));
  flipperL = new btBoxShape(btVector3(0.9, 2, 5));

  // Create kinematic flipperR object
  flipperRMotionState = NULL;
  flipperRMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 1.33, 0, 1), btVector3(-6, 2, -18.5)));
  btVector3 flipperRInertia(0, 0, 0);

  btRigidBody::btRigidBodyConstructionInfo flipperRRigidBodyCI(0,flipperRMotionState, flipperR, btVector3(0, 0, 0));
  btRigidBody* flipperRRigidBody = new btRigidBody(flipperRRigidBodyCI);
  flipperRRigidBody->setRestitution(1.3);
  flipperRRigidBody->setCollisionFlags(flipperRRigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
  flipperRRigidBody->setCollisionFlags(flipperRRigidBody->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
  flipperRRigidBody->setActivationState(DISABLE_DEACTIVATION);
  rigidBody.push_back(new struct bulletObject(flipperRRigidBody, 1, 0, 0));
  flipperRRigidBody->setUserPointer(rigidBody[rigidBody.size() - 1]);
  dynamicsWorld->addRigidBody(rigidBody[rigidBody.size() - 1]->body);

  btTransform trans;
  rigidBody[4]->body->getMotionState()->getWorldTransform(trans);
  flipperROrigPos = trans.getRotation();

  // Create kinematic flipperR object
  flipperLMotionState = NULL;
  flipperLMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, -1.33, 0, 1), btVector3(9, 2, -18.5)));
  btVector3 flipperLInertia(0, 0, 0);

  btRigidBody::btRigidBodyConstructionInfo flipperLRigidBodyCI(0,flipperLMotionState, flipperL, btVector3(0, 0, 0));
  btRigidBody* flipperLRigidBody = new btRigidBody(flipperLRigidBodyCI);
  flipperLRigidBody->setRestitution(1.3);
  flipperLRigidBody->setCollisionFlags(flipperLRigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
  flipperLRigidBody->setCollisionFlags(flipperLRigidBody->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
  flipperLRigidBody->setActivationState(DISABLE_DEACTIVATION);
  rigidBody.push_back(new struct bulletObject(flipperLRigidBody, 1, 0, 0));
  flipperLRigidBody->setUserPointer(rigidBody[rigidBody.size() - 1]);
  dynamicsWorld->addRigidBody(rigidBody[rigidBody.size() - 1]->body);

  rigidBody[5]->body->getMotionState()->getWorldTransform(trans);
  flipperLOrigPos = trans.getRotation();
}

void Graphics::createWalls()
{
  wall = new btBoxShape(btVector3(20, 2, 0));

  // Create rigid body information for north wall
  wallMotionState = NULL;
  wallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 2, 25)));
  btVector3 wallInertia(0, 0, 0);

  btRigidBody::btRigidBodyConstructionInfo wallRigidBodyCI(0, wallMotionState, wall, btVector3(0, 0, 0));
  btRigidBody* wallRigidBody = new btRigidBody(wallRigidBodyCI);
  wallRigidBody->setRestitution(1.2);
  rigidBody.push_back(new struct bulletObject(wallRigidBody, 3, 0, 0));
  wallRigidBody->setUserPointer(rigidBody[rigidBody.size() - 1]);
  dynamicsWorld->addRigidBody(rigidBody[rigidBody.size() - 1]->body);

  // create info for west wall
  wall = NULL;
  wall = new btBoxShape(btVector3(30, 2, 0));

  wallMotionState = NULL;
  wallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 1, 0, 1), btVector3(17.6, 2, 0)));

  wallRigidBody = NULL;
  btRigidBody::btRigidBodyConstructionInfo wall2RigidBodyCI(0, wallMotionState, wall, btVector3(0, 0, 0));
  wallRigidBody = new btRigidBody(wall2RigidBodyCI);
  wallRigidBody->setRestitution(1.2);
  rigidBody.push_back(new struct bulletObject(wallRigidBody, 4, 0, 0));
  wallRigidBody->setUserPointer(rigidBody[rigidBody.size() - 1]);
  dynamicsWorld->addRigidBody(rigidBody[rigidBody.size() - 1]->body);

  // create info for south kill plane wall
  wall = NULL;
  wall = new btBoxShape(btVector3(20, 2, 0));

  wallMotionState = NULL;
  wallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 2, -30)));

  wallRigidBody = NULL;
  btRigidBody::btRigidBodyConstructionInfo wall3RigidBodyCI(0, wallMotionState, wall, btVector3(0, 0, 0));
  wallRigidBody = new btRigidBody(wall3RigidBodyCI);
  wallRigidBody->setRestitution(0);
  wallRigidBody->setCollisionFlags(wallRigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
  wallRigidBody->setCollisionFlags(wallRigidBody->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
  wallRigidBody->setActivationState(DISABLE_DEACTIVATION);
  rigidBody.push_back(new struct bulletObject(wallRigidBody, 2, 0, 0));
  wallRigidBody->setUserPointer(rigidBody[rigidBody.size() - 1]);
  dynamicsWorld->addRigidBody(rigidBody[rigidBody.size() - 1]->body);

  // create info for east outer wall
  wall = NULL;
  wall = new btBoxShape(btVector3(22, 2, 0));

  wallMotionState = NULL;
  wallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 1, 0, 1), btVector3(-17.6, 2, 0)));

  wallRigidBody = NULL;
  btRigidBody::btRigidBodyConstructionInfo wall4RigidBodyCI(0, wallMotionState, wall, btVector3(0, 0, 0));
  wallRigidBody = new btRigidBody(wall4RigidBodyCI);
  wallRigidBody->setRestitution(1.2);
  rigidBody.push_back(new struct bulletObject(wallRigidBody, 6, 0, 0));
  wallRigidBody->setUserPointer(rigidBody[rigidBody.size() - 1]);
  dynamicsWorld->addRigidBody(rigidBody[rigidBody.size() - 1]->body);

  // create info for east inner wall
  wall = NULL;
  wall = new btBoxShape(btVector3(20, 2, 0.2));

  wallMotionState = NULL;
  wallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 1, 0, 1), btVector3(-15, 2, -10)));

  wallRigidBody = NULL;
  btRigidBody::btRigidBodyConstructionInfo wall5RigidBodyCI(0, wallMotionState, wall, btVector3(0, 0, 0));
  wallRigidBody = new btRigidBody(wall5RigidBodyCI);
  wallRigidBody->setRestitution(1.2);
  rigidBody.push_back(new struct bulletObject(wallRigidBody, 6, 0 ,0));
  wallRigidBody->setUserPointer(rigidBody[rigidBody.size() - 1]);
  dynamicsWorld->addRigidBody(rigidBody[rigidBody.size() - 1]->body);

  // create info for east top angle wall
  wall = NULL;
  wall = new btBoxShape(btVector3(5.6, 2, 0.2));

  wallMotionState = NULL;
  wallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, -0.5, 0, 1), btVector3(-13.8, 2, 21.1)));

  wallRigidBody = NULL;
  btRigidBody::btRigidBodyConstructionInfo wall6RigidBodyCI(0, wallMotionState, wall, btVector3(0, 0, 0));
  wallRigidBody = new btRigidBody(wall6RigidBodyCI);
  wallRigidBody->setRestitution(1.2);
  rigidBody.push_back(new struct bulletObject(wallRigidBody, 6, 0, 0));
  wallRigidBody->setUserPointer(rigidBody[rigidBody.size() - 1]);
  dynamicsWorld->addRigidBody(rigidBody[rigidBody.size() - 1]->body);

  // create info for west top angle wall
  wall = NULL;
  wall = new btBoxShape(btVector3(5.6, 2, 0.2));

  wallMotionState = NULL;
  wallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0.5, 0, 1), btVector3(13.8, 2, 21.1)));

  wallRigidBody = NULL;
  btRigidBody::btRigidBodyConstructionInfo wall7RigidBodyCI(0, wallMotionState, wall, btVector3(0, 0, 0));
  wallRigidBody = new btRigidBody(wall7RigidBodyCI);
  wallRigidBody->setRestitution(1.2);
  rigidBody.push_back(new struct bulletObject(wallRigidBody, 6, 0, 0));
  wallRigidBody->setUserPointer(rigidBody[rigidBody.size() - 1]);
  dynamicsWorld->addRigidBody(rigidBody[rigidBody.size() - 1]->body);

  // create info for west bottom angle wall
  wall = NULL;
  wall = new btBoxShape(btVector3(3.5, 2, 0.2));

  wallMotionState = NULL;
  wallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, -0.33, 0, 1), btVector3(15, 2, -15.5)));

  wallRigidBody = NULL;
  btRigidBody::btRigidBodyConstructionInfo wall8RigidBodyCI(0, wallMotionState, wall, btVector3(0, 0, 0));
  wallRigidBody = new btRigidBody(wall8RigidBodyCI);
  wallRigidBody->setRestitution(1.2);
  rigidBody.push_back(new struct bulletObject(wallRigidBody, 6, 0, 0));
  wallRigidBody->setUserPointer(rigidBody[rigidBody.size() - 1]);
  dynamicsWorld->addRigidBody(rigidBody[rigidBody.size() - 1]->body);

  // create info for west bottom angle wall
  wall = NULL;
  wall = new btBoxShape(btVector3(3.5, 2, 0.2));

  wallMotionState = NULL;
  wallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0.33, 0, 1), btVector3(-12, 2, -15.5)));

  wallRigidBody = NULL;
  btRigidBody::btRigidBodyConstructionInfo wall9RigidBodyCI(0, wallMotionState, wall, btVector3(0, 0, 0));
  wallRigidBody = new btRigidBody(wall9RigidBodyCI);
  wallRigidBody->setRestitution(1.2);
  rigidBody.push_back(new struct bulletObject(wallRigidBody, 6, 0, 0));
  wallRigidBody->setUserPointer(rigidBody[rigidBody.size() - 1]);
  dynamicsWorld->addRigidBody(rigidBody[rigidBody.size() - 1]->body);

  // create info for east plunger well bottom wall
  wall = NULL;
  wall = new btBoxShape(btVector3(1.6, 2, 0));

  wallMotionState = NULL;
  wallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(-16.4, 2, -14)));

  wallRigidBody = NULL;
  btRigidBody::btRigidBodyConstructionInfo wall10RigidBodyCI(0, wallMotionState, wall, btVector3(0, 0, 0));
  wallRigidBody = new btRigidBody(wall10RigidBodyCI);
  wallRigidBody->setRestitution(1.2);
  rigidBody.push_back(new struct bulletObject(wallRigidBody, 6, 0, 0));
  wallRigidBody->setUserPointer(rigidBody[rigidBody.size() - 1]);
  dynamicsWorld->addRigidBody(rigidBody[rigidBody.size() - 1]->body);
}

void Graphics::createBumpers()
{
  cylinder = new btCylinderShape(btVector3(1.5f, 1.5f, 1.5f));

  // Create rigid body information for static cylinder
  cylinderMotionState = NULL;
  cylinderMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 1.5, 15)));
  btVector3 cylinderIntertia(0, 0, 0);

  btRigidBody::btRigidBodyConstructionInfo cylinderRigidBodyCI(0, cylinderMotionState, cylinder, btVector3(0, 0, 0));
  btRigidBody* cylinderRigidBody = new btRigidBody(cylinderRigidBodyCI);
  cylinderRigidBody->setRestitution(2);
  rigidBody.push_back(new struct bulletObject(cylinderRigidBody, 5, 0, 0));
  cylinderRigidBody->setUserPointer(rigidBody[rigidBody.size() - 1]);
  dynamicsWorld->addRigidBody(rigidBody[rigidBody.size() - 1]->body);

  // Create rigid body information for static cylinder
  cylinderMotionState = NULL;
  cylinderMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(8, 1.5, 2)));

  btRigidBody::btRigidBodyConstructionInfo cylinder2RigidBodyCI(0, cylinderMotionState, cylinder, btVector3(0, 0, 0));
  cylinderRigidBody = new btRigidBody(cylinder2RigidBodyCI);
  cylinderRigidBody->setRestitution(2);
  rigidBody.push_back(new struct bulletObject(cylinderRigidBody, 5, 0, 0));
  cylinderRigidBody->setUserPointer(rigidBody[rigidBody.size() - 1]);
  dynamicsWorld->addRigidBody(rigidBody[rigidBody.size() - 1]->body);

  // Create rigid body information for static cylinder
  cylinderMotionState = NULL;
  cylinderMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(-8, 1.5, -5)));

  btRigidBody::btRigidBodyConstructionInfo cylinder3RigidBodyCI(0, cylinderMotionState, cylinder, btVector3(0, 0, 0));
  cylinderRigidBody = new btRigidBody(cylinder3RigidBodyCI);
  cylinderRigidBody->setRestitution(2);
  rigidBody.push_back(new struct bulletObject(cylinderRigidBody, 5, 0, 0));
  cylinderRigidBody->setUserPointer(rigidBody[rigidBody.size() - 1]);
  dynamicsWorld->addRigidBody(rigidBody[rigidBody.size() - 1]->body);
}