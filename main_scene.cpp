#include "main_scene.hpp"

#include <glm/gtc/type_ptr.hpp>

void MainScene::Draw() {}

void MainScene::DrawUI() {
  filebrowser.Display();
  ImGui::Begin("Font File");
  {
    ImGui::LabelText("Font", fontFilePath.filename().string().c_str());

    if (ImGui::Button("Select font")) {
      filebrowser.SetTitle("Select font file.");
      filebrowser.SetTypeFilters({".ttf", ".otf"});
      filebrowser.Open();
    }

    if (filebrowser.HasSelected()) {
      fontFilePath = filebrowser.GetSelected();
      filebrowser.ClearSelected();
    }

    ImGui::SliderInt("Font Size", &size, 1, 256);
    ImGui::ColorEdit3("Color", glm::value_ptr(color),
                      ImGuiColorEditFlags_Float);
  }
  ImGui::End();

  ImGui::Begin("Input Text");
  {
    std::array<char, 200> buffer{};
    if (ImGui::InputTextMultiline("Input", buffer.data(), buffer.size())) {
      text = std::string{buffer.begin(), buffer.end()};
    };
  }
  ImGui::End();
}
