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

        Mesh *lines;
        Mesh *course;
        std::vector<glm::vec3> tree_locations_0;
        std::vector<glm::vec3> tree_locations_1;
        std::vector<glm::vec3> tree_locations_2;
        std::vector<glm::vec3> tree_locations_3;
        std::vector<int> locations;
        std::vector<int> rotations;

    private:
        void Course::SetPolygonPoints();
        void Course::ComputeInnerOuterPoints();
        void Course::ComputeLinesMesh();
        void Course::ComputeCourseMesh();


    protected:
        float inner_dist = 0.5f;
        float outer_dist = inner_dist;

        float tree_dist_0 = inner_dist + 0.3f;
        float tree_dist_1 = inner_dist + 1;

        std::vector<glm::vec3> polygon_points;
        std::vector<glm::vec3> outer_points;
        std::vector<glm::vec3> inner_points;

        

        glm::vec3 color = glm::vec3(0.2, 0.2, 0.2);
    };
}