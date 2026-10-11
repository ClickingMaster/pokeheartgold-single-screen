#include "global.h"
#include "pokepic.h"
#include "sprite_system.h"

typedef struct W28070 {
    u8 state;
    u8 pad1;
    u8 substate;
    u8 pad3;
    void *owner;
    Pokepic *pokepic;
} W28070;

typedef struct W28174 {
    u8 state;
    u8 substate;
    s16 y;
    s16 x;
    u16 pad6;
    void *owner;
    Pokepic *pokepic;
} W28174;

typedef struct W28384 {
    u8 state;
    u8 substate;
    s16 y;
    s16 baseY;
    s16 x;
    void *owner;
    Pokepic *pokepic;
} W28384;

typedef struct W284D8 {
    u32 unk0;
    void *palette;
    void *owner;
    u32 unkC;
    Pokepic *pokepic;
} W284D8;

typedef struct W29220 {
    u8 pad0[4];
    void *owner;
    u8 pad8[0x20];
    u32 pos0;
    u32 pos1;
    Pokepic *pic0;
    Pokepic *pic1;
    ManagedSprite *sprite0;
    ManagedSprite *sprite1;
} W29220;

void *BattleAnimUtil_AllocWork(void *owner, u32 size);
u16 BattleAnimSystem_GetAttacker(void *owner);
Pokepic *BattleAnimSystem_GetBattlerPokepic(void *owner, u16 index);
void *BattleAnimSystem_GetPaletteData(void *owner);
u16 ov07_0221FAA0(void *owner, u16 index);
void BattleAnimSystem_CreateTaskDefaultPriority(void *owner, void (*callback)(void), void *work);
void ov07_02227FA4(void);
void ov07_022280A8(void);
void ov07_022281C0(void);
void ov07_022283D8(void);
void ov07_02228510(void);
void ov07_0222915C(void);
void BattleAnimUtil_InitCommonWork(void *owner, void *work);
ManagedSprite *BattleAnimSystem_GetBattlerSprite(void *owner, int index);
u16 BattleAnimSystem_GetDefender(void *owner);
void BattleAnimUtil_GetPicPosition(void *object, void *position);
void ov07_02228070(void *owner);
void ov07_02228174(void *owner);
void ov07_02228384(void *owner);
void ov07_022284D8(void *owner);
void ov07_02228638(void *owner);
void ov07_02229220(void *owner);
void ov07_0222A8D8(ManagedSprite *sprite);

void ov07_02228638(void *owner) {
    W28384 *work = BattleAnimUtil_AllocWork(owner, 0x58);
    work->state = 0;
    work->substate = 0;
    work->owner = owner;
    work->pokepic = BattleAnimSystem_GetBattlerPokepic(work->owner, BattleAnimSystem_GetAttacker(work->owner));
    work->baseY = Pokepic_GetAttr(work->pokepic, 1);
    work->x = ov07_0221FAA0(work->owner, BattleAnimSystem_GetAttacker(work->owner));
    BattleAnimSystem_CreateTaskDefaultPriority(work->owner, ov07_02228510, work);
}
