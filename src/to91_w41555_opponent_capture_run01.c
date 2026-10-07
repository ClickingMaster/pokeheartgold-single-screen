#include "global.h"
#include "overlay96_opponent_capture_r111_private.h"
#pragma force_active on
extern int ov96_02215614(int,int,int,int);
extern void *ov96_021EAF8C(u8*);
extern BOOL ov96_022156E8(const VecFx32*,fx32,const VecFx32*,fx32);
extern BOOL ov96_02215650(int,int,int,int,int,int);
extern int ov96_02214DBC(void*,const void*,u8,int);
extern void ov96_021E8228(u8*,u32,u32,int,int);
int ov96_02214C3C(void *a,u8 b,u8 c,Actor *actor,Target *targets) {
 Vec *pos;int radius,category; struct {u8 values[2];} decisions; u8 i; int result;
 if(actor->result) return 0;
 if(actor->busy) return 0;
 category=ov96_02215614(128,96,actor->pos.x/4096,actor->pos.y/4096);
 i=0;
 pos=&actor->pos;
 for(;i<2;i++) {
  Target *target=&targets[i]; int targetCategory;
  if(!target->disabled && target->state) targetCategory=ov96_02215614(128,96,target->pos.x/4096,target->pos.y/4096);
  else targetCategory=4;
  if(targetCategory!=4) {
   radius=0;
   if(target->state==1) radius=8;
   else if(target->state==2) radius=12;
   else GF_AssertFail();
   if(ov96_022156E8(pos,(u32)ov96_021EAF8C(actor->body)<<12,&target->pos,radius<<12)) decisions.values[i]=1;
   else if(category==targetCategory) {
    if(ov96_02215650(target->pos.x/4096,target->pos.y/4096,actor->pos.x/4096,actor->pos.y/4096,category,b)) decisions.values[i]=4;
    else decisions.values[i]=3;
   } else decisions.values[i]=2;
  } else decisions.values[i]=0;
 }
 if(decisions.values[0]>=decisions.values[1]) result=ov96_02214DBC(actor,targets,decisions.values[0],category);
 else { targets++; result=ov96_02214DBC(actor,targets,decisions.values[1],category); }
 if(actor->result==1) ov96_021E8228(a,b,c,6,1);
 return result;
}
