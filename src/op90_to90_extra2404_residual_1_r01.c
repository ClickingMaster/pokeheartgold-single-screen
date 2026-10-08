#pragma force_active on
#include "global.h"
#include "sprite_system.h"
typedef struct Ov98SpriteContext Ov98SpriteContext;
ManagedSprite *ov98_0221E6E0(Ov98SpriteContext *, const ManagedSpriteTemplate *);
void MI_CpuCopy8(const void *src, void *dest, u32 size);
typedef struct Work {
    u8 pad00[0x14];
    Ov98SpriteContext *spriteCtx;
    ManagedSprite *sprites[41];
    u32 flags[10];
} Work;
extern const int ov99_021EA394[6];
BOOL ov99_021E8518(Work *work, s8 idx, int a2);
void ov99_021E86D4(Work *work, u8 index, s16 x, s16 y, int page, int sub);
void ov99_021E8D88(Work *work);
// @fn ov99_021E8D88
void ov99_021E8D88(Work *work) {
    ManagedSpriteTemplate tmpl = {0};
    u32 r, c;
    u8 idx;
    int j;
    for (r = 0; r < 2; r++) {
        for (c = 0; c < 5; c++) {
            idx = c + r * 5;
            MI_CpuCopy8(ov99_021EA394, tmpl.resIdList, sizeof(tmpl.resIdList));
            tmpl.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
            tmpl.x = c * 48 + 32;
            tmpl.y = r * 64 + 72;
            tmpl.drawPriority = 0;
            tmpl.animation = 16;
            j = idx + 12;
            work->sprites[j] = ov98_0221E6E0(work->spriteCtx, &tmpl);
            ManagedSprite_SetDrawFlag(work->sprites[j], work->flags[idx] == 0);
            tmpl.drawPriority = 1;
            tmpl.animation = 4;
            j = idx + 2;
            work->sprites[j] = ov98_0221E6E0(work->spriteCtx, &tmpl);
            ManagedSprite_SetDrawFlag(work->sprites[j], ov99_021E8518(work, idx, 1));
            ManagedSprite_SetAnimateFlag(work->sprites[j], TRUE);
            ov99_021E86D4(work, idx, (u16)(c * 6 + 1), (u16)(r * 8 + 6), work->flags[idx] == 0, 0);
        }
    }
}
