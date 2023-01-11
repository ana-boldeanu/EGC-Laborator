#pragma once

#include "components/simple_scene.h"

namespace m1
{
    class Meshes : public gfxc::SimpleScene
    {
     public:
         Meshes();
        ~Meshes();

        Mesh* plane;

     private:
         Mesh* CreatePlane();
    };
} 
