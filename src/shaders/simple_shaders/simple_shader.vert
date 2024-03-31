#version 450

layout(location = 0) in vec2 position;

// Executed once per vertex.
void main()
{
    // gl_VertexIndex contains the index of the current vertex when main being called.
    gl_Position = vec4(position, 0.0, 1.0);
}
