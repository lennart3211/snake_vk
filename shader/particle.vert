#version 450

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec4 inColor;
layout(location = 2) in float inSize;

layout(location = 0) out vec2 outPosition;
layout(location = 1) out vec4 outColor;
layout(location = 2) out float outSize;

void main() {
    outPosition = inPosition;
    outColor = inColor;
    outSize = inSize;

    gl_Position = vec4(inPosition, 0.0, 1.0);
    gl_PointSize = inSize;
}