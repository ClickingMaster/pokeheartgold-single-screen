#pragma force_active on
#include "global.h"
#include "sprite_system.h"

typedef struct W {
    u8 pad[0xc];
    SpriteSystem *system;
    SpriteManager *manager;
} W;

void ov96_0220EB3C(W *w);

// @fn ov96_0220EB3C
#pragma opt_propagation off
void ov96_0220EB3C(W *w) {
    SpriteSystem *system = w->system;
    SpriteManager *manager = w->manager;
    int vram;

    GF_ASSERT(system != NULL);
    GF_ASSERT(manager != NULL);
    vram = NNS_G2D_VRAM_TYPE_2DMAIN;
    SpriteSystem_LoadCharResObj(system, manager, NARC_a_2_3_5, 22, TRUE, vram, 10000);
    SpriteSystem_LoadPlttResObj(system, manager, NARC_a_2_3_5, 19, FALSE, 2, vram, 10000);
    SpriteSystem_LoadCellResObj(system, manager, NARC_a_2_3_5, 21, TRUE, 10000);
    SpriteSystem_LoadAnimResObj(system, manager, NARC_a_2_3_5, 20, TRUE, 10000);
    vram = NNS_G2D_VRAM_TYPE_2DSUB;
    SpriteSystem_LoadCharResObj(system, manager, NARC_a_2_3_5, 26, TRUE, vram, 10001);
    SpriteSystem_LoadPlttResObj(system, manager, NARC_a_2_3_5, 23, FALSE, 4, vram, 10001);
    SpriteSystem_LoadCellResObj(system, manager, NARC_a_2_3_5, 25, TRUE, 10001);
    SpriteSystem_LoadAnimResObj(system, manager, NARC_a_2_3_5, 24, TRUE, 10001);
    vram = NNS_G2D_VRAM_TYPE_2DMAIN;
    SpriteSystem_LoadCharResObj(system, manager, NARC_a_2_3_5, 18, TRUE, vram, 10002);
    SpriteSystem_LoadPlttResObj(system, manager, NARC_a_2_3_5, 15, FALSE, 1, vram, 10002);
    vram = NNS_G2D_VRAM_TYPE_2DSUB;
    SpriteSystem_LoadCharResObj(system, manager, NARC_a_2_3_5, 18, TRUE, vram, 10003);
    SpriteSystem_LoadPlttResObj(system, manager, NARC_a_2_3_5, 15, FALSE, 1, vram, 10003);
    SpriteSystem_LoadCellResObj(system, manager, NARC_a_2_3_5, 17, TRUE, 10002);
    SpriteSystem_LoadAnimResObj(system, manager, NARC_a_2_3_5, 16, TRUE, 10002);
}
#pragma opt_propagation reset
