#pragma once
#include <GL/gl3w.h>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include "context.hpp"

class Scene {
public:
  void Init();
  void CleanUp();
  void DoFrame(SDL_Event &event, context& context);
  void DoUI();

private:
  glm::vec4 color{1.0f, 1.0f, 1.0f, 1.0f};

  glm::mat4 world;
  glm::mat4 view;
  glm::mat4 proj;
  glm::mat4 transform;

  GLuint program;
  GLuint vertShader;
  GLuint fragShader;

  GLuint vao;
  GLuint vbo;

  int width{100}, height{100};
};
