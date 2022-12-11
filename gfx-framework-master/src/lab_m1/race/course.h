#pragma once

#include "components/simple_scene.h"
#include "lab_m1/race/obstacle.h"
#include "lab_m1/race/environment.h"

#include <iostream>
#include <vector>

namespace m1
{
    // Class containing information about the race course and anything parralel to it (tree locations,
    // obstacle routes and track markers)
    class Course : public gfxc::SimpleScene
    {
    public:
        Course();
        ~Course();

        // Road data
        Mesh* lines;
        Mesh* course;
        float road_scale = 6;

        // Check if the car is still on the road
        bool Course::IsOnRoad(glm::vec2 car_position);

        // Check if the car has collided with any of the obstacles in `obstacles` vector
        bool Course::CollidedWithObstacle(glm::vec3 car_position);

        // Tree locations, 2 on both sides of the road, at 2 different distances from it
        std::vector<glm::vec3> tree_locations_0;
        std::vector<glm::vec3> tree_locations_1;
        std::vector<glm::vec3> tree_locations_2;
        std::vector<glm::vec3> tree_locations_3;
        
        // Tree placement
        std::vector<int> locations;     // Controls which tree_locations vector to use for a tree
        std::vector<int> rotations;     // Controls which rotation to use for a tree

        // Obstacles information
        std::vector<Obstacle*> obstacles;

    private:
        // Set the main polygon points, obtained by using Geogebra and some cool maths
        void Course::SetPolygonPoints();
        
        // Compute the points that define road margins (parallel segments)
        void Course::ComputeInnerOuterPoints();

        // Compute intermediary points for all segments given by vectors of points (tree locations, obstacle routes etc.)
        void Course::ComputeIntermediaryPoints();

        // Build vectors containing information about trees placement
        void Course::ComputeTreeLocations();

        // Create the mesh for the lines at the middle of the road
        void Course::ComputeLinesMesh();

        // Create the mesh for the course
        void Course::ComputeCourseMesh();

        // Create Obstacle objects, give them a predefined route and save them in the list of obstacles
        void Course::CreateObstacles();

        // Given a vector of main points and a step, create a new vector containing intermediary points
        void Course::ExtendPoints(std::vector<glm::vec3> &initialList, std::vector<glm::vec3> &extendedList, float step);

        // Compute the area of a triangle (for collision checks)
        float Course::TriangleArea(glm::vec2 P1, glm::vec2 P2, glm::vec2 P3);

    protected:
        float inner_dist = 0.5f;
        float outer_dist = inner_dist;
        float road_width = (inner_dist + outer_dist) * road_scale;

        float tree_dist_0 = inner_dist + 0.3f;
        float tree_dist_1 = inner_dist + 0.6f;

        float route_dist = inner_dist / 2;

        // Main polygon points
        std::vector<glm::vec3> polygon_points;

        // Generated inner and outer points
        std::vector<glm::vec3> outer_points;
        std::vector<glm::vec3> inner_points;

        // Generated points for obstacle routes
        std::vector<glm::vec3> obstacle_route_0;
        std::vector<glm::vec3> obstacle_route_1;

        // Extended points for obstacle routes
        std::vector<glm::vec3> obstacle_route_0_ext;
        std::vector<glm::vec3> obstacle_route_1_ext;

        // Extended polygon points
        std::vector<glm::vec3> polygon_points_extended;
        std::vector<glm::vec3> outer_points_extended;
        std::vector<glm::vec3> inner_points_extended;

        // Road color
        glm::vec3 color = glm::vec3(0.15, 0.15, 0.15);
    };
}