#pragma once

#include "components/simple_scene.h"
#include "lab_m1/lab3/transform2D.h"
#include "lab_m1/duckhunt/duck.h"

namespace m1
{
    class Flight : public gfxc::SimpleScene
    {
    public:
        Flight(float angle, float flightSpeed);
        ~Flight();
        glm::mat3 Flight::FlapWing(glm::mat3 modelMatrix);
        glm::mat3 Flight::TranslateDuck(glm::mat3 modelMatrix, float step, float &currX, float &currY);
        glm::mat3 Flight::RotateDuck(glm::mat3 modelMatrix, float angle);
        
    private:
        Duck* duck;
        float duckCenterX, duckCenterY;
        float scaleX, scaleY;               // Wings scale factor, used for flapping animation
        float scaledSoFarX, scaledSoFarY;   // Wings current scale factor
        bool scaleDown;                     // Wings scaling direction
        float translateX, translateY;
        float flightAngle;
        float flightSpeed;
        const float PI = 3.1415926f;

    protected:
    };
}