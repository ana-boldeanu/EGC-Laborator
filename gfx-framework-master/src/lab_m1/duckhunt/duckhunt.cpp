#include "lab_m1/duckhunt/duckhunt.h"

#include <vector>
#include <iostream>

#include "lab_m1/lab3/transform2D.h"
#include "lab_m1/lab3/object2D.h"

using namespace std;
using namespace m1;


DuckHunt::DuckHunt()
{
    duck = new Duck();
    flight = new Flight();
    modelMatrix = glm::mat3(1);
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

    modelMatrix = glm::mat3(1);
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
    modelMatrix = flight->FlapWing(modelMatrix);

    RenderMesh2D(meshes["duck_wing_front"], shaders["VertexColor"], modelMatrix);

    RenderMesh2D(meshes["duck_body"], shaders["VertexColor"], glm::mat3(1));

    RenderMesh2D(meshes["duck_wing_back"], shaders["VertexColor"], modelMatrix);

    RenderMesh2D(meshes["duck_head"], shaders["VertexColor"], glm::mat3(1));

    modelMatrix = glm::mat3(1);
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
