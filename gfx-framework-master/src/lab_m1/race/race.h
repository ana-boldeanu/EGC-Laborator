#pragma once

#include "components/simple_scene.h"
#include "lab_m1/race/camera.h"
#include "lab_m1/race/transform3D.h"
#include "lab_m1/race/course.h"


namespace m1
{
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
        void RenderScene();
        void RenderMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix) override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;

     protected:
        Camera *main_camera;
        Camera *minimap_camera;
        glm::mat4 projectionMatrix = glm::mat4(1);
        glm::mat4 orthoMatrix = glm::mat4(1);

        float PI = glm::pi<float>();

        float cameraSpeed = 1.0f;
        float fov = PI/2;
        float dist = 100;
        float left = -dist;
        float right = dist;
        float bottom = -dist;
        float top = dist;
        float z_near = 0.01f;
        float z_far = 200.0f;

        // Car position variables
        float center_x, center_y, center_z;
        float initial_x = 0, initial_z = 0, initial_y = 0.5f;
        float translate_x = 0, translate_z = 0, translate_y = initial_y;
        float move_angle = 0;
        float move_speed = 3;
        float rotate_speed = 50;

        // Camera variables
        glm::vec3 main_camera_position;
        glm::vec3 main_camera_center;
        glm::vec3 main_camera_up;

        glm::vec3 minimap_camera_position;
        glm::vec3 minimap_camera_center;
        glm::vec3 minimap_camera_up;

        bool project_ortho = false;

        // Course variables
        Course* course = new Course();

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
}   // namespace m1
