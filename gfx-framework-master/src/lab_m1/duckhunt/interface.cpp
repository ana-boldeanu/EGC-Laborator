#include "lab_m1/duckhunt/interface.h"


#include <vector>
#include <iostream>

using namespace std;
using namespace m1;

Interface::Interface()
{
	lifeCount = 3;
	bulletCount = 3;
	score = 0;
    maxScore = 300;
	lifeSymbol = CreateLife();
	bulletSymbol = CreateBullet();
    maxScoreBox = CreateWireframeBox();
    currScoreBox = CreateScoreBox();

    glm::vec3 color_0 = glm::vec3(0.596f, 0.988f, 0);
    glm::vec3 color_1 = glm::vec3(0.455f, 0.749f, 0);
    grass_lower = CreateGrass(grassPosX, grassPosY, color_0, color_1);
    grassPosX += 50;
    grassPosY += 60;
    color_0 = glm::vec3(0.40f, 0.65f, 0);
    color_1 = glm::vec3(0.30f, 0.45f, 0);
    grass_upper = CreateGrass(grassPosX, grassPosY, color_0, color_1);

    lifePosX = 30; lifePosY = 690; lifePosDist = 50;
    bulletPosX = 205; bulletPosY = 675; bulletPosDist = 50;
    scorePosX = 15; scorePosY = 625;
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

std::vector<Mesh*> Interface::GetClouds() 
{
    return clouds;
}

Mesh* Interface::CreateLife()
{
    float radius = 15;

    Duck* duck = new Duck();
	Mesh* life = duck->CreateCircle("life", 0, 0, radius, glm::vec3(1, 0, 0));

    return life;
}


Mesh* Interface::CreateBullet()
{
    float length = 30;
    glm::vec3 color = glm::vec3(1, 1, 0);

    Mesh* bullet = CreateSquare("bullet", length, color, true);

	return bullet;
}

Mesh* Interface::CreateWireframeBox() 
{
    float length = 20;
    glm::vec3 color = glm::vec3(0, 0.5, 1);

    Mesh* wireframe = CreateSquare("bullet", length, color, false);

    return wireframe;
}


Mesh* Interface::CreateScoreBox() 
{
    float length = 40;
    glm::vec3 color = glm::vec3(0, 0.5, 1);

    Mesh* scoreBox = CreateSquare("bullet", length, color, true);

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

std::vector<Mesh*> Interface::CreateClouds()
{
    std::vector<Mesh*> clouds;
    Duck* duck = new Duck();

    float radius = 50;
    float centerX = 925;
    float centerY = 625;
    
    Mesh* cloud = duck->CreateCircle("clouds", centerX, centerY, radius, glm::vec3(1, 1, 1));
    clouds.push_back(cloud);

    cloud = duck->CreateCircle("clouds", centerX + 50, centerY + 30, radius, glm::vec3(1, 1, 1));
    clouds.push_back(cloud);

    cloud = duck->CreateCircle("clouds", centerX - 50, centerY + 30, radius, glm::vec3(1, 1, 1));
    clouds.push_back(cloud);

    return clouds;
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

