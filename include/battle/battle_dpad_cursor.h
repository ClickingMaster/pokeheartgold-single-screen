#ifndef POKEHEARTGOLD_BATTLE_BATTLE_DPAD_CURSOR_H
#define POKEHEARTGOLD_BATTLE_BATTLE_DPAD_CURSOR_H

#include "dpad_menu_box.h"

struct BattleCursor;

// Key-driven selection over a table of DpadMenuBox entries, shared by the in-battle party and bag screens
// (overlay 8). The BattleCursor sprite frames the selected box once a key has been pressed.
typedef struct BattleDpadCursor {
    struct BattleCursor *battleCursor;
    const DpadMenuBox *boxes;
    u8 active;       // set by the first key press, cleared again by touch input
    u8 position;     // selected box
    u8 prevPosition; // box a return link leads back to, 0xFF for none
    u32 enabledMask; // one bit per selectable box
} BattleDpadCursor;

BattleDpadCursor *BattleDpadCursor_New(enum HeapID heapID);
void BattleDpadCursor_Free(BattleDpadCursor *cursor);
struct BattleCursor *BattleDpadCursor_GetBattleCursor(const BattleDpadCursor *cursor);
u8 BattleDpadCursor_GetPosition(const BattleDpadCursor *cursor);
u8 BattleDpadCursor_IsActive(const BattleDpadCursor *cursor);
void BattleDpadCursor_SetActive(BattleDpadCursor *cursor, u8 active);
void BattleDpadCursor_SetBattleCursor(BattleDpadCursor *cursor, struct BattleCursor *battleCursor);
void BattleDpadCursor_SetPosition(BattleDpadCursor *cursor, u8 position);
void BattleDpadCursor_ResetPosition(BattleDpadCursor *cursor);
void BattleDpadCursor_SetBoxes(BattleDpadCursor *cursor, const DpadMenuBox *boxes);
void BattleDpadCursor_SetEnabledMask(BattleDpadCursor *cursor, u32 enabledMask);
BOOL BattleDpadCursor_TryActivate(BattleDpadCursor *cursor);
BOOL BattleDpadCursor_HasReturnLink(const DpadMenuBox *box, u32 direction);
int BattleDpadCursor_HandleInput(BattleDpadCursor *cursor);

#endif // POKEHEARTGOLD_BATTLE_BATTLE_DPAD_CURSOR_H
