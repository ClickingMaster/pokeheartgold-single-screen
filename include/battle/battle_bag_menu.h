#ifndef POKEHEARTGOLD_BATTLE_BATTLE_BAG_MENU_H
#define POKEHEARTGOLD_BATTLE_BATTLE_BAG_MENU_H

#include "battle/battle.h"

#include "bag.h"
#include "player_data.h"

// Arguments of the in-battle bag screen (overlay 8), filled by the battle controller before
// BattleBagMenu_Create and read back once `done` is set.
typedef struct BattleBagContext {
    BattleSystem *battleSystem;
    PlayerProfile *profile;
    Bag *bag;
    enum HeapID heapID;
    int battlerId;
    int isTutorial;        // the bag is played back by the tutorial finger
    int embargo;           // the battler is under Embargo (msg 46)
    u16 selectedItem;      // item chosen to use, 0 if the screen was cancelled
    u8 selectedPocket;     // battle pocket of selectedItem
    u8 lastUsedPocket;     // pocket of lastUsedItem
    u16 lastUsedItem;      // "Item used last" button, 0 if none
    u8 ballBlocked[3];     // reasons a Poke Ball cannot be thrown (msgs 44, 47, 48)
    u8 cursorActive;       // d-pad cursor state, kept between openings
    u8 done;               // set when the screen has closed
    u8 pocketPositions[5]; // selected slot on the page, per battle pocket
    u8 pocketPages[5];     // item list page, per battle pocket
} BattleBagContext;

void BattleBagMenu_Create(BattleBagContext *context);

#endif // POKEHEARTGOLD_BATTLE_BATTLE_BAG_MENU_H
