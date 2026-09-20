#include "global.h"

#pragma require_prototypes off

typedef struct Ov112Work {
    void *args;
    u32 heapId;
    u8 pad08[8];
    void *narc;
    void *bgConfig;
    u8 pad18[0x54];
    void *spriteSystem;
    void *sprite70;
    void *sprite74;
    void *sprite78;
    u8 pad7C[0xC1];
    u8 selection;
    u8 count;
    u8 pad13F[5];
    union {
        u32 raw;
        struct {
            u32 pad0 : 10;
            u32 active : 1;
            u32 state : 4;
            u32 pad15 : 17;
        } bits;
    } flags;
    u8 pad148[8];
} Ov112Work;

typedef struct Ov112RecordInput {
    const void *name;
    void *ptr;
    u16 value;
    u8 value0 : 5;
    u8 value1 : 2;
    u8 value2 : 1;
    u8 pad0B;
} Ov112RecordInput;

typedef struct Ov112SmallInput {
    u16 value;
    u8 value0 : 5;
    u8 value1 : 2;
    u8 value2 : 1;
} Ov112SmallInput;

typedef enum Ov112MainState {
    OV112_MAIN_INIT,
    OV112_MAIN_SETUP,
    OV112_MAIN_WAIT,
    OV112_MAIN_FADE,
    OV112_MAIN_DONE
} Ov112MainState;

typedef struct Ov112Record {
    s32 unk00;
    u8 pad04[6];
    u16 value0A;
    u16 value0C;
    u8 pad0E[0x12];
    u8 name[0x16];
    u8 pad36[0x46];
    void *ptr7C;
    u8 pad80[4];
    u8 type84;
    u8 value85_0 : 5;
    u8 value85_1 : 2;
    u8 value85_2 : 1;
    u8 value86_0 : 5;
    u8 value86_1 : 2;
    u8 value86_2 : 1;
    u8 pad87;
} Ov112Record;

extern void ManagedSprite_SetDrawFlag(void *, int);
extern BOOL ManagedSprite_IsAnimated(void *);
extern void SpriteSystem_DrawSprites(void *);
extern void SpriteSystem_TransferOam(void);
extern void DoScheduledBgGpuUpdates(void *);
extern void GfGfx_SwapDisplay(void);
extern void PlaySE(int);
extern void BeginNormalPaletteFade(int, int, int, int, int, int, int);
extern void *OverlayManager_GetArgs(void *);
extern void *OverlayManager_GetData(void *);
extern void *OverlayManager_CreateAndGetData(void *, u32, int);
extern void OverlayManager_FreeData(void *);
extern void Heap_Create(int, int, u32);
extern void Heap_Destroy(int);
extern void *NARC_New(int, int);
extern void NARC_Delete(void *);
extern void ResetVisibleHardwareWindows(int);
extern void Main_SetVBlankIntrCB(void *, void *);
extern u8 gSystem[];

#ifndef W09_TAIL_ONLY
int ov112_021F2CD4(Ov112Work *work) {
    if (work->flags.bits.active == 0) {
        return 1;
    }
    switch (work->flags.bits.state) {
    case 0:
        BeginNormalPaletteFade(3, 0, 0, 0x7FFF, 2, 1, work->heapId);
        work->flags.bits.state++;
        break;
    case 1:
        if (IsPaletteFadeFinished()) {
            ManagedSprite_SetDrawFlag(work->sprite70, work->selection != 0);
            ManagedSprite_SetDrawFlag(work->sprite74, work->selection < work->count - 1);
            ov112_021F1CC8(work, 0);
            ov112_021F2328(work);
            ov112_021F2B80(work);
            BeginNormalPaletteFade(3, 1, 1, 0x7FFF, 2, 1, work->heapId);
            work->flags.bits.state++;
        }
        break;
    case 2:
        if (IsPaletteFadeFinished()) {
            work->flags.bits.state = 0;
            work->flags.bits.active = 0;
            return 1;
        }
        break;
    default:
        GF_AssertFail();
        break;
    }
    return 0;
}

void ov112_021F2EB0(Ov112Work *work);

void ov112_021F2DE4(void *manager) {
    Ov112Work *work;
    void *args = OverlayManager_GetArgs(manager);

    GF_ASSERT(args != NULL);
    Heap_Create(3, 0x9B, 3 << 17);
    work = OverlayManager_CreateAndGetData(manager, 0x150, 0x9B);
    MI_CpuFill8(work, 0, 0x150);
    work->heapId = 0x9B;
    work->args = args;
    ov112_021F0F90(work);
    ov112_021F0DC0();
    ov112_021F0DF4();
    gSystem[0x69] = TRUE;
    GfGfx_SwapDisplay();
    work->narc = NARC_New(0xFB, work->heapId);
    ov112_021F179C(work);
    ov112_021F1288(work);
    ov112_021F196C(work);
    ResetVisibleHardwareWindows(0);
    ResetVisibleHardwareWindows(1);
    Main_SetVBlankIntrCB(ov112_021F2EB0, work);
}

void ov112_021F2E70(void *manager) {
    Ov112Work *work = OverlayManager_GetData(manager);

    ov112_021F1324();
    ov112_021F1814(work);
    ov112_021F1A08(work);
    NARC_Delete(work->narc);
    gSystem[0x69] = FALSE;
    GfGfx_SwapDisplay();
    ov112_021F0DC0();
    OverlayManager_FreeData(manager);
    Heap_Destroy(0x9B);
}

void ov112_021F2EB0(Ov112Work *work) {
    GF_ASSERT(work != NULL);
    GF_ASSERT(work->spriteSystem != NULL);
    GF_ASSERT(work->bgConfig != NULL);
    SpriteSystem_DrawSprites(work->spriteSystem);
    SpriteSystem_TransferOam();
    DoScheduledBgGpuUpdates(work->bgConfig);
    {
        vu32 *base = (vu32 *)0x027E0000;
        base[0x3FF8 / sizeof(*base)] |= 1;
    }
}
#endif

#ifdef W09_INCLUDE_PARTIAL
BOOL ov112_021F2EF4(void *manager, Ov112MainState *state) {
    Ov112Work *work = OverlayManager_GetData(manager);
    Ov112MainState stateValue = *state;

    switch (stateValue) {
    case 0:
        ov112_021F2DE4(manager);
        (*state)++;
        break;
    case 1:
        ov112_021F1838();
        ov112_021F1A40(work);
        (*state)++;
        break;
    case 2:
        if (ov112_021F1DC0()) {
            ov112_021F1B94(work);
            ov112_021F178C(work);
            work->flags.raw = (work->flags.raw & ~0xFF) | 0xFF;
            ov112_021F2B80(work);
            (*state)++;
        }
        break;
    case 3:
        BeginNormalPaletteFade(0, 1, 1, 0x7FFF, 6, 1, work->heapId);
        (*state)++;
        break;
    case 4:
        if (IsPaletteFadeFinished()) {
            return TRUE;
        }
        break;
    default:
        GF_AssertFail();
        break;
    }
    return FALSE;
}
#endif

#ifndef W09_HEAD_ONLY
BOOL ov112_021F2FAC(void *manager, int *state) {
    Ov112Work *work = OverlayManager_GetData(manager);

    switch (*state) {
    case 0:
        if (!ManagedSprite_IsAnimated(work->sprite78)) {
            (*state)++;
        }
        break;
    case 1:
        BeginNormalPaletteFade(0, 0, 0, 0, 6, 1, work->heapId);
        (*state)++;
        break;
    case 2:
        if (IsPaletteFadeFinished()) {
            ov112_021F2E70(manager);
            return TRUE;
        }
        break;
    default:
        GF_AssertFail();
        break;
    }
    return FALSE;
}

BOOL ov112_021F3018(void *manager) {
    Ov112Work *work = OverlayManager_GetData(manager);

    if (!ov112_021F2CD4(work)) {
        return FALSE;
    }
    if (ov112_021F0EB4(work)) {
        if (work->selection != 0) {
            work->selection--;
        }
        ov112_021F2CC4(work);
        PlaySE(0x5DC);
    } else if (ov112_021F0EFC(work)) {
        work->selection = (work->selection + 1) % work->count;
        ov112_021F2CC4(work);
        PlaySE(0x5DC);
    } else if (ov112_021F0E60(work)) {
        return TRUE;
    }
    ov112_021F2338(work);
    ov112_021F1D28(work, 5);
    ov112_021F1D28(work, 6);
    ov112_021F1D28(work, 7);
    ov112_021F1D28(work, 8);
    return FALSE;
}

void ov112_021F30BC(Ov112Record *record, const Ov112RecordInput *input) {
    MI_CpuFill8(record, 0, sizeof(*record));
    MI_CpuCopy8(input->name, record->name, sizeof(record->name));
    record->value0A = input->value;
    record->value85_0 = input->value0;
    record->value85_1 = input->value1;
    record->value85_2 = input->value2;
    record->ptr7C = input->ptr;
    record->type84 = 0x1A;
    record->unk00 = -1;
}

void ov112_021F3140(Ov112Record *record, const Ov112SmallInput *input) {
    int i;

    for (i = 0; i < 24; i++, record++) {
        if (record->type84 == 0x1D) {
            record->value0C = input->value;
            record->value86_0 = input->value0;
            record->value86_1 = input->value1;
            record->value86_2 = input->value2;
        }
    }
}
#endif
