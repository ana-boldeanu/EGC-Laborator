#include "lab_m1/duckhunt/interface.h"

#include <vector>
#include <iostream>

using namespace std;
using namespace m1;


Interface::Interface()
{    
	lifeSymbol = CreateLife();
	bulletSymbol = CreateBullet();
    maxScoreBox = CreateWireframeBox();
    currScoreBox = CreateScoreBox();

    // Create front grass mesh
    glm::vec3 color_0 = glm::vec3(0.4f, 0.85f, 0.05);
    glm::vec3 color_1 = glm::vec3(0.45f, 0.9f, 0.05);
    grass_lower = CreateGrass(grassPosX, grassPosY, color_0, color_1);

    // Create background grass mesh
    grassPosX += 50;
    grassPosY += 60;
    glm::vec3 color_2 = glm::vec3(0.27f, 0.6f, 0.05f);
    glm::vec3 color_3 = glm::vec3(0.30f, 0.65f, 0.05f);
    grass_upper = CreateGrass(grassPosX, grassPosY, color_2, color_3);
}


Mesh* Interface::CreateLife()
{
	Mesh* life = new Mesh("life");
    glm::vec3 color = glm::vec3(1, 0, 0);

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(glm::vec3(22, 0, 0), color),
        VertexFormat(glm::vec3(43, 28, 0), color),
        VertexFormat(glm::vec3(34, 38, 0), color),
        VertexFormat(glm::vec3(22, 28, 0), color),
        VertexFormat(glm::vec3(11, 38, 0), color),
        VertexFormat(glm::vec3(0, 28, 0), color)
    };

    std::vector<unsigned int> indices = { 
        0, 1, 5, 
        1, 2, 3,
        3, 4, 5
    };

    life->InitFromData(vertices, indices);

    return life;
}


Mesh* Interface::CreateBullet()
{
    Mesh* bullet = new Mesh("bullet");
    glm::vec3 color = glm::vec3(0.45, 0.3, 0);

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(glm::vec3(0, 0, 0), color),
        VertexFormat(glm::vec3(20, 0, 0), color),
        VertexFormat(glm::vec3(20, 30, 0), color),
        VertexFormat(glm::vec3(0, 30, 0), color),
        VertexFormat(glm::vec3(10, 46, 0), glm::vec3(0.2, 0.1, 0))
    };

    std::vector<unsigned int> indices = {
        0, 1, 2,
        0, 2, 3,
        2, 3, 4
    };

    bullet->InitFromData(vertices, indices);

    return bullet;
}


Mesh* Interface::CreateWireframeBox() 
{
    float length = 20;
    glm::vec3 color = glm::vec3(0, 0.5, 1);

    Mesh* wireframe = CreateSquare("maxScore", length, color, false);

    return wireframe;
}


Mesh* Interface::CreateScoreBox() 
{
    float length = 20;
    glm::vec3 color = glm::vec3(0, 0.5, 1);

    Mesh* scoreBox = CreateSquare("currScore", length, color, true);

    return scoreBox;
}


Mesh* Interface::CreateGrass(float grassPosX, float grassPosY, glm::vec3 color_0, glm::vec3 color_1)
{
    glm::vec3 vtx_0 = glm::vec3(grassPosX, grassPosY, 0);
    glm::vec3 vtx_1 = glm::vec3(grassPosX + 120, grassPosY, 0);
    glm::vec3 vtx_2 = glm::vec3(0, 150 + grassPosY, 0);
    int idx = 0;

    glm::vec3 temp;

    Mesh* grass = new Mesh("grass");
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    while (vtx_0.x < 1280) {
        vertices.push_back(VertexFormat(vtx_0, color_0));
        vertices.push_back(VertexFormat(vtx_1, color_0));
        vertices.push_back(VertexFormat(vtx_2, color_0));

        vtx_0.x += 70;
        vtx_1.x += 70;
        vtx_2.x += 70;

        indices.push_back(idx++);
        indices.push_back(idx++);
        indices.push_back(idx++);

        temp = color_0;
        color_0 = color_1;
        color_1 = temp;
    }

    grass->InitFromData(vertices, indices);

    return grass;
}


Mesh* Interface::CreateSquare(const std::string &name, float length, glm::vec3 color, bool fill)
{
    Mesh* square = new Mesh(name);
    glm::vec3 corner = glm::vec3(0, 0, 0);

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);

    return square;
}


Interface::~Interface()
{
}

Mesh* Interface::GetLifeSymbol()
{
    return lifeSymbol;
}

Mesh* Interface::GetBulletSymbol()
{
    return bulletSymbol;
}

Mesh* Interface::GetMaxScoreBox()
{
    return maxScoreBox;
}

Mesh* Interface::GetCurrScoreBox()
{
    return currScoreBox;
}

Mesh* Interface::GetGrass(bool lower)
{
    if (lower) {
        return grass_lower;
    }
    else {
        return grass_upper;
    }
}
