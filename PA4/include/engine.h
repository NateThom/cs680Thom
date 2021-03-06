#ifndef ENGINE_H
#define ENGINE_H

#include <sys/time.h>
#include <assert.h>
#include <string>

#include "graphics.h"
#include "window.h"

class Engine
{
  public:
    Engine(string name, int width, int height);
    Engine(string name);
    ~Engine();
    bool Initialize(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath,
                    const std::string& modelFilePath);
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

    float m_planet_rotation_magnitude;
    float m_planet_translation_speed_magnitude;
    float m_planet_scale;

    float m_moon_rotation_magnitude;
    float m_moon_translation_speed_magnitude;
    float m_moon_scale;
};

#endif // ENGINE_H
