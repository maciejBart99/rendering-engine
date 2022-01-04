#ifndef ENGINE_H
#define ENGINE_H

#include "../data_model/data_model.h"
#include "../data_model/Component.h"
#include <map>

using namespace std;

class Engine {
 private:
    SceneState state;
    GLuint skyboxVAO, skyboxVBO, skyboxTexture = -1;
    map<string, shared_ptr<SimpleShader>> shadersCache;
    map<string, shared_ptr<Model>> modelsCache;
    shared_ptr<SimpleShader> skyboxShader;
 public:
  Engine(vector<shared_ptr<Component>> components, int width, int height);
  void initializeComponentModel();
  void nextFrame(float deltaTime, const set<int> &keys);
  void addSkybox(const Skybox& skybox, const char* shaderVertex, const char* shaderFragment);
 private:
  void updateShaders();
  void initializeResources();
  void renderSkybox(glm::mat4 projection);
  vector<shared_ptr<Light>> getLights();
};

#endif
