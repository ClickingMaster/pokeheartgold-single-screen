#pragma force_active on
#include "global.h"
#include "sprite_system.h"
typedef struct Work {
    void *unk0;
    u32 unk4;
    u8 pad8[0x98 - 8];
    ManagedSprite *sprites98[2];
    ManagedSprite *spritesA0[2];
    u8 padA8[0xc0 - 0xa8];
    u8 state;
} Work;
u32 ov96_021E5F24(void *);
void ov96_02219F20(Work *, s32, s32, s32);
void ov96_02219DA8(void *, u32, ManagedSprite *, u32, u8);
void ov96_02219EE0(Work *, s32);
int ov96_02219794(Work *work, void *unused, u8 kind);
// @fn ov96_02219794
int ov96_02219794(Work *work, void *unused, u8 kind) {
    u8 course = ov96_021E5F24(work->unk0);
    u8 next = (kind + 1) % 3;
    switch (work->state) {
    case 0: {
        s16 x, y;
        int d;
        ManagedSprite_GetPositionXYWithSubscreenOffset(work->sprites98[1], &x, &y, FX32_CONST(480));
        if (y < 216) {
            d = 272 - y;
            if (d < 0) {
                d = -d;
            }
            y += (s16)(d >> 2);
            if (y > 216) {
                y = 216;
            }
            ov96_02219F20(work, 15, x, y);
        } else {
            x = 40;
            y = -40;
            ov96_02219F20(work, 15, x, y);
            ov96_02219DA8(work->unk0, work->unk4, work->sprites98[1], course, kind);
            ov96_02219EE0(work, next);
            work->state++;
        }
        break;
    }
    case 1: {
        s16 x, y;
        s16 step;
        int d;
        ManagedSprite_GetPositionXYWithSubscreenOffset(work->sprites98[0], &x, &y, FX32_CONST(480));
        d = 136 - y;
        if (d < 0) {
            d = -d;
        }
        step = d * 3 / 4;
        if (y <= 136) {
            if (step == 0) {
                step = 1;
            }
            y += step;
            ov96_02219F20(work, 14, x, y);
            ManagedSprite_GetPositionXYWithSubscreenOffset(work->sprites98[1], &x, &y, FX32_CONST(480));
            y += step;
            ov96_02219F20(work, 15, x, y);
        } else {
            ManagedSprite *tmp = work->sprites98[0];
            work->sprites98[0] = work->sprites98[1];
            work->sprites98[1] = tmp;
            tmp = work->spritesA0[0];
            work->spritesA0[0] = work->spritesA0[1];
            work->spritesA0[1] = tmp;
            ov96_02219F20(work, 14, 40, 48);
            ov96_02219F20(work, 15, 40, 136);
            work->state++;
        }
        break;
    }
    case 2:
        work->state = 0;
        return 1;
    default:
        GF_AssertFail();
        break;
    }
    return 0;
}
