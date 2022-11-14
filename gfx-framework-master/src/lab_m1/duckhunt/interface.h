#pragma once

#include "components/simple_scene.h"

namespace m1
{
    // Class containing data and meshes for all UI elements and functions used to create them
    class Interface : public gfxc::SimpleScene
    {
    public:
        Interface();
        ~Interface();
        Mesh* Interface::GetLifeSymbol();
        Mesh* Interface::GetBulletSymbol();
        Mesh* Interface::GetCurrScoreBox();
        Mesh* Interface::GetMaxScoreBox();
        Mesh* Interface::GetGrass(bool lower);

        int lifeCount = 3;      // Initial number of lives
        int bulletCount = 3;    // Number of bullets allowed for each duck round
        float score = 0;        // Initial score
        float maxScore = 300;   // Maximum score needed to win

        // Coordinates for rendering grass meshes
        float grassPosX = -20;
        float grassPosY = 0;

        // Coordinates for rendering lives
        float lifePosX = 15;
        float lifePosY = 665;
        float lifePosDist = 50;

        // Coordinates for rendering bullets
        float bulletPosX = 1160;
        float bulletPosY = 655;
        float bulletPosDist = 40;

        // Coordinates for rendering the score boxes
        float scorePosX = 15;
        float scorePosY = 625;
        
    private:
        // Create meshes for all UI elements
        Mesh* Interface::CreateLife();
        Mesh* Interface::CreateBullet();
        Mesh* Interface::CreateSquare(const std::string& name, float length, glm::vec3 color, bool fill);
        Mesh* Interface::CreateWireframeBox();
        Mesh* Interface::CreateScoreBox();
        Mesh* Interface::CreateGrass(float grassPosX, float grassPosY, glm::vec3 color_0, glm::vec3 color_1);

    protected:
        Mesh* lifeSymbol;
        Mesh* bulletSymbol;
        Mesh* maxScoreBox;
        Mesh* currScoreBox;
        Mesh* grass_lower;
        Mesh* grass_upper;
    };
}