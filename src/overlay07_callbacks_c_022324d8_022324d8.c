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
extern int sub_02015530(void);
extern void sub_02015510(void *);
extern void ov07_02231A70(int,int,int,int,int);
extern void G2x_SetBlendAlpha_(u32,int,int,int,int);
extern void ManagedSprite_GetPositionXY(void *, s16 *, s16 *);
extern void *ov07_0221BFD0(void *);
extern void *Heap_Alloc(int,int);
extern void Heap_Free(void *);
extern void GF_AssertFail(void);
extern int sub_020154B0(void *);
extern void ov07_0221FF2C(void *);
extern void ov07_02233EB8(void *, int);
extern int ov07_02233EBC(void *, int);
extern int ov07_022335B4(void *);
extern void *ov07_0221C470(void *); extern void *ov07_0221FA48(void*,void*); extern int Pokepic_GetAttr(void*,int);
extern int ov07_02222004(void*,void*); extern int ov07_0222202C(void*,void*); extern void ov07_0221C410(void*,void*,void*);
extern void ManagedSprite_SetOamMode(void*,int); extern void ManagedSprite_SetDrawPriority(void*,int); extern void ManagedSprite_SetPriority(void*,int);
extern int ManagedSprite_GetPaletteOverrideOffset(void*); extern int ov07_0221FAF8(void*,int); extern int Pokepic_IsActive(void*);
extern int ov07_0221C530(void*); extern int ov07_0221C514(void*); extern int ov07_0221C528(void*); extern int ov07_0221C4A0(void*); extern int ov07_0221FA78(void*);
extern void sub_02015494(void*,int,void*,void*); extern void sub_02015528(void*,int); extern void SysTask_Destroy(void*);
extern void ov07_0221C69C(void*); extern void ov07_02232AFC(void*,void*); extern void *SysTask_CreateOnMainQueue(void*,void*,int); extern void ov07_02232A5C(void*,void*);
extern void ov07_02230BF8(void);
void *ov07_022324D8(void *, int);

#define WRAP5(n,v) void n(int a,int b,int c,int d){ov07_02231A70(a,b,c,d,v);}

#define BIGWRAP(n,fn) void n(void *p,int q,int x){int z=ov07_0221C478(p);int d=sub_02015530();int a=ov07_02231924(p,q);int b=ov07_0221BFC0(p); fn(a,x,b,d);}

extern const u32 ov07_0223738C[][2]; extern const u32 ov07_02237390[][2]; extern const u32 ov07_0223746C[][3]; extern const u32 ov07_02237470[][3]; extern const u32 ov07_02237474[][3]; extern const u32 ov07_022375BC[][4];

#define STEP(n,a,b) int n(u8 *p){int *state=(int*)(p+4);if(*state==0){(*state)++;ov07_02233EB8(p,a);}if(ov07_02233EBC(p,b)==1)return 0;return 1;}

extern Fn ov07_022371B8[]; extern Fn ov07_0223729C[];
#define RET1(n) int n(void){return 1;}
#define SETRET(n,v) int n(u8*p){ov07_02232F74(p,v);return 1;}

void *ov07_022324D8(void *p,int sz){void *q;if(!p)GF_AssertFail();q=Heap_Alloc((int)ov07_0221BFD0(p),sz);if(!q)GF_AssertFail();memset(q,0,sz);return q;}
