#version 330

// Input
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 light_position;
uniform vec3 eye_position;

uniform vec3 light_direction_2;
uniform vec3 light_position_2;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

// TODO(student): Declare any other uniforms
uniform vec3 object_color;
uniform int spotlight;
uniform float cut_off_angle;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    // TODO(student): Define ambient, diffuse and specular light components
    float ambient_light = 0.25;
    
    vec3 L = normalize( light_position - world_position );
    float diffuse_light = material_kd * ambient_light * max (dot(world_normal, L), 0);

    vec3 L_2 = normalize( light_position_2 - world_position );
    float diffuse_light_2 = material_kd * ambient_light * max (dot(world_normal, L_2), 0);

    vec3 R = reflect (-L, world_normal);
    vec3 V = normalize( eye_position - world_position );
    float specular_light = 0;

    vec3 R_2 = reflect (-L_2, world_normal);
    float specular_light_2 = 0;

    if (diffuse_light > 0)
    {
        specular_light = material_ks * ambient_light * 1 * pow(max(dot(V, R), 0), material_shininess);
    }

    if (diffuse_light_2 > 0)
    {
        specular_light_2 = material_ks * ambient_light * 1 * pow(max(dot(V, R_2), 0), material_shininess);
    }

    // TODO(student): If (and only if) the light is a spotlight, we need to do
    // some additional things.

    float cut_off, spot_light, spot_light_limit;
    float linear_att, light_att_factor;
    float spot_light_2, spot_light_limit_2;
    float linear_att_2, light_att_factor_2;
    float light = ambient_light;

    if (spotlight == 1) 
    {
        cut_off = 0.52f; // radians(30);
        spot_light = dot(-L, light_direction);
        spot_light_limit = cos(cut_off);
        
        // Quadratic attenuation
        if (spot_light > spot_light_limit)
        {
            linear_att = (spot_light - spot_light_limit) / (1.0f - spot_light_limit);
            light_att_factor = pow(linear_att, 2);
            light += light_att_factor * (diffuse_light + specular_light);
        }

        // Then add second light
        spot_light_2 = dot(-L_2, light_direction_2);
        spot_light_limit_2 = cos(cut_off);
        
        // Quadratic attenuation
        if (spot_light_2 > spot_light_limit_2)
        {
            linear_att_2 = (spot_light_2 - spot_light_limit_2) / (1.0f - spot_light_limit_2);
            light_att_factor_2 = pow(linear_att_2, 2);
            light += light_att_factor_2 * (diffuse_light_2 + specular_light_2);
        }
    } 
    else
    {
        float factorAtenuare = 1 / pow(distance(world_position, light_position), 2);
        light += factorAtenuare * (diffuse_light + specular_light);

        float factorAtenuare_2 = 1 / pow(distance(world_position, light_position_2), 2);
        light += factorAtenuare_2 * (diffuse_light_2 + specular_light_2);
    }
    

    // TODO(student): Write pixel out color
    out_color = vec4(object_color * light, 1);
}
