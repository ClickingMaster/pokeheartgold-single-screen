#include "global.h"

#include "battle/battle_system.h"

#include "heap.h"
#include "palette.h"
#include "pokemon.h"
#include "pokepic.h"
#include "sound.h"
#include "sys_task_api.h"
#include "unk_02005D10.h"
#include "unk_020163E0.h"

#define BATTLE(w) (*(BattleSystem **)(w))

typedef struct BattleFadeTask {
    BattleSystem *battleSystem;
    u8 battler;
    u8 side;
    u8 state;
    u8 pad07;
} BattleFadeTask;

typedef struct BattleTransitionTask {
    BattleSystem *battleSystem;
    void *unk04;
    u8 unk08[0x58];
    u8 unk60;
    u8 battler;
    u8 state;
    u8 attr;
    u32 flag;
} BattleTransitionTask;

void ov12_0226430C(BattleSystem *, int, int);
void ov12_022600F0(SysTask *, void *);
void ov12_0226037C(SysTask *, void *);
void ov12_022643C8(BattleSystem *, void *, void *, int, int, int, int, int);
void ov12_02261B80(BattleSystem *, void *, void *, void *);
void ov12_02261CA8(BattleSystem *, void *, void *, int);
void ov07_02234A20(void *, int);
void ov07_0223494C(void *, int);
BOOL ov07_0221C3B0(void *);
void ov07_0221C3C0(void *);
void ov07_0221C394(void);
void ov12_02260418(SysTask *, void *);

void ov12_0226037C(SysTask *task, void *arg) {
    BattleFadeTask *work = arg;
    PaletteData *palette = BattleSystem_GetPaletteData(work->battleSystem);
    PokepicManager *pokepic = BattleSystem_GetPokepicManager(work->battleSystem);

    switch (work->state) {
    case 0:
        sub_0201649C(BattleSystem_GetMessageIcon(work->battleSystem), 1);
        PaletteData_BeginPaletteFade(palette, 0xF, 0xFFFF, 1, 0, 16, 0);
        Pokepic_StartPaletteFadeAll(pokepic, 0, 16, 0, 0);
        GF_SndStartFadeOutBGM(0, 16);
        work->state++;
        /* fallthrough */
    case 1:
        if (PaletteData_GetSelectedBuffersBitmask(palette) != 0) {
            return;
        }
        work->state++;
        return;
    case 2:
        ov12_0226430C(work->battleSystem, work->side, work->battler);
        Heap_Free(work);
        SysTask_Destroy(task);
        return;
    default:
        return;
    }
}
