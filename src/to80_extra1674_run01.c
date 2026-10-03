#pragma force_active on
#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#define I(p,o) (*(int*)((u8*)(p)+(o)))
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*((u8*)(p)+(o)))
#define V(p,o) (*(void**)((u8*)(p)+(o)))
typedef struct {u8 pad[24];int sx,sy;s16 sn,cs;u8 pad2[8];u16 w,h;} Tex;
void texmtxCalc_flagT___3dsmax(int*,Tex*),texmtxCalc_flagTS___3dsmax(int*,Tex*);
extern void FX_DivAsync(int,int);extern int FX_GetDivResult(void);
void texmtxCalc_flagT___3dsmax(int*m,Tex*t){int w=t->w<<12,h=t->h<<12;int a,b,c,d,x,y;FX_DivAsync(h,w);a=(s64)t->sx*t->cs>>12;b=(s64)t->sx*t->sn>>12;c=(s64)t->sy*t->cs>>12;d=(s64)t->sy*t->sn>>12;m[0]=a;m[5]=c;m[1]=(d*FX_GetDivResult())>>12;FX_DivAsync(w,h);w=-(int)t->w<<11;h=-(int)t->h<<11;x=((s64)a*w-(s64)b*h)>>8;y=((s64)d*w+(s64)c*h)>>8;m[12]=x+(t->w<<15);m[13]=y+(t->h<<15);m[4]=(-b*FX_GetDivResult())>>12;}
