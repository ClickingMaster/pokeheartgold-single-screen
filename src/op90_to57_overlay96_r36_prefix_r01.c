#pragma force_active on
#include "global.h"
#include "sprite_system.h"
typedef struct {
    u8 pad0[0x18];
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    ManagedSprite *sprites[26];
} Work;
void ov96_021EC5C0(Work *work);
// @fn ov96_021EC5C0
void ov96_021EC5C0(Work *work) {
    ManagedSpriteTemplate tmpl = {0};
    int i;
    int j;
    for (i = 0; i < 4; i++) {
        int idx = i * 4 + 5;
        tmpl.resIdList[0] = 0x69;
        tmpl.resIdList[1] = 0x69;
        tmpl.resIdList[2] = 0x65;
        tmpl.resIdList[3] = 0x65;
        tmpl.vram = NNS_G2D_VRAM_TYPE_2DSUB;
        tmpl.bgPriority = 1;
        tmpl.drawPriority = 2;
        tmpl.x = i * 0x40 + 0x20;
        tmpl.y = 0x48;
        tmpl.animation = 0;
        tmpl.pal = i + 1;
        work->sprites[idx] = SpriteSystem_NewSpriteWithYOffset(work->spriteSystem, work->spriteManager, &tmpl, 0x20C000);
        ManagedSprite_SetAnimateFlag(work->sprites[idx], 1);
        tmpl.pal = 0;
        tmpl.bgPriority = 1;
        tmpl.drawPriority = 1;
        tmpl.x -= 16;
        tmpl.animation = 1;
        for (j = 0; j < 3; j++) {
            int k = idx + 1 + j;
            work->sprites[k] = SpriteSystem_NewSpriteWithYOffset(work->spriteSystem, work->spriteManager, &tmpl, 0x20C000);
            ManagedSprite_SetAnimateFlag(work->sprites[k], 1);
            tmpl.x += 16;
        }
    }
}
