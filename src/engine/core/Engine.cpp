#include "Engine.h"

#include <utility>

float skyboxVertices[] = {
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,
    1.0f,  1.0f, -1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
    1.0f, -1.0f,  1.0f
};

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

      string key = lightTypes[light->type] + "Lights[" + to_string(index) + "]";
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

    shaderEntry.second->set("fog.start", state.fog->start);
    shaderEntry.second->set("fog.end", state.fog->end);
    shaderEntry.second->set("fog.density", state.fog->density);
    shaderEntry.second->set("fog.color", state.fog->color);

    shaderEntry.second->set("useGouraud", state.shadingModel);
    shaderEntry.second->set("useBlinn", state.lightModel);
    shaderEntry.second->set("pointLightsCount", counts[Light::POINT]);
    shaderEntry.second->set("dirLightsCount", counts[Light::DIRECTIONAL]);
    shaderEntry.second->set("spotLightsCount", counts[Light::SPOT]);
    shaderEntry.second->set("cameraPosition", state.activeCamera->getPosition());
    shaderEntry.second->set("projection", projection);
    shaderEntry.second->set("view", view);
  }
}

void Engine::initializeComponentModel() {
  initializeResources();

  for (auto& component: state.components) {
    if (state.activeCamera == nullptr && !component->getCamera().empty()) {
      state.activeCamera = component->getCamera()[0];
    }

    component->init(state);
  }
}
void Engine::nextFrame(float deltaTime, const set<int> &keys) {
  glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  updateShaders();

  if (skyboxTexture != -1) {
    glm::mat4 projection = glm::perspective(glm::radians(state.activeCamera->Zoom),
                                            (float)state.width / (float)state.height,
                                            0.1f, 100.0f);
    renderSkybox(projection);
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
vector<shared_ptr<Light>> Engine::getLights() {
  vector<shared_ptr<Light>> result;

  for (auto& component: state.components) {
    if (!component->getLights().empty()) {
      result.insert(result.end(), component->getLights().begin(), component->getLights().end());
    }
  }

  return result;
}

Engine::Engine(vector<shared_ptr<Component>> components, int width, int height)
{
  state.components = move(components);
  state.height = height;
  state.width = width;
  state.lightness = 0.2;
  state.engine = this;

  state.fog = unique_ptr<Fog>(new Fog());
  state.fog->density = 0.02;
  state.fog->color = glm::vec3(0.1, 0.1, 0.1);

}
void Engine::addSkybox(const Skybox& skybox, const char* vPath, const char* fPath) {
  auto key = string(vPath) + "/" + string(fPath);

  if (shadersCache.count(key)) {
    skyboxShader = shadersCache[key];
  } else {
    shared_ptr<SimpleShader> shader(new SimpleShader(vPath, fPath));
    skyboxShader = shader;
    shadersCache.insert(make_pair(key, shader));
  }

  GLuint textureID;
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

  for (int i = 0; i < 6; i++) {
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, skybox.width, skybox.height, 0, GL_RGB, GL_UNSIGNED_BYTE, skybox.faces[i]);
  }

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  glGenVertexArrays(1, &skyboxVAO);
  glGenBuffers(1, &skyboxVBO);
  glBindVertexArray(skyboxVAO);
  glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)nullptr);

  skyboxTexture = textureID;
}
void Engine::renderSkybox(glm::mat4 projection) {
  glDepthMask(GL_FALSE);
  skyboxShader->use();
  glm::mat4 view = glm::mat4(glm::mat3(state.activeCamera->GetViewMatrix()));
  skyboxShader->set("skybox", 0);
  skyboxShader->set("view", view);
  skyboxShader->set("projection", projection);
  skyboxShader->set("fog.color", state.fog->color);
  skyboxShader->set("fog.density", state.fog->density);
  skyboxShader->set("lightness", state.lightness);

  glBindVertexArray(skyboxVAO);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glDepthMask(GL_TRUE);
}
