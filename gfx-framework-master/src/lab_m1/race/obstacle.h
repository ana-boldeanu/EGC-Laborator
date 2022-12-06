#pragma once

#include "components/simple_scene.h"

namespace m1
{
    class Obstacle : public gfxc::SimpleScene
    {
    public:
        Obstacle(std::vector<glm::vec3> route, int start) {
            this->route = route;
            this->idx = start;
        }
        ~Obstacle();
        Mesh* model;

        glm::vec3 GetPositionAndAdvance();
        bool HasCollidedWith(glm::vec3 player_position);

    private:
        std::vector<glm::vec3> route;
        int idx;

    protected:

    };
}