#pragma force_active on
#include "global.h"
typedef struct {void*s;u8 p[16];} SpriteSlot;
typedef struct {u8 p[16];u32 a:12;u32 flags:8;u32 b:12;} Work;
extern void*ov96_0220D13C(void*,void*,int,int,u16,int);extern void ManagedSprite_SetDrawFlag(void*,int);
u32 ov96_0220B164(Work*);void ov96_0220B178(SpriteSlot*,void*,void*);
u32 ov96_0220B164(Work*w){if(w==0)GF_AssertFail();return w->flags;}
void ov96_0220B178(SpriteSlot*p,void*a,void*b){int i;for(i=0;i<16;i++,p++){p->s=ov96_0220D13C(a,b,0,0,(u16)(i%3+28),1);ManagedSprite_SetDrawFlag(p->s,0);}}
