#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u32 pad0;void **object;u8 pad8[12];int kind;u8 pad18[0x14];int x,y;u8 pad34[0x1c];fx32 height;} Actor;
void ov96_021EAF78(void*,int,int,int*,int*,int*);
BOOL ov96_02218598(Actor*,Actor*);
BOOL ov96_02218598(Actor *a,Actor *b){
 VecFx32 delta,va,vb;int ra,rb;fx32 height=a->height-b->height;if(height<0)height=-height;if(height>5*FX32_ONE)return FALSE;
 ov96_021EAF78(*a->object,a->x,a->y,&va.x,&va.y,&ra);
 ov96_021EAF78(*b->object,b->x,b->y,&vb.x,&vb.y,&rb);
 if(a->kind==6)ra+=8;if(b->kind==6)rb+=8;
 fx32 radius=(ra+rb)*FX32_ONE;va.z=0;vb.z=0;VEC_Subtract(&va,&vb,&delta);if(VEC_Mag(&delta)<=radius)return TRUE;return FALSE;
}
