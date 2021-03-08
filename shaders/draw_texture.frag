#version 450 core
out vec4 FragColor;

in vec2 TexCoord;
in vec4 exColor;

layout(binding = 0) uniform sampler2D distanceMap;

const float smoothing = 1.0/16.0;

void main() {
  float distance = texture(distanceMap, TexCoord).r;
  float alpha = smoothstep(0.5 - smoothing, 0.5+smoothing, distance);

  FragColor = vec4(exColor.rgb, exColor.a* alpha);
}
