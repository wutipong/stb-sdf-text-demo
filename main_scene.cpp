#include "main_scene.hpp"

#include <fmt/core.h>
#include <glm/gtc/type_ptr.hpp>
#include <stb_truetype.h>

#include "io_util.hpp"
#include "shader.hpp"
#include "texture.hpp"

void MainScene::Init() {
  drawTextureVert =
      shader::LoadSource("shaders/draw_texture.vert", GL_VERTEX_SHADER);
  drawTextureFrag =
      shader::LoadSource("shaders/draw_texture.frag", GL_FRAGMENT_SHADER);

  drawTextureProgram = glCreateProgram();
  glUseProgram(drawTextureProgram);

  glAttachShader(drawTextureProgram, drawTextureVert);
  glAttachShader(drawTextureProgram, drawTextureFrag);

  glLinkProgram(drawTextureProgram);

  glCreateVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glGenBuffers(1, &vbo);

  // clang-format off
  float vertices[] = {
  /*x   , y   , u   , v   */
    0.0f, 0.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,
  };
  // clang-format on

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
                        reinterpret_cast<void *>(2 * sizeof(float)));

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
}

void MainScene::DoFrame(SDL_Event &event, context &ctx) {
  if (texture == 0)
    return;

  glBindVertexArray(vao);
  glBindTexture(GL_TEXTURE_2D, texture);

  auto program = drawTextureProgram;
  glUseProgram(program);

  auto colorUniform = glGetUniformLocation(program, "color");
  auto transformUniform = glGetUniformLocation(program, "transform");

  auto world =
      glm::scale(glm::mat4{1.0f}, glm::vec3(textureWidth, textureHeight, 1.0f));
  auto view = glm::lookAt(glm::vec3{0, 0, 10.0f}, glm::vec3{0, 0, 0},
                          glm::vec3{0, 1, 0});

  auto proj =
      glm::ortho(-static_cast<float>(ctx.screenWidth) / 2.0f,
                 static_cast<float>(ctx.screenWidth) / 2.0f,
                 -static_cast<float>(ctx.screenHeight) / 2.0f,
                 static_cast<float>(ctx.screenHeight) / 2.0f, -10.0f, 10.0f);

  auto transform = proj * view * world;

  glUniform4f(colorUniform, color.r, color.g, color.b, 1.0f);
  glUniformMatrix4fv(transformUniform, 1, false, glm::value_ptr(transform));
  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void MainScene::DoUI() {
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

  glDeleteBuffers(1, &vbo);
  glDeleteVertexArrays(1, &vao);

  glDeleteProgram(drawTextureProgram);
  glDeleteShader(drawTextureVert);
  glDeleteShader(drawTextureFrag);
}
