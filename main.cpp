#include <GL/gl3w.h>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <imgui.h>
#include <spdlog/spdlog.h>

#include "imgui_impl/imgui_impl_opengl3.h"
#include "imgui_impl/imgui_impl_sdl.h"

#include "context.hpp"
#include "main_scene.hpp"
#include "scene.hpp"

constexpr auto ProjectName = "stb-sdf-text-demo";
constexpr int WindowWidth = 800;
constexpr int WindowHeight = 600;

constexpr char GlslVersion[] = "#version 450";
constexpr int GlMajorVersion = 4;
constexpr int GlMinorVersion = 5;

constexpr glm::vec4 ClearColor = {0.33f, 0.67f, 1.0f, 1.00f};

void PrintDeviceInformation();

void GlDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
                     GLsizei length, const GLchar *message,
                     const void *userParam);

int main(int argc, char **argv) {
  SDL_Init(SDL_INIT_EVERYTHING);
  spdlog::set_level(spdlog::level::debug);

  spdlog::info("{} - starts.", ProjectName);

  SDL_GL_SetAttribute(
      SDL_GL_CONTEXT_FLAGS,
      SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, GlMajorVersion);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, GlMinorVersion);

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

  SDL_Window *window = SDL_CreateWindow(
      "stb-sdf-text-demo", SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED, WindowWidth, WindowHeight,
      SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_OPENGL);

  SDL_SetWindowMinimumSize(window, WindowWidth, WindowHeight);

  SDL_GLContext glCtx = SDL_GL_CreateContext(window);
  if (glCtx == nullptr) {
    spdlog::error("Error: {}.\n", SDL_GetError());

    return -1;
  }

  SDL_GL_MakeCurrent(window, glCtx);
  SDL_GL_SetSwapInterval(1); // Enable vsync

  gl3wInit();

  PrintDeviceInformation();

  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(GlDebugCallback, 0);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();

  ImGui_ImplSDL2_InitForOpenGL(window, glCtx);
  ImGui_ImplOpenGL3_Init(GlslVersion);

  MainScene scene;
  scene.Init();

  while (true) {
    context ctx{};
    SDL_Event event;
    if (SDL_PollEvent(&event)) {
      ImGui_ImplSDL2_ProcessEvent(&event);
      if (event.type == SDL_QUIT)
        break;
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(window);
    ImGui::NewFrame();

    scene.DoUI();

    ImGui::EndFrame();
    ImGui::Render();

    SDL_GetWindowSize(window, &ctx.screenWidth, &ctx.screenHeight);

    glViewport(0, 0, ctx.screenWidth, ctx.screenHeight);

    glClearColor(ClearColor.r, ClearColor.g, ClearColor.b, ClearColor.a);
    glClear(GL_COLOR_BUFFER_BIT);

    scene.DoFrame(event, ctx);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    SDL_GL_SwapWindow(window);
    SDL_Delay(1);
  }

  scene.CleanUp();

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  SDL_GL_DeleteContext(glCtx);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}

void PrintDeviceInformation() {
  spdlog::info("OpenGL Device Information.");
  spdlog::info("\tOpenGL: {}", glGetString(GL_VERSION));
  spdlog::info("\tGLSL: {}", glGetString(GL_SHADING_LANGUAGE_VERSION));
  spdlog::info("\tDevice: {}", glGetString(GL_RENDERER));
  spdlog::info("\tVendor: {}", glGetString(GL_VENDOR));
  spdlog::info("Supported Extensions:");
  int extCount;
  glGetIntegerv(GL_NUM_EXTENSIONS, &extCount);

  for (int i = 0; i < extCount; i++) {
    spdlog::info("\t{}", glGetStringi(GL_EXTENSIONS, i));
  }
}

void GlDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
                     GLsizei length, const GLchar *message,
                     const void *userParam) {

  std::string sourceStr;
  switch (source) {
  case GL_DEBUG_SOURCE_API:
    sourceStr = "API";
    break;
  case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
    sourceStr = "WINDOW_SYSTEM";
    break;
  case GL_DEBUG_SOURCE_SHADER_COMPILER:
    sourceStr = "SHADER_COMPILER";
    break;
  case GL_DEBUG_SOURCE_THIRD_PARTY:
    sourceStr = "THIRD_PARTY";
    break;
  case GL_DEBUG_SOURCE_APPLICATION:
    sourceStr = "APPLICATION";
    break;
  case GL_DEBUG_SOURCE_OTHER:
    sourceStr = "OTHER";
    break;
  }

  std::string typeStr;
  switch (type) {
  case GL_DEBUG_TYPE_ERROR:
    typeStr = "ERROR";
    break;

  case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
    typeStr = "DEPRECATED_BEHAVIOR";
    break;

  case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
    typeStr = "UNDEFINED_BEHAVIOR";
    break;

  case GL_DEBUG_TYPE_PORTABILITY:
    typeStr = "PORTABILITY";
    break;

  case GL_DEBUG_TYPE_PERFORMANCE:
    typeStr = "PERFORMANCE";
    break;

  case GL_DEBUG_TYPE_MARKER:
    typeStr = "MARKER";
    break;

  case GL_DEBUG_TYPE_PUSH_GROUP:
    typeStr = "PUSH_GROUP";
    break;

  case GL_DEBUG_TYPE_POP_GROUP:
    typeStr = "POP_GROUP";
    break;

  case GL_DEBUG_TYPE_OTHER:
    typeStr = "OTHER";
    break;
  }

  auto level = spdlog::level::critical;
  switch (severity) {
  case GL_DEBUG_SEVERITY_HIGH:
    level = spdlog::level::err;
    break;
  case GL_DEBUG_SEVERITY_MEDIUM:
    level = spdlog::level::warn;
    break;
  case GL_DEBUG_SEVERITY_LOW:
    level = spdlog::level::info;
    break;
  case GL_DEBUG_SEVERITY_NOTIFICATION:
    level = spdlog::level::debug;
    break;
  }

  spdlog::log(level, "OpenGL [{}] [{}]: {}", sourceStr, typeStr, std::string(message, length));
}
