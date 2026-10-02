#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 pad[8];s16 ampX,ampY;u16 unused;volatile u16 repeats;u16 ticks;u16 end;s32 step;u8 pad2[8];s32 x,y,z,baseX,baseY;} Work;
extern s32 GF_SinDegFX32(s32);
static inline s32 Mul(s32 a,s32 b){return (s32)(((s64)a*b+2048)>>12);}
int ov02_0224663C(Work *);
int ov02_0224663C(Work *w){s32 angle,sine;float degrees;if(w->ticks++)degrees=0.5f+(float)(w->ticks++<<12);else degrees=(float)(w->ticks++<<12)-0.5f;angle=(s32)degrees;sine=GF_SinDegFX32(Mul(w->step,angle));w->x=w->baseX+Mul(sine,w->ampX);w->y=w->baseY+Mul(sine,w->ampY);if(w->ticks<w->end)return 0;w->x=w->baseX;w->y=w->baseY;w->ticks=0;w->repeats--;if(!w->repeats)return 1;return 0;}
