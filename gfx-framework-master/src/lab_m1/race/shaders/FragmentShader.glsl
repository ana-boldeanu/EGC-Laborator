#version 330

// Input
// Get values from vertex shader
in vec3 frag_color;

// Output
layout(location = 0) out vec4 out_color;

void main()
{
    // Write pixel out color
    out_color = vec4(frag_color, 1);
}
