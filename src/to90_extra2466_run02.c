#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
#define W(p,o) (*(u32 *)((u8 *)(p)+(o)))
#define H(p,o) (*(u16 *)((u8 *)(p)+(o)))
#define B(p,o) (*(u8 *)((u8 *)(p)+(o)))
static inline u32 container(u8 *p) { if(p && W(p,8))return (u32)p+W(p,8);return 0; }
typedef struct{u8 pad;u8 count;u16 a,b,offset;}Dict;typedef struct{u8 pad[4];Dict dict;}Container;
static inline u8 *entry_t(Dict*d,u32 i){if(d && i<d->count){u16 *h=(u16*)((u8*)d+d->offset);return (u8*)(h+2)+h[0]*i;}return 0;}
static inline u8*material_t(Container*c,u32 i){if(c){u8*e=entry_t(&c->dict,i);if(e)return (u8*)c+*(u32*)e;}return 0;}

void NNS_G3dMdlSetMdlDiff(u8 *p,u32 i,u32 v);
void NNS_G3dMdlSetMdlAmb(u8 *p,u32 i,u32 v);
void NNS_G3dMdlSetMdlSpec(u8 *p,u32 i,u32 v);
void NNS_G3dMdlSetMdlEmi(u8 *p,u32 i,u32 v);
void NNS_G3dMdlSetMdlLightEnableFlag(u8 *p,u32 i,u32 v);
void NNS_G3dMdlSetMdlPolygonMode(u8 *p,u32 i,u32 v);
void NNS_G3dMdlSetMdlCullMode(u8 *p,u32 i,u32 v);
void NNS_G3dMdlSetMdlPolygonID(u8 *p,u32 i,u32 v);
void NNS_G3dMdlSetMdlAlpha(u8 *p,u32 i,u32 v);
void NNS_G3dMdlSetMdlFogEnableFlag(u8 *p,u32 i,int v);
void NNS_G3dMdlSetMdlDepthTestCond(u8 *p,u32 i,int v);
void NNS_G3dMdlSetMdl1Dot(u8 *p,u32 i,int v);
void NNS_G3dMdlSetMdlFarClip(u8 *p,u32 i,int v);
void NNS_G3dMdlSetMdlXLDepthUpdate(u8 *p,u32 i,int v);
u32 NNS_G3dMdlGetMdlLightEnableFlag(u8 *p,u32 i);
u32 NNS_G3dMdlGetMdlFogEnableFlag(u8 *p,u32 i);
#pragma force_active on
#pragma opt_lifetimes off
#pragma opt_common_subs off
void NNSi_G3dModifyPolygonAttrMask(NNSG3dResMdl *model,BOOL v,u32 mask){u8 *p=(u8*)model;u32 n=B(p,24),i;Container*c=(Container*)container(p);for(i=0;i<n;i++){u8*q=material_t(c,i);u32 x=W(q,16);if(v)x|=mask;else x&=~mask;W(q,16)=x;}}
