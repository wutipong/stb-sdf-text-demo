#include "main_scene.hpp"

#include <imgui.h>

void MainScene::Draw() {}

void MainScene::DrawUI() {
  ImGui::Begin("Hello");
  ImGui::LabelText("Hello", "Hello World");
  ImGui::End();
}
