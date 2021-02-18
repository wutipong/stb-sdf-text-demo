#include "main_scene.hpp"

#include <glm/gtc/type_ptr.hpp>

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

#include "io_util.hpp"

void MainScene::Draw() {}

void MainScene::DrawUI() {
  filebrowser.Display();
  ImGui::Begin("Font File");
  {
    ImGui::LabelText("Font", fontFilePath.filename().string().c_str());

    if (ImGui::Button("Select font")) {
      filebrowser.SetTitle("Select font file.");
      filebrowser.SetTypeFilters({".ttf"});
      filebrowser.Open();
    }

    if (filebrowser.HasSelected()) {
      fontFilePath = filebrowser.GetSelected();
      filebrowser.ClearSelected();
      fontData.clear();
      LoadFile(fontFilePath.string(), fontData);
      auto data = reinterpret_cast<const unsigned char *>(fontData.data());
      stbtt_InitFont(&font, data, stbtt_GetFontOffsetForIndex(data, 0));
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