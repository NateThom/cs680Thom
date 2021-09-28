
#include "engine.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"


Engine::Engine(string name, int width, int height)
{
  m_WINDOW_NAME = name;
  m_WINDOW_WIDTH = width;
  m_WINDOW_HEIGHT = height;
}

Engine::Engine(string name, int width, int height, std::string v_path, std::string f_path, std::string o_path, std::string m_path)
{
  m_WINDOW_NAME = name;
  m_WINDOW_WIDTH = width;
  m_WINDOW_HEIGHT = height;
  m_FULLSCREEN = false;
  m_VERTEX_FILEPATH = v_path;
  m_FRAGMENT_FILEPATH = f_path;
  m_OBJECT_FILEPATH = o_path;
  m_MATERIAL_FILEPATH = m_path;
}

Engine::Engine(string name)
{
  m_WINDOW_NAME = name;
  m_WINDOW_HEIGHT = 0;
  m_WINDOW_WIDTH = 0;
  m_FULLSCREEN = true;
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
  m_graphics = new Graphics(m_VERTEX_FILEPATH, m_FRAGMENT_FILEPATH, m_OBJECT_FILEPATH, m_MATERIAL_FILEPATH);
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
      Mouse();
    }

    // Update and render the graphics
    m_graphics->Update(m_DT);
    m_graphics->Render(m_running, m_DT);

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
  else if (m_event.type == SDL_KEYUP)
  {
    // handle key down events here
    if (m_event.key.keysym.sym == SDLK_ESCAPE)
    {
      m_running = false;
    }
    if (m_event.key.keysym.sym == SDLK_DOWN)
    {
      if(m_graphics->getPlanetOrbitSpeed() != 0) {
        orbit_speed = m_graphics->getPlanetOrbitSpeed();
        m_graphics->setPlanetOrbitSpeed(0.0f);
      }
      else
        m_graphics->setPlanetOrbitSpeed(orbit_speed);
    }
    if (m_event.key.keysym.sym == SDLK_UP)
    {
      if(m_graphics->getPlanetSpinSpeed() != 0) {
        spin_speed = m_graphics->getPlanetSpinSpeed();
        m_graphics->setPlanetSpinSpeed(0.0f);
      }
      else
        m_graphics->setPlanetSpinSpeed(spin_speed);
    }
    if (m_event.key.keysym.sym == SDLK_LEFT)
    {
      orbit_speed = m_graphics->getPlanetOrbitSpeed();
      m_graphics->setPlanetOrbitSpeed(-orbit_speed);
    }
    if (m_event.key.keysym.sym == SDLK_RIGHT)
    {
      spin_speed = m_graphics->getPlanetSpinSpeed();
      m_graphics->setPlanetSpinSpeed(-spin_speed);
    }
  }
}

void Engine::Mouse()
{
  if (m_event.type == SDL_MOUSEBUTTONUP)
  {
    if (m_event.button.button == SDL_BUTTON_RIGHT)
    {
      m_graphics->setPlanetSpinSpeed(0.0f);
      m_graphics->setPlanetOrbitSpeed(0.0f);
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
