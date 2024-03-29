#version 450

vec2 positions[3] = vec2[](vec2(0.0, -0.5), vec2(0.5, 0.5), vec2(-0.5, 0.5));

// Executed once per vertex.
void main()
{
    // gl_VertexIndex contains the index of the current vertex when main being called.
    gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0);
}
