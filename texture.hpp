#pragma once

#include <GL/gl3w.h>
#include <stb_truetype.h>

namespace texture {

namespace sdf {
constexpr float PixelHeight = 64.0f;
constexpr int OnEdgeValue = 180;
constexpr int Padding = 5;
constexpr float PixelDistScale = OnEdgeValue / Padding;

GLuint LoadCharactor(stbtt_fontinfo &info, const char &codepoint, int &width,
                     int &height);
} // namespace sdf

} // namespace texture
