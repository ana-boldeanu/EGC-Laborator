#include "lab_m1/duckhunt/duckhunt.h"

#include <vector>
#include <iostream>

#include "lab_m1/lab3/transform2D.h"
#include "lab_m1/lab3/object2D.h"

using namespace std;
using namespace m1;


DuckHunt::DuckHunt()
{
}


DuckHunt::~DuckHunt()
{
}
Mesh* DuckHunt::CreateCircle(
    const std::string& name,
    float centerX,
    float centerY,
    float radius,
    glm::vec3 color,
    bool fill)
{
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;
    Mesh* circle = new Mesh(name);

    circle->SetDrawMode(GL_TRIANGLE_FAN);
    
    indices.push_back(0);
    vertices.push_back(VertexFormat(glm::vec3(centerX, centerY, 0), color));
    for (int i = 1; i <= 16; i++) {
        indices.push_back(i);
        vertices.push_back(VertexFormat(glm::vec3(centerX + (radius * cos(i * 2 * 3.1415 / 16)),
            centerY + (radius * sin(i * 2 * 3.1415 / 16)), 0), color));
    }
    indices.push_back(1);

    circle->InitFromData(vertices, indices);
    return circle;
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

    // Duck body & beak triangles
    {
        vector<VertexFormat> vertices
        {
            VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0.33, 0, 0)),
            VertexFormat(glm::vec3(64, 22, 0), glm::vec3(0.4, 0, 0)),
            VertexFormat(glm::vec3(94, 80, 0), glm::vec3(0.4, 0, 0)),
            VertexFormat(glm::vec3(153, 14, 0), glm::vec3(0.42, 0, 0)),
            VertexFormat(glm::vec3(186, 90, 0), glm::vec3(0.4, 0, 0)),
            VertexFormat(glm::vec3(204, 22, 0), glm::vec3(0.4, 0, 0)),
            VertexFormat(glm::vec3(228, 49, 0), glm::vec3(0.4, 0, 0)),
            VertexFormat(glm::vec3(229, 85, 0), glm::vec3(0.4, 0, 0)),
            VertexFormat(glm::vec3(250, 95, 0), glm::vec3(0, 0.4, 0)),
            VertexFormat(glm::vec3(280, 65, 0), glm::vec3(0, 0.4, 0)),
            VertexFormat(glm::vec3(292, 82, 0), glm::vec3(1, 0.4, 0)),
            VertexFormat(glm::vec3(291, 94, 0), glm::vec3(1, 0.4, 0)),
            VertexFormat(glm::vec3(306, 90, 0), glm::vec3(1, 0.4, 0)),

            // Some copies for colors
            VertexFormat(glm::vec3(204, 22, 0), glm::vec3(1, 1, 1)),
            VertexFormat(glm::vec3(228, 49, 0), glm::vec3(1, 1, 1)),
            VertexFormat(glm::vec3(229, 85, 0), glm::vec3(1, 1, 1)),

            VertexFormat(glm::vec3(204, 22, 0), glm::vec3(0, 0.4, 0)),
            VertexFormat(glm::vec3(228, 49, 0), glm::vec3(0, 0.4, 0)),
            VertexFormat(glm::vec3(229, 85, 0), glm::vec3(0, 0.4, 0)),
        };

        vector<unsigned int> indices =
        {
            0, 1, 2,
            1, 3, 2,
            2, 3, 4,
            3, 5, 4,
            4, 5, 7,
            15, 13, 14,
            18, 17, 8,
            8, 16, 9,
            10, 12, 11
        };

        meshes["duck_body"] = new Mesh("duck_body");
        meshes["duck_body"]->InitFromData(vertices, indices);
    }

    // Duck wings triangles
    {
        vector<VertexFormat> vertices
        {
            VertexFormat(glm::vec3(131, 42, 0), glm::vec3(0.37, 0, 0)),
            VertexFormat(glm::vec3(220, 63, 0), glm::vec3(0.37, 0, 0)),
            VertexFormat(glm::vec3(105, 225, 0), glm::vec3(0.37, 0, 0))
        };

        vector<unsigned int> indices =
        {
            0, 1, 2
        };

        meshes["duck_wing_1"] = new Mesh("duck_wing_1");
        meshes["duck_wing_1"]->InitFromData(vertices, indices);
    }

    // Duck wings triangles
    {
        vector<VertexFormat> vertices
        {
            VertexFormat(glm::vec3(131, 42, 0), glm::vec3(0.34, 0, 0)),
            VertexFormat(glm::vec3(220, 63, 0), glm::vec3(0.34, 0, 0)),
            VertexFormat(glm::vec3(177, 213, 0), glm::vec3(0.34, 0, 0))
        };

        vector<unsigned int> indices =
        {
            0, 1, 2
        };

        meshes["duck_wing_2"] = new Mesh("duck_wing_2");
        meshes["duck_wing_2"]->InitFromData(vertices, indices);
    }

    meshes["duck_head"] = CreateCircle("duck_head", 270, 84, 23, glm::vec3(0, 0.4, 0), true);
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
    glEnable(GL_CULL_FACE);

    

    RenderMesh(meshes["duck_wing_1"], shaders["VertexColor"], glm::vec3(0, 0, 0));

    RenderMesh(meshes["duck_body"], shaders["VertexColor"], glm::vec3(0, 0, 0));

    RenderMesh(meshes["duck_wing_2"], shaders["VertexColor"], glm::vec3(0, 0, 0));

    RenderMesh(meshes["duck_head"], shaders["VertexColor"], glm::vec3(0, 0, 0));
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
