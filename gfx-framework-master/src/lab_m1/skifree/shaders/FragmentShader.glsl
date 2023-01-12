#version 330

// Input
in vec2 texcoord;

// Uniform properties
uniform sampler2D texture;
uniform int alpha;
uniform int renderSnow;
uniform float time;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    // TODO(student): Calculate the out_color using the texture2D() function.
    vec2 texcoords = texcoord;

    if (renderSnow == 1) {
        texcoords.y -= time * 0.1;
    }

    vec4 color = texture2D(texture, texcoords);

    if (color.a < 0.5f || color.a < 0.5f) {
	    discard;
    }

    out_color = color;
}
