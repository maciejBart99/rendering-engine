#include <utility>

#include "engine/application/Application.h"


class Arena : public Component
{
 public:
  Arena() {
    addModel("resources/models/arena/arena.obj",
             "resources/shaders/basic-vertex.vs",
             "resources/shaders/basic-fragment.fs");
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

    spot1 = addSpotLight(glm::vec3(0.02, 0.02, 0.02), glm::vec3(0.7, 0.7, 0.7), glm::vec3(0, 0, 1),
                             glm::cos(glm::radians(15.0f)), glm::cos(glm::radians(25.0f)), glm::vec3(.8, 0.8, 0.8), 0.1, 0.01, 0.005);
    getLightOffset(spot1).x = -0.8;
    getLightOffset(spot1).y = 0.3;
    getLightOffset(spot1).z = 0.9;

    spot2 = addSpotLight(glm::vec3(0.02, 0.02, 0.02), glm::vec3(0.7, 0.7, 0.7), glm::vec3(0, 0, 1),
                             glm::cos(glm::radians(15.0f)), glm::cos(glm::radians(25.0f)), glm::vec3(0.8, 0.8, 0.8), 0.1, 0.01, 0.005);
    getLightOffset(spot2).x = 0.8;
    getLightOffset(spot2).z = 0.9;
    getLightOffset(spot2).y = 0.3;

    point1 = addPointLight(glm::vec3(0.02, 0.02, 0.02), glm::vec3(1, 1, 1),  glm::vec3(1, 1, 1), 0.2, 0, 2);
    point2 = addPointLight(glm::vec3(0.02, 0.02, 0.02), glm::vec3(1, 1, 1),  glm::vec3(1, 1, 1), 0.2, 0, 2);

    getLightOffset(point1).x = -0.8;
    getLightOffset(point1).z = 1.25;
    getLightOffset(point1).y = 0.75;

    getLightOffset(point2).x = 0.8;
    getLightOffset(point2).y = 0.75;
    getLightOffset(point2).z = 1.25;

    int point3 = addPointLight(glm::vec3(0.02, 0.02, 0.02), glm::vec3(1, 0.5, 0.5),  glm::vec3(1, 0, 0), 0.2, 0, 2);
    getLightOffset(point3).z = -3.1;
    getLightOffset(point3).y = 0.5;


    cameraId = addCamera();
    getCameraOffset(cameraId).y = 4;
    getCameraOffset(cameraId).z = -14;
  }

  void init(SceneState& state) override { }

  void frameUpdate(SceneState& state) override {
    float deltaRad = (state.deltaTime / 10.0f) * 2 * glm::pi<float>();

    fi += deltaRad;

    getModelPosition(carId).y = 0.004f * glm::sin(15 * fi);
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
      getLight(spot1)->diffuse = glm::vec3(1, 1, 0.9);
      getLight(spot2)->diffuse = glm::vec3(1, 1, 0.9);
      getLight(point1)->diffuse = glm::vec3(1, 1, 0.9);
      getLight(point2)->diffuse = glm::vec3(1, 1, 0.9);
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
  int lightId;

 public:
  Sky(glm::vec3 night, glm::vec3 day) {
    this->day = day;
    this->night = night;

    addDirLight(glm::vec3(0.3, -1, 0.3),
                glm::vec3(0.1, 0.1, 0.1),
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

    Skybox skybox;
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

      state.lightness = 0.2f + (dayState * 0.8f);
      getLight(lightId)->diffuse = night + (dayState * (day - night));
      getLight(lightId)->specular = getLight(lightId)->diffuse;
      float godC = getLight(lightId)->diffuse.x / 2;
      state.fog->color = glm::vec3(godC, godC, godC);

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
    addPointLight(glm::vec3(0.02, 0.02, 0.02),
                  glm::vec3(1, 1, 1),
                  glm::vec3(1, 1, 1), 0.0001, 0.01, 0.02);
    addSpotLight(glm::vec3(0.02, 0.02, 0.02), glm::vec3(1, 1, 1), direction,
                 glm::cos(glm::radians(12.0f)), glm::cos(glm::radians(20.0f)), glm::vec3(0.9, 0.9, 0.9), 0.05, 0.001, 0.001);
    setPosition(pos);
  }

  void init(SceneState& state) override { }

  void frameUpdate(SceneState& state) override { }
};

class FollowCamera : public CamHolder
{
  int followCameraId;
  shared_ptr<Component> car;
  bool hasView = false;

 public:
  FollowCamera(shared_ptr<Component> c, glm::vec3 position) {
    followCameraId = addCamera();
    getCameraOffset(followCameraId) = position;
    car = std::move(c);
  }

  void init(SceneState& state) override { }

  void frameUpdate(SceneState& state) override {
    getCameraFronts(followCameraId) = glm::normalize(car->getPosition() - getCameraOffset(followCameraId));

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

class AnimationController: public Component
{
  vector<shared_ptr<CamHolder>> cameras;
  shared_ptr<Sky> sun;
  shared_ptr<Car> car;
  int currentCamera = 0;
  bool wasSpacePressed = false;
  bool wasFPressed = false;
  bool wasGPressed = false;
  bool wasDPressed = false;
  bool wasLPressed = false;
  bool wasSPressed = false;

 public:
  AnimationController(vector<shared_ptr<CamHolder>> cameras, shared_ptr<Sky> sun, shared_ptr<Car> car) {
    this->cameras = std::move(cameras);
    this->sun = std::move(sun);
    this->car = std::move(car);
  }

  void init(SceneState& state) override { }

  void frameUpdate(SceneState& state) override {
    if (state.activeKeys.count(GLFW_KEY_SPACE)) {
      if (!wasSpacePressed) {
        cameras[(++currentCamera) % cameras.size()]->choose();
        wasSpacePressed = true;
      }
    } else {
      wasSpacePressed = false;
    }

    if (state.activeKeys.count(GLFW_KEY_F)) {
      if (!wasFPressed) {
        if (state.fog->density < 0.1) {
          state.fog->density += 0.005;
        }
        wasFPressed = true;
      }
    } else {
      wasFPressed = false;
    }

    if (state.activeKeys.count(GLFW_KEY_G)) {
      if (!wasGPressed) {
        if (state.fog->density > 0) {
          state.fog->density -= 0.005;
        }
        wasGPressed = true;
      }
    } else {
      wasGPressed = false;
    }

    if (state.activeKeys.count(GLFW_KEY_D)) {
      if (!wasDPressed) {
        if (!sun->isMakingTransition()) {
          sun->transition();
          car->toggleLights();
        }
        wasDPressed = true;
      }
    } else {
      wasDPressed = false;
    }

    if (state.activeKeys.count(GLFW_KEY_S)) {
      if (!wasSPressed) {
        state.shadingModel = state.shadingModel == SceneState::SH_PHONG ? SceneState::GOURAUD : SceneState::SH_PHONG;
        wasSPressed = true;
      }
    } else {
      wasSPressed = false;
    }

    if (state.activeKeys.count(GLFW_KEY_L)) {
      if (!wasLPressed) {
        state.lightModel = state.lightModel == SceneState::PHONG ? SceneState::BLINN : SceneState::PHONG;
        wasLPressed = true;
      }
    } else {
      wasLPressed = false;
    }
  }
};



int main()
{
  Application application("Car race", 1000, 700);

  auto car = shared_ptr<Car>(new Car());
  auto camera0 = shared_ptr<CamHolder>(new FollowCamera(car, glm::vec3(25, 15, 25)));
  auto camera1 = shared_ptr<CamHolder>(new FollowCamera(car, glm::vec3(0, 2, 0)));
  auto camera2 = shared_ptr<CamHolder>(new StaticCamera(glm::vec3(20, 2, 20), glm::vec3(0, 8, 0)));

  auto sun = shared_ptr<Sky>(new Sky(glm::vec3(0.1, 0.1, 0.2), glm::vec3(2, 2, 1.7)));

  vector<shared_ptr<CamHolder>> cameras;
  cameras.push_back(car);
  cameras.push_back(camera0);
  cameras.push_back(camera1);
  cameras.push_back(camera2);

  application.addComponent(car);
  application.addComponent(camera0);
  application.addComponent(camera1);
  application.addComponent(camera2);
  application.addComponent(shared_ptr<Component>(new AnimationController(cameras, sun, car)));
  application.addComponent(shared_ptr<Component>(new Arena()));
  application.addComponent(sun);
  application.addComponent(shared_ptr<Component>(new Reflector(glm::vec3(29.5, 17.8, 0), glm::vec3(-1.5, -1, 0))));
  application.addComponent(shared_ptr<Component>(new Reflector(glm::vec3(-29.5, 17.8, 0), glm::vec3(1.5, -1, 0))));

  application.start();

  return 0;
}