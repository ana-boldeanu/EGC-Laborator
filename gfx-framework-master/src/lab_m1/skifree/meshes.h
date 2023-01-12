#pragma once

#include "components/simple_scene.h"
#include "lab_m1/race/transform3D.h"
#include <vector>

namespace m1
{
    class Meshes : public gfxc::SimpleScene
    {
     public:
         Meshes();
        ~Meshes();

        Mesh* plane; 
        Mesh* gift;
        std::vector<Mesh*> tree;
        std::vector<Mesh*> player;
        std::vector<Mesh*> rocks;
        std::vector<Mesh*> lamp;

        glm::mat4 plane_matrix = glm::mat4(1);
        glm::mat4 gift_matrix = glm::mat4(1);
        std::vector<glm::mat4> tree_matrix;
        std::vector<glm::mat4> player_matrix;
        std::vector<glm::mat4> rocks_matrix;
        std::vector<glm::mat4> lamp_matrix;

        std::vector<std::string> tree_tex;
        std::vector<std::string> player_tex;
        std::vector<std::string> rocks_tex;
        std::vector<std::string> lamp_tex;

     private:
         Mesh* CreatePlane();
         Mesh* CreateGift();
         std::vector<Mesh*> CreateTree();
         std::vector<Mesh*> CreatePlayer();
         std::vector<Mesh*> CreateRocks();
         std::vector<Mesh*> CreateLamp();
    };
} 
