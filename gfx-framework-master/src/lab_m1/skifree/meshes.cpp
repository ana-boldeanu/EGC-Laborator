#include "lab_m1/skifree/meshes.h"

#include <string>
#include <iostream>

using namespace std;
using namespace m1;


Meshes::Meshes()
{
    plane = CreatePlane();
    gift = CreateGift();
    tree = CreateTree();
    player = CreatePlayer();
    rocks = CreateRocks();
    lamp = CreateLamp();
}


Meshes::~Meshes()
{
}


Mesh* Meshes::CreatePlane()
{
    vector<glm::vec3> vertices
    {
        glm::vec3(0.5f, 0, 0.5f),    // top right
        glm::vec3(0.5f, 0, -0.5f),    // bottom right
        glm::vec3(-0.5f, 0, -0.5f),    // bottom left
        glm::vec3(-0.5f, 0, 0.5f),    // top left
    };

    vector<glm::vec3> normals
    {
        glm::vec3(0, 1, 1),
        glm::vec3(1, 0, 1),
        glm::vec3(1, 0, 0),
        glm::vec3(0, 1, 0)
    };

    vector<glm::vec2> textureCoords
    {
        glm::vec2(0.0f, 0.0f),
        glm::vec2(0.0f, 1.0f),
        glm::vec2(1.0f, 1.0f),
        glm::vec2(1.0f, 0.0f)
    };

    vector<unsigned int> indices =
    {
        0, 1, 3,
        1, 2, 3
    };

    // Build the mesh
    Mesh* mesh = new Mesh("plane");
    mesh->InitFromData(vertices, normals, textureCoords, indices); 

    // Build its model matrix
    float scale = 100;
    plane_matrix *= transform3D::Translate(0, -0.01f, 0);
    plane_matrix *= transform3D::Scale(scale, scale, scale);

    return mesh;
}

Mesh* Meshes::CreateGift()
{
    Mesh* mesh = new Mesh("gift");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");

    gift_matrix *= transform3D::Translate(0, 0.375f, 0);
    gift_matrix *= transform3D::Scale(1.25f, 1.25f, 1.25f);

    return mesh;
}

std::vector<Mesh*> Meshes::CreateTree()
{
    // Set meshes
    std::vector<Mesh*> tree;

    Mesh* cone_1 = new Mesh("cone_1");
    cone_1->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "cone.obj");

    Mesh* cone_2 = new Mesh("cone_2");
    cone_2->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "cone.obj");

    Mesh* cone_3 = new Mesh("cone_3");
    cone_3->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "cone.obj");

    Mesh* trunk = new Mesh("trunk");
    trunk->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");

    tree.push_back(cone_1);
    tree.push_back(cone_2);
    tree.push_back(cone_3);
    tree.push_back(trunk);

    // Set model matrix for each mesh
    glm::mat4 cone_1_matrix = glm::mat4(1);
    cone_1_matrix *= transform3D::Translate(0, 5, 0);
    cone_1_matrix *= transform3D::Scale(0.5f, 0.5f, 0.5f);

    glm::mat4 cone_2_matrix = glm::mat4(1);
    cone_2_matrix *= transform3D::Translate(0, 4.2f, 0);

    glm::mat4 cone_3_matrix = glm::mat4(1);
    cone_3_matrix *= transform3D::Translate(0, 3.2f, 0);
    cone_3_matrix *= transform3D::Scale(1.5f, 1.5f, 1.5f);

    glm::mat4 trunk_matrix = glm::mat4(1);
    trunk_matrix *= transform3D::Translate(0, 1, 0);
    trunk_matrix *= transform3D::Scale(0.5f, 2, 0.5f);

    tree_matrix.push_back(cone_1_matrix);
    tree_matrix.push_back(cone_2_matrix);
    tree_matrix.push_back(cone_3_matrix);
    tree_matrix.push_back(trunk_matrix);

    // Set textures for each mesh
    tree_tex.push_back("pine");
    tree_tex.push_back("pine");
    tree_tex.push_back("pine");
    tree_tex.push_back("trunk");

    return tree;
}

std::vector<Mesh*> Meshes::CreatePlayer()
{
    std::vector<Mesh*> player;

    Mesh* ski_1 = new Mesh("ski_1");
    ski_1->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");

    Mesh* ski_2 = new Mesh("ski_2");
    ski_2->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");

    Mesh* body = new Mesh("body");
    body->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");

    player.push_back(ski_1);
    player.push_back(ski_2);
    player.push_back(body);

    // Set model matrix for each mesh
    glm::mat4 ski_1_matrix = glm::mat4(1);
    ski_1_matrix *= transform3D::Translate(-0.35f, 0, 0);
    ski_1_matrix *= transform3D::Scale(0.5f, 0.05f, 3);

    glm::mat4 ski_2_matrix = glm::mat4(1);
    ski_2_matrix *= transform3D::Translate(0.35f, 0, 0);
    ski_2_matrix *= transform3D::Scale(0.5f, 0.05f, 3);

    glm::mat4 body_matrix = glm::mat4(1);
    body_matrix *= transform3D::Translate(0, 0.7f, 0);
    body_matrix *= transform3D::Scale(0.75f, 1.4f, 0.75f);

    player_matrix.push_back(ski_1_matrix);
    player_matrix.push_back(ski_2_matrix);
    player_matrix.push_back(body_matrix);

    // Set textures for each mesh
    player_tex.push_back("ski");
    player_tex.push_back("ski");
    player_tex.push_back("player");

    return player;
}

std::vector<Mesh*> Meshes::CreateRocks()
{
    std::vector<Mesh*> rocks;

    Mesh* rock_1 = new Mesh("rock_1");
    rock_1->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");

    Mesh* rock_2 = new Mesh("rock_2");
    rock_2->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");

    Mesh* rock_3 = new Mesh("rock_3");
    rock_3->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");

    Mesh* rock_4 = new Mesh("rock_4");
    rock_4->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");

    Mesh* rock_5 = new Mesh("rock_5");
    rock_5->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");

    rocks.push_back(rock_1);
    rocks.push_back(rock_2);
    rocks.push_back(rock_3);
    rocks.push_back(rock_4);
    rocks.push_back(rock_5);

    // Set model matrix for each mesh
    glm::mat4 rock_1_matrix = glm::mat4(1);
    rock_1_matrix *= transform3D::Translate(-0.5f, 0.1f, 0);
    rock_1_matrix *= transform3D::Scale(0.5f, 0.2f, 0.5f);

    glm::mat4 rock_2_matrix = glm::mat4(1);
    rock_2_matrix *= transform3D::Translate(0, 0.1f, -0.5f);
    rock_2_matrix *= transform3D::Scale(1, 0.1f, 1);

    glm::mat4 rock_3_matrix = glm::mat4(1);
    rock_3_matrix *= transform3D::Translate(0.5f, 0.1f, 0.2f);
    rock_3_matrix *= transform3D::Scale(0.5f, 0.2f, 0.5f);

    glm::mat4 rock_4_matrix = glm::mat4(1);
    rock_4_matrix *= transform3D::Translate(0.8f, 0.2f, 0.2f);
    rock_4_matrix *= transform3D::RotateOZ(-0.5f);
    rock_4_matrix *= transform3D::Scale(0.5f, 0.2f, 0.5f);

    glm::mat4 rock_5_matrix = glm::mat4(1);
    rock_5_matrix *= transform3D::Translate(-0.5f, 0.1f, -0.7f);
    rock_5_matrix *= transform3D::RotateOZ(0.5f);
    rock_5_matrix *= transform3D::Scale(0.5f, 0.2f, 0.5f);

    rocks_matrix.push_back(rock_1_matrix);
    rocks_matrix.push_back(rock_2_matrix);
    rocks_matrix.push_back(rock_3_matrix);
    rocks_matrix.push_back(rock_4_matrix);
    rocks_matrix.push_back(rock_5_matrix);

    // Set textures for each mesh
    rocks_tex.push_back("rock");
    rocks_tex.push_back("rock");
    rocks_tex.push_back("rock");
    rocks_tex.push_back("rock");
    rocks_tex.push_back("rock");

    return rocks;
}

std::vector<Mesh*> Meshes::CreateLamp()
{
    std::vector<Mesh*> lamp;

    Mesh* vertical_bar = new Mesh("vertical_bar");
    vertical_bar->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");

    Mesh* horizontal_bar = new Mesh("horizontal_bar");
    horizontal_bar->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");

    Mesh* light_1 = new Mesh("light_1");
    light_1->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");

    Mesh* light_2 = new Mesh("light_2");
    light_2->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");

    lamp.push_back(vertical_bar);
    lamp.push_back(horizontal_bar);
    lamp.push_back(light_1);
    lamp.push_back(light_2);

    // Set model matrix for each mesh
    glm::mat4 vertical_bar_matrix = glm::mat4(1);
    vertical_bar_matrix *= transform3D::Translate(0, 2, 0);
    vertical_bar_matrix *= transform3D::Scale(0.2f, 4, 0.2f);

    glm::mat4 horizontal_bar_matrix = glm::mat4(1);
    horizontal_bar_matrix *= transform3D::Translate(0, 4, 0);
    horizontal_bar_matrix *= transform3D::Scale(1.5f, 0.2f, 0.2f);

    lamp_offset_1 = glm::vec3(-0.6f, 3.7f, 0);
    lamp_offset_2 = glm::vec3(0.6f, 3.7f, 0);

    glm::mat4 light_1_matrix = glm::mat4(1);
    light_1_matrix *= transform3D::Translate(lamp_offset_1.x, lamp_offset_1.y, lamp_offset_1.z);
    light_1_matrix *= transform3D::Scale(0.4f, 0.4f, 0.4f);

    glm::mat4 light_2_matrix = glm::mat4(1);
    light_2_matrix *= transform3D::Translate(lamp_offset_2.x, lamp_offset_2.y, lamp_offset_2.z);
    light_2_matrix *= transform3D::Scale(0.4f, 0.4f, 0.4f);

    lamp_offset_1 *= 4;
    lamp_offset_2 *= 4;

    lamp_matrix.push_back(vertical_bar_matrix);
    lamp_matrix.push_back(horizontal_bar_matrix);
    lamp_matrix.push_back(light_1_matrix);
    lamp_matrix.push_back(light_2_matrix);

    // Set textures for each mesh
    lamp_tex.push_back("metal");
    lamp_tex.push_back("metal");
    lamp_tex.push_back("lightbulb");
    lamp_tex.push_back("lightbulb");

    return lamp;
}
