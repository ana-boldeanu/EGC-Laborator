#include "lab_m1/duckhunt/flight.h"


#include <vector>
#include <iostream>

using namespace std;
using namespace m1;

Flight::Flight()
{
    duck = new Duck();
    duckCenterX = duck->GetCenterX();
    duckCenterY = duck->GetCenterY();
    modelMatrix = glm::mat3(1);
    scaleX = 1;
    scaleY = 1;
    scaledSoFarX = scaleX;
    scaledSoFarY = scaleY;
    scaleDown = true;
}


Flight::~Flight()
{
}


glm::mat3 Flight::FlapWing(glm::mat3 modelMatrix) {
    float step = 0.1;

    modelMatrix *= transform2D::Translate(duckCenterX, duckCenterY);
    modelMatrix *= transform2D::Scale(1, scaleY);
    modelMatrix *= transform2D::Translate(-duckCenterX, -duckCenterY);

    if (scaledSoFarY <= -1) {
        scaleDown = false;
    }
    else if (scaledSoFarY >= 1) {
        scaleDown = true;
    }

    if (scaleDown) {
        scaleY -= step;
        scaledSoFarY = scaleY;
    }
    else {
        scaleY += step;
        scaledSoFarY = scaleY;
    }

    return modelMatrix;
}
