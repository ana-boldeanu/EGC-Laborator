#pragma once

#include "components/simple_scene.h"
#include "lab_m1/duckhunt/transform2D.h"
#include "components/text_renderer.h"
#include "duck.h"
#include "flight.h"
#include "interface.h"


namespace m1
{
    // Main class of the game, containing model rendering, game mechanics and round update logic
    class DuckHunt : public gfxc::SimpleScene
    {
     public:
         DuckHunt();
        ~DuckHunt();

        void Init() override;
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

     private:
        // Reset duck properties for the next round
        void DuckHunt::ResetDuck();

        // Render UI elements (lives, bullets, score bar etc.)
        void DuckHunt::RenderInterface(int lifeCount, int bulletCount, float score);

        // Render background/decorative elements (clouds)
        void DuckHunt::RenderEnvironment();

        // Checks and updates duckActive/Evaded/Dead states and round timePassed
        void DuckHunt::UpdateRoundState(float deltaTimeSeconds);

        // Checks for window border collisions and resets round if duck was dead or evaded
        void DuckHunt::UpdateFlyingDirection();

        // Key press events
        void DuckHunt::OnKeyPress(int key, int mods) override;

        // Process each mouse click as a gunshot and check if it hit the duck or not
        void DuckHunt::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;

     protected:
        glm::mat3 modelMatrix = glm::mat3(1);       // Matrix used to scale or rotate the whole model
        glm::mat3 flightMatrix = glm::mat3(1);      // Matrix used to translate the whole model
        glm::mat3 wingsMatrix = glm::mat3(1);       // Matrix used for wings flapping animation
        glm::mat3 interfaceMatrix = glm::mat3(1);   // Matrix used for UI transformations
        glm::vec3 skyColor = glm::vec3(0.8f, 1, 1); // Cyan (round), Red (evaded), Green (shot)
        glm::vec3 text_color = glm::vec3(0.30f, 0.65f, 0.05f);

        gfxc::TextRenderer *textRenderer;            // Used to render on-screen text updates

        Flight* flight;                             // Info about flying mechanics & functions
        Duck* duck = new Duck();                    // Info about duck model building
        Interface* gameStats = new Interface();     // Info about lives/bullets/score

        float duckLength;           // Length of the original duck model
        float duckWidth;            // Width of the original duck model
        float duckScale = 0.8f;     // Scale used to render the duck
        float timePassed = 0;       // Time passed for current duck / round
        float timeLimit = 5;        // Maximum time that a duck can stay on screen before evading

        float initialX, initialY;   // Initial coordinates for duck center (X randomized each round)
        float currX, currY;         // Current coordinates for duck center
        float translateX = 0;       // Current translation, relative to initial spawn point
        float translateY = 0;
        float flightAngle = 0;      // Current rotation angle of duck model, also gives flying direction
        float initialFlightSpeed = 500;     // Flying speed for first round
        float flightSpeed;                  // Current flying speed
        bool flyRight, flyUp = true;        // Current flight direction
        
        int lifeCount, bulletCount;         // Current game stats
        float score = 0;                    // Current score

        float pointsPerDuck = 2.75;         // Constant multiplier for each duck that is shot
        int ducksCount = 1;                 // Number of ducks spawned so far
        bool gameOver = true;               // Current game state

        bool deadlyShot = false;            // The mouse on-click coordinates collided with the duck model
        bool duckActive = true;             // The duck is currently flying freely
        bool duckEvaded = false;            // The duck is evading (time limit passed or no bullets left)
        bool duckDead = false;              // The duck is falling (it was deadlyShot)

        const float PI = 3.1415926f;
        float maxInitialX = 1280;           // Maximum value for spawning X coordinate
    };
}
