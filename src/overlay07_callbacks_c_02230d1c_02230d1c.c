#include "global.h"
#pragma require_prototypes off

typedef struct Obj { u8 x[0x200]; } Obj;
typedef int (*Fn)(Obj *);
extern void ManagedSprite_SetDrawFlag(void *, int);
extern int ov07_022222F0(void *, void *);
extern int ov07_02222748(void *, void *);
extern int BattleAnimSystem_GetBattlerType(void *, int);
extern int ov07_0221BFC0(void *);
extern int BattleAnimSystem_GetCurrentParticleSystem(void *);
extern int ParticleSystem_GetPerspectiveType(void);
extern void ParticleSystem_GetDefaultCameraUp(void *);
extern void BattleAnimUtil_GetEmitterPosition(int,int,int,int,int);
extern void G2x_SetBlendAlpha_(u32,int,int,int,int);
extern void ManagedSprite_GetPositionXY(void *, s16 *, s16 *);
extern void *BattleAnimSystem_GetHeapID(void *);
extern void *Heap_Alloc(int,int);
extern void Heap_Free(void *);
extern void GF_AssertFail(void);
extern int ParticleSystem_GetActiveEmitterCount(void *);
extern void BattleAnimSystem_FreeParticleSystem(void *);
extern void UnkBallData_SetCaptureState(void *, int);
extern int UnkBallData_IsCaptureState(void *, int);
extern int UnkBallData_RunThrowArc(void *);
extern void *BattleAnimSystem_GetDefender(void *); extern void *BattleAnimSystem_GetBattlerPokepic(void*,void*); extern int Pokepic_GetAttr(void*,int);
extern int ov07_02222004(void*,void*); extern int ov07_0222202C(void*,void*); extern void BattleAnimSystem_CreateTaskDefaultPriority(void*,void*,void*);
extern void ManagedSprite_SetOamMode(void*,int); extern void ManagedSprite_SetDrawPriority(void*,int); extern void ManagedSprite_SetPriority(void*,int);
extern int ManagedSprite_GetPaletteOverrideOffset(void*); extern int ov07_0221FAF8(void*,int); extern int Pokepic_IsActive(void*);
extern int BattleAnimSystem_GetSpriteSystem(void*); extern int BattleAnimSystem_GetBattlerSpriteManager(void*); extern int ov07_0221C528(void*); extern int BattleAnimSystem_GetBgConfig(void*); extern int BattleAnimSystem_GetPaletteData(void*);
extern void ParticleSystem_CreateEmitterWithParam(void*,int,void*,void*); extern void ParticleSystem_SetPerspectiveType(void*,int); extern void SysTask_Destroy(void*);
extern void BattleAnimSystem_InitBlend(void*); extern void BallSealEffect_Emit(void*,void*); extern void *SysTask_CreateOnMainQueue(void*,void*,int); extern void BallSealEffect_WaitTask(void*,void*);
extern void ov07_02230BF8(void);
void *BattleAnimUtil_AllocWork(void *, int);

#define WRAP5(n,v) void n(int a,int b,int c,int d){BattleAnimUtil_GetEmitterPosition(a,b,c,d,v);}

#define BIGWRAP(n,fn) void n(void *p,int q,int x){int z=BattleAnimSystem_GetCurrentParticleSystem(p);int d=ParticleSystem_GetPerspectiveType();int a=BattleAnimUtil_GetBattlerType(p,q);int b=ov07_0221BFC0(p); fn(a,x,b,d);}

extern const u32 ov07_0223738C[][2]; extern const u32 ov07_02237390[][2]; extern const u32 ov07_0223746C[][3]; extern const u32 ov07_02237470[][3]; extern const u32 ov07_02237474[][3]; extern const u32 sBallSpriteResources[][4];

#define STEP(n,a,b) int n(u8 *p){int *state=(int*)(p+4);if(*state==0){(*state)++;UnkBallData_SetCaptureState(p,a);}if(UnkBallData_IsCaptureState(p,b)==1)return 0;return 1;}

extern Fn sUnkBallDataBallAnimFuncs[]; extern Fn sUnkBallDataCaptureStateFuncs[];
#define RET1(n) int n(void){return 1;}
#define SETRET(n,v) int n(u8*p){UnkBallData_GoToCaptureState(p,v);return 1;}

void ov07_02230D1C(u8 *p){u8 *q=BattleAnimUtil_AllocWork(p,0x60);void *x;*(void**)q=p;x=BattleAnimSystem_GetDefender(p);*(void**)(q+0x10)=BattleAnimSystem_GetBattlerPokepic(p,x);*(u16*)(q+0x14)=Pokepic_GetAttr(*(void**)(q+0x10),0);*(u16*)(q+0x16)=Pokepic_GetAttr(*(void**)(q+0x10),1);*(int*)(q+8)=ov07_02222004(p,x);*(int*)(q+0xc)=ov07_0222202C(p,x);BattleAnimSystem_CreateTaskDefaultPriority(*(void**)q,ov07_02230BF8,q);}
