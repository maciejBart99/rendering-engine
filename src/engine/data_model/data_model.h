#ifndef DATA_MODEL_H
#define DATA_MODEL_H

#include "../core/Camera.h"

#include <glm/glm.hpp>

#include <vector>
#include <set>

using namespace std;

class Engine;
class Component;

struct Fog
{
  glm::vec3 color;
  float start;
  float end;
  float density;
};

struct SkyboxTextures {
  int width;
  int height;
  vector<unsigned char *> faces;
};

struct Light
{
  enum class LightType { DIRECTIONAL = 0, SPOT = 1, POINT = 2 } type = LightType::DIRECTIONAL;
  glm::vec3 position;
  glm::vec3 direction;
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
  float constant;
  float linear;
  float quadratic;
  float cutOff;
  float outerCutOff;
};

struct SceneState
{
  int width;
  int height;

  Engine* engine;
  vector<unique_ptr<Component>> components;

  Camera* activeCamera = nullptr;
  Fog fog;
  set<int> activeKeys;
  float deltaTime;
  float lightness;

  enum class LightModel { PHONG = 0, BLINN = 1 } lightModel = LightModel::BLINN;
  enum class ShadingMode { PHONG = 0, GOURAUD = 1 } shadingModel = ShadingMode::PHONG;
};

#endif
