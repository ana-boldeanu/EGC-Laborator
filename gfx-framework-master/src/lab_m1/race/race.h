#pragma once

#include "components/simple_scene.h"
#include "lab_m1/race/camera.h"
#include "lab_m1/race/transform3D.h"


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

        void RenderMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix) override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

     protected:
        Camera *camera;
        glm::mat4 projectionMatrix;

        // TODO(student): If you need any other class variables, define them here.
        float cameraSpeed = 1.0f;
        bool projectOrtho = false;
        float FoV = 90.0f;
        float left = -15;
        float right = 15;
        float top = 15;
        float bottom = -2;
        float Z_NEAR = 0.01f;
        float Z_FAR = 200.0f;
    };
}   // namespace m1
