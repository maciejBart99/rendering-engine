#ifndef SCENEBUILDER_H
#define SCENEBUILDER_H

#include "../data_model/Component.h"

class SceneBuilder {
 private:
  vector<unique_ptr<Component>> components;

 public:
  void addComponent(unique_ptr<Component> component) {
    components.push_back(move(component));
  }
  vector<unique_ptr<Component>>&& getComponents() {
    return move(components);
  }
};

#endif
