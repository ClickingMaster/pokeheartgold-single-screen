#ifndef TO54_W16_PRIVATE_H
#define TO54_W16_PRIVATE_H
#include "global.h"

void *ParticleSystem_GetEmitterParam(void *);
int ParticleSystem_GetPerspectiveType(void);
int BattleAnimSystem_GetAttacker(void *);
int BattleAnimSystem_GetDefender(void *);
int BattleAnimSystem_GetCurrentParticleSystem(void *);
int ov07_0221BFC0(void *);
int BattleAnimSystem_GetHeapID(void);
void ov07_0222050C(void *, int, int, int, int, int);
int BattleAnimUtil_GetBattlerSide(void *, int);
int BattleAnimUtil_GetBattlerType(void *, int);
void ov07_02231AD0(int, int, int, int);
int ov07_02221CB4(void);
int ov07_02221C8C(int);
void ov07_02221C74(int, void *, void *);
void ov07_02221EEC(void *, void *);
void BattleAnimSystem_ReadScriptArgs(void *, void *, int);
void *Heap_Alloc(int, u32);
void Heap_Free(void *);
void ov07_022208B4(void *, void *);
int ov07_02221664(void *);
void ov07_02220884(void *);
void ov07_02220978(void *, void *);
void ov07_02220CAC(void *, void *);
void ov07_0222150C(void *, void *);
void ov07_02221540(void *, void *);
void ov07_02221550(void *, void *);
void ov07_022215B4(void *);
void ov07_022208F8(void *, void **, int);
void ov07_02220938(void *, void **, int);
struct Vec3;
void ov07_022216A8(void *, int, struct Vec3 *);
void ov07_02221734(void *, struct Vec3 *);

#endif
