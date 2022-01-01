#ifndef SIMPLE_SHADER_H
#define SIMPLE_SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class SimpleShader
{
 private:
  unsigned int openglId;
 public:

  SimpleShader(const char* vertexPath, const char* fragmentPath);

  void use() const
  {
    glUseProgram(openglId);
  }

  void set(const std::string &name, bool value) const
  {
    glUniform1i(glGetUniformLocation(openglId, name.c_str()), (int)value);
  }

  void set(const std::string &name, int value) const
  {
    glUniform1i(glGetUniformLocation(openglId, name.c_str()), value);
  }

  void set(const std::string &name, float value) const
  {
    glUniform1f(glGetUniformLocation(openglId, name.c_str()), value);
  }

  void set(const std::string &name, const glm::vec2 &value) const
  {
    glUniform2fv(glGetUniformLocation(openglId, name.c_str()), 1, &value[0]);
  }

  void set(const std::string &name, float x, float y) const
  {
    glUniform2f(glGetUniformLocation(openglId, name.c_str()), x, y);
  }

  void set(const std::string &name, const glm::vec3 &value) const
  {
    glUniform3fv(glGetUniformLocation(openglId, name.c_str()), 1, &value[0]);
  }

  void set(const std::string &name, float x, float y, float z) const
  {
    glUniform3f(glGetUniformLocation(openglId, name.c_str()), x, y, z);
  }

  void set(const std::string &name, const glm::vec4 &value) const
  {
    glUniform4fv(glGetUniformLocation(openglId, name.c_str()), 1, &value[0]);
  }

  void set(const std::string &name, float x, float y, float z, float w) const
  {
    glUniform4f(glGetUniformLocation(openglId, name.c_str()), x, y, z, w);
  }

  void set(const std::string &name, const glm::mat2 &mat) const
  {
    glUniformMatrix2fv(glGetUniformLocation(openglId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
  }

  void set(const std::string &name, const glm::mat3 &mat) const
  {
    glUniformMatrix3fv(glGetUniformLocation(openglId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
  }

  void set(const std::string &name, const glm::mat4 &mat) const
  {
    glUniformMatrix4fv(glGetUniformLocation(openglId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
  }

  unsigned int getOpenglId() const { return openglId; }
};

std::string readShaderSourceCode(const char* path);

void checkCompilationResult(GLuint shader, bool linking);

#endif