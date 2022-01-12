#ifndef SKYBOX_H
#define SKYBOX_H

#include "glad/glad.h"
#include "SimpleShader.h"
#include "../data_model/data_model.h"

using namespace std;

class Skybox {
 private:
  GLuint skyboxVAO{}, skyboxVBO{}, skyboxTexture = -1;
  unique_ptr<SimpleShader> skyboxShader;
 public:
  Skybox(const SkyboxTextures& skybox, const char* vPath, const char* fPath);
  void render(SceneState& state);
};

#endif
