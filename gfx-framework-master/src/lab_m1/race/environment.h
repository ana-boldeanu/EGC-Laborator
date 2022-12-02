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

    private:
        void Environment::CreateGrass();
        Mesh* Environment::CreateCircle(const std::string& name, float centerX, float centerY,
            float radius, glm::vec3 color);

    protected:
        glm::vec3 grass_color = glm::vec3(0.3, 0.7, 0);
    };
}