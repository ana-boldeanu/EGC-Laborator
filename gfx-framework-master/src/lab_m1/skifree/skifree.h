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
        glm::mat4 rotationMatrix = transform3D::RotateOX(angle);

        float player_rotation = 0;
        float max_player_rotation = PI / 4;

        float step = 0.15f;

        // Arrays of positions for objects to render
        std::vector<glm::vec4> treeCoords{ rotationMatrix * glm::vec4(-4, 0, -2, 1),
            rotationMatrix * glm::vec4(12, 0, 1, 1),
            rotationMatrix * glm::vec4(4, 0, 4, 1),
            rotationMatrix * glm::vec4(-4, 0, 6, 1),
            rotationMatrix * glm::vec4(8, 0, 8, 1),
            rotationMatrix * glm::vec4(-7, 0, 10, 1)};

        std::vector<glm::vec4> rocksCoords{ rotationMatrix * glm::vec4(-10, 0, -3, 1),
            rotationMatrix * glm::vec4(-7, 0, 1, 1),
            rotationMatrix * glm::vec4(4, 0, 2, 1),
            rotationMatrix * glm::vec4(8, 0, 4, 1),
            rotationMatrix * glm::vec4(-2, 0, 5, 1),
            rotationMatrix * glm::vec4(2, 0, 8, 1) };

        std::vector<glm::vec4> lampCoords{ rotationMatrix * glm::vec4(10, 0, -3, 1),
            rotationMatrix * glm::vec4(4, 0, -1, 1),
            rotationMatrix * glm::vec4(-2, 0, 2, 1),
            rotationMatrix * glm::vec4(-8, 0, 5, 1),
            rotationMatrix * glm::vec4(12, 0, 6, 1),
            rotationMatrix * glm::vec4(-4, 0, 10, 1) };

        std::vector<glm::vec4> giftCoords{ rotationMatrix * glm::vec4(-7, 0, -3, 1),
            rotationMatrix* glm::vec4(0, 0, 3, 1),
            rotationMatrix* glm::vec4(4, 0, 6, 1),
            rotationMatrix* glm::vec4(1, 0, 9, 1) };

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

        // Light properties
        glm::vec3 lightDirection;
        unsigned int materialShininess;
        float materialKd;
        float materialKs;

        std::vector<glm::vec3> lampLightsCoords;
        std::vector<glm::vec3> treeLightsCoords;
        std::vector<glm::vec3> giftsLightsCoords;
        glm::vec4 light_offset = glm::vec4(0, 1, 0, 0);
    };
} 
