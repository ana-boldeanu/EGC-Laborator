#version 330

// Input
// Get vertex attributes from each location
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec3 v_coordinate;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 car_pos;

// Output values to fragment shader
out vec3 frag_color;

void main()
{ 
    // Send output to fragment shader
    frag_color = v_color; 

    // Compute world position
    vec3 world_pos = (Model * vec4(v_position, 1)).xyz;

    // Compute gl_Position
    float gamma = 0.01f;

    float len = distance(car_pos, world_pos);
    world_pos.y -= len * len * gamma;

    gl_Position = Projection * View * vec4(world_pos, 1.0);
}
