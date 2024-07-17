#version 450

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

layout(location = 0) in vec2 inPosition[];
layout(location = 1) in vec2 inExtent[];

layout(location = 0) out vec2 outTexCoord;

void main() {
    vec2 position = inPosition[0];
    vec2 extent = inExtent[0];

    // top left
    gl_Position = vec4(position, 0.0, 1.0);
    outTexCoord = vec2(0.0, 0.0);
    EmitVertex();

    // top right
    gl_Position = vec4(position.x + extent.x, position.y, 0.0, 1.0);
    outTexCoord = vec2(1.0, 0.0);
    EmitVertex();

    // bottom left
    gl_Position = vec4(position.x, position.y + extent.y, 0.0, 1.0);
    outTexCoord = vec2(0.0, 1.0);
    EmitVertex();

    // bottom right
    gl_Position = vec4(position + extent, 0.0, 1.0);
    outTexCoord = vec2(1.0, 1.0);
    EmitVertex();

    EndPrimitive();
}