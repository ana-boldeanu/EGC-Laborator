#include "lab_m1/skifree/skifree.h"

#include <vector>
#include <string>
#include <iostream>
#include <time.h>

using namespace std;
using namespace m1;


SkiFree::SkiFree()
{
    // Create the third-person camera object
    camera = new Camera();
    float dist = camera->distanceToTarget;
    camera_position = glm::vec3(0, 7, 10);
    camera_center = glm::vec3(0, 0, 0);
    camera_up = glm::vec3(0, 1, 0);
}


SkiFree::~SkiFree()
{
}


void SkiFree::Init()
{
    const string sourceTextureDir = PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "skifree", "textures");

    // Load textures
    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "snow.jpg").c_str(), GL_REPEAT);
        mapTextures["snow"] = texture;

        texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "gift.jpg").c_str(), GL_REPEAT);
        mapTextures["gift"] = texture;

        texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "pine1.jpg").c_str(), GL_REPEAT);
        mapTextures["pine"] = texture;

        texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "trunk.jpg").c_str(), GL_REPEAT);
        mapTextures["trunk"] = texture;

        texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "ski.jpg").c_str(), GL_REPEAT);
        mapTextures["ski"] = texture;

        texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "player.jpg").c_str(), GL_REPEAT);
        mapTextures["player"] = texture;

        texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "rock.jpg").c_str(), GL_REPEAT);
        mapTextures["rock"] = texture;

        texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "black_metal.jpg").c_str(), GL_REPEAT);
        mapTextures["metal"] = texture;

        texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "lightbulb1.jpg").c_str(), GL_REPEAT);
        mapTextures["lightbulb"] = texture;
    }

    // Load meshes
    {
        Mesh* plane = meshes_builder->plane;
        meshes[plane->GetMeshID()] = plane;

        Mesh* gift = meshes_builder->gift;
        meshes[gift->GetMeshID()] = gift;

        for (Mesh* mesh : meshes_builder->tree) {
            meshes[mesh->GetMeshID()] = mesh;
        }

        for (Mesh* mesh : meshes_builder->player) {
            meshes[mesh->GetMeshID()] = mesh;
        }

        for (Mesh* mesh : meshes_builder->rocks) {
            meshes[mesh->GetMeshID()] = mesh;
        }

        for (Mesh* mesh : meshes_builder->lamp) {
            meshes[mesh->GetMeshID()] = mesh;
        }
    }

    // Create a shader program for drawing face polygon with the color of the normal
    {
        Shader *shader = new Shader("LabShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "skifree", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "skifree", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // Set up camera
    camera->Set(camera_position, camera_center, camera_up);
    projectionMatrix = glm::perspective(fov, window->props.aspectRatio, z_near, z_far);
}


void SkiFree::FrameStart()
{
    // Clear the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();

    // Set the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void SkiFree::Update(float deltaTimeSeconds)
{
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= meshes_builder->plane_matrix;
        modelMatrix *= transform3D::RotateOX(angle);

        renderSnow = 1;
        RenderSimpleMesh(meshes["plane"], shaders["LabShader"], modelMatrix, mapTextures["snow"]);
        renderSnow = 0;
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(2, 0, 0);
        modelMatrix *= transform3D::RotateOX(angle);
        modelMatrix *= meshes_builder->gift_matrix;
        RenderSimpleMesh(meshes["gift"], shaders["LabShader"], modelMatrix, mapTextures["gift"]);
    }

    {
        glm::mat4 modelMatrix;
        for (int i = 0; i < meshes_builder->tree.size(); i++) {
            modelMatrix = glm::mat4(1);
            modelMatrix *= transform3D::Translate(4, 0, 0);
            modelMatrix *= meshes_builder->tree_matrix[i];
            RenderSimpleMesh(meshes[meshes_builder->tree[i]->GetMeshID()], shaders["LabShader"], modelMatrix, mapTextures[meshes_builder->tree_tex[i]]);
        }
    }

    {
        glm::mat4 modelMatrix;
        for (int i = 0; i < meshes_builder->player.size(); i++) {
            modelMatrix = glm::mat4(1);
            modelMatrix *= transform3D::Translate(-4, 0, 0);
            modelMatrix *= transform3D::RotateOX(angle);
            modelMatrix *= meshes_builder->player_matrix[i];
            RenderSimpleMesh(meshes[meshes_builder->player[i]->GetMeshID()], shaders["LabShader"], modelMatrix, mapTextures[meshes_builder->player_tex[i]]);
        }
    }

    {
        glm::mat4 modelMatrix;
        for (int i = 0; i < meshes_builder->rocks.size(); i++) {
            modelMatrix = glm::mat4(1);
            modelMatrix *= transform3D::Translate(-2, 0, 0);
            modelMatrix *= transform3D::RotateOX(angle);
            modelMatrix *= meshes_builder->rocks_matrix[i];
            RenderSimpleMesh(meshes[meshes_builder->rocks[i]->GetMeshID()], shaders["LabShader"], modelMatrix, mapTextures[meshes_builder->rocks_tex[i]]);
        }
    }

    {
        glm::mat4 modelMatrix;
        for (int i = 0; i < meshes_builder->lamp.size(); i++) {
            modelMatrix = glm::mat4(1);
            modelMatrix *= transform3D::Translate(0, 0, 0);
            modelMatrix *= meshes_builder->lamp_matrix[i];
            RenderSimpleMesh(meshes[meshes_builder->lamp[i]->GetMeshID()], shaders["LabShader"], modelMatrix, mapTextures[meshes_builder->lamp_tex[i]]);
        }
    }
}


void SkiFree::FrameEnd()
{
    // DrawCoordinateSystem();
}


void SkiFree::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, Texture2D* texture)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Send uniform values
    int location_snow = glGetUniformLocation(shader->GetProgramID(), "renderSnow");
    glUniform1i(location_snow, renderSnow);

    int location_time = glGetUniformLocation(shader->GetProgramID(), "time");
    glUniform1f(location_time, Engine::GetElapsedTime());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
    glUniform1i(glGetUniformLocation(shader->program, "texture"), 0);

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}



void SkiFree::OnInputUpdate(float deltaTime, int mods)
{
    if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        glm::vec3 up = glm::vec3(0, 1, 0);
        glm::vec3 right = GetSceneCamera()->m_transform->GetLocalOXVector();
        glm::vec3 forward = GetSceneCamera()->m_transform->GetLocalOZVector();
        forward = glm::normalize(glm::vec3(forward.x, 0, forward.z));
    }
}


void SkiFree::OnKeyPress(int key, int mods)
{
    // Add key press event
}

void SkiFree::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}