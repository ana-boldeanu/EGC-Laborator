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

    lifePosX = 30; lifePosY = 690; lifePosDist = 50;
    bulletPosX = 200; bulletPosY = 675; bulletPosDist = 50;
    scorePosX = 15; scorePosY = 600;
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
    float length = 40;
    glm::vec3 color = glm::vec3(0, 1, 1);

    Mesh* wireframe = CreateSquare("bullet", length, color, false);

    return wireframe;
}


Mesh* Interface::CreateScoreBox() 
{
    float length = 40;
    glm::vec3 color = glm::vec3(0, 1, 1);

    Mesh* scoreBox = CreateSquare("bullet", length, color, true);

    return scoreBox;
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

