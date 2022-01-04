#ifndef ENGINE_COMPONENT_H
#define ENGINE_COMPONENT_H

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "data_model.h"
#include "../core/Model.h"

using namespace std;


class Component {
 private:
  vector<shared_ptr<Model>> models;
  vector<shared_ptr<SimpleShader>> shaders;
  glm::vec3 position;
  glm::vec3 rotations;
  glm::vec3 scale = glm::vec3(1, 1, 1);
  vector<const char*> vertexShaderPaths;
  vector<const char*> fragmentShaderPaths;
  vector<const char*> modelPaths;
  vector<shared_ptr<Light>> lights;
  vector<shared_ptr<Camera>> camera;
  vector<glm::vec3> modelPosition;
  vector<glm::vec3> modelRotations;
  vector<glm::vec3> cameraOffsets;
  vector<glm::vec3> lightOffsets;
  vector<glm::vec3> lightDirections;
  vector<glm::vec3> cameraFronts;

  void updateCooridnates();
 protected:
  bool useLighting = true;

  int addModel(const char* modelPath, const char* vertexShader,
                const char* fragmentShader, glm::vec3 pos = glm::vec3(0, 0, 0),
               glm::vec3 rotation = glm::vec3(0, 0, 0));
  int addPointLight(glm::vec3 ambient, glm::vec3 specular,
                    glm::vec3 diffuse, float constant = 1, float linear = 0.1, float q = 0.03);
  int addSpotLight(glm::vec3 ambient, glm::vec3 specular, glm::vec3 direction, float cutOff, float outerCutOff,
                    glm::vec3 diffuse, float constant = 1, float linear = 0.1, float q = 0.03);
  int addDirLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 specular,
                    glm::vec3 diffuse);
  int addCamera();

  glm::vec3& getModelPosition(int i) {
      return modelPosition[i];
  }
  glm::vec3& getRotationPosition(int i) {
      return modelRotations[i];
  }
  glm::vec3& getLightOffset(int i) {
    return lightOffsets[i];
  }
  glm::vec3& getCameraOffset(int i) {
    return cameraOffsets[i];
  }
  glm::vec3& getLightDirections(int i) {
    return lightDirections[i];
  }
  glm::vec3& getCameraFronts(int i) {
    return cameraFronts[i];
  }
  shared_ptr<Light> getLight(int i) {
      return lights[i];
  }
  shared_ptr<Camera> getCamera(int i) const {
    return camera[i];
  }

 public:
  virtual ~Component();

  virtual void init(SceneState& state) = 0;
  virtual void frameUpdate(SceneState& state) = 0;

  void render(const SceneState& state);
  void renderDepthMap(SimpleShader& depthShader);
  void injectResources(vector<shared_ptr<Model>> models, vector<shared_ptr<SimpleShader>> shaders);

  const vector<const char*>& getVertexShaderPaths() const;
  const vector<const char*>& getFragmentShaderPaths() const;
  const vector<const char*>& getModelPaths() const;
  const vector<shared_ptr<Camera>>& getCamera() const;
  const vector<shared_ptr<Light>>& getLights() const;

  void translate(glm::vec3 delta);
  void rotate(glm::vec3 delta);

  const glm::vec3& getPosition() const;
  const glm::vec3& getRotations() const;
  const glm::vec3& getScale() const;

  void setPosition(const glm::vec3& position);
  void setRotations(const glm::vec3& rotations);
  void setScale(const glm::vec3& scale);
};

#endif
