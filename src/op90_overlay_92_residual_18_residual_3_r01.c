#pragma force_active on
#include "global.h"
typedef struct ManagedSprite ManagedSprite;
typedef struct SysTask SysTask;
typedef struct Ov92Entry {
    s32 timer;
    s32 frames;
    s32 start;
    s32 palA[3];
    s32 palB[3];
    void (*func)(SysTask *, void *);
} Ov92Entry;
typedef struct Ov92Stride {
    s32 value;
    u8 pad[0x24];
} Ov92Stride;
typedef struct Ov92FuncStride {
    void (*func)(SysTask *, void *);
    u8 pad[0x24];
} Ov92FuncStride;
typedef struct Ov92Obj {
    ManagedSprite *sprite;
    u8 pad[0x4C];
} Ov92Obj;
typedef struct Ov92Ctx {
    u8 pad[0x34];
    u8 blocked;
} Ov92Ctx;
typedef struct Ov92Work {
    Ov92Obj objsA[2];
    Ov92Obj objsB[2];
    u8 pad140[0x140 - 0x140];
    s32 x140;
    s32 x144;
    s32 x148;
    s32 x14C;
    u8 pad150[8];
    s32 *countPtr;
    u8 pad15C[8];
    u32 timer;
    u32 unk168;
    u8 pad16C[8];
    u32 enabled;
    u32 direction;
    Ov92Ctx *context;
} Ov92Work;
extern const Ov92Entry ov92_02263EA0[];
extern const Ov92Stride ov92_02263EA4[];
extern const Ov92Stride ov92_02263EA8[];
extern const Ov92FuncStride ov92_02263EC4[];
void ov92_022616D8(void *);
void ov92_02261850(void *);
void SysTask_Destroy(SysTask *);
SysTask *SysTask_CreateOnMainQueue(void (*)(SysTask *, void *), void *, u32);
BOOL IsPaletteFadeFinished(void);
void ManagedSprite_SetPaletteOverrideOffset(ManagedSprite *, int);
void ov92_02261A18(SysTask *, void *);
// @fn ov92_02261A18
void ov92_02261A18(SysTask *task, void *data) {
    Ov92Work *work = data;
    s32 n = *work->countPtr;
    int i;
    if (!IsPaletteFadeFinished() || work->context->blocked == 1) {
        SysTask_Destroy(task);
        return;
    }
    if (work->enabled) {
        if (work->direction == 0) {
            ov92_022616D8(work);
        }
        if (work->timer == 0 && n != 0 && n < 8) {
            work->timer = ov92_02263EA0[n].timer;
            work->unk168 = 0;
            work->x140 = ov92_02263EA4[n].value - 1;
            work->x144 = ov92_02263EA8[n].value;
            work->x148 = ov92_02263EA8[n].value + 5;
            for (i = 0; i < 2; i++) {
                ManagedSprite_SetPaletteOverrideOffset(work->objsA[i].sprite, ov92_02263EA0[n].palA[i] + 4);
                ManagedSprite_SetPaletteOverrideOffset(work->objsB[i].sprite, ov92_02263EA0[n].palB[i]);
            }
            SysTask_CreateOnMainQueue(ov92_02263EC4[n].func, work, 0x1001);
        }
    } else if (work->enabled == 0 && work->timer == 0 && work->direction == 1) {
        ov92_02261850(work);
    }
    if (work->timer) {
        work->timer--;
    }
}
