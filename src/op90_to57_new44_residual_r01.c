#pragma force_active on
#include "global.h"
#include "sprite_system.h"
#include "pokepic.h"
typedef struct {
    void *owner;
    int a;
    int b;
    int unk0C;
    Pokepic *pic;
    s16 x;
    s16 y;
    u8 movement[0x24];
    int height;
    Pokepic *pic2;
    s16 x2;
    s16 y2;
    ManagedSprite *sprite;
    u8 interp[0x24];
    u8 interp2[0x24];
} State;
extern State *BattleAnimUtil_AllocWork(void *owner, int size);
extern void *BattleAnimSystem_GetAttacker(void *owner);
extern void *BattleAnimSystem_GetDefender(void *owner);
extern int ov07_02222004(void *owner, void *battler);
extern Pokepic *BattleAnimSystem_GetBattlerPokepic(void *owner, void *battler);
extern void ov07_02222268(void *motion, s16 x0, s16 x1, s16 y0, s16 y1, int frames);
extern void ov07_02222338(void *a, void *b, s16 x0, s16 x1, s16 y0, s16 y1, int frames, int scale);
extern void BattleAnimSystem_CreateTaskDefaultPriority(void *owner, void *func, void *data);
extern void ov07_0222D7A4(void *, void *);
void ov07_0222D974(void *owner, int a, int b, ManagedSprite *sprite);

// @fn ov07_0222D974
void ov07_0222D974(void *owner, int a, int b, ManagedSprite *sprite) {
    State *s = BattleAnimUtil_AllocWork(owner, sizeof(State));
    s->owner = owner;
    s->a = a;
    s->b = b;
    s->height = ov07_02222004(s->owner, BattleAnimSystem_GetAttacker(s->owner));
    s->pic = BattleAnimSystem_GetBattlerPokepic(s->owner, BattleAnimSystem_GetAttacker(owner));
    s->x = Pokepic_GetAttr(s->pic, 0);
    s->y = Pokepic_GetAttr(s->pic, 1);
    ov07_02222268(s->movement, 0, s->height * -16, 0, 0, 4);
    s->pic2 = BattleAnimSystem_GetBattlerPokepic(s->owner, BattleAnimSystem_GetDefender(owner));
    s->x2 = Pokepic_GetAttr(s->pic2, 0);
    s->y2 = Pokepic_GetAttr(s->pic2, 1);
    s->sprite = sprite;
    ManagedSprite_SetAnimateFlag(s->sprite, TRUE);
    ManagedSprite_SetDrawPriority(s->sprite, 100);
    ManagedSprite_SetPriority(s->sprite, 1);
    ov07_02222338(s->interp, s->interp2, s->x, s->x2, s->y, s->y2, 10, 2 << 16);
    BattleAnimSystem_CreateTaskDefaultPriority(s->owner, ov07_0222D7A4, s);
}
