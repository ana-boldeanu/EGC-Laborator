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

        // Check if the object has collided with the player position
        bool HasCollidedWith(glm::vec3 player_position);

        // Get the current position of the obstacle
        glm::vec3 GetPosition();

        // Get the current position of the obstacle and increment it
        glm::vec3 GetPositionAndAdvance();

    private:
        std::vector<glm::vec3> route;   // The route that the obstacle will follow, made of consecutive positions
        int idx;                        // The current position index in the route vector

        float radius = 1;
        float car_radius = 1;
        float road_scale = 6;

        // Check if sphere A and sphere B are collided
        bool SpheresCollision(glm::vec3 center_A, glm::vec3 center_B, float radius_A, float radius_B);

    protected:
    };
}