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

void BattleBagMenu_CreateFinger(u8 *work) {
    SpriteSystem *sys = BattleSystem_GetSpriteSystem(*(BattleSystem **)(*(u8 **)work));
    BattleFinger_LoadResources(sys, *(SpriteManager **)(work + 0x30c), *(enum HeapID *)(*(u8 **)work + 0xc), *(PaletteData **)(work + 8), 0xB4BD, 0xB4BD, 0xB4BD - 5, 0xB4BD - 5);
    {
        u32 res = 0xB4BD;
        *(BattleFinger **)(work + 0x38) = BattleFinger_New(sys, *(SpriteManager **)(work + 0x30c), *(enum HeapID *)(*(u8 **)work + 0xc), res, res, res - 5, res - 5, 0, 0);
    }
}
void BattleBagMenu_FreeFinger(u8 *work) {
    BattleFinger_Delete(*(BattleFinger **)(work + 0x38));
    BattleFinger_FreeResources(*(SpriteManager **)(work + 0x30c), 0xB4BD, 0xB4BD, 0xB4B8, 0xB4B8);
}
void BattleBagMenu_UpdateTutorialFinger(u8 *work, int i) {
    if (*(u32 *)(*(u8 **)work + 0x14) == 1) {
        ov12_0226BD2C(*(BattleFinger **)(work + 0x38), sBattleBagTutorialFingerPos[i * 2], ov08_02225D30[i * 2]);
        ov12_0226BD4C(*(BattleFinger **)(work + 0x38), 60);
    } else {
        BattleFinger_Disable(*(BattleFinger **)(work + 0x38));
    }
}
