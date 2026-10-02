#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct W{void*first;void*second;u16 unknown;s16 x,y;u16 unknown2;u32 slot:4;u32 speed:8;u32 state:8;u32 offset:7;u32 other:5;}W;extern const s16 ov96_0221CDC8[4][2],ov96_0221CDCA[];extern u32 MTRandom(void);extern void ManagedSprite_OffsetPositionXY(void*,int,int),ManagedSprite_GetPositionXY(void*,s16*,s16*),ManagedSprite_SetPositionXY(void*,int,int),ov96_0220B068(W*);void ov96_0220AF64(W*),ov96_0220AFF8(W*);
void ov96_0220AF64(W*w){if(w->state==1){u32 bottom=168-w->offset*4;s16 x,y;w->speed+=20;ManagedSprite_OffsetPositionXY(w->first,0,w->speed);ManagedSprite_OffsetPositionXY(w->second,0,w->speed);ManagedSprite_GetPositionXY(w->second,&x,&y);ManagedSprite_GetPositionXY(w->first,&x,&y);if((u32)y>=bottom){y=bottom;ManagedSprite_SetPositionXY(w->first,x,y);ov96_0220B068(w);w->state=2;}}}
void ov96_0220AFF8(W*w){u8 slot;if(!w)GF_AssertFail();slot=MTRandom()%3;if(slot>=w->slot)slot++;w->slot=slot;w->x=ov96_0221CDC8[w->slot][0]+(MTRandom()&15);w->y=ov96_0221CDCA[w->slot*2]+MTRandom()%12;}
