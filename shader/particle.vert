#version 450

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec2 inVelocity;
layout(location = 2) in vec4 inColor;
layout(location = 3) in float inSize;
layout(location = 4) in float inLife;

layout(location = 0) out vec2 outPosition;
layout(location = 1) out vec2 outVelocity;
layout(location = 2) out vec4 outColor;
layout(location = 3) out float outSize;
layout(location = 4) out float outLife;

void main() {
    outPosition = inPosition;
    outVelocity = inVelocity;
    outColor = inColor;
    outSize = inSize;
    outLife = inLife;

    gl_Position = vec4(inPosition, 0.0, 1.0);
    gl_PointSize = inSize;
}