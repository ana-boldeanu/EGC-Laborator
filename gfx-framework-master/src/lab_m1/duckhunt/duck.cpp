#include "lab_m1/duckhunt/duck.h"

#include <vector>
#include <iostream>

using namespace std;
using namespace m1;


Duck::Duck()
{
    body = CreateDuckBody();
    head = CreateDuckHead();
    wingFront = CreateDuckWingFront();
    wingBack = CreateDuckWingBack();
}


Duck::~Duck()
{
}


Mesh* Duck::GetHead() 
{
    return head;
}


Mesh* Duck::GetBody()
{
    return body;
}


Mesh* Duck::GetWingFront()
{
    return wingFront;
}


Mesh* Duck::GetWingBack()
{
    return wingBack;
}


float Duck::GetCenterX()
{
    return (beakTipX - tailTipX) / 2;
}


float Duck::GetCenterY()
{
    return (beakTipY - tailTipY) / 2;
}


Mesh* Duck::CreateDuckWingFront()
{
    // Duck front wing triangles
    vector<VertexFormat> vertices
    {
        VertexFormat(glm::vec3(125, 42, 0), glm::vec3(0.44, 0, 0)),
        VertexFormat(glm::vec3(211, 63, 0), glm::vec3(0.44, 0, 0)),
        VertexFormat(glm::vec3(105, 225, 0), glm::vec3(0.44, 0, 0))
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
        VertexFormat(glm::vec3(131, 42, 0), glm::vec3(0.32, 0, 0)),
        VertexFormat(glm::vec3(211, 63, 0), glm::vec3(0.32, 0, 0)),
        VertexFormat(glm::vec3(185, 218, 0), glm::vec3(0.32, 0, 0))
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
    float radius, glm::vec3 color)
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
    Mesh* head = CreateCircle("duck_head", 270, 87, 25, glm::vec3(0, 0.4, 0));
    return head;
}


Mesh* Duck::CreateDuckBody()
{
    // Set limits for the model (drawing of the duck should be relative to these coordinates, i.e. x = tailTip + 64,
    // but for now I used global distances, tailTip set to 0, 0 anyway)
    tailTipX = 0; tailTipY = 0;
    beakTipX = 306; beakTipY = 100;

    // Duck body & beak triangles
    vector<VertexFormat> vertices
    {
        VertexFormat(glm::vec3(tailTipX, tailTipY, 0), glm::vec3(0.33, 0, 0)),
        VertexFormat(glm::vec3(64, 22, 0), glm::vec3(0.4, 0, 0)),
        VertexFormat(glm::vec3(94, 80, 0), glm::vec3(0.4, 0, 0)),
        VertexFormat(glm::vec3(153, 14, 0), glm::vec3(0.42, 0, 0)),
        VertexFormat(glm::vec3(186, 90, 0), glm::vec3(0.4, 0, 0)),
        VertexFormat(glm::vec3(204, 22, 0), glm::vec3(0.4, 0, 0)),
        VertexFormat(glm::vec3(228, 49, 0), glm::vec3(0.4, 0, 0)),
        VertexFormat(glm::vec3(229, 85, 0), glm::vec3(0.4, 0, 0)),
        VertexFormat(glm::vec3(250, 95, 0), glm::vec3(0, 0.4, 0)),
        VertexFormat(glm::vec3(280, 65, 0), glm::vec3(0, 0.4, 0)),
        VertexFormat(glm::vec3(294, 85, 0), glm::vec3(1, 0.4, 0)), // beak base
        VertexFormat(glm::vec3(292, 99, 0), glm::vec3(1, 0.4, 0)), // beak base
        VertexFormat(glm::vec3(beakTipX, beakTipY, 0), glm::vec3(1, 0.4, 0)), // beak tip

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
