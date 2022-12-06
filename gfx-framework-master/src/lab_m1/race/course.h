#pragma once

#include "components/simple_scene.h"
#include "lab_m1/race/obstacle.h"
#include "lab_m1/race/environment.h"

#include <iostream>
#include <vector>

namespace m1
{
    class Course : public gfxc::SimpleScene
    {
    public:
        Course();
        ~Course();
        bool Course::IsOnRoad(glm::vec2 car_position);

        Mesh *lines;
        Mesh *course;
        float road_scale = 6;

        // Tree locations
        std::vector<glm::vec3> tree_locations_0;
        std::vector<glm::vec3> tree_locations_1;
        std::vector<glm::vec3> tree_locations_2;
        std::vector<glm::vec3> tree_locations_3;
        std::vector<int> locations;
        std::vector<int> rotations;

        // Main polygon points
        std::vector<glm::vec3> polygon_points;  // Main polygon points
        std::vector<glm::vec3> outer_points;
        std::vector<glm::vec3> inner_points;

        // Obstacles information
        std::vector<Obstacle*> obstacles;
        std::vector<glm::vec3> obstacle_route_0;
        std::vector<glm::vec3> obstacle_route_1;

        std::vector<glm::vec3> obstacle_route_0_ext;
        std::vector<glm::vec3> obstacle_route_1_ext;


    private:
        void Course::SetPolygonPoints();
        void Course::ComputeInnerOuterPoints();
        void Course::ComputeIntermediaryPoints();
        void Course::ComputeTreeLocations();
        void Course::ComputeLinesMesh();
        void Course::ComputeCourseMesh();

        void Course::CreateObstacles();
        void Course::ExtendPoints(std::vector<glm::vec3> &initialList, std::vector<glm::vec3> &extendedList, float step);


        float Course::TriangleArea(glm::vec2 P1, glm::vec2 P2, glm::vec2 P3);

    protected:
        float inner_dist = 0.5f;
        float outer_dist = inner_dist;
        float road_width = (inner_dist + outer_dist) * road_scale;

        float tree_dist_0 = inner_dist + 0.3f;
        float tree_dist_1 = inner_dist + 0.6f;

        float route_dist = inner_dist / 2;

        // Extended polygon points
        std::vector<glm::vec3> polygon_points_extended;   // Extended polygon points with intermediaries
        std::vector<glm::vec3> outer_points_extended;
        std::vector<glm::vec3> inner_points_extended;

        glm::vec3 color = glm::vec3(0.15, 0.15, 0.15);
    };
}