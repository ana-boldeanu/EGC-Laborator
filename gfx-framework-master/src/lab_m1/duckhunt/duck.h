#pragma once

#include "components/simple_scene.h"


namespace m1
{
    class Duck : public gfxc::SimpleScene
    {
    public:
        Duck();
        ~Duck();
        Mesh* Duck::GetHead();
        Mesh* Duck::GetBody();
        Mesh* Duck::GetWingFront();
        Mesh* Duck::GetWingBack();
        float Duck::GetCenterX();
        float Duck::GetCenterY();
        Mesh* CreateCircle(const std::string& name, float centerX, float centerY,
            float radius, glm::vec3 color);


    private:
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