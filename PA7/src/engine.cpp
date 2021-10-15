
#include "engine.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"


// Engine parameterized constructor. Takes configuration filepath as input
Engine::Engine(string config_filepath)
{
  m_config.LoadConfigFile(config_filepath);
}

// Engine destructor. Deletes pointers and sets them to null before the object goes out of scope.
Engine::~Engine()
{
  delete m_window;
  delete m_graphics;
  m_window = NULL;
  m_graphics = NULL;
}

// Engine Initialize. Prepares important object member variables, particularly those that come from other classes.
// Prepares the window object, graphics object and start time in milliseconds
bool Engine::Initialize()
{
  // Start a window
  m_window = new Window();
  if(!m_window->Initialize(m_config.GetWindowName(), m_config.GetWindowWidthAddress(), m_config.GetWindowHeightAddress()))
  {
    printf("The window failed to initialize.\n");
    return false;
  }
  
  cameraLock = false;
  // Start the graphics
  m_graphics = new Graphics(cameraLock);
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

// Engine Run. This is the active loop of the program and handles updating the GUI, accepting various keyboard inputs,
// and watching for a program end/exit command.
void Engine::Run()
{
  // Program end/exit boolean
  m_running = true;

  // Continue looping while the program has not been ended/exited
  while(m_running)
  {
    // Update the DT
    m_DT = getDT();

    // Check the keyboard input
    while(SDL_PollEvent(&m_event) != 0)
    {
      // If there is keyboard input, handle it according to Keyboard()
      Keyboard();
    }

    // Update and render the graphics
    m_graphics->Update(m_DT);
    m_graphics->Render(m_running, m_DT);
    m_graphics->setDT(m_DT);

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
    // -----------------------------------------------
    // controls to end program
    // -----------------------------------------------
    if( m_event.key.keysym.sym == SDLK_ESCAPE )
    {
      m_running = false;
    }

    // -----------------------------------------------
    // camera adjustments
    // -----------------------------------------------

    // zoom out
    else if( m_event.key.keysym.sym == SDLK_s )
    {
      m_graphics->ZoomOut();
      cameraLock = false;
    }

    // zoom in
    else if( m_event.key.keysym.sym == SDLK_w )
    {
      m_graphics->ZoomIn();
      cameraLock = false;
    }

    else if(m_event.key.keysym.sym == SDLK_a)
    {
      m_graphics->MoveLeft();
      cameraLock = false;
    }

    else if(m_event.key.keysym.sym == SDLK_d)
    {
      m_graphics->MoveRight();
      cameraLock = false;
    }

    // look up
    else if( m_event.key.keysym.sym == SDLK_UP )
    {
      m_graphics->LookUp();
      cameraLock = false;
    }

    // look down
    else if( m_event.key.keysym.sym == SDLK_DOWN )
    {
      m_graphics->LookDown();
      cameraLock = false;
    }

    else if( m_event.key.keysym.sym == SDLK_LEFT )
    {
      m_graphics->CameraSpeedDown();
    }

    // look down
    else if( m_event.key.keysym.sym == SDLK_RIGHT )
    {
      m_graphics->CameraSpeedUp();
    }

    // look left
    else if( m_event.key.keysym.sym == SDLK_q )
    {
      m_graphics->MoveUp();
      cameraLock = false;
    }

    // look right
    else if( m_event.key.keysym.sym == SDLK_e )
    {
      m_graphics->MoveDown();
      cameraLock = false;
    }

    // Reset the camera to default
    else if( m_event.key.keysym.sym == SDLK_SPACE)
    {
      m_graphics->ResetCamera();
      cameraLock = false;
    }

    // Snap camera to mercury
    else if(m_event.key.keysym.sym == SDLK_1){
      cameraLock = true;
      m_graphics->setCameraLock(1);
    }

      // Snap camera to venus
    else if(m_event.key.keysym.sym == SDLK_2){
      cameraLock = true;
      m_graphics->setCameraLock(2);
    }


      // Snap camera to earth
    else if(m_event.key.keysym.sym == SDLK_3){
      cameraLock = true;
      m_graphics->setCameraLock(3);
    }

      // Snap camera to mars
    else if(m_event.key.keysym.sym == SDLK_4){
      cameraLock = true;
      m_graphics->setCameraLock(5);
    }

      // Snap camera to jupiter
    else if(m_event.key.keysym.sym == SDLK_5){
      cameraLock = true;
      m_graphics->setCameraLock(6);
    }

      // Snap camera to saturn
    else if(m_event.key.keysym.sym == SDLK_6){
      cameraLock = true;
      m_graphics->setCameraLock(11);
    }

      // Snap camera to uranus
    else if(m_event.key.keysym.sym == SDLK_7){
      cameraLock = true;
      m_graphics->setCameraLock(13);
    }

      // Snap camera to neptune
    else if(m_event.key.keysym.sym == SDLK_8){
      cameraLock = true;
      m_graphics->setCameraLock(14);
    }

      // Snap camera to pluto
    else if(m_event.key.keysym.sym == SDLK_9){
      cameraLock = true;
      m_graphics->setCameraLock(15);
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
