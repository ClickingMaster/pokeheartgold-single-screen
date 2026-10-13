#ifndef POKEHEARTGOLD_BATTLE_LINK_BATTLE_SCREEN_H
#define POKEHEARTGOLD_BATTLE_LINK_BATTLE_SCREEN_H

#include "battle/battle_setup.h"

#include "heap.h"
#include "party.h"
#include "pm_string.h"

// Screens shown around a link battle (overlay 5, started by battle overlay 12).
enum LinkBattleScreenMode {
    LINK_BATTLE_SCREEN_MODE_PARTY,  // multi battle, before: party preview of the local player and the partner
    LINK_BATTLE_SCREEN_MODE_VS,     // before the battle: "VS" demo with the players' names and party balls
    LINK_BATTLE_SCREEN_MODE_RESULT, // after the battle: win/lose/draw, then the Battle Video save prompt
};

typedef struct LinkBattleScreenArgs {
    BattleSetup *setup; // 0x00, result mode only
    Party *parties[4];  // 0x04, indexed by link slot
    String *names[4];   // 0x14, indexed by link slot
    enum HeapID heapId; // 0x24
    u8 mode;            // 0x28, enum LinkBattleScreenMode
    u8 multiOrSide;     // 0x29, party mode: the local player's side; other modes: multi battle
    u8 outcome;         // 0x2A, BATTLE_OUTCOME_*
    u8 done;            // 0x2B, set when the screen has finished
    u8 linkBattleMode;          // 0x2C, BattleSetup.linkBattleMode, passed to the battle record save
    u8 unk_2D[3];       // 0x2D
} LinkBattleScreenArgs; // size: 0x30

void LinkBattleScreen_Start(LinkBattleScreenArgs *args);

#endif // POKEHEARTGOLD_BATTLE_LINK_BATTLE_SCREEN_H
