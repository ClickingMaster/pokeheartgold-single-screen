#pragma force_active on
#include "global.h"
#include "sprite_system.h"

typedef struct CourseResources {
    u32 heapId;
    void *bgConfig;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
} CourseResources;

void ov96_022163AC(CourseResources *work);

// @fn ov96_022163AC
void ov96_022163AC(CourseResources *work) {
    SpriteSystem *spriteSystem;
    int i;
    SpriteManager *spriteManager;

    spriteSystem = work->spriteSystem;
    spriteManager = work->spriteManager;

    GF_ASSERT(spriteSystem != NULL);
    GF_ASSERT(spriteManager != NULL);
    {
        NNS_G2D_VRAM_TYPE vram = NNS_G2D_VRAM_TYPE_2DMAIN;
    SpriteSystem_LoadCharResObj(spriteSystem, spriteManager, NARC_a_2_4_0, 16, TRUE, vram, 10000);
    SpriteSystem_LoadPlttResObj(spriteSystem, spriteManager, NARC_a_2_4_0, 13, FALSE, 2, vram, 10000);
    SpriteSystem_LoadCellResObj(spriteSystem, spriteManager, NARC_a_2_4_0, 15, TRUE, 10000);
    SpriteSystem_LoadAnimResObj(spriteSystem, spriteManager, NARC_a_2_4_0, 14, TRUE, 10000);
    }
    {
        NNS_G2D_VRAM_TYPE vram = NNS_G2D_VRAM_TYPE_2DSUB;
    SpriteSystem_LoadCharResObj(spriteSystem, spriteManager, NARC_a_2_4_0, 20, TRUE, vram, 10001);
    SpriteSystem_LoadPlttResObj(spriteSystem, spriteManager, NARC_a_2_4_0, 17, FALSE, 2, vram, 10001);
    SpriteSystem_LoadCellResObj(spriteSystem, spriteManager, NARC_a_2_4_0, 19, TRUE, 10001);
    SpriteSystem_LoadAnimResObj(spriteSystem, spriteManager, NARC_a_2_4_0, 18, TRUE, 10001);
    }
    {
        NNS_G2D_VRAM_TYPE vram = NNS_G2D_VRAM_TYPE_2DSUB;
    for (i = 0; i < 2; i++) {
        SpriteSystem_LoadCharResObj(spriteSystem, spriteManager, NARC_a_2_4_0, 24, TRUE, vram, 10002 + i);
        SpriteSystem_LoadPlttResObj(spriteSystem, spriteManager, NARC_a_2_4_0, 21, FALSE, 1, vram, 10002 + i);
    }
    SpriteSystem_LoadCellResObj(spriteSystem, spriteManager, NARC_a_2_4_0, 23, TRUE, 10002);
    SpriteSystem_LoadAnimResObj(spriteSystem, spriteManager, NARC_a_2_4_0, 22, TRUE, 10002);
    }
}
