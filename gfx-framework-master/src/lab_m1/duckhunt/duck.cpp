#include "lab_m1/duckhunt/duck.h"

#include <vector>
#include <iostream>

using namespace std;
using namespace m1;

Duck::Duck()
{
}


Duck::~Duck()
{
}


Mesh* Duck::CreateDuckWingFront()
{
    // Duck front wing triangles
    vector<VertexFormat> vertices
    {
        VertexFormat(glm::vec3(131, 42, 0), glm::vec3(0.37, 0, 0)),
        VertexFormat(glm::vec3(220, 63, 0), glm::vec3(0.37, 0, 0)),
        VertexFormat(glm::vec3(105, 225, 0), glm::vec3(0.37, 0, 0))
    };

    vector<unsigned int> indices =
    {
        0, 1, 2
    };

    Mesh* duck_wing = new Mesh("duck_wing_front");
    duck_wing->InitFromData(vertices, indices);
    return duck_wing;
}


Mesh* Duck::CreateDuckWingBack()
{
    // Duck back wing triangles
    vector<VertexFormat> vertices
    {
        VertexFormat(glm::vec3(131, 42, 0), glm::vec3(0.34, 0, 0)),
        VertexFormat(glm::vec3(220, 63, 0), glm::vec3(0.34, 0, 0)),
        VertexFormat(glm::vec3(177, 213, 0), glm::vec3(0.34, 0, 0))
    };

    vector<unsigned int> indices =
    {
        0, 1, 2
    };

   Mesh *duck_wing = new Mesh("duck_wing_back");
   duck_wing->InitFromData(vertices, indices);
   return duck_wing;
}


Mesh* Duck::CreateCircle(const std::string& name, float centerX, float centerY,
    float radius, glm::vec3 color, bool fill)
{
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;
    Mesh* circle = new Mesh(name);

    circle->SetDrawMode(GL_TRIANGLE_FAN);

    indices.push_back(0);
    vertices.push_back(VertexFormat(glm::vec3(centerX, centerY, 0), color));
    for (int i = 1; i <= 16; i++) {
        indices.push_back(i);
        vertices.push_back(VertexFormat(glm::vec3(centerX + (radius * cos(i * 2 * 3.1415 / 16)),
            centerY + (radius * sin(i * 2 * 3.1415 / 16)), 0), color));
    }
    indices.push_back(1);

    circle->InitFromData(vertices, indices);
    return circle;
}


Mesh* Duck::CreateDuckHead()
{
    Mesh* head = CreateCircle("duck_head", 270, 84, 22, glm::vec3(0, 0.4, 0), true);
    return head;
}


Mesh* Duck::CreateDuckBody()
{
    // Duck body & beak triangles
    vector<VertexFormat> vertices
    {
        VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0.33, 0, 0)),
        VertexFormat(glm::vec3(64, 22, 0), glm::vec3(0.4, 0, 0)),
        VertexFormat(glm::vec3(94, 80, 0), glm::vec3(0.4, 0, 0)),
        VertexFormat(glm::vec3(153, 14, 0), glm::vec3(0.42, 0, 0)),
        VertexFormat(glm::vec3(186, 90, 0), glm::vec3(0.4, 0, 0)),
        VertexFormat(glm::vec3(204, 22, 0), glm::vec3(0.4, 0, 0)),
        VertexFormat(glm::vec3(228, 49, 0), glm::vec3(0.4, 0, 0)),
        VertexFormat(glm::vec3(229, 85, 0), glm::vec3(0.4, 0, 0)),
        VertexFormat(glm::vec3(250, 95, 0), glm::vec3(0, 0.4, 0)),
        VertexFormat(glm::vec3(280, 65, 0), glm::vec3(0, 0.4, 0)),
        VertexFormat(glm::vec3(291, 82, 0), glm::vec3(1, 0.4, 0)),
        VertexFormat(glm::vec3(290, 94, 0), glm::vec3(1, 0.4, 0)),
        VertexFormat(glm::vec3(306, 94, 0), glm::vec3(1, 0.4, 0)),

        // Some copies for colors (white neck detail)
        VertexFormat(glm::vec3(204, 22, 0), glm::vec3(1, 1, 1)),
        VertexFormat(glm::vec3(228, 49, 0), glm::vec3(1, 1, 1)),
        VertexFormat(glm::vec3(229, 85, 0), glm::vec3(1, 1, 1)),

        VertexFormat(glm::vec3(204, 22, 0), glm::vec3(0, 0.4, 0)),
        VertexFormat(glm::vec3(228, 49, 0), glm::vec3(0, 0.4, 0)),
        VertexFormat(glm::vec3(229, 85, 0), glm::vec3(0, 0.4, 0)),
    };

    vector<unsigned int> indices =
    {
        0, 1, 2,
        1, 3, 2,
        2, 3, 4,
        3, 5, 4,
        4, 5, 7,
        15, 13, 14,
        18, 17, 8,
        8, 16, 9,
        10, 12, 11
    };

    Mesh* duckBody = new Mesh("duck_body");
    duckBody->InitFromData(vertices, indices);
    return duckBody;
}
