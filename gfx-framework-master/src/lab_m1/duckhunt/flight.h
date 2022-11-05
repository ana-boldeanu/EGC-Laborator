#pragma once

#include "components/simple_scene.h"
#include "lab_m1/lab3/transform2D.h"
#include "lab_m1/duckhunt/duck.h"

namespace m1
{
    class Flight : public gfxc::SimpleScene
    {
    public:
        Flight();
        ~Flight();
        glm::mat3 Flight::FlapWing(glm::mat3 modelMatrix);
        
    private:
        Duck* duck;
        float duckCenterX, duckCenterY;
        float scaleX, scaleY;
        float scaledSoFarX, scaledSoFarY;
        float translateX, translateY;
        bool scaleDown;

    protected:
        glm::mat3 modelMatrix;
    };
}