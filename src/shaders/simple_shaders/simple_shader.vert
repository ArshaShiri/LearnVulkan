#version 450

layout(location = 0) in vec2 position;
layout(location = 1) in vec3 color;

layout(push_constant) uniform Push
{
    mat2 transform;
    vec2 offset;
    vec3 color;
}
push;

// Executed once per vertex.
void main()
{
    // gl_VertexIndex contains the index of the current vertex when main being called.
    gl_Position = vec4(push.transform * position + push.offset, 0.0, 1.0);
}
