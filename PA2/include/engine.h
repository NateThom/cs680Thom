#ifndef ENGINE_H
#define ENGINE_H

#include <sys/time.h>
#include <assert.h>

#include "graphics.h"
#include "window.h"

class Engine
{
  public:
    Engine(string name, int width, int height);
    Engine(string name);
    ~Engine();
    bool Initialize();
    bool Initialize(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);
    void Run();
    void Keyboard();
    unsigned int getDT();
    long long GetCurrentTimeMillis();
  
  private:
    // Window related variables
    Window *m_window;    
    string m_WINDOW_NAME;
    int m_WINDOW_WIDTH;
    int m_WINDOW_HEIGHT;
    bool m_FULLSCREEN;
    SDL_Event m_event;

    Graphics *m_graphics;
    unsigned int m_DT;
    long long m_currentTimeMillis;
    bool m_running;

    const char* glsl_version;

    bool m_rotate_flag;
    bool m_translate_flag;
    bool m_rotate_reverse_flag;
    bool m_translate_reverse_flag;
};

#endif // ENGINE_H
