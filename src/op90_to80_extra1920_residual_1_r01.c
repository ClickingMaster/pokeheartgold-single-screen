#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
#define W(p,o) (*(u32 *)((u8 *)(p)+(o)))
#define H(p,o) (*(u16 *)((u8 *)(p)+(o)))
#define B(p,o) (*(u8 *)((u8 *)(p)+(o)))
extern u32 _02110A10;
extern int NNS_G3dGetResDictIdxByName(void *,void *);
void NNSi_G3dAnmObjInitNsBtp(u8*,u8*,u8*);
static inline u8 *mdl(u8*p){if(p && W(p,8))return p+W(p,8);return 0;}
static inline u8 *name(u8*d,u32 i){u32 o;u8 *h;if(d&&i<B(d,1)){o=H(d,6);h=d+o;return h+H(h,2)+16*i;}return 0;}
// @fn NNSi_G3dAnmObjInitNsBtp
void NNSi_G3dAnmObjInitNsBtp(u8 *obj,u8 *anm,u8 *model){u8 *m=mdl(model);u32 i;W(obj,12)=_02110A10;B(obj,25)=B(model,24);W(obj,8)=(u32)anm;MIi_CpuClear16(0,(u16*)(obj+26),B(obj,25)*2);for(i=0;i<B(anm,13);i++){u8 *n=name(anm+12,i);int idx=m?NNS_G3dGetResDictIdxByName(m+4,n):-1;if(idx>=0)H(obj+idx*2,26)=i|0x100;}}
