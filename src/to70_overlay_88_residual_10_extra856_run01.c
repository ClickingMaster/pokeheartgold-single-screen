#pragma force_active on
#include "global.h"
typedef struct {u16 active,ticks,color,offset;u32 sound;} W;
extern void PlaySE(u16),BlendPalette(const void *,void *,int,u8,int);
void ov88_02259884(W *);
void ov88_02259884(W *w){if(w->ticks>28)w->active=0;if(w->active){int amount;u16 color;if(w->ticks==0)PlaySE((u16)w->sound);if(w->ticks<2)amount=(w->ticks*16)/2;else if(w->ticks<14)amount=16;else amount=16-((w->ticks-14)*16)/14;color=14;BlendPalette(&color,&w->color,1,(u8)amount,25);DC_FlushRange(&w->color,2);GX_LoadBGPltt(&w->color,w->offset,2);w->ticks++;}}
