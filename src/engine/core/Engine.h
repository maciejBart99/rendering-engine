#ifndef ENGINE_H
#define ENGINE_H

#include "../data_model/data_model.h"
#include "../data_model/Component.h"
#include "./Skybox.h"
#include <map>

using namespace std;

class Engine {
 private:
    SceneState state;
    unique_ptr<Skybox> skybox;
    map<string, shared_ptr<SimpleShader>> shadersCache;
    map<string, shared_ptr<Model>> modelsCache;
 public:
  Engine(vector<unique_ptr<Component>> components, int width, int height);
  void initializeComponentModel();
  void nextFrame(float deltaTime, const set<int> &keys);
  void addSkybox(const SkyboxTextures& skybox, const char* shaderVertex, const char* shaderFragment);
 private:
  void updateShaders();
  void initializeResources();
  vector<Light*> getLights();
};

#endif
