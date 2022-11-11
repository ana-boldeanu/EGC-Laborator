#include "lab_m1/duckhunt/flight.h"


#include <vector>
#include <iostream>

using namespace std;
using namespace m1;


Flight::Flight(float speed)
{
    Duck* duck = new Duck();
    duckCenterX = duck->GetCenterX();
    duckCenterY = duck->GetCenterY();
    flightSpeed = speed;
}


glm::mat3 Flight::FlapWing(glm::mat3 modelMatrix) {
    float step = 0.2f;

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


glm::mat3 Flight::TranslateDuck(glm::mat3 modelMatrix, float deltaTime, float angle, float &translateX, float &translateY)
{
    // Jocul incepe mereu cu o secunda intarziere (cel putin pe laptopul meu), iar rata va aparea un pic mai sus doar in primul frame
    float deltaX = flightSpeed * cos(angle) * deltaTime;
    float deltaY = flightSpeed * sin(angle) * deltaTime;

    translateX += deltaX;
    translateY += deltaY;

    modelMatrix *= transform2D::Translate(translateX, translateY);

    return modelMatrix;
}


glm::mat3 Flight::RotateDuck(glm::mat3 modelMatrix, float angle)
{
    modelMatrix *= transform2D::Translate(duckCenterX, duckCenterY);
    modelMatrix *= transform2D::Rotate(angle);
    modelMatrix *= transform2D::Translate(-duckCenterX, -duckCenterY);

    return modelMatrix;
}


Flight::~Flight()
{
}