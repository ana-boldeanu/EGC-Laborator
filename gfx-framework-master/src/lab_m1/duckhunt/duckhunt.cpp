#include "lab_m1/duckhunt/duckhunt.h"

#include <vector>
#include <iostream>

using namespace std;
using namespace m1;


DuckHunt::DuckHunt()
{
    srand(static_cast <unsigned> (time(0)));
    
    // Compute duck information
    duckLength = (duck->GetLength()) * duckScale;
    duckWidth = (duck->GetWidth()) * duckScale;

    // Randomize duck spawning point
    initialX = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / maxInitialX));
    initialY = 0;
    currX = initialX;
    currY = initialY;

    // Randomize duck flying angle
    flyRight = rand() % 2; 
    flightAngle = (float(rand() % 5 + 3) / 10) * PI / 2; // Angle between 30% and 70% of PI/2
    if (!flyRight) {
        flightAngle += PI / 2;
    }

    // Set duck flying speed
    flightSpeed = initialFlightSpeed;
    flight = new Flight(flightSpeed);

    // Set game stats
    bulletCount = gameStats->bulletCount;
    lifeCount = gameStats->lifeCount;

    // Initialize text renderer
    glm::ivec2 resolution = window->GetResolution();
    textRenderer = new gfxc::TextRenderer(window->props.selfDir, resolution.x, resolution.y);
    textRenderer->Load(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::FONTS, "Hack-Bold.ttf"), 18);
}


DuckHunt::~DuckHunt()
{
}


void DuckHunt::Init()
{
    // Set 2D camera functions
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    // Add all meshes used in the game
    meshes["duck_head"] = duck->GetHead();
    meshes["duck_body"] = duck->GetBody();
    meshes["duck_wing_front"] = duck->GetWingFront();
    meshes["duck_wing_back"] = duck->GetWingBack();
    meshes["life"] = gameStats->GetLifeSymbol();
    meshes["bullet"] = gameStats->GetBulletSymbol();
    meshes["maxScore"] = gameStats->GetMaxScoreBox();
    meshes["currScore"] = gameStats->GetCurrScoreBox();
    meshes["grass_lower"] = gameStats->GetGrass(true);
    meshes["grass_upper"] = gameStats->GetGrass(false);
}


void DuckHunt::FrameStart()
{
    // Clear the color buffer (using the previously set color) and depth buffer
    glClearColor(skyColor.x, skyColor.y, skyColor.z, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();

    // Set the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void DuckHunt::Update(float deltaTimeSeconds)
{
    // Round updates
    UpdateRoundState(deltaTimeSeconds);

    // Flying collision updates
    UpdateFlyingDirection();
    
    // Transformations for duck model position
    modelMatrix *= transform2D::Translate(initialX, initialY);

    modelMatrix *= transform2D::Translate(-duck->GetCenterX(), -duck->GetCenterY());

    modelMatrix = flight->TranslateDuck(modelMatrix, deltaTimeSeconds, flightAngle, translateX, translateY);

    flightMatrix = flight->RotateDuck(modelMatrix, flightAngle);

    flightMatrix *= transform2D::Scale(duckScale, duckScale);

    // Add flying animation to the duck wings model matrix
    if (!duckDead && !gameOver) {
        wingsMatrix = flight->FlapWing(flightMatrix);
    }
    else {
        wingsMatrix = flightMatrix;
    }

    // Render UI
    RenderInterface(lifeCount, bulletCount, score);
    if (gameOver) {
        if (lifeCount == 3) {
            // This is the start of the game
            textRenderer->RenderText("Press R to start!", 500, 300, 1.25, textColor);
        }
        else {
            // This is when the game is lost, but can be restarted
            textRenderer->RenderText("You lost! :(", 570, 300, 1.25, textColor);
            textRenderer->RenderText("Press R to try again", 500, 335, 1.25, textColor);
        }
        
    }
    
    // Render front grass
    RenderMesh2D(meshes["grass_lower"], shaders["VertexColor"], interfaceMatrix);

    // Render duck
    if (!gameOver) {
        RenderMesh2D(meshes["duck_wing_front"], shaders["VertexColor"], wingsMatrix);

        RenderMesh2D(meshes["duck_body"], shaders["VertexColor"], flightMatrix);

        RenderMesh2D(meshes["duck_wing_back"], shaders["VertexColor"], wingsMatrix);

        RenderMesh2D(meshes["duck_head"], shaders["VertexColor"], flightMatrix);
    }

    // Render background grass
    RenderMesh2D(meshes["grass_upper"], shaders["VertexColor"], interfaceMatrix);

    // Render clouds
    RenderEnvironment();

    // Reset transform matrix for next frame
    modelMatrix = glm::mat3(1);
    flightMatrix = glm::mat3(1);
    wingsMatrix = glm::mat3(1);
}


void DuckHunt::FrameEnd()
{
}


void DuckHunt::UpdateRoundState(float deltaTimeSeconds)
{
    // Check if the game is won
    if (score >= gameStats->maxScore) {
        skyColor = glm::vec3(1, 1, 0.8f);
        duckActive = false;
        duckEvaded = false;
        duckDead = false;
        textRenderer->RenderText("You won! ^-^", 550, 300, 1.25, textColor);
    }

    // Update round timer
    if (duckActive && !gameOver) {
        timePassed += deltaTimeSeconds;
    }

    // Check if duck evaded
    if (duckActive && (timePassed >= timeLimit || bulletCount <= 0)) {
        skyColor = glm::vec3(1, 0.8f, 0.8f);
        duckActive = false;
        duckEvaded = true;
        timePassed = 0;
        lifeCount--;
    }

    // Check if game is lost (player out of lives)
    if (lifeCount == 0) {
        gameOver = true;
        skyColor = glm::vec3(0.4f, 0.4f, 0.4f);
    }

    // Check if the duck was shot
    if (duckActive && deadlyShot && !gameOver) {
        skyColor = glm::vec3(0.8f, 1, 0.8f);
        duckActive = false;
        duckDead = true;
        timePassed = 0;
    }
}


void DuckHunt::ResetDuck()
{
    // Reset and randomize initial position
    initialX = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / maxInitialX)); initialY = 0;
    currX = initialX;
    currY = initialY;
    translateX = 0; translateY = 0;
    ducksCount++;

    // Increase flying speed after every 5 ducks
    if (ducksCount % 5 == 0) {
        flightSpeed *= 1.15f;
    }

    // Randomize flying angle
    flyRight = rand() % 2; flyUp = true;
    flightAngle = (float(rand() % 5 + 3) / 10) * PI / 2; // Angle between 30% and 70% of PI/2
    if (!flyRight) {
        flightAngle += PI / 2;
    }
    flight = new Flight(flightSpeed);

    // Other round-related elements
    bulletCount = 3;

    duckActive = true;
    duckEvaded = false;
    duckDead = false;
    deadlyShot = false;

    skyColor = glm::vec3(0.8f, 1, 1);
}


void DuckHunt::UpdateFlyingDirection()
{
    glm::ivec2 resolution = window->GetResolution();

    // Update duck center position
    currX = initialX + translateX;
    currY = initialY + translateY;

    // Check for collisions
    if (duckActive) {
        // Right wall
        if (flyRight && currX > resolution.x) {
            flyRight = false;
            flightAngle = PI - flightAngle;
        }
        // Top wall
        if (flyUp && currY > resolution.y) {
            flyUp = false;
            flightAngle = -flightAngle;
        }
        // Left wall
        if (!flyRight && currX < 0) {
            flyRight = true;
            flightAngle = PI - flightAngle;
        }
        // Bottom wall
        if (!flyUp && currY < 0) {
            flyUp = true;
            flightAngle = -flightAngle;
        }
    }
    // Reset duck if it evaded
    else if (duckEvaded && !gameOver) {
        flyUp = true;
        flightAngle = PI / 2;

        if (currY > resolution.y + duckLength) {
            ResetDuck();
        }
    }
    // Reset duck after it fell
    else if (duckDead && !gameOver) {
        flyUp = false;
        flightAngle = -PI / 2;

        if (currY < -duckLength) {
            ResetDuck();
        }
    }
}


void DuckHunt::OnKeyPress(int key, int mods)
{
    // Start or Reset the game
    if (key == GLFW_KEY_R) {
        bulletCount = gameStats->bulletCount;
        lifeCount = gameStats->lifeCount;
        ducksCount = 1;
        score = 0;
        gameOver = false;
        flightSpeed = initialFlightSpeed;
        ResetDuck();
    }

    // Hack to increase score B-)
    if (key == GLFW_KEY_V) {
        score += 99;
    }
}


void DuckHunt::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{    
    // Compute mouse position in the logical space
    glm::ivec2 resolution = window->GetResolution();
    glm::vec3 mousePosition = glm::vec3(mouseX, resolution.y - mouseY, 1);

    // Place the cursor in (0, 0) and undo the rotations
    glm::mat3 reverseTransform = glm::mat3(1);
    reverseTransform *= transform2D::Rotate(-flightAngle);
    reverseTransform *= transform2D::Translate(-currX, -currY);
    mousePosition = reverseTransform * mousePosition;

    // Check if cursor is within the initial duck size limits
    if (mousePosition.x <= duckLength / 2) {
        if (mousePosition.x >= -duckLength / 2) {
            if (mousePosition.y <= duckWidth / 2) {
                if (mousePosition.y >= -duckWidth / 2) {
                    deadlyShot = true;
                }
                else {
                    deadlyShot = false;
                }
            }
            else {
                deadlyShot = false;
            }
        }
        else {
            deadlyShot = false;
        }
    }
    else {
        deadlyShot = false;
    }

    // Update bullet count
    if (!gameOver) {
        bulletCount--;
    }

    // Update score
    if (deadlyShot) {
        score += pointsPerDuck * flightSpeed / 400;
    }
}


void DuckHunt::RenderInterface(int lifeCount, int bulletCount, float score)
{
    // Render current lives (position on screen is given by interface constants)
    interfaceMatrix = glm::mat3(1);
    interfaceMatrix *= transform2D::Translate(gameStats->lifePosX, gameStats->lifePosY);

    for (int i = 0; i < lifeCount; i++) {
        RenderMesh2D(meshes["life"], shaders["VertexColor"], interfaceMatrix);
        interfaceMatrix *= transform2D::Translate(gameStats->lifePosDist, 0);
    }

    // Render current bullets 
    interfaceMatrix = glm::mat3(1);
    interfaceMatrix *= transform2D::Translate(gameStats->bulletPosX, gameStats->bulletPosY);

    for (int i = 0; i < bulletCount; i++) {
        RenderMesh2D(meshes["bullet"], shaders["VertexColor"], interfaceMatrix);
        interfaceMatrix *= transform2D::Translate(gameStats->bulletPosDist, 0);
    }


    // Render current score
    float maxScaleFactor = 16;
    float currScaleFactor = 0;
    currScaleFactor = (score / gameStats->maxScore) * maxScaleFactor;

    interfaceMatrix = glm::mat3(1);
    interfaceMatrix *= transform2D::Translate(gameStats->scorePosX, gameStats->scorePosY);
    interfaceMatrix *= transform2D::Scale(maxScaleFactor, 1);
    RenderMesh2D(meshes["maxScore"], shaders["VertexColor"], interfaceMatrix);

    interfaceMatrix = glm::mat3(1);
    interfaceMatrix *= transform2D::Translate(gameStats->scorePosX, gameStats->scorePosY);
    interfaceMatrix *= transform2D::Scale(currScaleFactor, 1);
    RenderMesh2D(meshes["currScore"], shaders["VertexColor"], interfaceMatrix);

    interfaceMatrix = glm::mat3(1);
}


void DuckHunt::RenderEnvironment()
{
    // Render rightmost cloud
    float radius = 70;
    float centerX = 925;
    float centerY = 525;

    Mesh* cloud = duck->CreateCircle("clouds", centerX, centerY, radius + 12, glm::vec3(1, 1, 1));
    RenderMesh2D(cloud, shaders["VertexColor"], interfaceMatrix);

    cloud = duck->CreateCircle("clouds", centerX + 100, centerY, radius, glm::vec3(1, 1, 1));
    RenderMesh2D(cloud, shaders["VertexColor"], interfaceMatrix);

    cloud = duck->CreateCircle("clouds", centerX - 100, centerY, radius, glm::vec3(1, 1, 1));
    RenderMesh2D(cloud, shaders["VertexColor"], interfaceMatrix);

    // Render leftmost cloud
    radius = 50;
    centerX = 225;
    centerY = 425;

    cloud = duck->CreateCircle("clouds", centerX, centerY, radius + 8, glm::vec3(1, 1, 1));
    RenderMesh2D(cloud, shaders["VertexColor"], interfaceMatrix);

    cloud = duck->CreateCircle("clouds", centerX + 70, centerY, radius, glm::vec3(1, 1, 1));
    RenderMesh2D(cloud, shaders["VertexColor"], interfaceMatrix);

    cloud = duck->CreateCircle("clouds", centerX - 70, centerY, radius, glm::vec3(1, 1, 1));
    RenderMesh2D(cloud, shaders["VertexColor"], interfaceMatrix);

    // Render middle cloud
    radius = 45;
    centerX = 505;
    centerY = 615;

    cloud = duck->CreateCircle("clouds", centerX, centerY, radius + 8, glm::vec3(1, 1, 1));
    RenderMesh2D(cloud, shaders["VertexColor"], interfaceMatrix);

    cloud = duck->CreateCircle("clouds", centerX - 70, centerY, radius, glm::vec3(1, 1, 1));
    RenderMesh2D(cloud, shaders["VertexColor"], interfaceMatrix);

    cloud = duck->CreateCircle("clouds", centerX + 70, centerY, radius, glm::vec3(1, 1, 1));
    RenderMesh2D(cloud, shaders["VertexColor"], interfaceMatrix);
}