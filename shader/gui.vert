#version 450

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec2 inExtent;

layout(location = 0) out vec2 outPosition;
layout(location = 1) out vec2 outExtent;

void main() {
    outPosition = inPosition;
    outExtent = inExtent;

    gl_Position = vec4(inPosition, 0.0, 1.0);
}