#include "main_scene.hpp"

#include <glm/gtc/type_ptr.hpp>

#include <stb_truetype.h>

#include "io_util.hpp"
#include "texture.hpp"

void MainScene::Draw() {}

void MainScene::DrawUI() {
  filebrowser.Display();
  ImGui::Begin("Parameters");
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
      LoadFile(fontFilePath.string(), fontData,
               std::ios_base::in | std::ios_base::binary);
      auto data = reinterpret_cast<const unsigned char *>(fontData.data());
      stbtt_InitFont(&font, data, stbtt_GetFontOffsetForIndex(data, 0));
      UpdateTexture();
    }
    if (ImGui::InputText("Charactor", charactor, sizeof(charactor))) {
      UpdateTexture();
    };

    ImGui::SliderInt("Font Size", &size, 1, 256);
    ImGui::ColorEdit3("Color", glm::value_ptr(color),
                      ImGuiColorEditFlags_Float);
  }
  ImGui::End();

  ImGui::Begin("Information");
  {
    ImGui::LabelText("Texture Width", "%i", textureWidth);
    ImGui::LabelText("Texture Height", "%i", textureHeight);
  }
  ImGui::End();
}

void MainScene::UpdateTexture() {
  CleanupTexture();
  texture = texture::sdf::LoadCharactor(font, charactor[0], textureWidth,
                                        textureHeight);
}

void MainScene::CleanupTexture() {
  if (texture != 0) {
    glDeleteTextures(1, &texture);
  }
}

void MainScene::CleanUp() { CleanupTexture(); }
