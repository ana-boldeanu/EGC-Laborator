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
        void RenderEnvironment();

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;
        void DuckHunt::ResetDuck();

        void DuckHunt::OnKeyPress(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;

     protected:
        glm::mat3 flightMatrix = glm::mat3(1); // Matrix used to translate the whole model
        glm::mat3 modelMatrix = glm::mat3(1);  // Matrix used to scale or rotate the whole model
        glm::mat3 wingsMatrix = glm::mat3(1);  // Matrix used for wings flapping animation
        glm::mat3 interfaceMatrix = glm::mat3(1);
        glm::vec3 skyColor = glm::vec3(0.8f, 1, 1);

        Flight* flight;
        Duck* duck = new Duck();
        Interface* gameStats = new Interface();

        float duckLength;
        float duckWidth;
        float duckScale = 0.8f;
        float timePassed = 0;
        float timeLimit = 5;

        float initialX, initialY;
        float currX, currY;     // Current coordinates for duck center
        float translateX = 0; 
        float translateY = 0;
        float flightAngle = 0;
        float initialFlightSpeed = 500;
        float flightSpeed;
        bool flyRight, flyUp = true;
        
        int lifeCount = 3;
        int bulletCount = 3;
        float score = 0;

        float pointsPerDuck = 4;
        int ducksCount = 1;
        bool gameOver = true;

        bool deadlyShot = false;
        bool duckActive = true;
        bool duckEvaded = false;
        bool duckDead = false;

        const float PI = 3.1415926f;
        float maxInitialX = 1280;
    };
}
