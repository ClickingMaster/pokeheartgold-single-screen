#pragma force_active on
#include "global.h"
#include "w19_particle_motion_owner.h"

extern u32 ov07_0221C4A8(void *p, int i);
extern void *ov07_0221C494(void*,int);
extern void *ov07_0221C478(void*); extern u16 ov07_0221C468(void*),ov07_0221C470(void*);
void ov07_0222600C(void *p,Ov07Work104 *w);
void ov07_0222600C(void *p,Ov07Work104 *w){u32 v;w->arg0=ov07_0221C4A8(p, 0);w->argC=ov07_0221C4A8(p, 1);w->argE=ov07_0221C4A8(p, 2);w->delay=ov07_0221C4A8(p, 3);w->arg6=ov07_0221C4A8(p, 4);w->arg8=ov07_0221C4A8(p, 5);w->arg10=ov07_0221C4A8(p, 6);w->wave=ov07_0221C4A8(p, 8);v=ov07_0221C4A8(p, 7);w->unk1C=0;w->unk20=(v&0xffff0000)>>16;w->unk24=(u16)v;if(w->unk20==0)w->unk20=0;if(w->unk24==0)w->unk24=255;w->node=ov07_0221C494(p,w->arg0);w->resource=ov07_0221C478(p);if(w->arg10==0){w->handleA=ov07_0221C468(p);w->handleB=ov07_0221C470(p);}else if(w->arg10==1){w->handleA=ov07_0221C470(p);w->handleB=ov07_0221C468(p);}if(w->node==0)GF_AssertFail();}
