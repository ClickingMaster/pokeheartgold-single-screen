#pragma force_active on
#include "global.h"
#include "sprite_system.h"
typedef struct {
    u8 pad0[0x18];
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    ManagedSprite *sprites[26];
    u8 pad88[0xb1 - 0x88];
    u8 xb1;
} Work;
void ov96_021EC70C(Work *work);
// @fn ov96_021EC70C
void ov96_021EC70C(Work *work) {
    ManagedSpriteTemplate tmpl = {0};
    int i;
    for (i = 0; i < 2; i++) {
        int idx = i + 24;
        tmpl.resIdList[0] = i + 0x6b;
        tmpl.resIdList[1] = i + 0x6b;
        tmpl.resIdList[2] = 0x67;
        tmpl.resIdList[3] = 0x67;
        tmpl.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
        tmpl.bgPriority = 0;
        tmpl.drawPriority = 2;
        tmpl.x = i * 0x40 + 0x60;
        tmpl.y = 0x38;
        if (work->xb1 < 5) {
            tmpl.x = 0x80;
        }
        work->sprites[idx] = SpriteSystem_NewSpriteWithYOffset(work->spriteSystem, work->spriteManager, &tmpl, 0x20C000);
        ManagedSprite_SetAnimateFlag(work->sprites[idx], 1);
        ManagedSprite_SetDrawFlag(work->sprites[idx], 0);
    }
}
