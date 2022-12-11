#version 330

// Input
// Get values from vertex shader
in vec3 frag_color;

// Output
layout(location = 0) out vec4 out_color;

void main()
{
    // Use a specific color for the car
    out_color = vec4(0.62f, 0.45f, 0.83f, 1);
}
