#pragma once

#include "components/simple_scene.h"
#include "duck.h"
#include "flight.h"
#include "lab_m1/lab3/transform2D.h"


namespace m1
{
    class DuckHunt : public gfxc::SimpleScene
    {
     public:
         DuckHunt();
        ~DuckHunt();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

     protected:
        glm::mat3 flightMatrix; // Matrix used to translate the whole model
        glm::mat3 modelMatrix;  // Matrix used to scale or rotate the whole model
        glm::mat3 wingsMatrix;  // Matrix used for wings flapping animation
        Duck* duck;
        Flight* flight;
        float flightAngle;
        float flightStep;
        bool flyRight, flyUp;
        float currX, currY;     // Current coordinates for duck center
    };
}
