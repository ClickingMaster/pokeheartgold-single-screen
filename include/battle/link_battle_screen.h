#ifndef POKEHEARTGOLD_BATTLE_LINK_BATTLE_SCREEN_H
#define POKEHEARTGOLD_BATTLE_LINK_BATTLE_SCREEN_H

#include "battle/battle_setup.h"

#include "heap.h"
#include "party.h"
#include "pm_string.h"

// Screens shown around a link battle.
enum LinkBattleScreenMode {
    LINK_BATTLE_SCREEN_MODE_PARTY,  // multi battle, before: party preview of the local player and the partner
    LINK_BATTLE_SCREEN_MODE_VS,     // before the battle: "VS" demo with the players' names and party balls
    LINK_BATTLE_SCREEN_MODE_RESULT, // after the battle: win/lose/draw, then the Battle Video save prompt
};

typedef struct LinkBattleScreenArgs {
    BattleSetup *setup; // result mode only
    Party *parties[4];  // indexed by link slot
    String *names[4];   // indexed by link slot
    enum HeapID heapId;
    u8 mode;           // enum LinkBattleScreenMode
    u8 multiOrSide;    // party mode: the local player's side; other modes: multi battle
    u8 outcome;        // BATTLE_OUTCOME_*
    u8 done;           // set when the screen has finished
    u8 linkBattleMode; // BattleSetup.linkBattleMode, passed to the battle record save
    u8 unk_2D[3];
} LinkBattleScreenArgs;

void LinkBattleScreen_Start(LinkBattleScreenArgs *args);

#endif // POKEHEARTGOLD_BATTLE_LINK_BATTLE_SCREEN_H
