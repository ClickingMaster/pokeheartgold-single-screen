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
typedef struct W23B {
    u8 state;
    u8 pad01;
    s16 baseY;
    int arg4;
    u8 timer;
    u8 pad09[3];
    void *ctx;
    void *pic;
    u8 anim[0x24];
    int arg38;
} W23B;

void ov07_02227630(int, W23B *);
void ov07_02227630(int task, W23B *work) {
    switch (work->state) {
    case 0:
        if (ov07_02222558(work->anim) == 1) {
            ov07_022227D8(&work->arg38);
            Pokepic_SetAttr(work->pic, 12, *(s16 *)&work->anim[0]);
            Pokepic_SetAttr(work->pic, 13, *(s16 *)&work->anim[2]);
            Pokepic_AddAttr(work->pic, 0, *(s16 *)&work->arg38);
            Pokepic_SetAttr(work->pic, 1, work->baseY + ov07_02222674(work->baseY, work->arg4, *(int *)&work->anim[0x14]));
            return;
        }
        work->state++;
        return;
    case 1:
        Pokepic_StartPaletteFade(work->pic, 0, 10, 0, 31);
        work->state++;
        return;
    case 2:
        if (Pokepic_ResumePaletteFade(work->pic) != 0) {
            break;
        }
        Pokepic_StartPaletteFade(work->pic, 10, 0, 0, 31);
        work->state++;
        return;
    case 3:
        if (Pokepic_ResumePaletteFade(work->pic) != 0) {
            break;
        }
        work->timer++;
        if (work->timer >= 3) {
            int a = ((s32)ov07_0221C4A8(work->ctx, 0));
            int b = ((s32)ov07_0221C4A8(work->ctx, 1));
            ov07_02222508(work->anim, (s16)a, 100, (s16)b, ((s32)ov07_0221C4A8(work->ctx, 3)));
            work->state++;
            return;
        }
        work->state = 1;
        return;
    case 4:
        if (ov07_02222558(work->anim) == 1) {
            Pokepic_SetAttr(work->pic, 12, *(s16 *)&work->anim[0]);
            Pokepic_SetAttr(work->pic, 13, *(s16 *)&work->anim[2]);
            Pokepic_SetAttr(work->pic, 1, work->baseY + ov07_02222674(work->baseY, work->arg4, *(int *)&work->anim[0x14]));
            return;
        }
        work->state++;
        return;
    case 5:
        if (ov07_02222558(work->anim) == 1) {
            Pokepic_SetAttr(work->pic, 12, *(s16 *)&work->anim[0]);
            Pokepic_SetAttr(work->pic, 13, *(s16 *)&work->anim[2]);
            Pokepic_SetAttr(work->pic, 1, work->baseY + ov07_02222674(work->baseY, work->arg4, *(int *)&work->anim[0x14]));
            return;
        }
        Pokepic_SetAttr(work->pic, 12, 256);
        Pokepic_SetAttr(work->pic, 13, 256);
        Pokepic_SetAttr(work->pic, 1, work->baseY);
        work->state++;
        return;
    default:
        ov07_0221C448(work->ctx, task);
        Heap_Free(work);
        break;
    }
}
