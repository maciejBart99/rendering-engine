#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "SimpleShader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

class Model 
{
 private:
  vector<Texture> texturesCache;
  map<string, int> meshIndices;
  vector<Mesh> meshes;
  string directory;

 public:
  explicit Model(string const &path)
  {
    load(path);
  }
  void render(SimpleShader &shader)
  {
    for(auto & mesh : meshes) mesh.render(shader);
  }
  Mesh& getMeshByName(const string& name);

 private:
  void load(string const &path);
  void processNode(aiNode *node, const aiScene *scene);
  Mesh processMesh(aiMesh *mesh, const aiScene *scene);
  vector<Texture> loadMaterial(aiMaterial *mat, aiTextureType type, const string& typeName);
};

GLuint loadTexture(const char *path);

#endif