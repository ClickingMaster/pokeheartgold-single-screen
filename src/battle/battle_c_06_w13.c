#include "w13_private.h"

#define RAW_OPP(p)    ((u8 *)(p))
#define COMMAND(p)    (RAW_OPP(p) + 0x94)
#define BATTLER_ID(p) RAW_OPP(p)[0x65 * 4]

#define DEFINE_HANDLER3(name, worker)                                   \
    void worker(BattleSystem *, OpponentData *, u8 *);                  \
    void name(BattleSystem *battleSystem, OpponentData *opponentData) { \
        worker(battleSystem, opponentData, COMMAND(opponentData));      \
        ov12_02259928(opponentData);                                    \
    }

#define DEFINE_HANDLER2(name, worker)                                   \
    void worker(BattleSystem *, OpponentData *);                        \
    void name(BattleSystem *battleSystem, OpponentData *opponentData) { \
        worker(battleSystem, opponentData);                             \
        ov12_02259928(opponentData);                                    \
    }

void ov12_022598F8(BattleSystem *battleSystem, OpponentData *opponentData) {
    Window *window = BattleSystem_GetWindow(battleSystem, 0);
    FillWindowPixelBuffer(window, 0xFF);
    CopyWindowPixelsToVram_TextMode(window);
    BattleController_EmitCommandComplete(battleSystem, BATTLER_ID(opponentData), 0x42);
    ov12_02259928(opponentData);
}

void ov12_02259928(OpponentData *opponentData) {
    COMMAND(opponentData)[0] = 0;
}


DEFINE_HANDLER3(ov12_02259930, ov12_0225B454)


void ov12_02259944(BattleSystem *battleSystem, OpponentData *opponentData) {
    u8 *work = Heap_Alloc(HEAP_ID_BATTLE, 0x18);
    *(BattleSystem **)work = battleSystem;
    work[0x14] = 0;
    work[0x15] = 0;
    work[0x16] = 0;
    SysTask_CreateOnMainQueue(ov12_02260668, work, 0);
}
