#include <stdio.h>
#include <stdbool.h>
#include <tiny3d.h>

#include <sysmodule/sysmodule.h>
#include <pngdec/pngdec.h>

#define SCREEN_TOP 0
#define SCREEN_DOWN 511
#define SCREEN_RIGHT 847
#define SCREEN_LEFT 0

static bool running = true;

// textures
void *texMem;

pngData bg0d;
pngData bg1d;
u32 bg0o;
u32 bg1o;

bool loadPNGToRSX(pngData png, void **texPointer, u32 *offset)
{
    if (!texPointer || !*texPointer || !png.bmp_out)
        return 0;

    *offset = tiny3d_TextureOffset(*texPointer);

    printf("Loading png... %dx%d\n", png.width, png.height);

    RSX_MEMCPY(*texPointer, png.bmp_out, (size_t)png.pitch * png.height);
    free(png.bmp_out);

    size_t bytes = (size_t)png.pitch * png.height;
    size_t aligned = (bytes + 15) & ~((size_t)15) / 4; // align to 16 bytes

    *texPointer = (void *)((unsigned char *)*texPointer + aligned);

    printf("Loaded image.\n");
    return 1;
}

void drawBackground()
{
    // backmost
    tiny3d_SetTexture(0, bg1o, bg1d.width, bg1d.height, bg1d.pitch, TINY3D_TEX_FORMAT_A8R8G8B8, TEXTURE_LINEAR);

    tiny3d_SetPolygon(TINY3D_QUADS);

    tiny3d_VertexPos(0, 0, 0);
    tiny3d_VertexColor(0xffffffff);
    tiny3d_VertexTexture(0, 0);

    tiny3d_VertexPos(SCREEN_RIGHT, 0, 0);
    tiny3d_VertexTexture(1, 0);

    tiny3d_VertexPos(SCREEN_RIGHT, SCREEN_DOWN, 0);
    tiny3d_VertexTexture(1, 1);

    tiny3d_VertexPos(0, SCREEN_DOWN, 0);
    tiny3d_VertexTexture(0, 1);

    tiny3d_End();
    
    // frontmost
    tiny3d_SetTexture(0, bg0o, bg0d.width, bg0d.height, bg0d.pitch, TINY3D_TEX_FORMAT_A8R8G8B8, TEXTURE_LINEAR);

    tiny3d_SetPolygon(TINY3D_QUADS);

    tiny3d_VertexPos(0, 0, 0);
    tiny3d_VertexColor(0xffffffff);
    tiny3d_VertexTexture(0, 0);

    tiny3d_VertexPos(SCREEN_RIGHT, 0, 0);
    tiny3d_VertexTexture(1, 0);

    tiny3d_VertexPos(SCREEN_RIGHT, SCREEN_DOWN, 0);
    tiny3d_VertexTexture(1, 1);

    tiny3d_VertexPos(0, SCREEN_DOWN, 0);
    tiny3d_VertexTexture(0, 1);

    tiny3d_End();
}

void loadTextures()
{

    printf("Loading textures...\n");

    void *texPointer;

    texMem = tiny3d_AllocTexture(64 * 1024 * 1024);
    if (!texMem)
    {
        perror("Failed to alloc memory.\n");
        return;
    }
    texPointer = texMem;

    s32 bg1 = pngLoadFromFile("/dev_hdd0/game/FNHB00000/assets/bg1.png", &bg1d);
    loadPNGToRSX(bg1d, &texPointer, &bg1o);
    printf("bg1 offset %d\n", bg1o);

    s32 bg0 = pngLoadFromFile("/dev_hdd0/game/FNHB00000/assets/bg0.png", &bg0d);
    loadPNGToRSX(bg0d, &texPointer, &bg0o);
    printf("bg0 offset %d\n", bg0o);
}

void draw()
{
    bool drawTest = true;

    tiny3d_Clear(0xff000000, TINY3D_CLEAR_ALL);

    // Enable alpha Test
    tiny3d_AlphaTest(1, 0x10, TINY3D_ALPHA_FUNC_GEQUAL);

    // Enable alpha blending.
    tiny3d_BlendFunc(1, TINY3D_BLEND_FUNC_SRC_RGB_SRC_ALPHA | TINY3D_BLEND_FUNC_SRC_ALPHA_SRC_ALPHA,
                     TINY3D_BLEND_FUNC_DST_RGB_ONE_MINUS_SRC_ALPHA | TINY3D_BLEND_FUNC_DST_ALPHA_ZERO,
                     TINY3D_BLEND_RGB_FUNC_ADD | TINY3D_BLEND_ALPHA_FUNC_ADD);

    drawBackground();

    if (drawTest)
    {
        tiny3d_SetPolygon(TINY3D_QUADS);

        /*
        how quads are drawn:
        1----2
        |    |
        |    |
        |    |
        4----3
        */
        tiny3d_VertexPos(0, 0, 0);
        tiny3d_VertexColor(0xffffffff);
        tiny3d_VertexPos(300, 0, 0);
        tiny3d_VertexPos(300, 300, 0);
        tiny3d_VertexPos(0, 300, 0);
        tiny3d_End();
    }

    tiny3d_Flip();
}

int main()
{
    sysModuleLoad(SYSMODULE_PNGDEC);

    tiny3d_Init(1024 * 1024);
    //stole this from apollo-ps3, it wasn't in the docs so idk
    tiny3d_UserViewport(1, 0, 0, // 2D position
		(float) (Video_Resolution.width / 848.0f),  (float) (Video_Resolution.height / 512.0f),   // 2D scale
		(float) (Video_Resolution.width / 1920.0f), (float) (Video_Resolution.height / 1080.0f)); // 3D scale
    tiny3d_Project2D();

    loadTextures();

    while (running)
    {
        draw();
    }

    return 0;
}