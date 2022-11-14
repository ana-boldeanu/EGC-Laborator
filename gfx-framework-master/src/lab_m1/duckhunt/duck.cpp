#include "lab_m1/duckhunt/duck.h"

#include <vector>
#include <iostream>

using namespace std;
using namespace m1;


Duck::Duck()
{
    // Create meshes for all duck parts
    body = CreateDuckBody();
    head = CreateDuckHead();
    wingFront = CreateDuckWingFront();
    wingBack = CreateDuckWingBack();
}


Mesh* Duck::CreateDuckWingFront()
{
    glm::vec3 wingColor = glm::vec3(0.47, 0, 0);

    // Duck front wing vertices
    vector<VertexFormat> vertices
    {
        VertexFormat(glm::vec3(125, 42, 0), wingColor),
        VertexFormat(glm::vec3(211, 63, 0), wingColor),
        VertexFormat(glm::vec3(105, 225, 0), wingColor)
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
    glm::vec3 wingColor = glm::vec3(0.27, 0, 0);
    
    // Duck back wing vertices
    vector<VertexFormat> vertices
    {
        VertexFormat(glm::vec3(131, 42, 0), wingColor),
        VertexFormat(glm::vec3(211, 63, 0), wingColor),
        VertexFormat(glm::vec3(185, 218, 0), wingColor)
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

    // Add center vertex
    indices.push_back(0);
    vertices.push_back(VertexFormat(glm::vec3(centerX, centerY, 0), color));

    // Add all points on radius
    for (int i = 1; i <= 16; i++) {
        indices.push_back(i);
        vertices.push_back(VertexFormat(glm::vec3(centerX + (radius * cos(i * 2 * 3.1415 / 16)),
            centerY + (radius * sin(i * 2 * 3.1415 / 16)), 0), color));
    }
    // Add the first point again (close the triangle fan)
    indices.push_back(1);

    circle->InitFromData(vertices, indices);
    return circle;
}


Mesh* Duck::CreateDuckHead()
{
    Mesh* head = CreateCircle("duck_head", 285, 60, 24, glm::vec3(0, 0.4, 0));
    return head;
}


Mesh* Duck::CreateDuckBody()
{
    // Set limits for the model (drawing of the duck should be relative to these coordinates, i.e. x = tailTip + 64,
    // but for now I used global distances as tailTip is set to (0, 0) anyway)
    tailTipX = 0; tailTipY = 0;
    beakTipX = 323; beakTipY = 59;
    centerY = 50;

    glm::vec3 bodyColor = glm::vec3(0.4, 0, 0);
    glm::vec3 headColor = glm::vec3(0, 0.4, 0);
    glm::vec3 neckColor = glm::vec3(1, 1, 1);
    glm::vec3 beakColor = glm::vec3(1, 0.4, 0);

    // Duck body & beak triangles
    vector<VertexFormat> vertices
    {
        VertexFormat(glm::vec3(tailTipX, tailTipY, 0), glm::vec3(0.33, 0, 0)),
        VertexFormat(glm::vec3(64, 22, 0), bodyColor),  // 1
        VertexFormat(glm::vec3(94, 80, 0), bodyColor),  // 2
        VertexFormat(glm::vec3(153, 14, 0), bodyColor),// 3
        VertexFormat(glm::vec3(186, 90, 0), bodyColor), // 4
        VertexFormat(glm::vec3(204, 22, 0), bodyColor), // 5
        VertexFormat(glm::vec3(233, 57, 0), headColor), // 6
        VertexFormat(glm::vec3(229, 85, 0), bodyColor), // 7
        VertexFormat(glm::vec3(310, 56, 0), headColor), // 8
        VertexFormat(glm::vec3(308, 54, 0), beakColor), // beak base
        VertexFormat(glm::vec3(308, 63, 0), beakColor), // beak base
        VertexFormat(glm::vec3(beakTipX, beakTipY, 0), beakColor), // beak tip

        // Some copies for colors (white neck detail)
        VertexFormat(glm::vec3(204, 22, 0), neckColor), // 12
        VertexFormat(glm::vec3(233, 57, 0), neckColor), // 13
        VertexFormat(glm::vec3(229, 85, 0), neckColor), // 14

        VertexFormat(glm::vec3(204, 22, 0), headColor), // 15
        VertexFormat(glm::vec3(233, 57, 0), headColor), // 16
        VertexFormat(glm::vec3(229, 85, 0), headColor), // 17
    };

    vector<unsigned int> indices =
    {
        0, 1, 2,
        1, 3, 2,
        2, 3, 4,
        3, 5, 4,
        4, 5, 7,
        14, 12, 13,
        17, 16, 8,
        6, 15, 8,
        9, 11, 10
    };

    Mesh* duckBody = new Mesh("duck_body");
    duckBody->InitFromData(vertices, indices);
    return duckBody;
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
    return centerY;
}

float Duck::GetLength()
{
    return beakTipX - tailTipX;
}

float Duck::GetWidth()
{
    return 2 * centerY;
}