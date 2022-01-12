#include "Engine.h"

#include <utility>

void Engine::initializeResources() {
  for (const auto &component: state.components) {
    auto& modelPaths = component->getModelPaths();
    auto& shaderVPaths = component->getVertexShaderPaths();
    auto& shaderFPaths = component->getFragmentShaderPaths();

    vector<shared_ptr<Model>> models;
    vector<shared_ptr<SimpleShader>> shaders;

    for (auto& path: modelPaths) {
      if (modelsCache.count(path)) {
        models.push_back(modelsCache[path]);
      } else {
        shared_ptr<Model> model(new Model(path));
        models.push_back(model);
        modelsCache.insert(make_pair(path, model));
      }
    }

    for (int i = 0; i < shaderVPaths.size(); i++) {
      auto& vPath = shaderVPaths[i];
      auto& fPath = shaderFPaths[i];
      auto key = string(vPath) + "/" + string(fPath);

      if (shadersCache.count(key)) {
        shaders.push_back(shadersCache[key]);
      } else {
        shared_ptr<SimpleShader> shader(new SimpleShader(vPath, fPath));
        shaders.push_back(shader);
        shadersCache.insert(make_pair(key, shader));
      }
    }

    component->injectResources(models, shaders);
  }
}

void Engine::updateShaders() {
  glm::mat4 projection = glm::perspective(glm::radians(state.activeCamera->Zoom),
                                          (float)state.width / (float)state.height,
                                          0.1f, 100.0f);
  glm::mat4 view = state.activeCamera->GetViewMatrix();

  auto lights = getLights();
  string lightTypes[] { "dir", "spot", "point" };

  for (const auto& shaderEntry: shadersCache) {
    map<Light::LightType, int> counts;

    for (auto& light: lights) {
      int index = counts.count(light->type) ? counts[light->type] : 0;
      counts[light->type] = index + 1;

      string key = lightTypes[(int)light->type] + "Lights[" + to_string(index) + "]";
      shaderEntry.second->set(key + ".position", light->position);
      shaderEntry.second->set(key + ".direction", light->direction);
      shaderEntry.second->set(key + ".ambient", light->ambient);
      shaderEntry.second->set(key + ".diffuse", light->diffuse);
      shaderEntry.second->set(key + ".specular", light->specular);
      shaderEntry.second->set(key + ".constant", light->constant);
      shaderEntry.second->set(key + ".linear", light->linear);
      shaderEntry.second->set(key + ".quadratic", light->quadratic);
      shaderEntry.second->set(key + ".cutOff", light->cutOff);
      shaderEntry.second->set(key + ".outerCutOff", light->outerCutOff);
    }

    shaderEntry.second->set("fog.start", state.fog.start);
    shaderEntry.second->set("fog.end", state.fog.end);
    shaderEntry.second->set("fog.density", state.fog.density);
    shaderEntry.second->set("fog.color", state.fog.color);

    shaderEntry.second->set("useGouraud", (int) state.shadingModel);
    shaderEntry.second->set("useBlinn", (int) state.lightModel);
    shaderEntry.second->set("cameraFront", state.activeCamera->getFront());
    shaderEntry.second->set("pointLightsCount", counts[Light::LightType::POINT]);
    shaderEntry.second->set("dirLightsCount", counts[Light::LightType::DIRECTIONAL]);
    shaderEntry.second->set("spotLightsCount", counts[Light::LightType::SPOT]);
    shaderEntry.second->set("cameraPosition", state.activeCamera->getPosition());
    shaderEntry.second->set("projection", projection);
    shaderEntry.second->set("view", view);
  }
}

void Engine::initializeComponentModel() {
  initializeResources();

  for (auto& component: state.components) {
    auto& camera = component->getCamera();
    if (state.activeCamera == nullptr && !camera.empty()) {
      state.activeCamera = camera[0].get();
    }

    component->init(state);
  }
}
void Engine::nextFrame(float deltaTime, const set<int> &keys) {
  glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  updateShaders();

  if (skybox != nullptr) {
    skybox->render(state);
  }

  state.deltaTime = deltaTime;
  state.activeKeys = keys;

  for (auto& component: state.components) {
    component->frameUpdate(state);
  }

  for (auto& component: state.components) {
    component->render(state);
  }
}
vector<Light*> Engine::getLights() {
  vector<Light*> result;

  for (auto& component: state.components) {
    if (!component->getLights().empty()) {
      for (auto& light: component->getLights()) {
        result.push_back(light.get());
      }
    }
  }

  return result;
}

Engine::Engine(vector<unique_ptr<Component>> components, int width, int height)
{
  state.components = move(components);
  state.height = height;
  state.width = width;
  state.lightness = 0.2;
  state.engine = this;

  state.fog.density = 0.02;
  state.fog.color = glm::vec3(0.1, 0.1, 0.1);

}
void Engine::addSkybox(const SkyboxTextures& textures, const char* vPath, const char* fPath) {
  skybox.reset(new Skybox(textures, vPath, fPath));
}