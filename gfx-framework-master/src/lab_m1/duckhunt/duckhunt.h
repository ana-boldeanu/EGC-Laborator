#pragma once

#include "components/simple_scene.h"
#include "duck.h"
#include "flight.h"
#include "interface.h"
#include "lab_m1/lab3/transform2D.h"


namespace m1
{
    class DuckHunt : public gfxc::SimpleScene
    {
     public:
         DuckHunt();
        ~DuckHunt();

        void Init() override;
        void RenderInterface(int lifeCount, int bulletCount, float score);

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;
        void DuckHunt::ResetDuck();

        void DuckHunt::OnKeyPress(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;

     protected:
        glm::mat3 flightMatrix; // Matrix used to translate the whole model
        glm::mat3 modelMatrix;  // Matrix used to scale or rotate the whole model
        glm::mat3 wingsMatrix;  // Matrix used for wings flapping animation
        glm::mat3 interfaceMatrix;
        glm::vec3 skyColor;

        Duck* duck;
        Flight* flight;
        Interface* gameStats;

        float duckLength;
        float duckWidth;
        float duckScale;
        float timePassed = 0;
        float timeLimit = 7;

        float initialX, initialY;
        float currX, currY;     // Current coordinates for duck center
        float translateX, translateY;
        float flightAngle;
        float flightSpeed;
        bool flyRight, flyUp;
        
        int lifeCount;
        int bulletCount;
        float score;

        bool deadlyShot = false;
        bool duckActive = true;
        bool duckEvaded = false;
        bool duckDead = false;

        const float PI = 3.1415926f;
        float maxInitialX = 1280;
    };
}
