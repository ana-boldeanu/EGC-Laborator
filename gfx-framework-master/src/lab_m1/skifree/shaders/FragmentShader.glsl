#version 330

// Input
in vec2 texcoord;
in vec3 world_position;
in vec3 world_normal;

// Texture properties
uniform sampler2D texture;
uniform int alpha;
uniform int renderSnow;
uniform float time;

// Light properties
uniform vec3 eye_position;
uniform vec3 light_direction;
uniform vec3 lamp_light_position[12];
uniform vec3 tree_light_position[6];
uniform vec3 gift_light_position[4];

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

// Output
layout(location = 0) out vec4 out_color;

vec3 compute_light(vec3 light_position, vec3 light_color, int spotlight) 
{
    vec3 V = normalize(eye_position - world_position);
    vec3 L = normalize(light_position - world_position);
    vec3 H = normalize(V + L);

    // Define diffuse and specular light components
	float diffuse_light = material_kd * max (dot(world_normal,L), 0);
    float specular_light = 0;

    // For both types, start from the ambient light (defined in main)
    vec3 light_result = vec3(0);

	if (diffuse_light > 0)
    {
        specular_light =  material_ks * pow(max(dot(world_normal, H), 0), material_shininess);
    }

    if (spotlight == 0) {
        // Add normal distance attenuation for normal point light
        float d = distance(light_position, world_position);
	    float light_att_factor =  1 / (d * d);

        light_result += light_att_factor * (diffuse_light + specular_light) * light_color;

    } else {
        float cut_off = 0.52f;
        float spot_light = dot(-L, light_direction);  
        float spot_light_limit = cos(cut_off);
        float linear_att, light_att_factor;

        // Quadratic attenuation
        if (spot_light > spot_light_limit)
        {
            linear_att = (spot_light - spot_light_limit) / (1.0f - spot_light_limit);
            light_att_factor = pow(linear_att, 2) / 2;
            light_result += light_att_factor * (diffuse_light + specular_light) * light_color;
        }
    }

	return light_result;
}

vec3 get_sunlight() {
    vec3 light_position = glm::vec3(0, 10, 0);
    vec3 light_color = glm::vec3(1);
    vec3 L = glm::vec3(0, -1, 0);

    vec3 V = normalize(eye_position - world_position);
    vec3 H = normalize(V + L);

    // Define diffuse and specular light components
	float diffuse_light = material_kd * max (dot(world_normal,L), 0);
    float specular_light = 0;

	if (diffuse_light > 0)
    {
        specular_light =  material_ks * pow(max(dot(world_normal, H), 0), material_shininess);
    }

    // Add normal distance attenuation for normal point light
    float d = distance(light_position, world_position);
	float light_att_factor =  1;
    
    vec3 light_result = light_att_factor * (diffuse_light + specular_light) * light_color;
    return light_result;
}

void main()
{
    // Calculate the out_color using the texture2D() function.
    // Snow plane motion effect
    vec2 texcoords = texcoord;
    if (renderSnow == 1) {
        texcoords.y -= time * 0.1;
    }

    // Add texture
    vec4 color = texture2D(texture, texcoords);

    if (color.a < 0.5f || color.a < 0.5f) {
	    discard;
    }

    // Compute the light for each type of object in the scene
    vec3 light_color;

    // Start with ambient light
    float ambient_light = 0.5f;
    vec3 light = ambient_light * vec3(1);

    // Add direct sun light
    light += get_sunlight();

    // The lamps are spotlights
    for (int i = 0; i < 12; i++) {
        light_color = vec3(1, 0.85f, 0.4f);
        light += compute_light(lamp_light_position[i], light_color, 1);
    }
    
    // The trees are point lights
    for (int i = 0; i < 6; i++) {
        light_color = vec3(0.2f, 1, 0.2f);
        light += compute_light(tree_light_position[i], light_color, 0);
    }

    // The gifts are point lights
    for (int i = 0; i < 4; i++) {
        light_color = vec3(1, 0, 0);
        light += compute_light(gift_light_position[i], light_color, 0);
    }

    // Write pixel out color
    out_color = color * vec4(light, 1);
}
