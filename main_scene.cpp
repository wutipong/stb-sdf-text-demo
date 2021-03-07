#include "main_scene.hpp"

#include <fmt/core.h>
#include <glm/gtc/type_ptr.hpp>
#include <stb_truetype.h>

#include "io_util.hpp"
#include "shader.hpp"
#include "texture.hpp"

void MainScene::Init() {
  drawTextureVert =
      shader::FromSource("shaders/draw_texture.vert", GL_VERTEX_SHADER);
  drawTextureFrag =
      shader::FromSource("shaders/draw_texture.frag", GL_FRAGMENT_SHADER);

  drawTextureProgram = glCreateProgram();
  glUseProgram(drawTextureProgram);

  glAttachShader(drawTextureProgram, drawTextureVert);
  glAttachShader(drawTextureProgram, drawTextureFrag);

  glLinkProgram(drawTextureProgram);
}

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

    ImGui::Checkbox("Draw Texture", &drawTexture);
    // TODO: SDF is not implemented. Remove this when it is.
    drawTexture = true;
 
    ImGui::LabelText("Texture Width", fmt::format("{}", textureWidth).c_str());
    ImGui::LabelText("Texture Height",
                     fmt::format("{}", textureHeight).c_str());
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

void MainScene::CleanUp() {
  CleanupTexture();
  glDeleteProgram(drawTextureProgram);
  glDeleteShader(drawTextureVert);
  glDeleteShader(drawTextureFrag);
}
