#pragma once
#include <GL/gl3w.h>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>

class Scene {
public:
  void Init();
  void CleanUp();
  void DoFrame(SDL_Event &event);
  void DoUI();

private:
  glm::vec4 color{1.0f, 1.0f, 1.0f, 1.0f};

  GLuint program;
  GLuint vertShader;
  GLuint fragShader;

  GLuint vao;
  GLuint vbo;

  int width{100}, height{100};
  float scale{1.0f};
};
