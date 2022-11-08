#pragma once

#include "components/simple_scene.h"
#include "lab_m1/lab3/transform2D.h"
#include "lab_m1/duckhunt/duck.h"

namespace m1
{
    class Interface : public gfxc::SimpleScene
    {
    public:
        Interface();
        ~Interface();
        int lifeCount;
        int bulletCount;
        float score, maxScore;
        Mesh* Interface::GetLifeSymbol();
        Mesh* Interface::GetBulletSymbol();
        Mesh* Interface::GetCurrScoreBox();
        Mesh* Interface::GetMaxScoreBox();
        Mesh* Interface::GetGrass(bool lower);
        float lifePosX, lifePosY, lifePosDist;
        float bulletPosX, bulletPosY, bulletPosDist;
        float scorePosX, scorePosY;
        float grassPosX = -20;
        float grassPosY = 0;
        
    private:
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