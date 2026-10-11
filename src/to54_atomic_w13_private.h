#ifndef TO54_W13_PRIVATE_H
#define TO54_W13_PRIVATE_H

#include "global.h"

#include "camera.h"
#include "spl.h"

typedef struct Ov07Base {
    u32 unk00;
    VecFx32 position;
} Ov07Base;

typedef struct Ov07CallbackPosition {
    u8 pad00[0x20];
    Ov07Base **base;
    u32 unk24;
    VecFx32 position;
} Ov07CallbackPosition;

void *ParticleSystem_GetEmitterParam(void);
void ParticleSystem_SetCameraUp(void *, const VecFx32 *);
int ParticleSystem_GetPerspectiveType(void *);
Camera *ParticleSystem_GetCamera(SPLEmitter *);
void ParticleEmitter_SetConvergencePos(SPLEmitter *, VecFx32 *);
void ParticleEmitter_GetConvergencePos(SPLEmitter *, VecFx32 *);
void ParticleEmitter_SetMagnetPos(SPLEmitter *, VecFx32 *);
void ParticleEmitter_GetMagnetPos(SPLEmitter *, VecFx32 *);

int BattleAnimSystem_GetAttacker(void *);
int BattleAnimSystem_GetDefender(void *);
SPLEmitter *BattleAnimSystem_GetCurrentParticleSystem(void *);
void *ov07_0221BFC0(void *);
int BattleAnimUtil_GetBattlerType(void *, int);
int BattleAnimUtil_GetBattlerSide(void *, int);
int BattleAnimUtil_GetOpposingBattlerType(int);
void ov07_02231AD0(int, VecFx32 *, void *, int);
void ov07_02231B90(void *, int, VecFx32 *);
void BattleAnimUtil_GetCameraUp(int, VecFx32 *);
void BattleAnimSystem_ReadScriptArgs(void *, void *, int);
int BattleAnimSystem_GetSideSign(void *, int, int);

void ov07_0222036C(void *, SPLEmitter *, int, int, int, s8, VecFx32 *);
void ov07_0222043C(void *, SPLEmitter *, int, int, int, s8, VecFx32 *);
void ov07_0222050C(void *, void *, int, int, int, int);
void ov07_02220030(Ov07CallbackPosition *);
void ov07_0222008C(Ov07CallbackPosition *);
void ov07_022200D0(Ov07CallbackPosition *);
void ov07_02220114(Ov07CallbackPosition *);
void ov07_02220160(Ov07CallbackPosition *);
void ov07_022201C8(Ov07CallbackPosition *);
void ov07_022202D8(int, int, VecFx32 *);
void ov07_022207D0(void *, int, int);
void ov07_0221FF78(Ov07CallbackPosition *);

#endif
