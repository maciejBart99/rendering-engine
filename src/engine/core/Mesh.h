#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "SimpleShader.h"

#include <string>
#include <vector>

using namespace std;

struct Vertex {
  glm::vec3 Position;
  glm::vec3 Normal;
  glm::vec2 TexCoords;
  glm::vec3 Tangent;
};

struct Texture {
  GLuint id;
  string path;
  string type;
};

struct Material {
  glm::vec3 ks;
  glm::vec3 kd;
  glm::vec3 ka;
  float opacity;
  int shiness;
  vector<Texture> textures;
};

class Mesh {
 private:
  vector<unsigned int> indices;
  Material material;
  unsigned int VAO, VBO, EBO;

 public:
  Mesh(vector<Vertex> vertices, vector<unsigned int> indices, Material material);
  void render(SimpleShader &shader);
};
#endif