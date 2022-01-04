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

struct Skybox {
  int width;
  int height;
  vector<unsigned char *> faces;
};

struct Light
{
  enum LightType { DIRECTIONAL = 0, SPOT = 1, POINT = 2 } type;
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
  Engine* engine;
  enum LightModel { PHONG = 0, BLINN = 1 } lightModel = BLINN;
  enum ShadingMode { SH_PHONG = 0, GOURAUD = 1 } shadingModel = SH_PHONG;
  vector<shared_ptr<Component>> components;
  float deltaTime;
  float lightness;
  shared_ptr<Camera> activeCamera;
  unique_ptr<Fog> fog;
  set<int> activeKeys;
  int width;
  int height;
};

#endif
