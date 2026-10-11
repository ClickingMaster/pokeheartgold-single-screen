#include "global.h"
#pragma require_prototypes off

typedef struct Obj { u8 x[0x200]; } Obj;
typedef int (*Fn)(Obj *);
extern void ManagedSprite_SetDrawFlag(void *, int);
extern int ov07_022222F0(void *, void *);
extern int ov07_02222748(void *, void *);
extern int ov07_0221FA04(void *, int);
extern int ov07_0221BFC0(void *);
extern int ov07_0221C478(void *);
extern int ParticleSystem_GetPerspectiveType(void);
extern void ParticleSystem_GetDefaultCameraUp(void *);
extern void BattleAnimUtil_GetEmitterPosition(int,int,int,int,int);
extern void G2x_SetBlendAlpha_(u32,int,int,int,int);
extern void ManagedSprite_GetPositionXY(void *, s16 *, s16 *);
extern void *ov07_0221BFD0(void *);
extern void *Heap_Alloc(int,int);
extern void Heap_Free(void *);
extern void GF_AssertFail(void);
extern int ParticleSystem_GetActiveEmitterCount(void *);
extern void ov07_0221FF2C(void *);
extern void ov07_02233EB8(void *, int);
extern int ov07_02233EBC(void *, int);
extern int ov07_022335B4(void *);
extern void *ov07_0221C470(void *); extern void *ov07_0221FA48(void*,void*); extern int Pokepic_GetAttr(void*,int);
extern int ov07_02222004(void*,void*); extern int ov07_0222202C(void*,void*); extern void ov07_0221C410(void*,void*,void*);
extern void ManagedSprite_SetOamMode(void*,int); extern void ManagedSprite_SetDrawPriority(void*,int); extern void ManagedSprite_SetPriority(void*,int);
extern int ManagedSprite_GetPaletteOverrideOffset(void*); extern int ov07_0221FAF8(void*,int); extern int Pokepic_IsActive(void*);
extern int ov07_0221C530(void*); extern int ov07_0221C514(void*); extern int ov07_0221C528(void*); extern int ov07_0221C4A0(void*); extern int ov07_0221FA78(void*);
extern void ParticleSystem_CreateEmitterWithParam(void*,int,void*,void*); extern void ParticleSystem_SetPerspectiveType(void*,int); extern void SysTask_Destroy(void*);
extern void ov07_0221C69C(void*); extern void ov07_02232AFC(void*,void*); extern void *SysTask_CreateOnMainQueue(void*,void*,int); extern void ov07_02232A5C(void*,void*);
extern void ov07_02230BF8(void);
void *BattleAnimUtil_AllocWork(void *, int);

#define WRAP5(n,v) void n(int a,int b,int c,int d){BattleAnimUtil_GetEmitterPosition(a,b,c,d,v);}

#define BIGWRAP(n,fn) void n(void *p,int q,int x){int z=ov07_0221C478(p);int d=ParticleSystem_GetPerspectiveType();int a=BattleAnimUtil_GetBattlerType(p,q);int b=ov07_0221BFC0(p); fn(a,x,b,d);}

extern const u32 ov07_0223738C[][2]; extern const u32 ov07_02237390[][2]; extern const u32 ov07_0223746C[][3]; extern const u32 ov07_02237470[][3]; extern const u32 ov07_02237474[][3]; extern const u32 ov07_022375BC[][4];

#define STEP(n,a,b) int n(u8 *p){int *state=(int*)(p+4);if(*state==0){(*state)++;ov07_02233EB8(p,a);}if(ov07_02233EBC(p,b)==1)return 0;return 1;}

extern Fn ov07_022371B8[]; extern Fn ov07_0223729C[];
#define RET1(n) int n(void){return 1;}
#define SETRET(n,v) int n(u8*p){ov07_02232F74(p,v);return 1;}

u32 ov07_022325F4(int x){return ov07_0223738C[ov07_0223251C(x)][0];}
u32 ov07_02232608(int x){return ov07_02237390[ov07_0223251C(x)][0];}
u32 ov07_0223261C(int x){return ov07_0223746C[ov07_0223251C(x)][0];}
u32 ov07_02232630(int x){return ov07_02237474[ov07_0223251C(x)][0];}
u32 ov07_02232644(int x){return ov07_02237470[ov07_0223251C(x)][0];}
u32 ov07_02232658(int x,int y){return ov07_022375BC[ov07_0223251C(x)][y];}
