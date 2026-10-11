#include "global.h"

#include "constants/heap.h"

#include "battle/battle_finger.h"
#include "battle/battle_system.h"

#include "sprite_system.h"
extern const s32 sBattleBagTutorialFingerPos[];
extern const s32 ov08_02225D30[];
void BattleBagMenu_CreateFinger(u8 *);
void BattleBagMenu_FreeFinger(u8 *);
void BattleBagMenu_UpdateTutorialFinger(u8 *, int);
void BattleBagMenu_CopyTilemapRect(u16 *, const u16 *, u16, u16, u8, u8);

typedef struct Ov08StateTail {
    u8 pad[0x113e];
    u8 a, b, c;
    u8 low : 4;
    u8 high : 4;
} Ov08StateTail;
void BattleBagMenu_StartButtonAnim(Ov08StateTail *, u8, u8);

void BattleBagMenu_StartButtonAnim(Ov08StateTail *work, u8 a1, u8 a2) {
    work->a = 0;
    work->b = 0;
    work->c = a1;
    work->high = a2;
    work->low = 1;
}
