#pragma force_active on
#include "global.h"
#include "sprite_system.h"
#include "pokepic.h"
#include "heap.h"
typedef struct Scale07 {
    u8 data[0x14];
} Scale07;
typedef struct Bounce07 {
    u8 state;
    u8 timer;
    u8 unk02[2];
    ManagedSprite *sprite;
    Scale07 scale;
    int unk1C;
    u8 unk20[0xC];
} Bounce07;
typedef struct Squash07 {
    u8 state;
    u8 count;
    s16 y;
    int x;
    s8 scale;
    u8 unk09[3];
    Pokepic *pokepic;
    void *ctx;
    SpriteManager *spriteManager;
    Bounce07 entries[3];
} Squash07;
extern const u8 ov07_022366C8[];
void ov07_02222590(Scale07 *scale, int a1, int a2, int a3, int a4, int a5, int a6);
BOOL ov07_0222260C(Scale07 *scale);
void ov07_02222644(Scale07 *scale, f32 *x, f32 *y);
void ov07_022226FC(ManagedSprite *sprite, s16 x, s16 y, int a3, int a4);
void ov07_0221C448(void *ctx, void *task);
void ov07_02228834(void *task, Squash07 *work);
// @fn ov07_02228834
void ov07_02228834(void *task, Squash07 *work) {
    switch (work->state) {
    case 0:
        Pokepic_SetAttr(work->pokepic, 14, 1);
        work->state++;
        break;
    case 1: {
        int i;
        int done = 0;
        f32 sx;
        f32 sy;
        for (i = 0; i < 3; i++) {
            switch (work->entries[i].state) {
            case 0:
                work->entries[i].timer++;
                if (work->entries[i].timer >= ov07_022366C8[i]) {
                    ov07_02222590(&work->entries[i].scale, 100, 5, 100, 5, 100, 5);
                    work->entries[i].state++;
                }
                break;
            case 1:
                if (ov07_0222260C(&work->entries[i].scale) == FALSE) {
                    work->entries[i].state++;
                } else {
                    ov07_02222644(&work->entries[i].scale, &sx, &sy);
                    ManagedSprite_SetAffineScale(work->entries[i].sprite, sx * work->scale, sy);
                    ov07_022226FC(work->entries[i].sprite, work->y, work->x, work->entries[i].unk1C, 0);
                }
                break;
            default:
                done++;
                break;
            }
            ManagedSprite_TickFrame(work->entries[i].sprite);
        }
        if (done < 3) {
            break;
        }
        work->count++;
        if (work->count < 3) {
            s16 x;
            s16 y;
            for (i = 0; i < 3; i++) {
                ManagedSprite_GetPositionXY(work->entries[i].sprite, &x, &y);
                ManagedSprite_SetPositionXY(work->entries[i].sprite, x, work->y);
                ManagedSprite_SetAffineScale(work->entries[i].sprite, work->scale, 1.0f);
                work->entries[i].state = 0;
                work->entries[i].timer = 0;
            }
            work->state--;
        } else {
            work->state++;
        }
        break;
    }
    case 2: {
        int i;
        for (i = 0; i < 3; i++) {
            ManagedSprite_SetOamMode(work->entries[i].sprite, GX_OAM_MODE_NORMAL);
            ov07_02222590(&work->entries[i].scale, 5, 100, 5, 100, 100, 5);
        }
        work->state++;
        break;
    }
    case 3: {
        int done = 0;
        int i;
        f32 sx;
        f32 sy;
        for (i = 0; i < 3; i++) {
            if (ov07_0222260C(&work->entries[i].scale) == FALSE) {
                done++;
            } else {
                ov07_02222644(&work->entries[i].scale, &sx, &sy);
                ManagedSprite_SetAffineScale(work->entries[i].sprite, sx * work->scale, sy);
                ov07_022226FC(work->entries[i].sprite, work->y, work->x, work->entries[i].unk1C, 0);
            }
        }
        if (done >= 3) {
            work->state++;
        }
        break;
    }
    default:
        Pokepic_SetAttr(work->pokepic, 14, 0);
        ov07_0221C448(work->ctx, task);
        Heap_Free(work);
        return;
    }
    SpriteSystem_DrawSprites(work->spriteManager);
}
