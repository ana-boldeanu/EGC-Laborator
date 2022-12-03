#include "lab_m1/race/race.h"

#include <iostream>

using namespace std;
using namespace m1;


Race::Race()
{
    main_camera = new Camera();
    float dist = main_camera->distanceToTarget;
    main_camera_position = glm::vec3(center_x - dist, center_y + dist / 2, center_z);
    main_camera_center = glm::vec3(center_x, center_y, center_z);
    main_camera_up = glm::vec3(0, 1, 0);

    minimap_camera = new Camera();
    minimap_camera_position = glm::vec3(0, minimap_camera->distanceToTarget, z_near);
    minimap_camera_center = glm::vec3(center_x, center_y, center_z);
    minimap_camera_up = glm::vec3(0, 1, 0);

    glm::ivec2 resolution = window->GetResolution();
    minimap = ViewportArea(1000, 20, resolution.x / 5, resolution.y / 5);
}


Race::~Race()
{
}


void Race::Init()
{
    main_camera->Set(main_camera_position, main_camera_center, main_camera_up);
    minimap_camera->Set(minimap_camera_position, minimap_camera_center, minimap_camera_up);

    projectionMatrix = glm::perspective(fov, window->props.aspectRatio, z_near, z_far);
    orthoMatrix = glm::ortho(left, right, bottom, top, z_near, z_far);

    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("tree");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "race"), "tree.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    meshes["course"] = course->course;
    meshes["lines"] = course->lines;
    meshes["grass"] = environment->grass;
}


void Race::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.35f, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set the screen area where to draw
    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}


void Race::RenderTrees()
{
    glm::mat4 modelMatrix = glm::mat4(1);
    float scale = 0.5f;
    size_t size = course->tree_locations_0.size();
    int side, rotation;
    float x, y, z;

    for (int i = 0; i < size; i += 501) {
        side = course->locations[i];
        switch (side) {
        case 0:
            x = course->tree_locations_0[i].x;
            y = course->tree_locations_0[i].y;
            z = course->tree_locations_0[i].z;
            break;

        case 1:
            x = course->tree_locations_1[i].x;
            y = course->tree_locations_1[i].y;
            z = course->tree_locations_1[i].z;
            break;

        case 2:
            x = course->tree_locations_2[i].x;
            y = course->tree_locations_2[i].y;
            z = course->tree_locations_2[i].z;
            break;

        default:
            x = course->tree_locations_3[i].x;
            y = course->tree_locations_3[i].y;
            z = course->tree_locations_3[i].z;
            break;
        }

        rotation = course->rotations[i];

        float road_scale = course->road_scale;
        modelMatrix *= transform3D::Translate(road_scale * x, road_scale * y, road_scale * z);
        modelMatrix *= transform3D::Scale(scale, scale, scale);
        modelMatrix *= transform3D::RotateOY((float)rotation);
        RenderMesh(meshes["tree"], shaders["VertexNormal"], modelMatrix);
        modelMatrix = glm::mat4(1);
    }
}


void Race::RenderScene()
{
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(translate_x, translate_y, translate_z);
        modelMatrix *= transform3D::RotateOY(1);    // Change this for model-specific rotation
        modelMatrix *= transform3D::RotateOY(move_angle);

        RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
    }

    RenderTrees();

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        float road_scale = course->road_scale;
        modelMatrix *= transform3D::Scale(road_scale, road_scale, road_scale);
        RenderMesh(meshes["course"], shaders["VertexColor"], modelMatrix);
        modelMatrix *= transform3D::Translate(0, 0.01f, 0);
        RenderMesh(meshes["lines"], shaders["VertexColor"], modelMatrix);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        RenderMesh(meshes["grass"], shaders["VertexColor"], modelMatrix);
    }


    //cout << "x = " << center_x << " y = " << center_y << " z = " << center_z << endl;
}


void Race::Update(float deltaTimeSeconds)
{
    // Render the scene
    project_ortho = false;
    projectionMatrix = glm::perspective(fov, window->props.aspectRatio, z_near, z_far);

    RenderScene();

    // Render the scene again, in the minimap
    glClear(GL_DEPTH_BUFFER_BIT);

    project_ortho = true;
    glViewport(minimap.x, minimap.y, minimap.width, minimap.height);
    orthoMatrix = glm::ortho(left, right, bottom, top, z_near, z_far);
    minimap_camera->Set(minimap_camera_position, minimap_camera_center, minimap_camera_up);

    RenderScene();

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
    
    // Either use minimap camera or main camera
    if (project_ortho) {
        glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(minimap_camera->GetViewMatrix()));
        glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(orthoMatrix));
    }
    else {
        glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(main_camera->GetViewMatrix()));
        glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    }

    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}


void Race::OnInputUpdate(float deltaTime, int mods)
{
    cout << course->IsOnRoad(glm::vec2(center_x, center_z)) << endl;

    // Car and camera rotations
    float sensitivity = 0.01f;
    float angle = deltaTime * rotate_speed * sensitivity;

    if (window->KeyHold(GLFW_KEY_A)) {
        move_angle += angle;
        main_camera->RotateThirdPerson_OY(angle);
    }

    if (window->KeyHold(GLFW_KEY_D)) {
        move_angle -= angle;
        main_camera->RotateThirdPerson_OY(-angle);
    }

    // Car and camera translations
    float step = move_speed * deltaTime;

    if (window->KeyHold(GLFW_KEY_W)) {
        translate_x += step * cos(move_angle);
        translate_z -= step * sin(move_angle);

        main_camera->MoveForward(step);
    }

    if (window->KeyHold(GLFW_KEY_S)) {
        translate_x -= step * cos(move_angle);
        translate_z += step * sin(move_angle);

        main_camera->MoveForward(-step);
    }

    // Update car position 
    center_x = translate_x;
    center_z = translate_z;

    // Update sky camera position
    minimap_camera_position = glm::vec3(center_x, 50, center_z + z_near);
    minimap_camera_center = glm::vec3(center_x, center_y, center_z);
}


void Race::OnKeyPress(int key, int mods)
{
}
