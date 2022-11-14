#pragma once

#include "components/simple_scene.h"
#include "lab_m1/duckhunt/transform2D.h"
#include "lab_m1/duckhunt/duck.h"

namespace m1
{
    // Class containing transformation functions used for duck flying animations
    class Flight : public gfxc::SimpleScene
    {
    public:
        Flight(float speed);
        ~Flight();

        // Move the duck relative to (0, 0)
        glm::mat3 Flight::TranslateDuck(glm::mat3 modelMatrix, float deltaTime, float flightAngle, float &translateX, float &translateY);

        // Rotate the duck relative to (0, 0), must be used before translation
        glm::mat3 Flight::RotateDuck(glm::mat3 modelMatrix, float angle);

        // Add wings flapping animation (relative to duck center)
        glm::mat3 Flight::FlapWing(glm::mat3 modelMatrix, float deltaTime);
        
    private:
        float duckCenterX, duckCenterY; // Center of the duck
        float scaleX = 1;
        float scaleY = 1;               // Wings scale factor, used for flapping animation
        float scaledSoFarX = 1;
        float scaledSoFarY = 1;         // Wings current scale factor
        bool scaleDown = true;          // Wings scaling direction
        float flightSpeed;
        const float PI = 3.1415926f;

    protected:
    };
}