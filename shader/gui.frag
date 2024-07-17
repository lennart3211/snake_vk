#version 450

layout(location = 0) in vec2 inTexCoord;

layout(location = 0) out vec4 outColor;

void main() {
    float distance = length(inTexCoord - vec2(0.5));
    float alpha = smoothstep(0.5, 0.4, distance);


    outColor = inColor;
    outColor.a *= alpha;
}