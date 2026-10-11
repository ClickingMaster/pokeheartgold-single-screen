#include "global.h"

#include "battle/battle_cursor.h"

#include "party.h"
#include "sprite_system.h"
#include "unk_0208805C.h"

extern void BattlePartyMenu_SetSpriteAnim(void *, u8);
extern void BattlePartyMenu_ShowSpriteAt(void *, s32, s32);
extern void BattleDpadCursor_SetBattleCursor(void *, BattleCursor *);
extern BattleCursor *BattleDpadCursor_GetBattleCursor(void *);
extern SpriteSystem *BattleSystem_GetSpriteSystem(void *);

int BattlePartyMenu_GetMonIconAnim(u8 *entry);
void BattlePartyMenu_UpdateMonIcons(u8 *work);
s16 BattlePartyMenu_CalcConditionMarkerCoord(u8 value, int target, int base);
void BattlePartyMenu_ShowConditionMarkers(u8 *work);
void BattlePartyMenu_CreateCursor(u8 *work);
void BattlePartyMenu_FreeCursor(u8 *work);

static inline void *PTR(void *p, u32 off) {
    return *(void **)((u8 *)p + off);
}
static inline u16 U16(void *p, u32 off) {
    return *(u16 *)((u8 *)p + off);
}
static inline u8 U8(void *p, u32 off) {
    return *((u8 *)p + off);
}

typedef struct Ov08Root {
    Party *party;
    u8 pad_04[0xD];
    u8 slot;
} Ov08Root;

typedef struct Ov08PartyEntry {
    u8 pad_00[4];
    u16 active;
    u8 pad_06[0xA];
    u16 hp;
    u16 maxHp;
    u8 pad_14[3];
    u8 low : 3;
    u8 status : 4;
    u8 high : 1;
    u8 pad_18[0x10];
    u8 values[5];
    u8 pad_2D[0x23];
} Ov08PartyEntry;

typedef struct Ov08Work {
    Ov08Root *root;
    Ov08PartyEntry entries[6];
    u8 pad_01E4[0x1DF0];
    ManagedSprite *partySprites[6];
    u8 pad_01FEC[0x38];
    ManagedSprite *positionSprites[5];
} Ov08Work;

typedef struct Ov08IconEntry {
    u8 pad_00[0x10];
    u16 hp;
    u16 maxHp;
    u8 pad_14[3];
    u8 low : 3;
    u8 status : 4;
    u8 high : 1;
} Ov08IconEntry;

void BattlePartyMenu_CreateCursor(u8 *work) {
    SpriteSystem *spriteSystem = BattleSystem_GetSpriteSystem(PTR(PTR(work, 0), 8));
    BattleCursor *cursor;
    BattleCursor_LoadResources(spriteSystem, PTR(work, 0x1FB4), PTR(work, 0x1E8), (enum HeapID) * (u32 *)((u8 *)PTR(work, 0) + 0xC), 0xB018, 0xB00C, 0xB00C, 0xB00C);
    cursor = BattleCursor_New(spriteSystem, PTR(work, 0x1FB4), (enum HeapID) * (u32 *)((u8 *)PTR(work, 0) + 0xC), 0xB018, 0xB00C, 0xB00C, 0xB00C, 0, 1);
    BattleDpadCursor_SetBattleCursor(PTR(work, 0x2088), cursor);
}

void BattlePartyMenu_FreeCursor(u8 *work) {
    BattleCursor_Delete(BattleDpadCursor_GetBattleCursor(PTR(work, 0x2088)));
    BattleCursor_FreeResources(PTR(work, 0x1FB4), 0xB018, 0xB00C, 0xB00C, 0xB00C);
}
