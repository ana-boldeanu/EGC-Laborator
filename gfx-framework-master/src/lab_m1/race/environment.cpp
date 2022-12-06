#include "lab_m1/race/environment.h"

using namespace std;
using namespace m1;


Environment::Environment()
{
	CreateGrass();
}

void Environment::CreateGrass() 
{
    grass = CreateSquare("grass", grass_length, grass_color_1, grass_color_2);
}


void Environment::CreateMeshFromData(Mesh* mesh, std::vector<VertexFormat> vertices, std::vector<unsigned int> indices)
{
    // Create the VAO and bind it
    unsigned int VAO = 0;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create the VBO and bind it
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Send vertices data into the VBO buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    // Create the IBO and bind it
    unsigned int IBO;
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    // Send indices data into the IBO buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

    // Set vertex position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

    // Set vertex normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

    // Set texture coordinate attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

    // Set vertex color attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));

    // Unbind the VAO
    glBindVertexArray(0);

    // Check for OpenGL errors
    CheckOpenGLError();

    // Save Mesh information
    mesh->InitFromBuffer(VAO, static_cast<unsigned int>(indices.size()));
    mesh->vertices = vertices;
    mesh->indices = indices;
}


Mesh* Environment::CreateSquare(const std::string& name, float length, glm::vec3 color_1, glm::vec3 color_2)
{
    // Create indices and vertices vectors
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;
    Mesh* square = new Mesh(name);

    square->SetDrawMode(GL_TRIANGLE_STRIP);

    int segments = 500;
    float len = length / segments;

    float y = -0.01f;
    float x = 0, z = 0;

    int idx = 0;

    for (int j = 0; j < segments; j += 1) {
        if (j % 2 == 0) {
            for (int i = 0; i < segments; i += 1) {
                // Add a square
                vertices.push_back(VertexFormat(glm::vec3(x + len * i, y, z + len * j), color_1));
                vertices.push_back(VertexFormat(glm::vec3(x + len * i, y, z + len * (j + 1)), color_1));
                vertices.push_back(VertexFormat(glm::vec3(x + len * (i + 1), y, z + len * j), color_1));
                vertices.push_back(VertexFormat(glm::vec3(x + len * (i + 1), y, z + len * (j + 1)), color_1));

                indices.push_back(idx++);
                indices.push_back(idx++);
                indices.push_back(idx++);
                indices.push_back(idx++);
            }
        }
        else {
            for (int i = segments - 1; i >= 0; i -= 1) {
                // Add a square
                vertices.push_back(VertexFormat(glm::vec3(x + len * i, y, z + len * j), color_2));
                vertices.push_back(VertexFormat(glm::vec3(x + len * i, y, z + len * (j + 1)), color_2));
                vertices.push_back(VertexFormat(glm::vec3(x + len * (i + 1), y, z + len * j), color_2));
                vertices.push_back(VertexFormat(glm::vec3(x + len * (i + 1), y, z + len * (j + 1)), color_2));

                indices.push_back(idx++);
                indices.push_back(idx++);
                indices.push_back(idx++);
                indices.push_back(idx++);
            }
        }
    }

    CreateMeshFromData(square, vertices, indices);
    return square;
}


Environment::~Environment()
{
}