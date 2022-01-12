#include "Application.h"
#include "../EngineLogger.h"
#include "stb_image.h"

#include <set>

set<int> activeKeys;


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (action == GLFW_PRESS) {
    activeKeys.insert(key);
  } else if (action == GLFW_RELEASE) {
    activeKeys.erase(key);
  }
}

void Application::start() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  #endif

  GLFWwindow* window = glfwCreateWindow(width, height, name, nullptr, nullptr);
  if (window == nullptr)
  {
    glfwTerminate();
    EngineLogger::getInstance()->error("Cannot initialize GLFW");
    exit(1);
  }

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, resizeCallback);
  glfwSetKeyCallback(window, key_callback);
  glfwMakeContextCurrent(window);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    EngineLogger::getInstance()->error("Cannot initialize GLAD");
    exit(1);
  }

  glEnable(GL_DEPTH_TEST);

  float deltaTime = 0;
  float lastFrame = 0;

  Engine engine(sceneBuilder->getComponents(), width, height);
  engine.initializeComponentModel();

  while (!glfwWindowShouldClose(window))
  {
    auto currentFrame = (float) glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);

    engine.nextFrame(deltaTime, activeKeys);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
}

void resizeCallback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}