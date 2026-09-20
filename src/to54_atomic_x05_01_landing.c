#include "global.h"

#pragma require_prototypes off

typedef struct PartyGaugeWork {
    void *battleSystem;
    u8 result;
    u8 battler;
    u8 kind;
    u8 state;
    u8 config[6];
    u8 flag;
} PartyGaugeWork;

typedef struct BattleObjectHolder {
    u8 filler_00[0x8C];
    void *object;
} BattleObjectHolder;

typedef struct BattleObjectWork {
    void *battleSystem;
    BattleObjectHolder *holder;
    u8 filler_08[4];
    u8 result;
    u8 battler;
    u8 state;
} BattleObjectWork;

extern int BattleSystem_GetFieldSide(void *, int);
extern void *BattleSystem_GetSpriteSystem(void *);
extern void *BattleSystem_GetSpriteManager(void *);
extern void *PartyGauge_NewAndShow(void *, int, int, int, void *, void *);
extern void *ov12_0223A908(void *, int);
extern void ov12_0223A914(void *, int, void *);
extern int PartyGauge_IsArrowTaskFinished(void *);
extern void PartyGauge_StartHideTask(void *, int, int);
extern int PartyGauge_IsHideTaskFinished(void *);
extern void PartyGauge_DeleteAndFreeResources(void *);
extern void ov12_0226430C(void *, int, int);
extern void Heap_Free(void *);
extern void SysTask_Destroy(void *);
extern void GF_AssertFail(void);
extern void *ov07_0221FB90(void *, int, int);
extern int ov07_0221FE70(void *);
extern void ov07_0221FE3C(void *, int);

void ov12_02260BA0(void *task, PartyGaugeWork *work) {
    int show;
    int variant;
    int side;
    void *spriteSystem;
    void *spriteManager;
    void *gauge;

    switch (BattleSystem_GetFieldSide(work->battleSystem, work->battler)) {
    case 0:
        side = 0;
        break;
    default:
        side = 1;
        break;
    }
    switch (work->state) {
    case 0:
        if (work->flag == 0) {
            show = 0;
            variant = 1;
        } else {
            show = 1;
            switch (work->kind) {
            case 3:
                variant = 0;
                break;
            case 5:
                variant = 2;
                break;
            default:
                variant = show;
                break;
            }
        }
        spriteSystem = BattleSystem_GetSpriteSystem(work->battleSystem);
        spriteManager = BattleSystem_GetSpriteManager(work->battleSystem);
        gauge = PartyGauge_NewAndShow(work->config, side, show, variant, spriteSystem, spriteManager);
        ov12_0223A914(work->battleSystem, side, gauge);
        work->state++;
        break;
    case 1:
        gauge = ov12_0223A908(work->battleSystem, side);
        if (PartyGauge_IsArrowTaskFinished(gauge) == 1) {
            work->state++;
        }
        break;
    default:
        ov12_0226430C(work->battleSystem, work->battler, work->result);
        Heap_Free(work);
        SysTask_Destroy(task);
        break;
    }
}

void ov12_02260C58(void *task, PartyGaugeWork *work) {
    int side;
    int hide;
    void *gauge;

    switch (BattleSystem_GetFieldSide(work->battleSystem, work->battler)) {
    case 0:
        side = 0;
        break;
    default:
        side = 1;
        break;
    }
    gauge = ov12_0223A908(work->battleSystem, side);
    switch (work->state) {
    case 0:
        switch (work->flag) {
        case 0:
            hide = 0;
            break;
        default:
            hide = 1;
            break;
        }
        PartyGauge_StartHideTask(gauge, hide, hide);
        work->state++;
        break;
    case 1:
        if (PartyGauge_IsHideTaskFinished(gauge) == 1) {
            PartyGauge_DeleteAndFreeResources(gauge);
            ov12_0223A914(work->battleSystem, side, NULL);
            work->state++;
        }
        break;
    default:
        ov12_0226430C(work->battleSystem, work->battler, work->result);
        Heap_Free(work);
        SysTask_Destroy(task);
        break;
    }
}

void ov12_02260CDC(void *task, BattleObjectWork *work) {
    if (work->state == 0) {
        if (work->holder->object != NULL) {
            GF_AssertFail();
        }
        work->holder->object = ov07_0221FB90(work->battleSystem, 5, work->battler);
        work->state++;
    } else {
        ov12_0226430C(work->battleSystem, work->battler, work->result);
        Heap_Free(work);
        SysTask_Destroy(task);
    }
}

void ov12_02260D28(void *task, BattleObjectWork *work) {
    if (work->holder->object == NULL) {
        GF_AssertFail();
    }
    if (ov07_0221FE70(work->holder->object) == 3 && work->state < 5) {
        work->state++;
        return;
    }
    ov07_0221FE3C(work->holder->object, work->battler);
    work->holder->object = NULL;
    ov12_0226430C(work->battleSystem, work->battler, work->result);
    Heap_Free(work);
    SysTask_Destroy(task);
}

typedef struct PaletteTaskWork {
    void *battleSystem;
    u8 result;
    u8 battler;
    u8 state;
    u8 mode;
    u8 counter;
} PaletteTaskWork;

extern void *BattleSystem_GetPaletteData(void *);
extern void *BattleSystem_GetPokepicManager(void *);
extern void *BattleSystem_GetMessageIcon(void *);
extern void sub_0201649C(void *, int);
extern void Pokepic_StartPaletteFadeAll(void *, int, int, int, u16);
extern void GF_SndStartFadeOutBGM(int, int);
extern void PaletteData_BlendPalette(void *, int, int, int, int, u16);
extern void ToggleBgLayer(int, int);

void ov12_02260D84(void *task, PaletteTaskWork *work) {
    void *paletteData = BattleSystem_GetPaletteData(work->battleSystem);
    void *pokepicManager = BattleSystem_GetPokepicManager(work->battleSystem);

    switch (work->state) {
    case 0:
        if (work->mode == 0) {
            sub_0201649C(BattleSystem_GetMessageIcon(work->battleSystem), 1);
            Pokepic_StartPaletteFadeAll(pokepicManager, 0, 0x10, 0, 0xFFFF);
        } else {
            GF_SndStartFadeOutBGM(0, 0x10);
        }
        work->state++;
    case 1:
        if (work->mode == 0) {
            PaletteData_BlendPalette(paletteData, 0, 0, 0xA0, work->counter, 0xFFFF);
            PaletteData_BlendPalette(paletteData, 0, 0xC0, 0x40, work->counter, 0xFFFF);
            PaletteData_BlendPalette(paletteData, 2, 0, 0xE0, work->counter, 0xFFFF);
        } else {
            PaletteData_BlendPalette(paletteData, 0, 0xA0, 0x20, work->counter, 0xFFFF);
            PaletteData_BlendPalette(paletteData, 1, 0, 0x100, work->counter, 0xFFFF);
            PaletteData_BlendPalette(paletteData, 3, 0, 0x100, work->counter, 0xFFFF);
        }
        if (work->counter++ == 0x10) {
            if (work->mode == 0) {
                ToggleBgLayer(0, 0);
            }
            work->state++;
        }
        break;
    case 2:
        ov12_0226430C(work->battleSystem, work->battler, work->result);
        Heap_Free(work);
        SysTask_Destroy(task);
        break;
    }
}
