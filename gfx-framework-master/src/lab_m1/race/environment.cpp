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


Mesh* Environment::CreateSquare(const std::string& name, float length, glm::vec3 color_1, glm::vec3 color_2)
{
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;
    Mesh* square = new Mesh(name);

    square->SetDrawMode(GL_TRIANGLE_STRIP);

    int segments = 500;
    float len = length / segments;

    float y = -0.01f;
    float x = 0, z = 0;

    float idx = 0;

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

    square->InitFromData(vertices, indices);
    return square;
}


Environment::~Environment()
{
}