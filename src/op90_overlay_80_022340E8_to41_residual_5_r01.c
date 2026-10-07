#pragma force_active on
#include "global.h"
#include "party.h"
#include "sprite.h"

typedef struct Ov80MonSprite {
    Sprite *sprite;
} Ov80MonSprite;

typedef struct Ov80Pos {
    s16 x;
    s16 y;
} Ov80Pos;

typedef struct Ov80Work {
    u8 pad0[0x10];
    u8 type;
    u8 pad11[0x30 - 0x11];
    Ov80MonSprite *sprites[4];
    Ov80MonSprite *oppSprites[4];
    u8 pad50[0x70 - 0x50];
    Party *party;
    Party *oppParty;
} Ov80Work;

extern const Ov80Pos ov80_0223BEB8[];
extern const Ov80Pos ov80_0223BEC8[];
u8 BattleArcade_GetMonCount(u8 type, int mode);
u8 BattleArcade_GetOpponentMonCount(u8 type, int mode);
Ov80MonSprite *ov80_0222F29C(void *system, Pokemon *mon, int index, int x, int y);
void ov80_02234A74(Ov80Work *work, void *system, int opponent);

// @fn ov80_02234A74
void ov80_02234A74(Ov80Work *work, void *system, int opponent) {
    int i;
    u8 count = BattleArcade_GetMonCount(work->type, 1);
    u8 oppCount = BattleArcade_GetOpponentMonCount(work->type, 1);

    if (opponent == 0) {
        for (i = 0; i < count; i++) {
            work->sprites[i] = ov80_0222F29C(system, Party_GetMonByIndex(work->party, i), i, ov80_0223BEB8[i].x, ov80_0223BEB8[i].y);
            Sprite_SetAnimActiveFlag(work->sprites[i]->sprite, FALSE);
        }
    } else {
        for (i = 0; i < oppCount; i++) {
            work->oppSprites[i] = ov80_0222F29C(system, Party_GetMonByIndex(work->oppParty, i), count + i, ov80_0223BEC8[i].x, ov80_0223BEC8[i].y);
            Sprite_SetAnimActiveFlag(work->oppSprites[i]->sprite, FALSE);
        }
    }
}
