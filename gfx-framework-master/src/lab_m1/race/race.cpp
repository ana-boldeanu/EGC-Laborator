#include "lab_m1/race/race.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;


Race::Race()
{
}


Race::~Race()
{
}


void Race::Init()
{
    camera = new Camera();
    camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    projectionMatrix = glm::perspective(RADIANS(FoV), window->props.aspectRatio, Z_NEAR, Z_FAR);
}


void Race::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set the screen area where to draw
    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}


void Race::Update(float deltaTimeSeconds)
{
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(0, 1, 0);
        modelMatrix *= transform3D::RotateOY(RADIANS(45.0f));

        RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(2, 0.5f, 0);
        modelMatrix *= transform3D::RotateOX(RADIANS(60.0f));
        RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
    }
}


void Race::FrameEnd()
{
    DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}


// Attention! The `RenderMesh()` function overrides the usual `RenderMesh()` that we've been 
// using up until now. This new function uses the view matrix from the camera that you just
// implemented, and the local projection matrix.
void Race::RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}


void Race::OnInputUpdate(float deltaTime, int mods)
{
    // move the camera only if MOUSE_RIGHT button is pressed
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        if (window->KeyHold(GLFW_KEY_W)) {
            // TODO(student): Translate the camera forward
            camera->MoveForward(cameraSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_A)) {
            // TODO(student): Translate the camera to the left
            camera->TranslateRight(-cameraSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_S)) {
            // TODO(student): Translate the camera backward
            camera->MoveForward(-cameraSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_D)) {
            // TODO(student): Translate the camera to the right
            camera->TranslateRight(cameraSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_Q)) {
            // TODO(student): Translate the camera downward
            camera->TranslateUpward(-cameraSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_E)) {
            // TODO(student): Translate the camera upward
            camera->TranslateUpward(cameraSpeed * deltaTime);
        }
    }

    // TODO(student): Change projection parameters. Declare any extra
    // variables you might need in the class header. Inspect this file
    // for any hardcoded projection arguments (can you find any?) and
    // replace them with those extra variables.

    // Task 8
    if (window->KeyHold(GLFW_KEY_N))
    {
        FoV -= deltaTime * 10;

        if (!projectOrtho)
        {
            projectionMatrix = glm::perspective(RADIANS(FoV), window->props.aspectRatio, Z_NEAR, Z_FAR);
        }
    }

    if (window->KeyHold(GLFW_KEY_M))
    {
        FoV += deltaTime * 10;

        if (!projectOrtho)
        {
            projectionMatrix = glm::perspective(RADIANS(FoV), window->props.aspectRatio, Z_NEAR, Z_FAR);
        }
    }

    // Task 9
    if (window->KeyHold(GLFW_KEY_I))
    {
        bottom -= deltaTime;
        top += deltaTime;

        if (projectOrtho)
        {
            projectionMatrix = glm::ortho(left, right, bottom, top, Z_NEAR, Z_FAR);
        }
    }

    if (window->KeyHold(GLFW_KEY_K))
    {
        bottom += deltaTime;
        top -= deltaTime;

        if (projectOrtho)
        {
            projectionMatrix = glm::ortho(left, right, bottom, top, Z_NEAR, Z_FAR);
        }
    }

    if (window->KeyHold(GLFW_KEY_J))
    {
        left -= deltaTime;
        right += deltaTime;

        if (projectOrtho)
        {
            projectionMatrix = glm::ortho(left, right, bottom, top, Z_NEAR, Z_FAR);
        }
    }

    if (window->KeyHold(GLFW_KEY_L))
    {
        left += deltaTime;
        right -= deltaTime;

        if (projectOrtho)
        {
            projectionMatrix = glm::ortho(left, right, bottom, top, Z_NEAR, Z_FAR);
        }
    }
}


void Race::OnKeyPress(int key, int mods)
{
    // TODO(student): Switch projections
    if (key == GLFW_KEY_O)
    {
        projectOrtho = true;
        projectionMatrix = glm::ortho(left, right, bottom, top, Z_NEAR, Z_FAR);
    }
    if (key == GLFW_KEY_P)
    {
        projectOrtho = false;
        projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, Z_NEAR, Z_FAR);
    }
}


void Race::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Race::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event

    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float sensitivityOX = 0.002f;
        float sensitivityOY = 0.002f;
        if (window->GetSpecialKeyState() == 0) {
            // TODO(student): Rotate the camera in first-person mode around
            // OX and OY using `deltaX` and `deltaY`. Use the sensitivity
            // variables for setting up the rotation speed.
            camera->RotateFirstPerson_OX(sensitivityOX * deltaY);
            camera->RotateFirstPerson_OY(sensitivityOY * deltaX);
        }

        if (window->GetSpecialKeyState() & GLFW_MOD_CONTROL) {
            // TODO(student): Rotate the camera in third-person mode around
            // OX and OY using `deltaX` and `deltaY`. Use the sensitivity
            // variables for setting up the rotation speed.
            camera->RotateThirdPerson_OX(sensitivityOX * deltaY);
            camera->RotateThirdPerson_OY(sensitivityOY * deltaX);
        }
    }
}


void Race::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Race::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Race::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Race::OnWindowResize(int width, int height)
{
}
