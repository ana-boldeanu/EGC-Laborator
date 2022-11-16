#version 330

// Input
// TODO(student): Get vertex attributes from each location
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec3 v_coordinate;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float Time;

// Output
// TODO(student): Output values to fragment shader
out vec3 frag_color;

void main()
{
    // Bonus: Change position and color according to time    
    // TODO(student): Send output to fragment shader
    frag_color = v_normal + sin(Time);      // before Bonus, frag_color = v_color; 

    // TODO(student): Compute gl_Position
    gl_Position = Projection * View * Model * vec4(v_position + tan(Time), 1.0);
}
