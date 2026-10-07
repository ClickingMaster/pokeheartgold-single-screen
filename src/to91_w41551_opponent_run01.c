#include "global.h"
#include "overlay96_opponent_selection_r111_private.h"
#pragma force_active on
extern int ov96_02215614(int,int,int,int);
extern BOOL ov96_0221567C(int,int,int,int,int,int);
extern int ov96_02215058(void*,void*,int,int);
extern void ov96_021E8228(u8*,u32,u32,int,int);
int ov96_02214EB4(void *p,u8 side,u8 other,W40081Parent *s,W40081Actor *items){int own,result;u32 region;u8 i,which[2];VecFx32 vec;if(s->state)return 0;if(s->busy)return 0;own=ov96_02215614(128,96,s->pos.x/4096,s->pos.y/4096);for(i=0;i<2;i++){W40081Actor *a=&items[i];if(!a->disabled&&a->enabled)region=ov96_02215614(128,96,a->pos.x/4096,a->pos.y/4096);else region=4;if(region!=4){if(side==region){if(ov96_0221567C(a->pos.x/4096,a->pos.y/4096,s->pos.x/4096,s->pos.y/4096,own,side))which[i]=7;else which[i]=6;}else which[i]=6;}else which[i]=5;}region=which[1];if((which[0]==7)==(region==7)){int dist;VEC_Subtract(&s->pos,&items[0].pos,&vec);dist=VEC_Mag(&vec);VEC_Subtract(&s->pos,&items[1].pos,&vec);if(dist>=VEC_Mag(&vec))result=ov96_02215058(s,items,which[0],side);else{items++;result=ov96_02215058(s,items,region,side);}}else{if(which[0]>=region)result=ov96_02215058(s,items,which[0],side);else{items++;result=ov96_02215058(s,items,region,side);}}if(s->state==1)ov96_021E8228((u8*)p,side,other,6,1);return result;}
