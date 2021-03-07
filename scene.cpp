#include "scene.hpp"

#include <array>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <vector>

#include "io_util.hpp"
#include "shader.hpp"

void Scene::Init() {
  vertShader = shader::LoadSource("shaders/shader.vert", GL_VERTEX_SHADER);
  fragShader = shader::LoadSource("shaders/shader.frag", GL_FRAGMENT_SHADER);

  program = glCreateProgram();
  glAttachShader(program, vertShader);
  glAttachShader(program, fragShader);

  glLinkProgram(program);

  glCreateVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glGenBuffers(1, &vbo);

  // clang-format off
    float vertices[] = {
      0.0f, 0.0f,
      1.0f, 0.0f,
      1.0f, 1.0f,
      0.0f, 1.0f,
    };
  // clang-format on

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
  glEnableVertexAttribArray(0);
}

void Scene::CleanUp() {
  glDeleteBuffers(1, &vbo);
  glDeleteVertexArrays(1, &vao);

  glDeleteProgram(program);
  glDeleteShader(vertShader);
  glDeleteShader(fragShader);
}

void Scene::DoFrame(SDL_Event &event, context &ctx) {
  glBindVertexArray(vao);
  glUseProgram(program);

  auto colorUniform = glGetUniformLocation(program, "in_Color");
  auto transformUniform = glGetUniformLocation(program, "transform");

  world = glm::mat4{1.0f};
  world = glm::scale(world, glm::vec3(width, height, 1.0f));
  view = glm::lookAt(glm::vec3{ 0, 0, 10.0f }, glm::vec3{ 0, 0, 0 },
                     glm::vec3{0, 1, 0}); 

  proj = glm::ortho(-static_cast<float>(ctx.screenWidth) / 2.0f,
                    static_cast<float>(ctx.screenWidth) / 2.0f,
                    -static_cast<float>(ctx.screenHeight) / 2.0f,
                    static_cast<float>(ctx.screenHeight) / 2.0f, -10.0f, 10.0f);

  transform = proj * view * world;

  glUniform4f(colorUniform, color.r, color.g, color.b, color.a);
  glUniformMatrix4fv(transformUniform, 1, false, glm::value_ptr(transform));
  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void Scene::DoUI() {
  ImGui::Begin("Editor");
  ImGui::ColorPicker4("Color", glm::value_ptr(color),
                      ImGuiColorEditFlags_Float);

  ImGui::InputInt("Width", &width);
  ImGui::InputInt("Height", &height);

  ImGui::End();
}
