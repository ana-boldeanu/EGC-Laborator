#version 330

// Input
in vec2 texcoord;

// Uniform properties
uniform sampler2D texture_1;
uniform sampler2D texture_2;

// TODO(student): Declare various other uniforms
uniform int alpha;
uniform float time;
uniform int render_sphere;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    // TODO(student): Calculate the out_color using the texture2D() function.
    vec2 texcoords = texcoord;

    if (render_sphere == 1) {
        texcoords.x += cos(time);
    }

    vec4 color1 = texture2D(texture_1, texcoords);
    vec4 color2 = texture2D(texture_2, texcoords);

    vec3 color = mix(color1.xyz, color2.xyz, 0.3f);

    if (color1.a < 0.5f || color2.a < 0.5f) {
	    discard;
    }

    out_color = vec4(color, 1);
}
