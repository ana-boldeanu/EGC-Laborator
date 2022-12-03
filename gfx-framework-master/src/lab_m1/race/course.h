#pragma once

#include "components/simple_scene.h"

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
        std::vector<glm::vec3> tree_locations_0;
        std::vector<glm::vec3> tree_locations_1;
        std::vector<glm::vec3> tree_locations_2;
        std::vector<glm::vec3> tree_locations_3;
        std::vector<int> locations;
        std::vector<int> rotations;
        float road_scale = 6;

    private:
        void Course::SetPolygonPoints();
        void Course::ComputeIntermediaryPoints();
        void Course::ComputeInnerOuterPoints();
        void Course::ComputeInnerOuterPointsExtended();
        void Course::ComputeLinesMesh();
        void Course::ComputeCourseMesh();


    protected:
        float inner_dist = 0.5f;
        float outer_dist = inner_dist;
        float road_width = (inner_dist + outer_dist) * road_scale;

        float tree_dist_0 = inner_dist + 0.3f;
        float tree_dist_1 = inner_dist + 1;

        std::vector<glm::vec3> polygon_points;  // Main polygon points
        std::vector<glm::vec3> outer_points;
        std::vector<glm::vec3> inner_points;

        std::vector<glm::vec3> polygon_points_extended;   // Extended polygon points with intermediaries
        std::vector<glm::vec3> outer_points_extended;
        std::vector<glm::vec3> inner_points_extended;

        glm::vec3 color = glm::vec3(0.15, 0.15, 0.15);
    };
}