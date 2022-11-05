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
        float lifePosX, lifePosY, lifePosDist;
        float bulletPosX, bulletPosY, bulletPosDist;
        float scorePosX, scorePosY, scoreLength, scoreWidth;
        
    private:
        Mesh* Interface::CreateLife();
        Mesh* Interface::CreateBullet();
        Mesh* Interface::CreateSquare(const std::string& name, float length, glm::vec3 color, bool fill);
        Mesh* Interface::CreateWireframeBox();
        Mesh* Interface::CreateScoreBox();

    protected:
        Mesh* lifeSymbol;
        Mesh* bulletSymbol;
        Mesh* maxScoreBox;
        Mesh* currScoreBox;
    };
}