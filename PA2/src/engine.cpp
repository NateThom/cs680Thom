
#include "engine.h"

Engine::Engine(string name, int width, int height)
{
  m_WINDOW_NAME = name;
  m_WINDOW_WIDTH = width;
  m_WINDOW_HEIGHT = height;
  m_FULLSCREEN = false;

  m_rotate_flag = true;
  m_rotate_reverse_flag = false;
  m_translate_flag = true;
  m_translate_reverse_flag = true;
}

Engine::Engine(string name)
{
  m_WINDOW_NAME = name;
  m_WINDOW_HEIGHT = 0;
  m_WINDOW_WIDTH = 0;
  m_FULLSCREEN = true;

  m_rotate_flag = true;
  m_rotate_reverse_flag = true;
  m_translate_flag = false;
  m_translate_reverse_flag = false;
}

Engine::~Engine()
{
  delete m_window;
  delete m_graphics;
  m_window = NULL;
  m_graphics = NULL;
}

bool Engine::Initialize()
{
  // Start a window
  m_window = new Window();
  if(!m_window->Initialize(m_WINDOW_NAME, &m_WINDOW_WIDTH, &m_WINDOW_HEIGHT))
  {
    printf("The window failed to initialize.\n");
    return false;
  }

  // Start the graphics
  m_graphics = new Graphics();
  if(!m_graphics->Initialize(m_WINDOW_WIDTH, m_WINDOW_HEIGHT))
  {
    printf("The graphics failed to initialize.\n");
    return false;
  }

  // Set the time
  m_currentTimeMillis = GetCurrentTimeMillis();

  glsl_version = "#version 130";

  // No errors
  return true;
}

bool Engine::Initialize(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath)
{
  // Start a window
  m_window = new Window();
  if(!m_window->Initialize(m_WINDOW_NAME, &m_WINDOW_WIDTH, &m_WINDOW_HEIGHT))
  {
    printf("The window failed to initialize.\n");
    return false;
  }

  // Start the graphics
  m_graphics = new Graphics();
  if(!m_graphics->Initialize(m_WINDOW_WIDTH, m_WINDOW_HEIGHT, vertexShaderFilePath, fragmentShaderFilePath))
  {
    printf("The graphics failed to initialize.\n");
    return false;
  }

  ImGui::CreateContext();
  ImGui_ImplSDL2_InitForOpenGL(m_window->Get_SDL_Window(), m_window->Get_GLContext());
  ImGui_ImplOpenGL3_Init(glsl_version);

  // Set the time
  m_currentTimeMillis = GetCurrentTimeMillis();

  // No errors
  return true;
}

void Engine::Run()
{
  m_running = true;
  static float red = 0.0;
  static float green = 00;
  static float blue = 0.2;
  static float alpha = 1.0;
  ImGuiIO& io = ImGui::GetIO(); (void)io;

  while(m_running)
  {
    // Update the DT
    m_DT = getDT();

    // Update and render the graphics
    m_graphics->Update(m_DT, m_rotate_flag, m_rotate_reverse_flag, m_translate_flag, m_translate_reverse_flag);
    m_graphics->Render(red, green, blue, alpha);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Swanky menu!");                          // Create a window called "Hello, world!" and append into it.

//    ImGui::Text("Try out a different background color by adjusting the sliders.");               // Display some text (you can use a format strings too)
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    ImGui::SliderFloat("Red", &red, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
    ImGui::SliderFloat("Green", &green, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
    ImGui::SliderFloat("Blue", &blue, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
//    ImGui::SliderFloat("Alpha", &alpha, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

    if (ImGui::Button("Reset Color")) {                         // Buttons return true when clicked (most widgets return true when edited/activated)
      red = 0.0;
      green = 0.0;
      blue = 0.2;
//      alpha = 1.0;
    }
    if (ImGui::Button("Invert Rotation")) {
      // start/stop rotation
      if (m_rotate_reverse_flag) {
        m_rotate_reverse_flag = false;
      } else if (!m_rotate_reverse_flag) {
        m_rotate_reverse_flag = true;
      }
    }
    ImGui::SameLine();
    if (ImGui::Button("Invert Translation")) {
      // start/stop rotation
      if (m_translate_reverse_flag) {
        m_translate_reverse_flag = false;
      } else if (!m_translate_reverse_flag) {
        m_translate_reverse_flag = true;
      }
    }
    if (ImGui::Button("Start/Stop Rotation")) {
      // start/stop rotation
      if (m_rotate_flag) {
        m_rotate_flag = false;
      } else if (!m_rotate_flag) {
        m_rotate_flag = true;
      }
    }
    ImGui::SameLine();
    if (ImGui::Button("Start/Stop Translation")) {
      // start/stop rotation
      if (m_translate_flag) {
        m_translate_flag = false;
      } else if (!m_translate_flag) {
        m_translate_flag = true;
      }
    }
    ImGui::End();
    ImGui::Render();
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Check the keyboard input
    while(SDL_PollEvent(&m_event) != 0)
    {
      // Process keyboard and mouse events
      Keyboard();
    }

    // Swap to the Window
    m_window->Swap();
  }
}

void Engine::Keyboard()
{
  if(m_event.type == SDL_QUIT)
  {
    m_running = false;
  }
  else if (m_event.type == SDL_KEYDOWN)
  {
    // handle key down events here
    if (m_event.key.keysym.sym == SDLK_ESCAPE){
      m_running = false;
    }

    // start/stop rotation
    if (m_event.key.keysym.sym == SDLK_LEFT && m_rotate_flag){
      m_rotate_flag = false;
    }
    else if (m_event.key.keysym.sym == SDLK_LEFT && !m_rotate_flag){
      m_rotate_flag = true;
    }

    // invert rotation direction
    if (m_event.key.keysym.sym == SDLK_UP && !m_rotate_reverse_flag){
      m_rotate_reverse_flag = true;
    }
    else if (m_event.key.keysym.sym == SDLK_UP && m_rotate_reverse_flag){
      m_rotate_reverse_flag = false;
    }

    // start/stop translation
    if (m_event.key.keysym.sym == SDLK_RIGHT && m_translate_flag){
      m_translate_flag = false;
    }
    else if (m_event.key.keysym.sym == SDLK_RIGHT && !m_translate_flag){
      m_translate_flag = true;
    }

    // invert translate direction
    if (m_event.key.keysym.sym == SDLK_DOWN && !m_translate_reverse_flag){
      m_translate_reverse_flag = true;
    }
    else if (m_event.key.keysym.sym == SDLK_DOWN && m_translate_reverse_flag){
      m_translate_reverse_flag = false;
    }
  }
//  else if (m_event.type == SDL_MOUSEBUTTONDOWN){
//    // invert rotation direction
//    if (!m_stop){
//      m_rotate_reverse_flag = true;
//    }
//    else if (m_rotate_reverse_flag){
//      m_rotate_reverse_flag = false;
//    }
//  }
}

unsigned int Engine::getDT()
{
  long long TimeNowMillis = GetCurrentTimeMillis();
  assert(TimeNowMillis >= m_currentTimeMillis);
  unsigned int DeltaTimeMillis = (unsigned int)(TimeNowMillis - m_currentTimeMillis);
  m_currentTimeMillis = TimeNowMillis;
  return DeltaTimeMillis;
}

long long Engine::GetCurrentTimeMillis()
{
  timeval t;
  gettimeofday(&t, NULL);
  long long ret = t.tv_sec * 1000 + t.tv_usec / 1000;
  return ret;
}
