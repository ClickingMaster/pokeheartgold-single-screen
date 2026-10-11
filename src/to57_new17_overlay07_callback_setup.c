#include "global.h"

#include "pokepic.h"
#include "sprite_system.h"
#define F(p, t, o) (*(t *)((u8 *)(p) + (o)))
typedef struct Work Work;
void *BattleAnimUtil_AllocWork(void *, u32);
void BattleAnimUtil_InitCommonWork(void *, void *);

extern u32 BattleAnimSystem_GetArg(void *p, int i);
int BattleAnimSystem_GetAttacker(void *);
int BattleAnimSystem_GetDefender(void *);
int ov07_02222004(void *, int);
Pokepic *BattleAnimSystem_GetBattlerPokepic(void *, int);
void BattleAnimUtil_GetPicPosition(void *, void *);
void BattleAnimUtil_InitLinearSinMotion(void *, void *, int, int, int, int, int, int);
int BattleAnimSystem_GetBattlerType(void *, int);
void BattleAnimUtil_InitValueMotion(void *, int, int, int);
BOOL BattleAnimUtil_UpdateLinearSinMotionSprite(void *, void *, ManagedSprite *);
void BattleAnimSystem_CreateTaskDefaultPriority(void *, void (*)(int, void *), void *);
void ov07_0222928C(int, void *);
void ov07_022292E4(void *, int, int, ManagedSprite *);
void ov07_022292E4(void *arg0, int unused1, int unused2, ManagedSprite *arg3) {
    int var_r5;
    void *w;
    w = BattleAnimUtil_AllocWork(arg0, 0xE8);
    BattleAnimUtil_InitCommonWork(arg0, (u8 *)w + 0x10);
    F(w, s16, 0xC) = ((s32)BattleAnimSystem_GetArg(arg0, 0));
    F(w, s16, 0xE) = ((s32)BattleAnimSystem_GetArg(arg0, 1));
    F(w, int, 4) = ((s32)BattleAnimSystem_GetArg(arg0, 2));
    F(w, int, 8) = ((s32)BattleAnimSystem_GetArg(arg0, 3));
    var_r5 = ov07_02222004(arg0, BattleAnimSystem_GetAttacker(arg0));
    F(w, ManagedSprite *, 0x38) = arg3;
    ManagedSprite_SetAffineOverwriteMode(arg3, 2);
    F(w, Pokepic *, 0x44) = BattleAnimSystem_GetBattlerPokepic(arg0, BattleAnimSystem_GetAttacker(arg0));
    F(w, Pokepic *, 0x58) = BattleAnimSystem_GetBattlerPokepic(arg0, BattleAnimSystem_GetDefender(arg0));
    BattleAnimUtil_GetPicPosition(F(w, void *, 0x44), (u8 *)w + 0x40);
    BattleAnimUtil_GetPicPosition(F(w, void *, 0x58), (u8 *)w + 0x54);
    BattleAnimUtil_InitLinearSinMotion((u8 *)w + 0x8C, (u8 *)w + 0xB0, F(w, s16, 0x40), (s16)(F(w, s16, 0x54) + F(w, s16, 0xC) * var_r5), F(w, s16, 0x42), (s16)(F(w, s16, 0x56) + F(w, s16, 0xE) * var_r5), (u16)F(w, int, 4), F(w, int, 8) << 12);
    if (BattleAnimSystem_GetBattlerType(arg0, BattleAnimSystem_GetAttacker(arg0)) == 4 && BattleAnimSystem_GetBattlerType(arg0, BattleAnimSystem_GetDefender(arg0)) == 2) {
        var_r5 *= -1;
    }
    if (BattleAnimSystem_GetBattlerType(arg0, BattleAnimSystem_GetAttacker(arg0)) == 5 && BattleAnimSystem_GetBattlerType(arg0, BattleAnimSystem_GetDefender(arg0)) == 3) {
        var_r5 *= -1;
    }
    if (var_r5 > 0) {
        BattleAnimUtil_InitValueMotion((u8 *)w + 0xD4, 0xE38 * var_r5, 0x5C71 * var_r5, 10);
    } else {
        BattleAnimUtil_InitValueMotion((u8 *)w + 0xD4, 0x3FFF * var_r5, 0x5C71 * var_r5, 10);
    }
    ManagedSprite_SetAffineZRotation(F(w, ManagedSprite *, 0x38), (u16)F(w, int, 0xD4));
    BattleAnimUtil_UpdateLinearSinMotionSprite((u8 *)w + 0x8C, (u8 *)w + 0xB0, F(w, ManagedSprite *, 0x38));
    ManagedSprite_TickFrame(F(w, ManagedSprite *, 0x38));
    BattleAnimSystem_CreateTaskDefaultPriority(F(w, void *, 0x14), ov07_0222928C, w);
}
