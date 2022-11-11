#include "lab_m1/duckhunt/duckhunt.h"

#include <vector>
#include <iostream>

using namespace std;
using namespace m1;


DuckHunt::DuckHunt()
{
    srand(static_cast <unsigned> (time(0)));
    duckLength = (duck->GetLength()) * duckScale;
    duckWidth = (duck->GetWidth()) * duckScale;

    initialX = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / maxInitialX));
    initialY = 0;
    currX = initialX;
    currY = initialY;

    flyRight = rand() % 2; 
    flightAngle = (float(rand() % 5 + 3) / 10) * PI / 2; // Angle between 30% and 70% of PI/2
    if (!flyRight) {
        flightAngle += PI / 2;
    }

    flightSpeed = initialFlightSpeed;
    flight = new Flight(flightAngle, flightSpeed);
}


DuckHunt::~DuckHunt()
{
}


void DuckHunt::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    meshes["duck_head"] = duck->GetHead();
    meshes["duck_body"] = duck->GetBody();
    meshes["duck_wing_front"] = duck->GetWingFront();
    meshes["duck_wing_back"] = duck->GetWingBack();
    meshes["life"] = gameStats->GetLifeSymbol();
    meshes["bullet"] = gameStats->GetBulletSymbol();
    meshes["maxScore"] = gameStats->GetMaxScoreBox();
    meshes["currScore"] = gameStats->GetCurrScoreBox();
    meshes["grass_lower"] = gameStats->GetGrass(true);
    meshes["grass_upper"] = gameStats->GetGrass(false);
}


void DuckHunt::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(skyColor.x, skyColor.y, skyColor.z, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();

    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void DuckHunt::RenderInterface(int lifeCount, int bulletCount, float score) 
{
    gameStats->lifeCount = lifeCount;
    gameStats->bulletCount = bulletCount;
    gameStats->score = score;

    // Render lives
    interfaceMatrix *= transform2D::Translate(gameStats->lifePosX, gameStats->lifePosY);

    for (int i = 0; i < lifeCount; i++) {
        RenderMesh2D(meshes["life"], shaders["VertexColor"], interfaceMatrix);
        interfaceMatrix *= transform2D::Translate(gameStats->lifePosDist, 0);
    }

    // Render bullets
    interfaceMatrix = glm::mat3(1);
    interfaceMatrix *= transform2D::Translate(gameStats->bulletPosX, gameStats->bulletPosY);

    for (int i = 0; i < bulletCount; i++) {
        RenderMesh2D(meshes["bullet"], shaders["VertexColor"], interfaceMatrix);
        interfaceMatrix *= transform2D::Translate(gameStats->bulletPosDist, 0);
    }


    // Render score
    float maxScaleFactor = 16;
    float currScaleFactor = 0;
    if (score) {
        currScaleFactor = (gameStats->score / gameStats->maxScore) * maxScaleFactor;
    }

    interfaceMatrix = glm::mat3(1);
    interfaceMatrix *= transform2D::Translate(gameStats->scorePosX, gameStats->scorePosY);
    interfaceMatrix *= transform2D::Scale(maxScaleFactor, 1);
    RenderMesh2D(meshes["maxScore"], shaders["VertexColor"], interfaceMatrix);

    interfaceMatrix = glm::mat3(1);
    interfaceMatrix *= transform2D::Translate(gameStats->scorePosX, gameStats->scorePosY);
    interfaceMatrix *= transform2D::Scale(currScaleFactor, 1);
    RenderMesh2D(meshes["currScore"], shaders["VertexColor"], interfaceMatrix);

    interfaceMatrix = glm::mat3(1);
}


void DuckHunt::RenderEnvironment() 
{
    float radius = 70;
    float centerX = 925;
    float centerY = 525;

    Mesh* cloud = duck->CreateCircle("clouds", centerX, centerY, radius + 12, glm::vec3(1, 1, 1));
    RenderMesh2D(cloud, shaders["VertexColor"], interfaceMatrix);

    cloud = duck->CreateCircle("clouds", centerX + 100, centerY, radius, glm::vec3(1, 1, 1));
    RenderMesh2D(cloud, shaders["VertexColor"], interfaceMatrix);

    cloud = duck->CreateCircle("clouds", centerX - 100, centerY, radius, glm::vec3(1, 1, 1));
    RenderMesh2D(cloud, shaders["VertexColor"], interfaceMatrix);

    radius = 50;
    centerX = 225;
    centerY = 425;

    cloud = duck->CreateCircle("clouds", centerX, centerY, radius + 8, glm::vec3(1, 1, 1));
    RenderMesh2D(cloud, shaders["VertexColor"], interfaceMatrix);

    cloud = duck->CreateCircle("clouds", centerX + 70, centerY, radius, glm::vec3(1, 1, 1));
    RenderMesh2D(cloud, shaders["VertexColor"], interfaceMatrix);

    cloud = duck->CreateCircle("clouds", centerX - 70, centerY, radius, glm::vec3(1, 1, 1));
    RenderMesh2D(cloud, shaders["VertexColor"], interfaceMatrix);

    radius = 45;
    centerX = 505;
    centerY = 615;

    cloud = duck->CreateCircle("clouds", centerX, centerY, radius + 8, glm::vec3(1, 1, 1));
    RenderMesh2D(cloud, shaders["VertexColor"], interfaceMatrix);

    cloud = duck->CreateCircle("clouds", centerX - 70, centerY, radius, glm::vec3(1, 1, 1));
    RenderMesh2D(cloud, shaders["VertexColor"], interfaceMatrix);

    cloud = duck->CreateCircle("clouds", centerX + 70, centerY, radius, glm::vec3(1, 1, 1));
    RenderMesh2D(cloud, shaders["VertexColor"], interfaceMatrix);
}


void DuckHunt::ResetDuck() 
{
    initialX = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / maxInitialX)); initialY = 0;
    currX = initialX;
    currY = initialY;
    translateX = 0; translateY = 0;
    ducksCount++;

    if (ducksCount % 5 == 0) {
        flightSpeed *= 1.15f;
    }
    
    flyRight = rand() % 2; flyUp = true;
    flightAngle = (float(rand() % 5 + 3) / 10) * PI / 2; // Angle between 30% and 70% of PI/2
    if (!flyRight) {
        flightAngle += PI / 2;
    }
    flight = new Flight(flightAngle, flightSpeed);

    bulletCount = 3;

    duckActive = true;
    duckEvaded = false;
    duckDead = false;
    deadlyShot = false;

    skyColor = glm::vec3(0.8f, 1, 1);
}

void DuckHunt::Update(float deltaTimeSeconds)
{
    glm::ivec2 resolution = window->GetResolution();
    
    if (score >= gameStats->maxScore) {
        skyColor = glm::vec3(1, 1, 0.8f);
        duckActive = false;
        duckEvaded = false;
        duckDead = false;
    }

    if (duckActive && !gameOver) {
        timePassed += deltaTimeSeconds;
    }
    
    currX = initialX + translateX;
    currY = initialY + translateY;

    if (duckActive && (timePassed >= timeLimit || bulletCount <= 0)) {
        skyColor = glm::vec3(1, 0.8f, 0.8f);
        duckActive = false;
        duckEvaded = true;
        timePassed = 0;
        lifeCount--;
    }

    if (lifeCount == 0) {
        gameOver = true;
        skyColor = glm::vec3(0.4f, 0.4f, 0.4f);
    }

    if (duckActive && deadlyShot && !gameOver) {
        skyColor = glm::vec3(0.8f, 1, 0.8f);
        duckActive = false;
        duckDead = true;
        timePassed = 0;
    }

    if (duckActive) {
        // Right wall
        if (flyRight && currX > resolution.x) {
            flyRight = false;
            flightAngle = PI - flightAngle;
        }
        // Top wall
        if (flyUp && currY > resolution.y) {
            flyUp = false;
            flightAngle = -flightAngle;
        }
        // Left wall
        if (!flyRight && currX < 0) {
            flyRight = true;
            flightAngle = PI - flightAngle;
        }
        // Bottom wall
        if (!flyUp && currY < 0) {
            flyUp = true;
            flightAngle = -flightAngle;
        }
    }
    else if (duckEvaded && !gameOver) {
        flyUp = true;
        flightAngle = PI / 2;

        if (currY > resolution.y + duckLength) {
            ResetDuck();
        }
    }
    else if (duckDead && !gameOver) {
        flyUp = false;
        flightAngle = -PI / 2;

        if (currY < -duckLength) {
            ResetDuck();
        }
    }
    
    modelMatrix *= transform2D::Translate(initialX, initialY);

    modelMatrix *= transform2D::Translate(-duck->GetCenterX(), -duck->GetCenterY());

    modelMatrix = flight->TranslateDuck(modelMatrix, deltaTimeSeconds, flightAngle, translateX, translateY);

    flightMatrix = flight->RotateDuck(modelMatrix, flightAngle);

    flightMatrix *= transform2D::Scale(duckScale, duckScale);

    if (!duckDead) {
        wingsMatrix = flight->FlapWing(flightMatrix);
    }
    else {
        wingsMatrix = flightMatrix;
    }

    RenderInterface(lifeCount, bulletCount, score);
    
    RenderMesh2D(meshes["grass_lower"], shaders["VertexColor"], interfaceMatrix);

    if (!gameOver) {
        RenderMesh2D(meshes["duck_wing_front"], shaders["VertexColor"], wingsMatrix);

        RenderMesh2D(meshes["duck_body"], shaders["VertexColor"], flightMatrix);

        RenderMesh2D(meshes["duck_wing_back"], shaders["VertexColor"], wingsMatrix);

        RenderMesh2D(meshes["duck_head"], shaders["VertexColor"], flightMatrix);
    }

    RenderMesh2D(meshes["grass_upper"], shaders["VertexColor"], interfaceMatrix);

    RenderEnvironment();
    

    // Reset transform matrix for next frame
    modelMatrix = glm::mat3(1);
    flightMatrix = glm::mat3(1);
    wingsMatrix = glm::mat3(1);

    //printf("currX = %f ?? translateX = %f  ||  currY = %f ??  translateY = %f  ||  deltaTime = %f\n", currX, translateX, currY, translateY, deltaTimeSeconds);
}


void DuckHunt::FrameEnd()
{
}


void DuckHunt::OnKeyPress(int key, int mods)
{
    // Start or Reset the game
    if (key == GLFW_KEY_R) {
        gameOver = false;
        bulletCount = 3;
        lifeCount = 3;
        score = 0;
        flightSpeed = initialFlightSpeed;
        ResetDuck();
    }

    // Hack to increase score B-)
    if (key == GLFW_KEY_V) {
        score += 100;
    }
}


void DuckHunt::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    //printf("%d || %d\n", mouseX, mouseY);
}


void DuckHunt::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{    
    glm::ivec2 resolution = window->GetResolution();
    float revY = resolution.y - currY;

    bulletCount--;

    if (mouseX <= currX + duckLength / 2) {
        if (mouseX >= currX - duckLength / 2) {
            if (mouseY <= revY + duckWidth / 2) {
                if (mouseY >= revY - duckWidth / 2) {
                    deadlyShot = true;
                }
                else {
                    deadlyShot = false;
                }
            }
            else {
                deadlyShot = false;
            }
        }
        else {
            deadlyShot = false;
        }
    }
    else {
        deadlyShot = false;
    }

    if (deadlyShot) {
        score += pointsPerDuck * flightSpeed / 400;
    }
}
