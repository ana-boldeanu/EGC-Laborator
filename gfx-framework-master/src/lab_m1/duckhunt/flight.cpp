#include "lab_m1/duckhunt/flight.h"


#include <vector>
#include <iostream>

using namespace std;
using namespace m1;

Flight::Flight(float angle, float speed)
{
    duck = new Duck();
    duckCenterX = duck->GetCenterX();
    duckCenterY = duck->GetCenterY();
    translateX = 0;
    translateY = 0;
    scaleX = 1;
    scaleY = 1;
    scaledSoFarX = scaleX;
    scaledSoFarY = scaleY;
    scaleDown = true;
    flightAngle = angle;
    flightSpeed = speed;
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


glm::mat3 Flight::TranslateDuck(glm::mat3 modelMatrix, float step, bool flyRight, bool flyUp, float &currX, float &currY) 
{
    float deltaX = flightSpeed * cos(flightAngle);
    float deltaY = flightSpeed * sin(flightAngle);

    if (!flyRight) {
        deltaX = -deltaX;
    }

    if (!flyUp) {
        deltaY = -deltaY;
    }

    modelMatrix *= transform2D::Translate(deltaX, deltaY);
    currX += deltaX;
    currY += deltaY;

    flightSpeed += step;

    return modelMatrix;
}


glm::mat3 Flight::RotateDuck(glm::mat3 modelMatrix, float angle)
{
    flightAngle = angle;

    modelMatrix *= transform2D::Translate(duckCenterX, duckCenterY);
    modelMatrix *= transform2D::Rotate(flightAngle);
    modelMatrix *= transform2D::Translate(-duckCenterX, -duckCenterY);

    return modelMatrix;
}