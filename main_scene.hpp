#pragma once

#include <glm/glm.hpp>
#include <imgui.h>
#include <stb_truetype.h>
#include <string>
#include <vector>

#include "imgui-filebrowser/imfilebrowser.h"

class MainScene {
public:
  void DrawUI();
  void Draw();

private:
  ImGui::FileBrowser filebrowser;
  std::filesystem::path fontFilePath;
  int size{64};
  glm::vec3 color{};
  char charactor[2] {"A"};

  stbtt_fontinfo font;
  std::vector<char> fontData;
};
