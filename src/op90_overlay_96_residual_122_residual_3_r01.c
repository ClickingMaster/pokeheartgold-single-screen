#pragma force_active on
#include "global.h"
#include "sprite_system.h"
typedef struct Work {
    void *unk0;
    u32 unk4;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    u8 pad10[0x12];
    u8 unk22;
    u8 pad23[0x60 - 0x23];
    ManagedSprite *sprite60;
    u8 pad64[4];
    ManagedSprite *spritesA[4];
    ManagedSprite *spritesB[4];
    ManagedSprite *spritesC[4];
    ManagedSprite *spritesD[2];
    ManagedSprite *spritesE[2];
} Work;
ManagedSprite *ov96_022199A8(SpriteSystem *, SpriteManager *, s16 x, s16 y, u16 anim, u16 prio);
void ov96_02219DD0(Work *, u8, u32);
void ov96_02219DA8(void *, u32, ManagedSprite *, u32, u8);
void ov96_02219E60(Work *);
void ov96_02219C30(Work *work);
// @fn ov96_02219C30
void ov96_02219C30(Work *work) {
    ManagedSpriteTemplate tmpl = {0};
    int i;
    s16 x;
    work->sprite60 = ov96_022199A8(work->spriteSystem, work->spriteManager, 134, 160, work->unk22 + 12, 6);
    ManagedSprite_SetPaletteOverride(work->sprite60, 1);
    for (i = 0; i < 4; i++) {
        x = i * 32 + 120;
        work->spritesA[i] = ov96_022199A8(work->spriteSystem, work->spriteManager, x, 36, i + 1, 5);
        ov96_02219DD0(work, i, 0);
        work->spritesB[i] = ov96_022199A8(work->spriteSystem, work->spriteManager, x, 36, 10, 4);
        ManagedSprite_SetDrawFlag(work->spritesB[i], FALSE);
        work->spritesC[i] = ov96_022199A8(work->spriteSystem, work->spriteManager, i * 32 + 120, 24, 6, 0);
        ManagedSprite_SetDrawFlag(work->spritesC[i], FALSE);
    }
    for (i = 0; i < 2; i++) {
        tmpl.resIdList[0] = 10002 + i;
        tmpl.resIdList[1] = 10002 + i;
        tmpl.resIdList[2] = 10002;
        tmpl.resIdList[3] = 10002;
        tmpl.vram = NNS_G2D_VRAM_TYPE_2DSUB;
        tmpl.x = 40;
        tmpl.y = i * 88 + 48;
        tmpl.drawPriority = i + 2;
        tmpl.bgPriority = 2;
        work->spritesD[i] = SpriteSystem_NewSpriteWithYOffset(work->spriteSystem, work->spriteManager, &tmpl, FX32_CONST(480));
        ManagedSprite_SetAnimateFlag(work->spritesD[i], TRUE);
    }
    for (i = 0; i < 2; i++) {
        ov96_02219DA8(work->unk0, work->unk4, work->spritesD[i], work->unk22, 2 - i);
        work->spritesE[i] = ov96_022199A8(work->spriteSystem, work->spriteManager, 40, i * 88 + 24, 5, 1);
        ManagedSprite_SetDrawFlag(work->spritesE[i], FALSE);
    }
    ov96_02219E60(work);
}
