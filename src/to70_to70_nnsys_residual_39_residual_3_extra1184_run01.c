#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
#define W(p,o) (*(u32 *)((u8 *)(p)+(o)))
#define H(p,o) (*(u16 *)((u8 *)(p)+(o)))
#define B(p,o) (*(u8 *)((u8 *)(p)+(o)))
static inline u8 *container(u8 *p) { if(p && W(p,8))return p+W(p,8);return 0; }
static inline u8 *entry(u32 i,u8 *d) { if(d && i<B(d,1)){u32 o=H(d,6);u32 z=*(u16 *)((u32)d+o);u8 *e=(u8 *)((u32)d+o+4);return (u8 *)((u32)e+z*i);}return 0; }
static inline u8 *material(u8 *p,u32 i) {if(p){i=(u32)entry(i,p+4);if(i)return p+W((u8*)i,0);}return 0;}

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
void NNS_G3dMdlSetMdlDiff(u8 *p,u32 i,u32 v){u8 *q=material(container(p),i);W(q,4)=(W(q,4)&~0x7fff)|v;}
void NNS_G3dMdlSetMdlAmb(u8 *p,u32 i,u32 v){u8 *q=material(container(p),i);W(q,4)=(W(q,4)&0x8000ffff)|(v<<16);}
void NNS_G3dMdlSetMdlSpec(u8 *p,u32 i,u32 v){u8 *q=material(container(p),i);W(q,8)=(W(q,8)&~0x7fff)|v;}
void NNS_G3dMdlSetMdlEmi(u8 *p,u32 i,u32 v){u8 *q=material(container(p),i);W(q,8)=(W(q,8)&0x8000ffff)|(v<<16);}
void NNS_G3dMdlSetMdlLightEnableFlag(u8 *p,u32 i,u32 v){u8 *q=material(container(p),i);W(q,12)=(W(q,12)&~15)|v;}
void NNS_G3dMdlSetMdlPolygonMode(u8 *p,u32 i,u32 v){u8 *q=material(container(p),i);W(q,12)=(W(q,12)&~0x30)|(v<<4);}
void NNS_G3dMdlSetMdlCullMode(u8 *p,u32 i,u32 v){u8 *q=material(container(p),i);W(q,12)=(W(q,12)&~0xc0)|(v<<6);}
void NNS_G3dMdlSetMdlPolygonID(u8 *p,u32 i,u32 v){u8 *q=material(container(p),i);W(q,12)=(W(q,12)&~0x3f000000)|(v<<24);}
void NNS_G3dMdlSetMdlAlpha(u8 *p,u32 i,u32 v){u8 *q=material(container(p),i);W(q,12)=(W(q,12)&~0x1f0000)|(v<<16);}
void NNS_G3dMdlSetMdlFogEnableFlag(u8 *p,u32 i,int v){u8 *q=material(container(p),i);u32 x=W(q,12);if(v)x|=32768;else x&=~32768;W(q,12)=x;}
void NNS_G3dMdlSetMdlDepthTestCond(u8 *p,u32 i,int v){u8 *q=material(container(p),i);u32 x=W(q,12);if(v)x|=16384;else x&=~16384;W(q,12)=x;}
void NNS_G3dMdlSetMdl1Dot(u8 *p,u32 i,int v){u8 *q=material(container(p),i);u32 x=W(q,12);if(v)x|=8192;else x&=~8192;W(q,12)=x;}
void NNS_G3dMdlSetMdlFarClip(u8 *p,u32 i,int v){u8 *q=material(container(p),i);u32 x=W(q,12);if(v)x|=4096;else x&=~4096;W(q,12)=x;}
void NNS_G3dMdlSetMdlXLDepthUpdate(u8 *p,u32 i,int v){u8 *q=material(container(p),i);u32 x=W(q,12);if(v)x|=2048;else x&=~2048;W(q,12)=x;}
u32 NNS_G3dMdlGetMdlLightEnableFlag(u8 *p,u32 i){u8 *q=material(container(p),i);return W(q,12)&15;}
u32 NNS_G3dMdlGetMdlFogEnableFlag(u8 *p,u32 i){u8 *q=material(container(p),i);return W(q,12)&32768;}
