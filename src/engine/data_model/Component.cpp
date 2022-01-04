#include "Component.h"
#include "glm/gtx/euler_angles.hpp"

#include <utility>

void Component::render(const SceneState& state) {
  updateCooridnates();

  for (int i = 0; i < models.size(); i++) {
    auto& shader = shaders[i];
    auto& model = models[i];

    shader->use();
    glm::mat4 modelM = glm::mat4(1.0f);
    modelM = glm::translate(modelM, position + modelPosition[i]);
    modelM = glm::rotate(modelM, rotations.y + modelRotations[i].y, glm::vec3(0, 1, 0));
    modelM = glm::scale(modelM, scale);
    shader->set("model", modelM);
    shader->set("useLights", (int)useLighting);

    model->render(*shader);
  }
}
const vector<const char*>& Component::getVertexShaderPaths() const {
  return vertexShaderPaths;
}
const vector<const char*>& Component::getFragmentShaderPaths() const {
  return fragmentShaderPaths;
}
const vector<const char*>& Component::getModelPaths() const {
  return modelPaths;
}
void Component::injectResources(vector<shared_ptr<Model>> models, vector<shared_ptr<SimpleShader>> shaders) {
  this->models = move(models);
  this->shaders = move(shaders);
}
const vector<shared_ptr<Camera>>& Component::getCamera() const {
  return camera;
}
const vector<shared_ptr<Light>>& Component::getLights() const {
  return lights;
}
void Component::translate(glm::vec3 delta)
{
  position += delta;
  std::for_each(lights.begin(), lights.end(),
                [&delta] (const shared_ptr<Light>& light) { light->position += delta; });
  std::for_each(camera.begin(), camera.end(),
                [&delta] (const shared_ptr<Camera>& cam) { cam->setPosition(cam->getPosition() + delta); });
}
void Component::rotate(glm::vec3 delta)
{
  rotations += delta;
}
int Component::addModel(const char* modelPath,
                        const char* vertexShader,
                        const char* fragmentShader,
                        glm::vec3 pos,
                        glm::vec3 rotation) {
  vertexShaderPaths.push_back(vertexShader);
  fragmentShaderPaths.push_back(fragmentShader);
  modelPaths.push_back(modelPath);
  modelPosition.emplace_back(pos);
  modelRotations.emplace_back(rotation);

  return vertexShaderPaths.size() - 1;
}
int Component::addPointLight(glm::vec3 ambient, glm::vec3 specular, glm::vec3 diffuse,
                             float constant, float linear, float q) {
  shared_ptr<Light> light(new Light());
  light->type = Light::POINT;
  light->position = position;
  light->ambient = ambient;
  light->diffuse = diffuse;
  light->specular = specular;
  light->constant = constant;
  light->linear = linear;
  light->quadratic = q;

  lights.push_back(light);
  lightDirections.push_back(glm::vec3(0, 0, 0));
  lightOffsets.push_back(glm::vec3(0, 0, 0));

  return lights.size() - 1;
}
int Component::addDirLight(glm::vec3 direction, glm::vec3 ambient,
                           glm::vec3 specular, glm::vec3 diffuse) {
  shared_ptr<Light> light(new Light());
  light->type = Light::DIRECTIONAL;
  light->direction = direction;
  light->ambient = ambient;
  light->diffuse = diffuse;
  light->specular = specular;

  lights.push_back(light);
  lightDirections.push_back(direction);
  lightOffsets.push_back(glm::vec3(0, 0, 0));

  return lights.size() - 1;
}

int Component::addCamera() {
  camera.push_back(shared_ptr<Camera>(new Camera(position)));
  cameraOffsets.emplace_back(0, 0, 0);
  cameraFronts.emplace_back(0, 0, 1);

  return camera.size() - 1;
}
void Component::renderDepthMap(SimpleShader& shader) {

}
const glm::vec3& Component::getPosition() const {
  return position;
}
const glm::vec3& Component::getRotations() const {
  return rotations;
}
void Component::setPosition(const glm::vec3& pos) {
  position = pos;

  updateCooridnates();
}
void Component::setRotations(const glm::vec3& rot) {
  rotations = rot;
}
const glm::vec3& Component::getScale() const {
  return scale;
}
void Component::setScale(const glm::vec3& scale) {
  Component::scale = scale;
}
int Component::addSpotLight(glm::vec3 ambient, glm::vec3 specular, glm::vec3 direction, float cutOff, float outerCutOff,
                    glm::vec3 diffuse, float constant, float linear, float q)
{
  shared_ptr<Light> light(new Light());
  light->type = Light::SPOT;
  light->position = position;
  light->ambient = ambient;
  light->diffuse = diffuse;
  light->specular = specular;
  light->constant = constant;
  light->linear = linear;
  light->quadratic = q;
  light->specular = specular;
  light->cutOff = cutOff;
  light->outerCutOff = outerCutOff;
  light->direction = direction;

  lights.push_back(light);
  lightDirections.push_back(direction);
  lightOffsets.push_back(glm::vec3(0, 0, 0));

  return lights.size() - 1;
}

void Component::updateCooridnates() {
  auto rotationMatrix = glm::rotate(glm::mat4(1.0f), rotations.y, glm::vec3(0, 1.0f, 0));

  for (int i = 0; i < lightOffsets.size(); ++i) {
    lights[i]->position = position + glm::vec3(rotationMatrix * glm::vec4(lightOffsets[i], 1.0));
    lights[i]->direction = rotationMatrix * glm::vec4(lightDirections[i], 1.0);
  }

  for (int i = 0; i < cameraOffsets.size(); ++i) {
    camera[i]->Position = position + glm::vec3(rotationMatrix * glm::vec4(cameraOffsets[i], 1.0));

    camera[i]->setFront(rotationMatrix * glm::vec4(cameraFronts[i], 1.0));
  }
}

Component::~Component() = default;
