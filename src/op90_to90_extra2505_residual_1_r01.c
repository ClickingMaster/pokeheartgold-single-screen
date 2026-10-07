#pragma force_active on
#include "global.h"
#include "sprite_system.h"

void ov96_02209B04(SpriteSystem *spriteSystem, SpriteManager *spriteManager);

// @fn ov96_02209B04
#pragma opt_propagation off
void ov96_02209B04(SpriteSystem *spriteSystem, SpriteManager *spriteManager) {
    int i;
    int vram;

    vram = NNS_G2D_VRAM_TYPE_2DMAIN;
    SpriteSystem_LoadCharResObj(spriteSystem, spriteManager, NARC_a_2_3_2, 16, TRUE, vram, 10002);
    SpriteSystem_LoadPlttResObj(spriteSystem, spriteManager, NARC_a_2_3_2, 15, FALSE, 1, vram, 10003);
    vram = NNS_G2D_VRAM_TYPE_2DSUB;
    i = 0;
    do {
        SpriteSystem_LoadCharResObj(spriteSystem, spriteManager, NARC_a_2_3_2, 16, TRUE, vram, 10003 + i);
        SpriteSystem_LoadPlttResObj(spriteSystem, spriteManager, NARC_a_2_3_2, 15, FALSE, 1, vram, 10004 + i);
        i++;
    } while (i < 2);
    SpriteSystem_LoadCellResObj(spriteSystem, spriteManager, NARC_a_2_3_2, 17, TRUE, 10002);
    SpriteSystem_LoadAnimResObj(spriteSystem, spriteManager, NARC_a_2_3_2, 18, TRUE, 10002);
}
#pragma opt_propagation reset
