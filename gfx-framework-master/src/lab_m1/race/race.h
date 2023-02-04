#pragma once

#include "components/simple_scene.h"
#include "lab_m1/race/camera.h"
#include "lab_m1/race/transform3D.h"
#include "lab_m1/race/course.h"
#include "lab_m1/race/environment.h"


namespace m1
{
    // Main class containing the logic for the entire game
    class Race : public gfxc::SimpleScene
    {
     public:
         Race();
        ~Race();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;
        void OnInputUpdate(float deltaTime, int mods) override;

        // Render a mesh object, using the implemented cameras and shaders
        void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix) override;

        // Used in Update() to render the entire scene
        void RenderScene();

        // Render the trees in the landscape
        void RenderTrees();

        // Render the obstacles that appear on the track
        void RenderObstacles();

     protected:
        Camera *main_camera;        // Third-person camera, used to follow the car
        Camera *minimap_camera;     // Orthogonal camera, used to look at the car from the 'sky'
        glm::mat4 projectionMatrix = glm::mat4(1);  // Matrix used for main_camera
        glm::mat4 orthoMatrix = glm::mat4(1);       // Matrix used with minimap_camera

        // Car position variables
        float initial_x = 0, initial_z = 7, initial_y = 0.5f;
        float translate_x = initial_x;
        float translate_z = initial_z;
        float translate_y = initial_y;
        float center_x = initial_x;
        float center_y = initial_y;
        float center_z = initial_z;
        float move_angle = 0;
        float move_speed = 9;
        float rotate_speed = 90;

        // Camera variables
        float PI = glm::pi<float>();
        float fov = PI / 2;
        float dist = 15;
        float left = -dist;
        float right = dist;
        float bottom = -dist;
        float top = dist;
        float z_near = 0.01f;
        float z_far = 200.0f;

        glm::vec3 main_camera_position;     // The position of the camera
        glm::vec3 main_camera_center;       // The position of the object that the camera looks at (the car)
        glm::vec3 main_camera_up;           // Perpendicular on XoZ

        glm::vec3 minimap_camera_position;
        glm::vec3 minimap_camera_center;
        glm::vec3 minimap_camera_up;

        bool project_ortho = false;

        // Course variables
        Course *course = new Course();
        Environment* environment = new Environment();

        // Minimap variables
        struct ViewportArea
        {
            ViewportArea() : x(0), y(0), width(1), height(1) {}
            ViewportArea(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {}
            int x;
            int y;
            int width;
            int height;
        };

        ViewportArea minimap;
    };
}
