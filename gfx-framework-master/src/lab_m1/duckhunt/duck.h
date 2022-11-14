#pragma once

#include "components/simple_scene.h"


namespace m1
{
    // Class containing data and meshes for the duck model and the functions used to create them
    class Duck : public gfxc::SimpleScene
    {
    public:
        Duck();
        ~Duck();
        Mesh* Duck::GetHead();
        Mesh* Duck::GetBody();
        Mesh* Duck::GetWingFront();
        Mesh* Duck::GetWingBack();
        float Duck::GetCenterX();       // Returns center coordinates
        float Duck::GetCenterY();
        float Duck::GetLength();        // Returns duck sizes
        float Duck::GetWidth();
        Mesh* CreateCircle(const std::string& name, float centerX, float centerY,
            float radius, glm::vec3 color);     // Returns a circle mesh


    private:
        // Create meshes for all duck parts
        Mesh* Duck::CreateDuckWingFront();
        Mesh* Duck::CreateDuckWingBack();
        Mesh* Duck::CreateDuckBody();
        Mesh* Duck::CreateDuckHead();


    protected:
        Mesh* head;
        Mesh* body;
        Mesh* wingFront;
        Mesh* wingBack;
        float tailTipX, tailTipY;  // Tail tip coordinates (duck drawing starts here)
        float beakTipX, beakTipY;  // Beak tip coordinates (duck drawing ends here)
        float centerY;
    };
}