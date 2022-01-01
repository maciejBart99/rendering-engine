#include "Application.h"
#include "stb_image.h"
#include "../core/SimpleShader.h"
#include "../core/Model.h"
#include "../core/Camera.h"

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
    throw "Cannot construct application window";
  }

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, resizeCallback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    throw "Cannot initialize graphics";
  }

  stbi_set_flip_vertically_on_load(true);
  glEnable(GL_DEPTH_TEST);

  Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
  float lastX = width / 2.0f;
  float lastY = height / 2.0f;

  SimpleShader ourShader("resources/shaders/basic-vertex.vs", "resources/shaders/basic-fragment.fs");

  // load models
  // -----------
  Model ourModel("resources/models/backpack/backpack.obj");


  // draw in wireframe
  //  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  float deltaTime = 0.0f;
  float lastFrame = 0.0f;

  // render loop
  // -----------
  while (!glfwWindowShouldClose(window))
  {
    // per-frame time logic
    // --------------------
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    // render
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);
    // ------
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // don't forget to enable shader before setting uniforms
    ourShader.use();

    // view/projection transformations
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)width / (float)height, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    ourShader.set("projection", projection);
    ourShader.set("view", view);

    // render the loaded model
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
    ourShader.set("model", model);
    ourModel.render(ourShader);

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // glfw: terminate, clearing all previously allocated GLFW resources.
  // ------------------------------------------------------------------
  glfwTerminate();
}

void resizeCallback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}