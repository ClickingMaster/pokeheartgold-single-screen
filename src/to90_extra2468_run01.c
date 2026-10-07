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
void texmtxCalc_flagTS___3dsmax(int*m,Tex*t){int w=t->w<<12,h=t->h<<12;FX_DivAsync(h,w);m[0]=t->cs;m[5]=t->cs;m[1]=((s32)((u32)(s32)t->sn*(u32)FX_GetDivResult()))>>12;FX_DivAsync(w,h);w=(s32)((0u-(u32)t->w)<<11);h=(s32)((0u-(u32)t->h)<<11);m[12]=(s32)((u32)(((s64)t->cs*w-(s64)t->sn*h)>>8)+((u32)t->w<<15));m[13]=(s32)((u32)(((s64)t->sn*w+(s64)t->cs*h)>>8)+((u32)t->h<<15));m[4]=((s32)((0u-(u32)(s32)t->sn)*(u32)FX_GetDivResult()))>>12;}
