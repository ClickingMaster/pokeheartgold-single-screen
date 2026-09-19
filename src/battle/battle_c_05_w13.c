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


DEFINE_HANDLER3(ov12_022595B8, ov12_0225B0A0)

DEFINE_HANDLER3(ov12_022595CC, ov12_0225B0E8)


void ov12_022595E0(BattleSystem *battleSystem, OpponentData *opponentData) {
    u8 *command = COMMAND(opponentData);
    if (BattleSystem_GetFieldSide(battleSystem, BATTLER_ID(opponentData)) != 0) {
        ov12_0225B120(battleSystem, opponentData, command);
    } else {
        ov12_0226430C(battleSystem, BATTLER_ID(opponentData), 0x32);
    }
    ov12_02259928(opponentData);
}

void ov12_0225961C(BattleSystem *battleSystem, OpponentData *opponentData) {
    u8 *command = COMMAND(opponentData);
    if (BattleSystem_GetFieldSide(battleSystem, BATTLER_ID(opponentData)) != 0) {
        ov12_0225B16C(battleSystem, opponentData, command);
    } else {
        ov12_0226430C(battleSystem, BATTLER_ID(opponentData), 0x33);
    }
    ov12_02259928(opponentData);
}

void ov12_02259658(BattleSystem *battleSystem, OpponentData *opponentData) {
    SpriteSystem *spriteSystem = BattleSystem_GetSpriteSystem(battleSystem);
    SpriteManager *spriteManager = BattleSystem_GetSpriteManager(battleSystem);
    PaletteData *paletteData = BattleSystem_GetPaletteData(battleSystem);
    PartyGauge_LoadGraphics(spriteSystem, spriteManager, paletteData);
    ov12_0226430C(battleSystem, BATTLER_ID(opponentData), 0x34);
    ov12_02259928(opponentData);
}

void ov12_02259694(BattleSystem *battleSystem, OpponentData *opponentData) {
    int offset = 0x65;
    PartyGauge_FreeGraphics(BattleSystem_GetSpriteManager(battleSystem));
    ov12_0226430C(battleSystem, ((u8 *)opponentData)[offset * 4], 0x35);
    ov12_02259928(opponentData);
}

void ov12_022596B8(BattleSystem *battleSystem, OpponentData *opponentData) {
    u8 *command = COMMAND(opponentData);
    if (command[1] == 0) {
        if (RAW_OPP(opponentData)[0x196] == 0) {
            BattleSystem_GameStatIncrement(battleSystem, *(u16 *)(command + 2));
        }
    } else if (RAW_OPP(opponentData)[0x196] != 0) {
        BattleSystem_GameStatIncrement(battleSystem, *(u16 *)(command + 2));
    }
    ov12_0226430C(battleSystem, BATTLER_ID(opponentData), command[0]);
    ov12_02259928(opponentData);
}

void ov12_02259700(BattleSystem *battleSystem, OpponentData *opponentData) {
    u8 *command = COMMAND(opponentData);
    ov12_0223BF14(battleSystem, *(u16 *)(command + 2), command + 4);
    ov12_0225B1A8(battleSystem, opponentData);
    ov12_02259928(opponentData);
}


DEFINE_HANDLER3(ov12_02259724, ov12_0225B200)

DEFINE_HANDLER2(ov12_02259738, ov12_0225B234)

DEFINE_HANDLER2(ov12_02259748, ov12_0225B26C)

DEFINE_HANDLER2(ov12_02259758, ov12_0225B2A4)


void ov12_02259768(BattleSystem *battleSystem, OpponentData *opponentData) {
    u8 *command = COMMAND(opponentData);
    ov12_0223BF14(battleSystem, *(u16 *)(command + 2), command + 4);
    ov12_0225B2F8(battleSystem, opponentData, command);
    ov12_02259928(opponentData);
}

void ov12_0225978C(BattleSystem *battleSystem, OpponentData *opponentData) {
    u8 *command = COMMAND(opponentData);
    ov12_0223BF14(battleSystem, *(u16 *)(command + 2), command + 4);
    ov12_0225B34C(battleSystem, opponentData);
    ov12_02259928(opponentData);
}


DEFINE_HANDLER3(ov12_022597B0, ov12_0225B3A0)

DEFINE_HANDLER3(ov12_022597C4, ov12_0225B3D4)

DEFINE_HANDLER3(ov12_022597D8, ov12_0225B434)

