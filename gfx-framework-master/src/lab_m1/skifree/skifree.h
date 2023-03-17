#pragma once

#include <string>
#include <unordered_map>

#include "components/simple_scene.h"
#include "components/transform.h"
#include "lab_m1/duckhunt/transform2D.h"
#include "lab_m1/skifree/camera.h"
#include "lab_m1/skifree/meshes.h"

namespace m1
{
    class SkiFree : public gfxc::SimpleScene
    {
     public:
         SkiFree();
        ~SkiFree();

        void Init() override;
        
        void UpdateObjectsLocations();
        glm::vec4 RespawnObject(glm::vec4 oldCoords, int maxOffsetX);
        bool SpheresCollision(glm::vec3 center_A, glm::vec3 center_B, float radius_A, float radius_B);
        bool ObstacleCollision();
        void CheckGiftCollection();

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderObjects();
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

        float playerX = 0, playerY = 0, playerZ = -4;
        glm::vec4 playerPosition = rotationMatrix * glm::vec4(playerX, playerY, playerZ, 1);
        float player_rotation = 0;
        float max_player_rotation = PI / 4;

        float normal_step = 0.15f;
        float collided_step = 0.1f;
        float step = normal_step;

        // Arrays of positions for objects to render
        std::vector<glm::vec4> treeCoords{ rotationMatrix * glm::vec4(-4, 0, -10, 1),
            rotationMatrix * glm::vec4(12, 0, -6, 1),
            rotationMatrix * glm::vec4(4, 0, -1, 1),
            rotationMatrix * glm::vec4(-4, 0, 6, 1),
            rotationMatrix * glm::vec4(8, 0, 10, 1),
            rotationMatrix * glm::vec4(-7, 0, 15, 1)};

        std::vector<glm::vec4> rocksCoords{ rotationMatrix * glm::vec4(-10, 0, -9, 1),
            rotationMatrix * glm::vec4(-10, 0, -7, 1),
            rotationMatrix * glm::vec4(4, 0, -2, 1),
            rotationMatrix * glm::vec4(12, 0, 4, 1),
            rotationMatrix * glm::vec4(-2, 0, 12, 1),
            rotationMatrix * glm::vec4(2, 0, 17, 1) };

        std::vector<glm::vec4> lampCoords{ rotationMatrix * glm::vec4(10, 0, -12, 1),
            rotationMatrix * glm::vec4(4, 0, -8, 1),
            rotationMatrix * glm::vec4(-2, 0, -3, 1),
            rotationMatrix * glm::vec4(-8, 0, 3, 1),
            rotationMatrix * glm::vec4(12, 0, 7, 1),
            rotationMatrix * glm::vec4(-4, 0, 13, 1) };

        std::vector<glm::vec4> giftCoords{ rotationMatrix * glm::vec4(-7, 0, -5, 1),
            rotationMatrix* glm::vec4(0, 0, 5, 1),
            rotationMatrix* glm::vec4(4, 0, 9, 1),
            rotationMatrix* glm::vec4(1, 0, 14, 1) };

        // Keep initial coords for the reset
        std::vector<glm::vec4> initialTreeCoords = treeCoords;
        std::vector<glm::vec4> initialRocksCoords = rocksCoords;
        std::vector<glm::vec4> initialLampCoords = lampCoords;
        std::vector<glm::vec4> initialGiftCoords = giftCoords;

        // Camera variables
        CameraSki* camera;        // Third-person camera, used to follow the player
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

        // Respawn variables
        int maxOffsetX = 30;
        int maxOffsetZ = 14;
        int respawnDistanceZ = 18;

        // Game variables
        bool gameOver = false;
        bool collided = false;
        int numGifts = 0;
        int lifeCount = 3;
        float timePassed = 0;

        // Collision variables
        float treeRadius = 0.25f;
        float lampRadius = 0.07f;
        float rocksRadius = 0.5f;
        float giftRadius = 0.65f;
        float playerRadius = 0.8f;
    };
} 
