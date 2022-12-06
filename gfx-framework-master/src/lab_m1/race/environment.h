#pragma once

#include "components/simple_scene.h"

namespace m1
{
    class Environment : public gfxc::SimpleScene
    {
    public:
        Environment();
        ~Environment();

        Mesh* grass;
        float grass_length = 200;

        static void Environment::CreateMeshFromData(Mesh* mesh, std::vector<VertexFormat> vertices, std::vector<unsigned int> indices);

    private:
        void Environment::CreateGrass();
        Mesh* Environment::CreateSquare(const std::string& name, float length, glm::vec3 color_1, glm::vec3 color_2);

    protected:
        glm::vec3 grass_color_1 = glm::vec3(0.92f, 0.4f, 0.14f);
        glm::vec3 grass_color_2 = glm::vec3(0.92f, 0.4f, 0.14f);
    };
}