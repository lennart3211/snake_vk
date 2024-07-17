#version 450

layout(location = 0) in vec4 inColor;
layout(location = 1) in vec2 inTexCoord;

layout(binding = 0) uniform sampler2D tex;

layout(location = 0) out vec4 outColor;

void main() {
    outColor = texture(tex, inTexCoord);
}
