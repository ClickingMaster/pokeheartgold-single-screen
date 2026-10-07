#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u8 p[8];s32 x;u32 q;s32 y;}Vector;typedef struct{u8 p[0xf8];Vector*vector;u16 x,y,dx,dy;s16 timer;}State;
extern u16 FX_Atan2Idx(fx32,fx32);extern int ov91_0225E6F8(Vector*);extern void sub_020182E0(void*,int,int);
void ov91_02260B5C(State*);
#pragma force_active off
static inline s64 component(s32 table,s32 scale){return (s64)table*scale;}
#pragma force_active on
void ov91_02260B5C(State*s){s->timer=(s16)(s->timer-1);if(s->timer<0){int radius=ov91_0225E6F8(s->vector);int angle=(FX_Atan2Idx(s->vector->y,s->vector->x)>>4)*2;s32 value=(u16)((s32)((u32)radius<<9)/0x1e200)<<12;s->dx=(((fx32)((component(FX_SinCosTable_[angle],value)+2048)>>12))>>12)+512;s->dy=(((fx32)((component(FX_SinCosTable_[angle+1],value)+2048)>>12))>>12)+512;s->timer=8;}s->x+=s->dx;s->y+=s->dy;sub_020182E0((u8*)s+4,s->x,0);sub_020182E0((u8*)s+4,s->y,2);}
