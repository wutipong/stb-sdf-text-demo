#pragma once

#include <GL/gl3w.h>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <imgui.h>
#include <stb_truetype.h>
#include <string>
#include <vector>

#include "context.hpp"
#include "imgui-filebrowser/imfilebrowser.h"

class MainScene {
public:
  void Init();
  void DoUI();
  void UpdateTexture();
  void CleanupTexture();
  void DoFrame(SDL_Event &event, context &context);
  void CleanUp();

private:
  ImGui::FileBrowser filebrowser;
  std::filesystem::path fontFilePath;
  int size{64};
  glm::vec3 color{};
  char charactor[2]{"A"};

  stbtt_fontinfo font;
  std::vector<char> fontData;

  GLuint texture{0};
  int textureWidth{0}, textureHeight{0};

  bool drawTexture{true};

  GLuint drawTextureVert{0};
  GLuint drawTextureFrag{0};
  GLuint drawTextureProgram{0};

  GLuint vao{0};
  GLuint vbo{0};
};
