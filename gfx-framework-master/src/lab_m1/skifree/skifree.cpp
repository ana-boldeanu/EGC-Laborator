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
    camera_position = glm::vec3(playerX, playerY + dist, playerZ + dist);
    camera_center = glm::vec3(playerX, playerY, playerZ);
    camera_up = glm::vec3(0, 1, 0);

    // Set light & material properties
    lightDirection = glm::vec3(0, -1, 0);
    materialShininess = 100;
    materialKd = 0.7f;
    materialKs = 0.5f;
}


SkiFree::~SkiFree()
{
}


//////////////////////////////////////////////////////////////////////////////
// NOTE TO SELF
//////////////////////////////////////////////////////////////////////////////
// Trebuie neaparat sa schimb regula de miscare (sa misc jucatorul, nu obiectele),
// pentru ca altfel nu se vede miscarea si fata de zapada
//////////////////////////////////////////////////////////////////////////////
// Pt generare de obiecte, o sa tin un vector de vreo 5 pozitii (vector3) pt 
// fiecare tip de obiect, si le randez intr-un for. Tot intr-un for in Update(),
// incrementez pasul si atunci cand iese din viewport resetez pozitia initiala




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

    // Compute lights positions
    for (auto& coords : lampCoords) {
        lampLightsCoords.push_back(coords + glm::vec4(meshes_builder->lamp_offset_1, 0));
        lampLightsCoords.push_back(coords + glm::vec4(meshes_builder->lamp_offset_2, 0));
    }

    for (auto& coords : treeCoords) {
        treeLightsCoords.push_back(coords + light_offset);
    }

    for (auto& coords : giftCoords) {
        giftsLightsCoords.push_back(coords + light_offset);
    }
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
        // Plane
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(playerX, playerY, playerZ);
        modelMatrix *= rotationMatrix;
        modelMatrix *= meshes_builder->plane_matrix;

        renderSnow = 1;
        RenderSimpleMesh(meshes["plane"], shaders["LabShader"], modelMatrix, mapTextures["snow"]);
        renderSnow = 0;
    }

    {
        // Gift
        for (auto& gift : giftCoords) {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix *= transform3D::Translate(gift.x, gift.y, gift.z);
            modelMatrix *= transform3D::RotateOX(angle);
            modelMatrix *= meshes_builder->gift_matrix;
            RenderSimpleMesh(meshes["gift"], shaders["LabShader"], modelMatrix, mapTextures["gift"]);
        }
    }

    {
        // Tree
        for (auto& tree : treeCoords) {
            glm::mat4 modelMatrix;
            for (int i = 0; i < meshes_builder->tree.size(); i++) {
                modelMatrix = glm::mat4(1);
                modelMatrix *= transform3D::Translate(tree.x, tree.y, tree.z);
                modelMatrix *= meshes_builder->tree_matrix[i];
                RenderSimpleMesh(meshes[meshes_builder->tree[i]->GetMeshID()], shaders["LabShader"], modelMatrix, mapTextures[meshes_builder->tree_tex[i]]);
            }
        }
    }

    {
       // Player
        glm::mat4 modelMatrix;
        for (int i = 0; i < meshes_builder->player.size(); i++) {
            modelMatrix = glm::mat4(1);
            modelMatrix *= transform3D::Translate(playerX, playerY, playerZ);
            modelMatrix *= transform3D::RotateOX(angle);
            modelMatrix *= transform3D::RotateOY(player_rotation);
            modelMatrix *= meshes_builder->player_matrix[i];
            RenderSimpleMesh(meshes[meshes_builder->player[i]->GetMeshID()], shaders["LabShader"], modelMatrix, mapTextures[meshes_builder->player_tex[i]]);
        }
    }

    {
        // Rocks
        for (auto& rocks : rocksCoords) {
            glm::mat4 modelMatrix;
            for (int i = 0; i < meshes_builder->rocks.size(); i++) {
                modelMatrix = glm::mat4(1);
                modelMatrix *= transform3D::Translate(rocks.x, rocks.y, rocks.z);
                modelMatrix *= transform3D::RotateOX(angle);
                modelMatrix *= meshes_builder->rocks_matrix[i];
                RenderSimpleMesh(meshes[meshes_builder->rocks[i]->GetMeshID()], shaders["LabShader"], modelMatrix, mapTextures[meshes_builder->rocks_tex[i]]);
            }
        }
    }

    {
        // Lamp
        for (auto& lamp : lampCoords) {
            glm::mat4 modelMatrix;
            for (int i = 0; i < meshes_builder->lamp.size(); i++) {
                modelMatrix = glm::mat4(1);
                modelMatrix *= transform3D::Translate(lamp.x, lamp.y, lamp.z);
                modelMatrix *= meshes_builder->lamp_matrix[i];
                RenderSimpleMesh(meshes[meshes_builder->lamp[i]->GetMeshID()], shaders["LabShader"], modelMatrix, mapTextures[meshes_builder->lamp_tex[i]]);
            }
        }
    }

    // Move objects
    /*playerX += step * cos(PI / 2 - player_rotation);
    playerY -= step * sin(angle);
    playerZ += step * cos(angle);*/

    // Update camera
    float dist = camera->distanceToTarget;
    camera_position = glm::vec3(playerX, playerY + dist, playerZ + dist);
    camera_center = glm::vec3(playerX, playerY, playerZ);
    camera->Set(camera_position, camera_center, camera_up);
    camera->TranslateUpward(-dist/2);

    // printf("TreeX - PlayerX = %f || Y = %f || z = %f\n", treeX - playerX, treeY - playerY, treeZ - playerZ);

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

    // Uniforms for lights
    int location_lamps = glGetUniformLocation(shader->program, "lamp_light_position");
    glUniform3fv(location_lamps, 12, glm::value_ptr(lampLightsCoords[0]));

    int location_trees = glGetUniformLocation(shader->program, "tree_light_position");
    glUniform3fv(location_trees, 6, glm::value_ptr(treeLightsCoords[0]));

    int location_gifts = glGetUniformLocation(shader->program, "gift_light_position");
    glUniform3fv(location_gifts, 4, glm::value_ptr(giftsLightsCoords[0]));

    int light_direction = glGetUniformLocation(shader->program, "light_direction");
    glUniform3f(light_direction, lightDirection.x, lightDirection.y, lightDirection.z);

    // Set eye position uniform
    int eye_position = glGetUniformLocation(shader->program, "eye_position");
    glUniform3f(eye_position, camera_position.x, camera_position.y, camera_position.z);

    // Set material property uniforms (shininess, kd, ks, object color) 
    int material_shininess = glGetUniformLocation(shader->program, "material_shininess");
    glUniform1i(material_shininess, materialShininess);

    int material_kd = glGetUniformLocation(shader->program, "material_kd");
    glUniform1f(material_kd, materialKd);

    int material_ks = glGetUniformLocation(shader->program, "material_ks");
    glUniform1f(material_ks, materialKs);

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}



void SkiFree::OnInputUpdate(float deltaTime, int mods)
{
    
}


void SkiFree::OnKeyPress(int key, int mods)
{
    
}

void SkiFree::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    glm::ivec2 resolution = window->GetResolution();
    int posX = mouseX;
    int width = resolution.x / 2;

    // Map [0, width] to [-width/2, width/2]
    posX -= width;

    // Map to [-max_angle, max_angle]
    player_rotation = max_player_rotation * posX / width;
}