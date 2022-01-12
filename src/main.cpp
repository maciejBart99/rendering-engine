#include <utility>

#include "engine/application/Application.h"
#include "engine/application/KeyListener.h"
#include "demo/demo_components.h"



class AnimationController: public Component
{
  vector<CamHolder*> cameras;
  Sky* sky;
  Car* car;
  int currentCamera = 0;
  bool lastWasEmptyKeys = true;

  KeyListener space = KeyListener(GLFW_KEY_SPACE);
  KeyListener f = KeyListener(GLFW_KEY_F);
  KeyListener g = KeyListener(GLFW_KEY_G);
  KeyListener d = KeyListener(GLFW_KEY_D);
  KeyListener l = KeyListener(GLFW_KEY_L);
  KeyListener s = KeyListener(GLFW_KEY_S);

 public:
  AnimationController(vector<CamHolder*> cameras, Sky* sky, Car* car):
        cameras(std::move(cameras)), sky(sky), car(car) { }

  void init(SceneState& state) override { }

  void frameUpdate(SceneState& state) override {
    if (!state.activeKeys.empty() || !lastWasEmptyKeys) {
      if (space.isDownEvent(state.activeKeys)) {
        cameras[(++currentCamera) % cameras.size()]->choose();
      }

      if (f.isDownEvent(state.activeKeys)) {
        state.fog.density += 0.005;
      }

      if (g.isDownEvent(state.activeKeys)) {
        state.fog.density -= 0.005;
      }

      if (d.isDownEvent(state.activeKeys)) {
        if (!sky->isMakingTransition()) {
          sky->transition();
          car->toggleLights();
        }
      }

      if (s.isDownEvent(state.activeKeys)) {
        state.shadingModel = state.shadingModel == SceneState::ShadingMode::PHONG
                                 ? SceneState::ShadingMode::GOURAUD
                                 : SceneState::ShadingMode::PHONG;
      }

      if (l.isDownEvent(state.activeKeys)) {
        state.lightModel = state.lightModel == SceneState::LightModel::PHONG
                               ? SceneState::LightModel::BLINN
                               : SceneState::LightModel::PHONG;
      }
      lastWasEmptyKeys = false;
    } else {
      lastWasEmptyKeys = true;
    }
  }
};



int main()
{
  Application application("Car race", 1000, 700);

  unique_ptr<Car> car(new Car());
  unique_ptr<CamHolder> camera0(new FollowCamera(car.get(), glm::vec3(25, 15, 25)));
  unique_ptr<CamHolder> camera1(new FollowCamera(car.get(), glm::vec3(0, 2, 0)));
  unique_ptr<CamHolder> camera2(new StaticCamera(glm::vec3(20, 2, 20), glm::vec3(0, 8, 0)));

  auto sun = unique_ptr<Sky>(new Sky(glm::vec3(0.01, 0.01, 0.02), glm::vec3(0.9, 0.9, 0.9)));

  vector<CamHolder*> cameras;
  cameras.push_back(car.get());
  cameras.push_back(camera0.get());
  cameras.push_back(camera1.get());
  cameras.push_back(camera2.get());

  application.getSceneBuilder()->addComponent(unique_ptr<Component>(new AnimationController(cameras, sun.get(), car.get())));
  application.getSceneBuilder()->addComponent(move(car));
  application.getSceneBuilder()->addComponent(move(camera0));
  application.getSceneBuilder()->addComponent(move(camera1));
  application.getSceneBuilder()->addComponent(move(camera2));
  application.getSceneBuilder()->addComponent(unique_ptr<Component>(new Arena()));
  application.getSceneBuilder()->addComponent(move(sun));
  application.getSceneBuilder()->addComponent(unique_ptr<Component>(new Reflector(glm::vec3(29.5, 17.2, 0), glm::vec3(-1.5, -1, 0))));
  application.getSceneBuilder()->addComponent(unique_ptr<Component>(new Reflector(glm::vec3(-29.5, 17.2, 0), glm::vec3(1.5, -1, 0))));

  application.start();

  return 0;
}