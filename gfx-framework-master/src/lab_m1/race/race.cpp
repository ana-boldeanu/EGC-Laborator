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
        Mesh* mesh = new Mesh("obstacle_0");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
        course->obstacles[0]->model = mesh;
    }

    {
        Mesh* mesh = new Mesh("obstacle_1");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
        course->obstacles[1]->model = mesh;
    }

    {
        Mesh* mesh = new Mesh("obstacle_2");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
        course->obstacles[2]->model = mesh;
    }

    {
        Mesh* mesh = new Mesh("tree");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "race"), "tree.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    meshes["course"] = course->course;
    meshes["lines"] = course->lines;
    meshes["grass"] = environment->grass;

    // Create a shader program
    {
        Shader* shader = new Shader("CurveShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "race", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "race", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
}


void Race::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.45, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set the screen area where to draw
    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}


void Race::RenderObstacles() 
{
    float road_scale = course->road_scale;
    glm::vec3 position;
    glm::mat4 modelMatrix;

    Obstacle *obstacle = course->obstacles[0];

    //for each (auto & obstacle in course->obstacles) {
        position = obstacle->GetPositionAndAdvance();
        position *= road_scale;

        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(position.x, translate_y, position.z);

        RenderMesh(obstacle->model, shaders["CurveShader"], modelMatrix);
    //}

}


void Race::RenderTrees()
{
    glm::mat4 modelMatrix = glm::mat4(1);
    float scale = 0.5f;
    size_t size = course->tree_locations_0.size();
    int side, rotation;
    float x, y, z;

    for (int i = 0; i < size; i += 401) {
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
        RenderMesh(meshes["tree"], shaders["CurveShader"], modelMatrix);
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

        RenderMesh(meshes["box"], shaders["CurveShader"], modelMatrix);
    }

    RenderTrees();
    RenderObstacles();

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        float road_scale = course->road_scale;
        modelMatrix *= transform3D::Translate(0, 0.06f, 0);
        modelMatrix *= transform3D::Scale(road_scale, road_scale, road_scale);

        RenderMesh(meshes["course"], shaders["CurveShader"], modelMatrix);
        RenderMesh(meshes["lines"], shaders["CurveShader"], modelMatrix);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        float scale = 2;
        float length = environment->grass_length;
        modelMatrix *= transform3D::Scale(scale, scale, scale);
        modelMatrix *= transform3D::Translate(-length/2, 0, -length/2);
        RenderMesh(meshes["grass"], shaders["CurveShader"], modelMatrix);
    }
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
    glUseProgram(shader->program);  // Maybe here 
    
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

    int location_car_pos = glGetUniformLocation(shader->GetProgramID(), "car_pos");
    glm::vec3 car_pos = glm::vec3(center_x, center_y, center_z);
    glUniform3fv(location_car_pos, 1, glm::value_ptr(car_pos));

    mesh->Render();
}


void Race::OnInputUpdate(float deltaTime, int mods)
{
    //cout << ">>>> " << course->IsOnRoad(glm::vec2(center_x, center_z)) << " " << glm::vec2(center_x, center_z) << endl;

    // Car and camera rotations
    float sensitivity = 0.01f;
    float angle = deltaTime * rotate_speed * sensitivity;
    float new_x, new_z;

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
        new_x = translate_x + step * cos(move_angle);
        new_z = translate_z - step * sin(move_angle);

        if (course->IsOnRoad(glm::vec2(new_x, new_z))) {
            translate_x = new_x;
            translate_z = new_z;

            main_camera->MoveForward(step);
        }
    }

    if (window->KeyHold(GLFW_KEY_S)) {
        new_x = translate_x - step * cos(move_angle);
        new_z = translate_z + step * sin(move_angle);

        if (course->IsOnRoad(glm::vec2(new_x, new_z))) {
            translate_x = new_x;
            translate_z = new_z;

            main_camera->MoveForward(-step);
        }
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
