#ifndef W03_PRIVATE_H
#define W03_PRIVATE_H
#include "global.h"
void *Heap_Alloc(u32, u32);
void Heap_Free(void *);
void GF_AssertFail(void);
void SysTask_Destroy(void *);
void *SysTask_CreateOnMainQueue(void *, void *, u32);
void ParticleSystem_CreateEmitterWithParam(void *, int, void *, void *);
void ParticleSystem_SetPerspectiveType(void *, int);
void *ParticleSystem_GetEmitterParam(void *);
int ov07_02232608(void *);
int ov07_0223261C(int);
int ov07_02232630(int);
int ov07_02232644(int);
int ov07_02234530(void *);
void ov07_02231DE0(int, void *, int, int);
int SealOnCapsuleGetID(void *);
int SealOnCapsuleGetX(void *);
int SealOnCapsuleGetY(void *);
void sub_0209109C(int);
void *BattleAnimSystem_NewParticleSystemFromNarc(int, int, int, int);
void ov07_02232AC0(void *, u8 *);
void ov07_02232C64(void *);
void ov07_02232CD8(void *);
void ov07_02232AFC(u8 *, void *);
void ov07_02232BB0(int, s32 *);
void ov07_02232C28(u8 *);
void ov07_02232D80(u8 *);
#endif
