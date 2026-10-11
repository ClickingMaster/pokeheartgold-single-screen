#ifndef POKEHEARTGOLD_MOVE_RELEARNER_H
#define POKEHEARTGOLD_MOVE_RELEARNER_H

#include "menu_input_state.h"
#include "player_data.h"
#include "pokemon_types_def.h"

#define MOVE_RELEARNER_TUTOR   0
#define MOVE_RELEARNER_RELEARN 1

typedef struct MoveRelearnerArgs {
    Pokemon *mon;
    PlayerProfile *profile;
    Options *options;
    MenuInputStateMgr *menuInputStatePtr;
    u16 *eligibleMoves;
    u16 cursorPos;      // 0x14, selected row of the visible move list
    u16 scrollOffset;   // 0x16, index of the first visible move
    u8 showContestType; // 0x18, 0: the type icons show move types, else contest types
    u8 type;            // 0x19, MOVE_RELEARNER_TUTOR or MOVE_RELEARNER_RELEARN
    u8 cancelled;       // 0x1A, set when the player quits without learning a move
    u8 moveSlot;        // 0x1B, slot of the move to replace (0-3)
} MoveRelearnerArgs;

MoveRelearnerArgs *MoveRelearner_New(enum HeapID heapID);
void MoveRelearner_Delete(MoveRelearnerArgs *moveRelearner);
u16 *MoveRelearner_GetEligibleLevelUpMoves(Pokemon *mon, enum HeapID heapID);
BOOL MoveRelearner_IsValidMove(const u16 *ptr);

#endif // POKEHEARTGOLD_MOVE_RELEARNER_H
