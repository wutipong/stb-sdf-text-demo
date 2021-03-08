#include "texture.hpp"
#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

#include <vector>

GLuint texture::sdf::LoadCharactor(stbtt_fontinfo &info, const char &codepoint,
                                   int &width, int &height) {
  auto scale = stbtt_ScaleForPixelHeight(&info, PixelHeight);

  int xoff;
  int yoff;

  auto data =
      stbtt_GetCodepointSDF(&info, scale, codepoint, Padding, OnEdgeValue,
                            PixelDistScale, &width, &height, &xoff, &yoff);

  std::vector<unsigned char> vec{ data, data + (width * height) };

  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, width, height, 0, GL_RED,
               GL_UNSIGNED_BYTE, vec.data());

  stbtt_FreeSDF(data, nullptr);

  return texture;
}
