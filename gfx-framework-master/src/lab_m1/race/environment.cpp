#include "lab_m1/race/environment.h"

using namespace std;
using namespace m1;


Environment::Environment()
{
	CreateGrass();
}

void Environment::CreateGrass() 
{
    float radius = 140;
    float center_x = 0, center_z = 0;
    grass = CreateCircle("grass", center_x, center_z, radius, grass_color);
}


Mesh* Environment::CreateCircle(const std::string& name, float centerX, float centerZ,
    float radius, glm::vec3 color)
{
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;
    Mesh* circle = new Mesh(name);

    circle->SetDrawMode(GL_TRIANGLE_FAN);

    // Add center vertex
    indices.push_back(0);
    vertices.push_back(VertexFormat(glm::vec3(centerX, centerZ, 0), color));

    // Add all points on radius
    int segments = 100;
    float y = -0.01f;
    float x, z;

    for (int i = 1; i <= segments; i++) {
        indices.push_back(i);

        x = centerX + (radius * cos(i * 2 * 3.1415 / segments));
        z = centerZ + (radius * sin(i * 2 * 3.1415 / segments));
        vertices.push_back(VertexFormat(glm::vec3(x, y, z), color));
    }
    // Add the first point again (close the triangle fan)
    indices.push_back(1);

    circle->InitFromData(vertices, indices);
    return circle;
}


Environment::~Environment()
{
}