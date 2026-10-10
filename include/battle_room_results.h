#ifndef POKEHEARTGOLD_BATTLE_ROOM_RESULTS_H
#define POKEHEARTGOLD_BATTLE_ROOM_RESULTS_H

#include "overlay_manager.h"
#include "save.h"

typedef struct BattleRoomResultsArgs {
    SaveData *saveData;
    u16 unk_4;
    u16 unk_6;
} BattleRoomResultsArgs;

BOOL BattleRoomResults_Init(OverlayManager *man, int *state);
BOOL BattleRoomResults_Main(OverlayManager *man, int *state);
BOOL BattleRoomResults_Exit(OverlayManager *man, int *state);

#endif // POKEHEARTGOLD_BATTLE_ROOM_RESULTS_H
