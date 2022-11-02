#pragma once

#include "components/simple_scene.h"


namespace m1
{
    class Duck : public gfxc::SimpleScene
    {
    public:
        Duck();
        ~Duck();
        Mesh* Duck::CreateDuckWingFront();
        Mesh* Duck::CreateDuckWingBack();
        Mesh* Duck::CreateDuckBody();
        Mesh* Duck::CreateDuckHead();

    private:
        Mesh* CreateCircle(const std::string& name, float centerX, float centerY,
            float radius, glm::vec3 color, bool fill);
        

    protected:
        glm::mat3 modelMatrix;

    };
}