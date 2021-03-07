#include "texture.hpp"
#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

GLuint texture::sdf::LoadCharactor(stbtt_fontinfo &info, const char &codepoint,
                                   int &width, int &height) {
  auto scale = stbtt_ScaleForPixelHeight(&info, PixelHeight);

  int xoff;
  int yoff;

  auto data =
      stbtt_GetCodepointSDF(&info, scale, codepoint, Padding, OnEdgeValue,
                            PixelDistScale, &width, &height, &xoff, &yoff);

  GLuint texture;
  glGenTextures(1, &texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, width, height, 0, GL_RED,
               GL_UNSIGNED_BYTE, data);

  stbtt_FreeSDF(data, nullptr);

  return texture;
}