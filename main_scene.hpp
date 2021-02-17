#pragma once

#include <glm/glm.hpp>
#include <imgui.h>
#include <string>

#include "imgui-filebrowser/imfilebrowser.h"

class MainScene {
public:
  void DrawUI();
  void Draw();

private:
  ImGui::FileBrowser filebrowser;
  std::string text;
  std::filesystem::path fontFilePath;
  int size{64};
  glm::vec3 color{};
};
