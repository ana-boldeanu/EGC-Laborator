#include "lab_m1/duckhunt/duckhunt.h"

#include <vector>
#include <iostream>

using namespace std;
using namespace m1;


DuckHunt::DuckHunt()
{
    duck = new Duck();
    duckScale = 0.8f;
    duckLength = duck->GetLength();
    duckWidth = duck->GetWidth();
    currX = duck->GetCenterX();
    currY = duck->GetCenterY();

    initialX = 0; initialY = 0;
    flightAngle = 0.8f;
    flightSpeed = 6;
    flyRight = true; flyUp = true;
    flight = new Flight(flightAngle, flightSpeed);

    lifeCount = 3; 
    bulletCount = 3; 
    score = 0;
    gameStats = new Interface();

    modelMatrix = glm::mat3(1);
    flightMatrix = glm::mat3(1);
    wingsMatrix = glm::mat3(1);
    interfaceMatrix = glm::mat3(1);
    skyColor = glm::vec3(0.8f, 1, 1);
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

void DuckHunt::ResetDuck() 
{
    currX = duck->GetCenterX();
    currY = duck->GetCenterY();

    initialX = 0; initialY = 0;
    flightAngle = 0.8f;
    flightSpeed = 6;
    flyRight = true; flyUp = true;
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
    
    timePassed += deltaTimeSeconds;

    if (duckActive && timePassed >= timeLimit) {
        skyColor = glm::vec3(1, 0.8f, 0.8f);
        duckActive = false;
        duckEvaded = true;
        timePassed = 0;
    }

    if (duckActive && deadlyShot) {
        skyColor = glm::vec3(0.8f, 1, 0.8f);
        duckActive = false;
        duckDead = true;
        timePassed = 0;
    }

    if (duckActive) {
        // Right wall
        if (flyRight && currX + duckLength / 2 > resolution.x) {
            flyRight = false;
            flightAngle = PI - flightAngle;
        }
        // Top wall
        if (flyUp && currY + duckWidth / 2 > resolution.y) {
            flyUp = false;
            flightAngle = -flightAngle;
        }
        // Left wall
        if (!flyRight && currX + duckLength / 2 < 0) {
            flyRight = true;
            flightAngle = PI - flightAngle;
        }
        // Bottom wall
        if (!flyUp && currY + duckWidth / 2 < 0) {
            flyUp = true;
            flightAngle = -flightAngle;
        }
    }
    else if (duckEvaded) {
        flyUp = true;
        flightAngle = PI / 2;

        if (currY > resolution.y + duckLength) {
            ResetDuck();
        }
    }
    else if (duckDead) {
        flyUp = false;
        flightAngle = -PI / 2;

        if (currY < -duckLength) {
            ResetDuck();
        }
    }
    
    modelMatrix *= transform2D::Translate(initialX, initialY);

    modelMatrix = flight->TranslateDuck(modelMatrix, flightAngle, currX, currY);

    flightMatrix = flight->RotateDuck(modelMatrix, flightAngle);

    flightMatrix *= transform2D::Scale(duckScale, duckScale);

    wingsMatrix = flight->FlapWing(flightMatrix);

   
    RenderMesh2D(meshes["duck_wing_front"], shaders["VertexColor"], wingsMatrix);

    RenderMesh2D(meshes["duck_body"], shaders["VertexColor"], flightMatrix);

    RenderMesh2D(meshes["duck_wing_back"], shaders["VertexColor"], wingsMatrix);

    RenderMesh2D(meshes["duck_head"], shaders["VertexColor"], flightMatrix);

    RenderInterface(lifeCount, bulletCount, score);

    // Reset transform matrix for next frame
    modelMatrix = glm::mat3(1);
    flightMatrix = glm::mat3(1);
    wingsMatrix = glm::mat3(1);

    //printf("currX = %f ?? %d  ||  currY = %f ?? %d\n", currX, resolution.x, currY, resolution.y);
}


void DuckHunt::FrameEnd()
{
}


void DuckHunt::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_D) {
        deadlyShot = true;
    }
}


void DuckHunt::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    //printf("mouseX = %d  ||  mouseY = %d\n", mouseX, mouseY);    
}


void DuckHunt::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{    
    if (button == GLFW_MOUSE_BUTTON_1) {
        printf("Nice!\n");
    }
}


void DuckHunt::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}
