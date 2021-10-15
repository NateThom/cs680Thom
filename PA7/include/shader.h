#ifndef SHADER_H
#define SHADER_H

#include <vector>

#include "graphics_headers.h"
#include <fstream>
#include <sstream>

class Shader
{
  public:
    Shader();
    Shader(std::string v_path, std::string f_path);
    ~Shader();
    bool Initialize();
    void Enable();
    bool AddShader(GLenum ShaderType);
    bool Finalize();
    GLint GetUniformLocation(const char* pUniformName);

  private:
    GLuint m_shaderProg;    
    std::vector<GLuint> m_shaderObjList;
    std::string m_VERTEX_FILEPATH;
    std::string m_FRAGMENT_FILEPATH;
};

#endif  /* SHADER_H */
