#version 330

// Input
// Get values from vertex shader
in vec3 frag_color;
in vec3 eye_position;
in vec3 world_position;

// Output
layout(location = 0) out vec4 out_color;

void main()
{
    // Compute final color
    vec4 local_color = vec4(frag_color, 1);

    // Add fog effect
    float fog_density = 0.1f;
    vec4 fog_color = vec4(0.44f, 0.73f, 0.82f, 1);
    float distance = distance(eye_position, world_position);
    float k_fog = 1 - pow(2.71f, -fog_density * distance);

    // Write pixel out color
    out_color = (1 - k_fog) * local_color + k_fog * fog_color;
}
