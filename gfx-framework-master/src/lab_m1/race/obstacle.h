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

        glm::vec3 GetPosition();
        glm::vec3 GetPositionAndAdvance();

        bool SpheresCollision(glm::vec3 center_A, glm::vec3 center_B, float radius_A, float radius_B);
        bool HasCollidedWith(glm::vec3 player_position);

    private:
        std::vector<glm::vec3> route;
        int idx;

        float radius = 1;
        float car_radius = 1;

        float road_scale = 6;

    protected:

    };
}