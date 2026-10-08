#pragma force_active on
#include "global.h"
typedef struct SpriteSystem SpriteSystem;
typedef struct SpriteManager SpriteManager;
typedef struct ManagedSprite ManagedSprite;
typedef struct ManagedSpriteTemplate {
    s16 x;
    s16 y;
    s16 z;
    u16 animation;
    int drawPriority;
    int pal;
    int vram;
    int resIdList[6];
    int bgPriority;
    int vramTransfer;
} ManagedSpriteTemplate;
ManagedSprite *SpriteSystem_NewSprite(SpriteSystem *, SpriteManager *, const ManagedSpriteTemplate *);
void ManagedSprite_SetPaletteOverrideOffset(ManagedSprite *, int);
void ManagedSprite_SetAnim(ManagedSprite *, int);
void ManagedSprite_TickTwoFrames(ManagedSprite *);
typedef struct Ov92Gfx {
    u8 pad0[8];
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
} Ov92Gfx;
typedef struct Ov92Obj {
    int self;
    int count;
    u8 pad8[0xC];
    Ov92Gfx *gfx;
    u8 pad18[4];
    ManagedSprite *spritesA[3];
    ManagedSprite *spritesB[3];
    ManagedSprite *spritesC[3];
    u8 pad40[0xF4 - 0x40];
    u16 slots[4];
} Ov92Obj;
extern const s16 ov92_02263B74[][3];
int ov92_0226077C(void *, u32);
void ov92_0225DDD8(ManagedSpriteTemplate *, s16, s16, u32, u32, u32);
void ov92_0225DF58(Ov92Obj *, int, int);
void ov92_0225E6A4(void *arg, Ov92Obj *obj);
// @fn ov92_0225E6A4
void ov92_0225E6A4(void *arg, Ov92Obj *obj) {
    ManagedSpriteTemplate tmpl;
    SpriteSystem *spriteSystem = obj->gfx->spriteSystem;
    SpriteManager *spriteManager = obj->gfx->spriteManager;
    int n = 0;
    int i;
    for (i = 0; i < obj->count; i++) {
        obj->slots[i] = 0xFF;
        if (i == obj->self) {
            continue;
        }
        obj->slots[i] = n;
        ov92_0225DDD8(&tmpl, ov92_02263B74[obj->count - 1][n], 0xF0, 2, 0, 0x2328);
        tmpl.drawPriority = 1;
        obj->spritesA[n] = SpriteSystem_NewSprite(spriteSystem, spriteManager, &tmpl);
        ov92_0225DDD8(&tmpl, ov92_02263B74[obj->count - 1][n], 0xF0, 2, 0, 0x2328);
        tmpl.drawPriority = 2;
        obj->spritesB[n] = SpriteSystem_NewSprite(spriteSystem, spriteManager, &tmpl);
        ManagedSprite_SetPaletteOverrideOffset(obj->spritesB[n], ov92_0226077C(arg, i));
        ov92_0225DDD8(&tmpl, ov92_02263B74[obj->count - 1][n], 0xF0, 2, 0, 0x2328);
        tmpl.drawPriority = 3;
        obj->spritesC[n] = SpriteSystem_NewSprite(spriteSystem, spriteManager, &tmpl);
        ManagedSprite_SetPaletteOverrideOffset(obj->spritesC[n], ov92_0226077C(arg, i));
        ManagedSprite_SetAnim(obj->spritesC[n], 5);
        ManagedSprite_TickTwoFrames(obj->spritesA[n]);
        ManagedSprite_TickTwoFrames(obj->spritesB[n]);
        ManagedSprite_TickTwoFrames(obj->spritesC[n]);
        ov92_0225DF58(obj, i, 0);
        n++;
    }
}
