#include "lab_m1/race/race.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;


Race::Race()
{
    camera = new Camera();
    camera_position = glm::vec3(0, camera->distanceToTarget, -camera->distanceToTarget);
    camera_center = glm::vec3(initial_x, initial_y, initial_z);
    camera_up = glm::vec3(0, 1, 0);

    center_x = initial_x;
    center_y = initial_y;
    center_z = initial_z;

    glm::ivec2 resolution = window->GetResolution();
    minimap = ViewportArea(1000, 20, resolution.x / 5, resolution.y / 5);
}


Race::~Race()
{
}


void Race::Init()
{
    camera->Set(camera_position, camera_center, camera_up);

    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    meshes["course"] = course->course;

    projectionMatrix = glm::perspective(fov, window->props.aspectRatio, z_near, z_far);

    

    /////////////////////////////////////////////
    /////////////////////////////////////////////

    
    for (auto& i : course->polygon_points) {
        cout << i << endl;
    }

    cout << endl;

    for (auto& i : course->inner_points) {
        cout << i << endl;
    }

    cout << endl;

    for (auto& i : course->outer_points) {
        cout << i << endl;
    }
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


void Race::RenderScene()
{
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(translate_x, translate_y, translate_z);
        modelMatrix *= transform3D::RotateOY(move_angle);

        RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(2, 0.5f, 0);
        modelMatrix *= transform3D::RotateOX(RADIANS(60.0f));
        RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
    }

    glm::mat4 modelMatrix = glm::mat4(1);
    RenderMesh(meshes["course"], shaders["VertexColor"], modelMatrix);
}

void Race::Update(float deltaTimeSeconds)
{
    // Render the scene
    projectionMatrix = glm::perspective(fov, window->props.aspectRatio, z_near, z_far);
    RenderScene();
    DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);

    // Render the scene again, in the minimap
    glClear(GL_DEPTH_BUFFER_BIT);
    projectionMatrix = glm::ortho(left, right, bottom, top, z_near, z_far);

    glViewport(minimap.x, minimap.y, minimap.width, minimap.height);

    RenderScene();
    DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}


void Race::FrameEnd()
{
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
    float sensitivity = 0.01f;
    float angle = deltaTime * rotate_speed * sensitivity;

    if (window->KeyHold(GLFW_KEY_A)) {
        move_angle += angle;
        camera->RotateThirdPerson_OY(angle);
    }

    if (window->KeyHold(GLFW_KEY_D)) {
        move_angle -= angle;
        camera->RotateThirdPerson_OY(-angle);
    }

    float step = move_speed * deltaTime;

    if (window->KeyHold(GLFW_KEY_W)) {
        translate_x += step * sin(move_angle);
        translate_z += step * cos(move_angle);

        camera->MoveForward(step);
    }

    if (window->KeyHold(GLFW_KEY_S)) {
        translate_x -= step * sin(move_angle);
        translate_z -= step * cos(move_angle);

        camera->MoveForward(-step);
    }

    center_x = translate_x;
    center_z = translate_z;
}


void Race::OnKeyPress(int key, int mods)
{
}
