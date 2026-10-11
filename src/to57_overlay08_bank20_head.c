#include "global.h"

#include "battle/battle_cursor.h"

#include "party.h"
#include "sprite_system.h"
#include "unk_0208805C.h"

extern void ov08_022213E4(void *, u8);
extern void ov08_02220A8C(void *, s32, s32);
extern void BattleDpadCursor_SetBattleCursor(void *, BattleCursor *);
extern BattleCursor *BattleDpadCursor_GetBattleCursor(void *);
extern SpriteSystem *BattleSystem_GetSpriteSystem(void *);

int ov08_02221404(u8 *entry);
void ov08_0222145C(u8 *work);
s16 ov08_022214BC(u8 value, int target, int base);
void ov08_02221500(u8 *work);
void ov08_0222162C(u8 *work);
void ov08_02221698(u8 *work);

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

int ov08_02221404(u8 *raw) {
    Ov08IconEntry *entry = (Ov08IconEntry *)raw;
    if (entry->hp == 0) {
        return 0;
    }
    if (entry->status != 7 && entry->status != 6) {
        return 5;
    }
    switch (CalculateHpBarColor(entry->hp, entry->maxHp, 0x30)) {
    case 4:
        return 1;
    case 3:
        return 2;
    case 2:
        return 3;
    case 1:
        return 4;
    default:
        return 0;
    }
}

void ov08_0222145C(u8 *work) {
    u16 i;
    for (i = 0; i < Party_GetCount(PTR(PTR(work, 0), 0)); i++) {
        if (*(u16 *)(work + 8 + i * 0x50) != 0) {
            int anim = ov08_02221404(work + 4 + i * 0x50);
            ManagedSprite **sprite = (ManagedSprite **)(work + i * 4 + 0x1FD4);
            ov08_022213E4(*sprite, (u8)anim);
            ManagedSprite_TickNFrames(*sprite, 0x1000);
        }
    }
}

s16 ov08_022214BC(u8 value, int target, int base) {
    u32 n;
    if (base > target) {
        n = ((u32)((base - target) * (value + 0x2C)) << 16) / 300;
        return base - (s16)(n >> 16);
    }
    n = ((u32)((target - base) * (value + 0x2C)) << 16) / 300;
    return base + (s16)(n >> 16);
}
