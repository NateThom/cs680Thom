
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

  // Set the time
  m_currentTimeMillis = GetCurrentTimeMillis();

  // No errors
  return true;
}

void Engine::Run()
{
  m_running = true;

  while(m_running)
  {
    // Update the DT
    m_DT = getDT();

    // Check the keyboard input
    while(SDL_PollEvent(&m_event) != 0)
    {
      Keyboard();
    }
    // Update and render the graphics
    m_graphics->Update(m_DT, m_rotate_flag, m_rotate_reverse_flag, m_translate_flag, m_translate_reverse_flag);
    m_graphics->Render();

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
