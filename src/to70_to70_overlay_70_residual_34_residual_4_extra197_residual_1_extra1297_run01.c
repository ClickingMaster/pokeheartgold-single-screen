#pragma force_active on
#include "global.h"
typedef struct Work { char pad0[12]; void *sprite; char gap[0x2c]; s16 type; char gap2[10]; int selection; char gap3[0x32]; u8 previous; } Work;
extern struct { char pad[0x48]; u32 pressed,held; } gSystem;
extern u8 ov70_02245DC0[],ov70_02245DC1[],ov70_02245DC2[],ov70_02245DC3[],ov70_02245E26[],ov70_02245E27[];
extern u8 *ov70_02245E84[];
extern void PlaySE(int),Gts_SetSpritePosition(void*,int,int),Sprite_SetAnimCtrlSeq(void*,int);
extern int ov70_02242164(Work*,int),ov70_02242364(Work*,int);
int ov70_02242390(Work*,int);
int ov70_02242390(Work*w,int offset) {
 int old=w->selection,result;
 if(old<3) w->previous=old;
 if(gSystem.held&0x40) w->selection=ov70_02245DC0[w->selection*4];
 else if(gSystem.held&0x80) w->selection=ov70_02245DC1[w->selection*4];
 else if(gSystem.held&0x20) w->selection=ov70_02245DC2[w->selection*4];
 else if(gSystem.held&0x10) w->selection=ov70_02245DC3[w->selection*4];
 if(old==3 && w->selection<3) w->selection=w->previous;
 if(old!=w->selection) { PlaySE(1500); if(w->selection==3) { Gts_SetSpritePosition(w->sprite,192,136); Sprite_SetAnimCtrlSeq(w->sprite,48); }
 else { Gts_SetSpritePosition(w->sprite,(ov70_02245E26[w->selection*2]+16)*8,ov70_02245E27[w->selection*2]*8); Sprite_SetAnimCtrlSeq(w->sprite,47); } }
 result=ov70_02242164(w,5);
 if(result!=-1) {
 result=ov70_02242364(w,result); if(result==-1) return result;
 if(result!=-2 && offset && !ov70_02245E84[w->type][offset+result]) goto failed;
 PlaySE(1500); return result;
 } else if(gSystem.pressed&1) {
 result=ov70_02242364(w,w->selection); if(result==-1) return result;
 if(result!=-2 && offset && !ov70_02245E84[w->type][offset+result]) goto failed;
 PlaySE(1500); return result;
 } else if(gSystem.pressed&2) { PlaySE(1500); return -2; }
failed:
 return -1;
}
