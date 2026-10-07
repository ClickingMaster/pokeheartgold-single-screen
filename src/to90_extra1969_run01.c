#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
#define RND(a,b,c,d) ((fx32)(((s64)(a)*(b)-(s64)(c)*(d)+0x800)>>12))
#define RAW(a,b,c,d) ((fx32)(((s64)(a)*(b)-(s64)(d)*(c))>>12))
#define MUL(a,b) ((fx32)(((s64)(a)*(b))>>12))
void FX_InvAsync(fx32);fx32 FX_GetDivResult(void);void MI_Copy36B(const void*,void*);int MTX_Inverse33(const MtxFx33*,MtxFx33*);
int MTX_Inverse33(const MtxFx33 *s,MtxFx33 *dst){MtxFx33 temp;fx32 a22,a10,a20,a12,a11,a21;a12=s->_12;a20=s->_20;a10=s->_10;a22=s->_22;a11=s->_11;a21=s->_21;MtxFx33*out=s==dst?&temp:dst;s64 c0=RND(a10,a22,a12,a20);s64 c1=RND(a11,a22,a12,a21);s64 c2=RND(a10,a21,a11,a20);fx32 det=(fx32)(((s64)s->_00*(fx32)c1-(s64)s->_01*(fx32)c0+(s64)s->_02*(fx32)c2+0x800)>>12);fx32 d01,d02,d11,d12,inv;if(!det)return -1;FX_InvAsync(det);d01=RAW(s->_01,s->_22,s->_02,s->_21);d02=RAW(s->_01,s->_12,s->_02,s->_11);d11=RAW(s->_00,s->_22,s->_02,s->_20);d12=RAW(s->_00,s->_12,s->_02,s->_10);inv=FX_GetDivResult();out->_00=MUL(inv,c1);out->_01=-MUL(inv,d01);out->_02=MUL(inv,d02);out->_10=-MUL(inv,c0);out->_11=MUL(inv,d11);out->_12=-MUL(inv,d12);out->_20=MUL(inv,c2);out->_21=-MUL(inv,RAW(s->_00,s->_21,s->_01,s->_20));out->_22=MUL(inv,RAW(s->_00,s->_11,s->_01,s->_10));if(out==&temp)MI_Copy36B(&temp,dst);return 0;}
