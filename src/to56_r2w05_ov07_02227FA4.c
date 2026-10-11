#include "global.h"
#pragma force_active on

typedef struct WFA4 {
    u8 state;
    u8 count;
    u8 toggle;
    u8 pad03;
    void *ctx;
    void *pokepic;
    s16 animX;
    s16 animY;
} WFA4;

extern const u8 ov07_022366CC[];
extern const u8 ov07_022366CD[];
extern const u8 ov07_022366CE[];
void BattleAnimUtil_InitScaleMotion(void *, s16, int, s16, int);
int BattleAnimUtil_UpdateScaleMotion(void *);
void Pokepic_StartPaletteFade(void *, int, int, int, int);
int Pokepic_ResumePaletteFade(void *);
void Pokepic_SetAttr(void *, int, int);
void BattleAnimSystem_DestroyTask(void *, int);
void Heap_Free(void *);
void ov07_02227FA4(int, WFA4 *);

void ov07_02227FA4(int task, WFA4 *work) {
    switch (work->state) {
    case 0: {
        int i = work->toggle * 3;
        BattleAnimUtil_InitScaleMotion(&work->animX, ov07_022366CC[i], 100, ov07_022366CD[i], ov07_022366CE[i]);
        if (work->toggle == 0) {
            Pokepic_StartPaletteFade(work->pokepic, 0, 6, 0, 0x7FFF);
        } else {
            Pokepic_StartPaletteFade(work->pokepic, 6, 0, 0, 0x7FFF);
        }
        work->toggle ^= 1;
        work->count++;
        work->state++;
        break;
    }
    case 1:
        if (BattleAnimUtil_UpdateScaleMotion(&work->animX) == 0 && Pokepic_ResumePaletteFade(work->pokepic) == 0) {
            if (work->count < 4) {
                work->state--;
            } else {
                work->state++;
            }
        }
        Pokepic_SetAttr(work->pokepic, 0xC, work->animX);
        Pokepic_SetAttr(work->pokepic, 0xD, work->animY);
        break;
    default:
        BattleAnimSystem_DestroyTask(work->ctx, task);
        Heap_Free(work);
        break;
    }
}
