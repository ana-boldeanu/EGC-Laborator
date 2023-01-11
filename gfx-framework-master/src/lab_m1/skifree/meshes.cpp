#include "lab_m1/skifree/meshes.h"

#include <vector>
#include <string>
#include <iostream>
#include <time.h>

using namespace std;
using namespace m1;


Meshes::Meshes()
{
    plane = CreatePlane();
}


Meshes::~Meshes()
{
}


Mesh* Meshes::CreatePlane()
{
    // Create a simple quad
    vector<glm::vec3> vertices
    {
        glm::vec3(0.5f,   0.5f, 0.0f),    // top right
        glm::vec3(0.5f,  -0.5f, 0.0f),    // bottom right
        glm::vec3(-0.5f, -0.5f, 0.0f),    // bottom left
        glm::vec3(-0.5f,  0.5f, 0.0f),    // top left
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

    Mesh* mesh = new Mesh("plane");
    mesh->InitFromData(vertices, normals, textureCoords, indices); 
    return mesh;
}