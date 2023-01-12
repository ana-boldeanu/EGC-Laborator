#pragma once

#include <string>
#include <unordered_map>

#include "components/simple_scene.h"
#include "components/transform.h"
#include "lab_m1/race/camera.h"
#include "lab_m1/skifree/meshes.h"

namespace m1
{
    class SkiFree : public gfxc::SimpleScene
    {
     public:
         SkiFree();
        ~SkiFree();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, Texture2D *texture1 = NULL);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        std::unordered_map<std::string, Texture2D *> mapTextures;

        // Meshes container
        Meshes* meshes_builder = new Meshes();
        int renderSnow = 0;

        // Movement variables
        float PI = glm::pi<float>();
        float angle = PI / 6;

        float player_rotation = 0;
        float max_player_rotation = PI / 4;

        float step = 0.1f;
        float giftX = 2, giftY = 0, giftZ = 0;  // Translate values for gift
        float treeX = 4, treeY = 0, treeZ = 0;  // Translate values for tree
        float rocksX = -2, rocksY = 0, rocksZ = 0;  // Translate values for rocks
        float lampX = -4, lampY = 0, lampZ = 0;  // Translate values for lamp

        float playerX = 0, playerY = 4 * sin(angle), playerZ = -4 * cos(angle);

        // Camera variables
        Camera* camera;        // Third-person camera, used to follow the player
        glm::mat4 projectionMatrix = glm::mat4(1);  // Matrix used for the camera

        float fov = PI / 2;
        float z_near = 0.01f;
        float z_far = 200.0f;

        glm::vec3 camera_position;     // The position of the camera
        glm::vec3 camera_center;       // The position of the object that the camera looks at (the player)
        glm::vec3 camera_up;           // Perpendicular on XoZ
    };
} 
