#pragma once

#include <string>
#include <unordered_map>

#include "components/simple_scene.h"
#include "components/transform.h"


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

        void RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, Texture2D *texture1 = NULL, Texture2D *texture2 = NULL);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        std::unordered_map<std::string, Texture2D *> mapTextures;
        int render_sphere = 0;
    };
} 
