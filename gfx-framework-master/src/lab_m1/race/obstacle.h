#pragma once

#include "components/simple_scene.h"

namespace m1
{
    class Obstacle : public gfxc::SimpleScene
    {
    public:
        Obstacle();
        ~Obstacle();

        void Advance();
        glm::vec3 GetCurrPosition();
        bool HasCollidedWith(glm::vec3 player_position);

    private:
        std::vector<glm::vec3> course;
        Mesh* model;
        int idx = 0;

    protected:

    };
}