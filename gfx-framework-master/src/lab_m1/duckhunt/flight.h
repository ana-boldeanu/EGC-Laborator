#pragma once

#include "components/simple_scene.h"
#include "lab_m1/lab3/transform2D.h"
#include "lab_m1/duckhunt/duck.h"

namespace m1
{
    class Flight : public gfxc::SimpleScene
    {
    public:
        Flight(float speed);
        ~Flight();
        glm::mat3 Flight::FlapWing(glm::mat3 modelMatrix);
        glm::mat3 Flight::TranslateDuck(glm::mat3 modelMatrix, float deltaTime, float flightAngle, float &translateX, float &translateY);
        glm::mat3 Flight::RotateDuck(glm::mat3 modelMatrix, float angle);
        
    private:
        float duckCenterX, duckCenterY;
        float scaleX = 1;
        float scaleY = 1;               // Wings scale factor, used for flapping animation
        float scaledSoFarX = 1;
        float scaledSoFarY = 1;   // Wings current scale factor
        bool scaleDown = true;                     // Wings scaling direction
        float flightSpeed;
        const float PI = 3.1415926f;

    protected:
    };
}