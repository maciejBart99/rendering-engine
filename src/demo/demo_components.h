#ifndef DEMO_COMPONENTS_H
#define DEMO_COMPONENTS_H

#include "../engine/application/Application.h"


class Arena : public Component
{
 public:
  Arena() {
    addModel("resources/models/arena/arena.obj",
             "resources/shaders/basic-vertex.vs",
             "resources/shaders/basic-fragment.fs");
    int pLight1 = addPointLight(glm::vec3(1, 1, 0.5), 0.00001, 0.1, 0.01);
    getLightOffset(pLight1) = glm::vec3(22, 2, 0);
  }

  void init(SceneState& state) override { }

  void frameUpdate(SceneState& state) override { }
};

class CamHolder: public Component {
 public:
  virtual void choose() = 0;
};


class Car: public CamHolder
{
 private:
  float fi = 0;
  float tireSpinFi = 0;
  float r = 10;
  bool hasView = false;
  int cameraId;
  int carId;
  int spot1;
  int spot2;
  int point1;
  int point2;
  bool lightsOn = true;

 public:
  Car() {
    carId = addModel("resources/models/supercar/supercar.obj",
                     "resources/shaders/basic-vertex.vs",
                     "resources/shaders/basic-fragment.fs");
    setPosition(glm::vec3(10, 1, 0));

    spot1 = addSpotLight(glm::vec3(0, 0, 1),
                         glm::cos(glm::radians(15.0f)),
                         glm::cos(glm::radians(20.0f)),
                         glm::vec3(.8, 0.8, 1),
                         0.1, 0.01, 0.005);
    getLightOffset(spot1).x = -0.8;
    getLightOffset(spot1).y = 0.3;
    getLightOffset(spot1).z = 0.9;

    spot2 = addSpotLight(glm::vec3(0, 0, 1),
                         glm::cos(glm::radians(15.0f)),
                         glm::cos(glm::radians(20.0f)),
                         glm::vec3(0.8, 0.8, 1),
                         0.1, 0.01, 0.005);
    getLightOffset(spot2).x = 0.8;
    getLightOffset(spot2).z = 0.9;
    getLightOffset(spot2).y = 0.3;

    point1 = addPointLight( glm::vec3(0.8, 0.8, 1), 0.2, 0, 2);
    point2 = addPointLight(glm::vec3(0.8, 0.8, 1), 0.2, 0, 2);

    getLightOffset(point1).x = -0.8;
    getLightOffset(point1).z = 1.25;
    getLightOffset(point1).y = 0.75;

    getLightOffset(point2).x = 0.8;
    getLightOffset(point2).y = 0.75;
    getLightOffset(point2).z = 1.25;

    int point3 = addPointLight(glm::vec3(1, 0, 0), 0.2, 0, 2);
    getLightOffset(point3).z = -3.1;
    getLightOffset(point3).y = 0.5;


    cameraId = addCamera();
    getCameraOffset(cameraId).y = 1;
    getCameraOffset(cameraId).z = 5;
    getCameraOffset(cameraId).x = -4;

    getCameraFronts(cameraId) = glm::normalize(-getCameraOffset(cameraId));
  }

  void init(SceneState& state) override { }

  void frameUpdate(SceneState& state) override {
    float deltaRad = (state.deltaTime / 10.0f) * 2 * glm::pi<float>();

    tireSpinFi += state.deltaTime / 0.02 * 2 * glm::pi<float>();
    fi += deltaRad;


    auto& tire1 = getModel(carId)->getMeshByName("Wheel_01_RF.001_Wheel_01_RF.001");
    auto& tire1b = getModel(carId)->getMeshByName("Wheel_01_RF.001_Wheel_01_RF.003");
    auto& tire1c = getModel(carId)->getMeshByName("CaliperRF.001_CaliperRF.002");
    tire1.setUseIndividualTransformation(true);
    tire1b.setUseIndividualTransformation(true);
    tire1c.setUseIndividualTransformation(true);
    glm::mat4 tire1Mat = glm::translate(glm::mat4(1.0f), glm::vec3(-0.96, 0.36, 0.4));
    tire1Mat = glm::rotate(tire1Mat, -0.4f + (0.2f * glm::sin(2.0f * fi + 1.0f)), glm::vec3(0, 1, 0));
    glm::mat4 alt = glm::translate(tire1Mat, glm::vec3(0.96, -0.36, -0.4));
    tire1Mat = glm::rotate(tire1Mat, glm::radians(tireSpinFi), glm::vec3(1, 0, 0));
    tire1Mat = glm::translate(tire1Mat, glm::vec3(0.96, -0.36, -0.4));
    tire1.setIndividualTransformation(tire1Mat);
    tire1b.setIndividualTransformation(tire1Mat);
    tire1c.setIndividualTransformation(alt);

    auto& tire2 = getModel(carId)->getMeshByName("Wheel_01_LF.001_Wheel_01_LF.001");
    auto& tire2b = getModel(carId)->getMeshByName("Wheel_01_LF.001_Wheel_01_LF.003");
    tire2.setUseIndividualTransformation(true);
    tire2b.setUseIndividualTransformation(true);
    glm::mat4 tire2Mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.96, 0.36, 0.4));
    tire2Mat = glm::rotate(tire2Mat, -0.4f + (0.2f * glm::sin(2.0f * fi + 1.0f)), glm::vec3(0, 1, 0));
    tire2Mat = glm::rotate(tire2Mat, glm::radians(tireSpinFi), glm::vec3(1, 0, 0));
    tire2Mat = glm::translate(tire2Mat, glm::vec3(-0.96, -0.36, -0.4));
    tire2.setIndividualTransformation(tire2Mat);
    tire2b.setIndividualTransformation(tire2Mat);

    auto& tire3 = getModel(carId)->getMeshByName("Wheel_01_LR.001_Wheel_01_LR.002");
    tire3.setUseIndividualTransformation(true);
    glm::mat4 tire3Mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.95, 0.38, -2.44));
    tire3Mat = glm::rotate(tire3Mat, glm::radians(tireSpinFi), glm::vec3(1, 0, 0));
    tire3Mat = glm::translate(tire3Mat, glm::vec3(-0.95, -0.38, 2.44));
    tire3.setIndividualTransformation(tire3Mat);

    auto& tire4 = getModel(carId)->getMeshByName("Wheel_01_RR.001_Wheel_01_RR.002");
    tire4.setUseIndividualTransformation(true);
    glm::mat4 tire4Mat = glm::translate(glm::mat4(1.0f), glm::vec3(-1, 0.388, -2.44));
    tire4Mat = glm::rotate(tire4Mat, glm::radians(tireSpinFi), glm::vec3(1, 0, 0));
    tire4Mat = glm::translate(tire4Mat, glm::vec3(1, -0.388, 2.44));
    tire4.setIndividualTransformation(tire4Mat);

    getPosition().y = 1 + 0.004f * glm::sin(15 * fi);
    setPosition(glm::vec3(glm::cos(fi) * r, getPosition().y, glm::sin(fi) * r));
    setRotations(glm::vec3(0, -fi + (0.5 * glm::sin(2 * fi)), 0));

    if (hasView) {
      state.activeCamera = getCamera(cameraId);
      hasView = false;
    }
  }

  void choose() override {
    hasView = true;
  }

  void toggleLights() {
    if (lightsOn) {
      getLight(spot1)->diffuse = glm::vec3(0, 0, 0);
      getLight(spot2)->diffuse = glm::vec3(0, 0, 0);
      getLight(point1)->diffuse = glm::vec3(0, 0, 0);
      getLight(point2)->diffuse = glm::vec3(0, 0, 0);
    } else {
      getLight(spot1)->diffuse = glm::vec3(.8, 0.8, 1);
      getLight(spot2)->diffuse = glm::vec3(.8, 0.8, 1);
      getLight(point1)->diffuse = glm::vec3(1, 1, 1);
      getLight(point2)->diffuse = glm::vec3(1, 1, 1);
    }

    getLight(spot1)->specular = getLight(spot1)->diffuse;
    getLight(spot2)->specular = getLight(spot2)->diffuse;
    getLight(point1)->specular = getLight(point1)->diffuse;
    getLight(point2)->specular = getLight(point2)->diffuse;

    lightsOn = !lightsOn;
  }
};

class Sky: public Component
{
  float dayState = 0;
  bool shouldBeDay = false;
  glm::vec3 day;
  glm::vec3 night;
  int lightId{};

 public:
  Sky(glm::vec3 night, glm::vec3 day) {
    this->day = day;
    this->night = night;

    addDirLight(glm::vec3(0, -1, -0.5),
                glm::vec3(0.02, 0.02, 0.2),
                night, night);
  }

  void init(SceneState& state) override {
    vector<const char*> facePaths {
        "resources/models/skybox/clouds1_east.bmp",
        "resources/models/skybox/clouds1_west.bmp",
        "resources/models/skybox/clouds1_up.bmp",
        "resources/models/skybox/clouds1_down.bmp",
        "resources/models/skybox/clouds1_north.bmp",
        "resources/models/skybox/clouds1_south.bmp"
    };

    vector<unsigned char*> faces;

    int width, height, nrComponents;
    for (unsigned int i = 0; i < facePaths.size(); i++)
    {
      unsigned char *data = stbi_load(facePaths[i], &width, &height, &nrComponents, 0);
      if (data)
      {
        faces.push_back(data);
      }
      else
      {
        std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
      }
    }

    SkyboxTextures skybox;
    skybox.faces = faces;
    skybox.width = width;
    skybox.height = height;

    state.engine->addSkybox(skybox, "resources/shaders/sky-vertex.vs", "resources/shaders/sky-fragment.fs");
  }

  void frameUpdate(SceneState& state) override {
    if (isMakingTransition()) {
      float deltaLight = state.deltaTime;

      if (shouldBeDay) {
        dayState += deltaLight;
      } else {
        dayState -= deltaLight;
      }

      state.lightness = 0.1f + (dayState * 0.9f);
      getLight(lightId)->diffuse = night + (dayState * (day - night));
      getLight(lightId)->specular = getLight(lightId)->diffuse;
      float godC = getLight(lightId)->diffuse.x / 2;
      state.fog.color = glm::vec3(godC, godC, godC);

      if (dayState <= 0) {
        dayState = 0;
      } else if (dayState >= 1) {
        dayState = 1;
      }
    }
  }

  void transition() {
    shouldBeDay = !shouldBeDay;
  }

  bool isMakingTransition() const {
    return (float) shouldBeDay != dayState;
  }


};

class Reflector: public Component
{
 public:
  Reflector(glm::vec3 pos, glm::vec3 direction) {
    addPointLight(glm::vec3(1, 1, 0.7), 0.00001, 0, 0.02);
    addSpotLight(direction, glm::cos(glm::radians(12.0f)),
                 glm::cos(glm::radians(20.0f)),
                 glm::vec3(0.9, 0.9, 0.9),
                 0.01, 0.001, 0.001);
    setPosition(pos);
  }

  void init(SceneState& state) override { }

  void frameUpdate(SceneState& state) override { }
};

class FollowCamera : public CamHolder
{
  int followCameraId;
  Component* c;
  bool hasView = false;

 public:
  FollowCamera(Component* c, glm::vec3 position): c(c) {
    followCameraId = addCamera();
    getCameraOffset(followCameraId) = position;
  }

  void init(SceneState& state) override { }

  void frameUpdate(SceneState& state) override {
    getCameraFronts(followCameraId) = glm::normalize(c->getPosition() - getCameraOffset(followCameraId));

    if (hasView) {
      state.activeCamera = getCamera(followCameraId);
      hasView = false;
    }
  }

  void choose() override {
    hasView = true;
  }
};

class StaticCamera: public CamHolder
{
  int staticCameraId;
  bool hasView = false;

 public:
  explicit StaticCamera(glm::vec3 pos, glm::vec3 origin) {
    staticCameraId = addCamera();
    getCameraOffset(staticCameraId) = pos;
    getCameraFronts(staticCameraId) = glm::normalize(origin - getCameraOffset(staticCameraId));
  }

  void init(SceneState& state) override { }

  void frameUpdate(SceneState& state) override {
    if (hasView) {
      state.activeCamera = getCamera(staticCameraId);
      hasView = false;
    }
  }

  void choose() override {
    hasView = true;
  }
};

#endif
