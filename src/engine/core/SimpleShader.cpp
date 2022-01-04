#include "SimpleShader.h"

#define MSG_BUFF 1024

SimpleShader::SimpleShader(const char* vertexPath, const char* fragmentPath) {
  std::string vCode = readShaderSourceCode(vertexPath);
  std::string fCode = readShaderSourceCode(fragmentPath);

  const char* cvCode = vCode.c_str();
  const char* cfCode = fCode.c_str();

  GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &cvCode, nullptr);
  glCompileShader(vertex);
  checkCompilationResult(vertex, false);

  GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &cfCode, nullptr);
  glCompileShader(fragment);
  checkCompilationResult(fragment, false);

  openglId = glCreateProgram();
  glAttachShader(openglId, vertex);
  glAttachShader(openglId, fragment);
  glLinkProgram(openglId);
  checkCompilationResult(openglId, true);

  glDeleteShader(vertex);
  glDeleteShader(fragment);
}
SimpleShader::SimpleShader(const char* vertexPath, const char* fragmentPath,
                           const char* geometryShader)
{
  std::string vCode = readShaderSourceCode(vertexPath);
  std::string fCode = readShaderSourceCode(fragmentPath);
  std::string gCode = readShaderSourceCode(geometryShader);

  const char* cvCode = vCode.c_str();
  const char* cfCode = fCode.c_str();
  const char* cgCode = gCode.c_str();

  GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &cvCode, nullptr);
  glCompileShader(vertex);
  checkCompilationResult(vertex, false);

  GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &cfCode, nullptr);
  glCompileShader(fragment);
  checkCompilationResult(fragment, false);

  GLuint geometry = glCreateShader(GL_GEOMETRY_SHADER);
  glShaderSource(geometry, 1, &cgCode, nullptr);
  glCompileShader(geometry);
  checkCompilationResult(geometry, false);

  openglId = glCreateProgram();
  glAttachShader(openglId, vertex);
  glAttachShader(openglId, fragment);
  glAttachShader(openglId, geometry);
  glLinkProgram(openglId);
  checkCompilationResult(openglId, true);

  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

std::string readShaderSourceCode(const char* path) {
  std::ifstream file;

  file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  try {
    file.open(path);
    std::stringstream stream;
    stream << file.rdbuf();

    return stream.str();
  } catch (std::ifstream::failure& e) {
    std::cout << "IO Shader error" << std::endl;
    throw e;
  }
}

void checkCompilationResult(GLuint shader, bool linking)
{
  GLint success;
  GLchar infoLog[MSG_BUFF];

  if(linking)
  {
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if(!success)
    {
      glGetProgramInfoLog(shader, MSG_BUFF, nullptr, infoLog);
      std::cout << "Shader link error\n" << infoLog << std::endl;
    }
  }
  else
  {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
      glGetShaderInfoLog(shader, MSG_BUFF, nullptr, infoLog);
      std::cout << "Shader compile error\n" << infoLog << std::endl;
    }
  }
}