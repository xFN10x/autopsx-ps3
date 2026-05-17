#include <stdio.h>
#include <stdbool.h>
#include <tiny3d.h>

static bool running = true;

int main()
{
    bool drawTest = true;
    tiny3d_Init(1024*1024);
    tiny3d_Project2D();
    

    while (running)
    {
        tiny3d_Clear(0xffff0000, TINY3D_CLEAR_ALL);

        if (drawTest) {
            tiny3d_SetPolygon(TINY3D_QUADS);

            tiny3d_VertexPos(0,0,0);
            tiny3d_VertexColor(0xffffffff);
            tiny3d_VertexPos(400,0,0);
            tiny3d_VertexPos(400,400,0);
            tiny3d_VertexPos(0,400,0);
            tiny3d_End();
        }

        tiny3d_Flip();
    }
    
    return 0;
}