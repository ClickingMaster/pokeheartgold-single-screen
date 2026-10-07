#include "global.h"

void Heap_Free(void *);
int ManagedSprite_GetDrawFlag(void *);
void ManagedSprite_GetPositionXY(void *, s16 *, s16 *);
void ManagedSprite_SetAffineScale(void *, int, int);
void ManagedSprite_SetDrawFlag(void *, int);
void Pokepic_AddAttr(void *, int, int);
int Pokepic_ResumePaletteFade(void *);
void Pokepic_SetAttr(void *, int, int);
void Pokepic_StartPaletteFade(void *, int, int, int, int);
void SpriteSystem_DrawSprites(void *);
void Sprite_DeleteAndFreeResources(void *);
void Sprite_TickFrame(void *);
void ov07_0221C448(void *, int);

extern u32 ov07_0221C4A8(void *p, int i);
void ov07_02222508(void *, s16, int, s16, int);
int ov07_02222558(void *);
void ov07_02222590(void *, int, int, int, int, int, int);
int ov07_0222260C(void *);
void ov07_02222644(void *, int *, int *);
int ov07_02222674(s16, int, int);
void ov07_022227D8(void *);

extern const u8 ov07_022366D2[][2];

typedef struct W23A {
    u8 state;
    u8 spriteState[3];
    u8 timer;
    u8 blendA;
    u8 blendB;
    u8 pad07;
    void *ctx;
    void *spriteSystemA;
    void *spriteSystemB;
    void *sprites[3];
    u8 anim[0x24];
} W23A;

void ov07_02227300(int, W23A *);
void ov07_02227300(int task, W23A *work) {
    int sx;
    int sy;
    s16 pos[2];
    int i;
    switch (work->state) {
    case 0:
        work->timer++;
        for (i = 0; i < ((s32)ov07_0221C4A8(work->ctx, 0)); i++) {
            ManagedSprite_GetPositionXY(work->sprites[i], &pos[1], &pos[0]);
            if (work->timer >= ov07_022366D2[i][0]) {
                work->spriteState[i]++;
                if (work->spriteState[i] >= ov07_022366D2[i][1]) {
                    work->spriteState[i] = 0;
                    if (ManagedSprite_GetDrawFlag(work->sprites[i]) == 1) {
                        ManagedSprite_SetDrawFlag(work->sprites[i], 0);
                    } else {
                        ManagedSprite_SetDrawFlag(work->sprites[i], 1);
                    }
                }
            } else {
                ManagedSprite_SetDrawFlag(work->sprites[i], 0);
            }
        }
        if (work->timer >= 45) {
            for (i = 0; i < ((s32)ov07_0221C4A8(work->ctx, 0)); i++) {
                ManagedSprite_SetDrawFlag(work->sprites[i], 1);
            }
            work->timer = 0;
            work->state++;
        }
        break;
    case 1:
        ov07_02222590(work->anim, 100, 60, 100, 100, 100, 10);
        work->state++;
        break;
    case 2:
        if (ov07_0222260C(work->anim) == 1) {
            for (i = 0; i < ((s32)ov07_0221C4A8(work->ctx, 0)); i++) {
                ov07_02222644(work->anim, &sx, &sy);
                ManagedSprite_SetAffineScale(work->sprites[i], sx, sy);
            }
        } else {
            work->timer++;
            if (work->timer >= 45) {
                work->state++;
            }
        }
        break;
    case 3:
        if (work->blendA != 0) {
            work->blendA--;
        }
        if (work->blendB < 15) {
            work->blendB++;
        }
        if (work->blendA == 0 && work->blendB == 15) {
            work->state++;
        }
        *(vu16 *)0x04000052 = work->blendA | (work->blendB << 8);
        break;
    default:
        for (i = 0; i < ((s32)ov07_0221C4A8(work->ctx, 0)); i++) {
            Sprite_DeleteAndFreeResources(work->sprites[i]);
        }
        ov07_0221C448(work->ctx, task);
        Heap_Free(work);
        return;
    }
    for (i = 0; i < ((s32)ov07_0221C4A8(work->ctx, 0)); i++) {
        Sprite_TickFrame(*(void **)work->sprites[i]);
    }
    SpriteSystem_DrawSprites(work->spriteSystemB);
}
