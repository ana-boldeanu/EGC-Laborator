#include "lab_m1/duckhunt/duckhunt.h"

#include <vector>
#include <iostream>

using namespace std;
using namespace m1;


DuckHunt::DuckHunt()
{
    duck = new Duck();
    flightAngle = 0.4;
    flightStep = 2;
    flyRight = true;
    flyUp = true;
    flight = new Flight(flightAngle, flightStep);
    modelMatrix = glm::mat3(1);
    flightMatrix = glm::mat3(1);
    wingsMatrix = glm::mat3(1);
    currX = duck->GetCenterX();
    currY = duck->GetCenterY();
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
}


void DuckHunt::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();

    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void DuckHunt::Update(float deltaTimeSeconds)
{
    // Check duck current position
    glm::ivec2 resolution = window->props.resolution;

    if (currX > resolution.x) {
        flyRight = false;
    }
    else if (currX < 0) {
        flyRight = true;
    }

    if (currY > resolution.y) {
        flyUp = false;
    }
    else if (currY < 0) {
        flyUp = true;
    }


    modelMatrix = flight->RotateDuck(modelMatrix, flightAngle);

    flightMatrix = flight->TranslateDuck(modelMatrix, flightStep, flyRight, flyUp, currX, currY);

    wingsMatrix = flight->FlapWing(flightMatrix);
   
    RenderMesh2D(meshes["duck_wing_front"], shaders["VertexColor"], wingsMatrix);

    RenderMesh2D(meshes["duck_body"], shaders["VertexColor"], flightMatrix);

    RenderMesh2D(meshes["duck_wing_back"], shaders["VertexColor"], wingsMatrix);

    RenderMesh2D(meshes["duck_head"], shaders["VertexColor"], flightMatrix);

    // Reset transform matrix for next frame
    modelMatrix = glm::mat3(1);
    flightMatrix = glm::mat3(1);
    wingsMatrix = glm::mat3(1);

    printf("%d  ||  %d  ||  %f == %d  ||  %f == %d\n", flyRight, flyUp, currX, resolution.x, currY, resolution.y);
}


void DuckHunt::FrameEnd()
{
}


void DuckHunt::OnInputUpdate(float deltaTime, int mods)
{
}


void DuckHunt::OnKeyPress(int key, int mods)
{
}


void DuckHunt::OnKeyRelease(int key, int mods)
{
}


void DuckHunt::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
}


void DuckHunt::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
}


void DuckHunt::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}


void DuckHunt::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void DuckHunt::OnWindowResize(int width, int height)
{
}
