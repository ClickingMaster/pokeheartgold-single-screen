#include "to55_bank_w16_private.h"

#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))

void ov07_02220884(void *arg) {
    void *p = ParticleSystem_GetEmitterParam(arg);
    int a = BattleAnimSystem_GetAttacker(p);
    int b = BattleAnimSystem_GetDefender(p);
    ov07_0222050C(p, (int)arg, b, a, 0, 0);
}

void ov07_022208B4(void *a, void *p) {
    if (U32(p, 0x14) != 0) {
        ov07_02220978(a, p);
    }
    if (U32(p, 0x18) != 0) {
        ov07_02220CAC(a, p);
    }
    if (U32(p, 0x1c) != 0) {
        ov07_0222150C(a, p);
    }
    ov07_02221540(a, p);
    if (U32(p, 0xc) != 0) {
        ov07_02221550(a, p);
    }
}

void ov07_022208F8(void *unused, void **pp, int x) {
    void *p = *pp;
    BattleAnimSystem_GetCurrentParticleSystem(p);
    int d = ParticleSystem_GetPerspectiveType();
    int a = BattleAnimSystem_GetDefender(p);
    int n = BattleAnimUtil_GetBattlerSide(p, a) != 3;
    int b = ov07_0221BFC0(p);
    ov07_02231AD0(n, x, b, d);
}

void ov07_02220938(void *unused, void **pp, int x) {
    void *p = *pp;
    BattleAnimSystem_GetCurrentParticleSystem(p);
    int d = ParticleSystem_GetPerspectiveType();
    int a = BattleAnimSystem_GetAttacker(p);
    int n = BattleAnimUtil_GetBattlerSide(p, a) != 3;
    int b = ov07_0221BFC0(p);
    ov07_02231AD0(n, x, b, d);
}
