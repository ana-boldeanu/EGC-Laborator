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

        std::vector<glm::vec3> polygon_points;
        std::vector<glm::vec3> outer_points;
        std::vector<glm::vec3> inner_points;
        Mesh* course;

    private:
        void Course::SetPolygonPoints();
        void Course::ComputeInnerOuterPoints();
        void Course::ComputeCourseMesh();


    protected:
        
        

        float inner_dist = 0.2f;
        float outer_dist = inner_dist;

        glm::vec3 color = glm::vec3(1, 1, 1);
    };
}