#version 450 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoord;

uniform mat4 transform;
uniform vec4 color;

out vec2 TexCoord;
out vec4 exColor;

void main() {
   gl_Position = transform * vec4(aPos, 1.0f, 1.0f);

  TexCoord = aTexCoord;
  exColor = color;
}
