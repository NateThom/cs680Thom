#ifndef ENGINE_H
#define ENGINE_H

#include <sys/time.h>
#include <assert.h>

#include "window.h"
#include "graphics.h"
#include "config.h"

class Engine
{
  public:
    explicit Engine(string config_filepath);
    ~Engine();
    bool Initialize();
    void Run();
    void Keyboard();
    unsigned int getDT();
    long long GetCurrentTimeMillis();
  
  private:
    // Window related variables
    Window *m_window;    
    string m_WINDOW_NAME;
    string m_VERTEX_FILEPATH;
    string m_FRAGMENT_FILEPATH;
    string m_OBJECT_FILEPATH;
    string m_MATERIAL_FILEPATH;
    int m_WINDOW_WIDTH;
    int m_WINDOW_HEIGHT;
    bool m_FULLSCREEN;
    SDL_Event m_event;

    Graphics *m_graphics;
    unsigned int m_DT;
    long long m_currentTimeMillis;
    bool m_running;

    Configuration m_config;
    bool cameraLock;
};

#endif // ENGINE_H
