#pragma force_active on
#include "global.h"
#pragma force_active on
#define W(p,o) (*(int*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*(u8*)((u8*)(p)+(o)))
extern const int ov89_0225CD3C[],ov89_0225CD40[],ov89_0225CD44[],ov89_0225CD48[];extern u32 ov89_0225C8BC(int,int);extern void ModelInstance_GetPosition(void*,int*,int*,int*),ModelInstance_GetScale(void*,int*,int*,int*);void ov89_0225ADA4(void*,void*,int*,int);
extern s64 _ll_mul(s64,s64);
void ov89_0225ADA4(void*sprite,void*info,int*out,int scaled){u32 kind=ov89_0225C8BC(H(info,2),B(info,8));int x,y,z,sx,sy,sz,width; s64 delta;GF_ASSERT(kind<3);ModelInstance_GetPosition(sprite,&x,&y,&z);out[0]=x+ov89_0225CD3C[kind*4];out[1]=x+ov89_0225CD40[kind*4];out[2]=y+ov89_0225CD44[kind*4];out[3]=y+ov89_0225CD48[kind*4];if(scaled==1){ModelInstance_GetScale(sprite,&sx,&sy,&sz);width=out[1]-out[0];delta=((_ll_mul((s64)sx*100/4096,width))/100-width)/2;out[1]+=delta;out[0]-=delta;width=out[2]-out[3];delta=((_ll_mul((s64)sy*100/4096,width))/100-width)/2;out[2]+=delta;out[3]-=delta;}}
