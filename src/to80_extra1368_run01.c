#pragma force_active on
#include "global.h"
#include "grid_cursor_internal.h"
typedef struct Work { char pad0[0x7b0]; void *sprite; char pad1[0x10c]; void *target; char pad2[0x3dbc]; int f4680,id; VecFx32 position; int f4694,f4698,mode; void *sprites; int f46a4,f46a8,f46ac,counter,f46b4; void *sub; int flag; } Work;
extern void Sprite_SetMatrix(void*,VecFx32*),Sprite_SetDrawPriority(void*,int),Sprite_SetAnimCtrlSeq(void*,int),Sprite_SetDrawFlag(void*,int);
void sub_02094A90(Work*,int);
void sub_02094A90(Work*w,int mode) {
 VecFx32 position;
 if(mode) {
 u8 index;
 if(w->mode==2) {
 if(mode==2) { position=w->position; index=(u8)w->id; }
 else { u8 current=(u8)w->counter; position.x=0xd4000; position.y=40*(current+1)<<12; position.z=0; index=(u8)w->counter; }
 } else { u8 current=(u8)w->counter; position.x=0xd4000; position.y=40*(current+1)<<12; position.z=0; index=(u8)w->counter; }
 sub_02095D40(w->sub,3,index);
 } else {
 position=w->position;
 if(w->mode==2) sub_02095D40(w->sub,3,(u8)w->id); else sub_02095D40(w->sub,2,(u8)w->id);
 }
 Sprite_SetMatrix(*(void**)((u32)w->sprites+52*w->id),&w->position); Sprite_SetDrawPriority(*(void**)((u32)w->sprites+52*w->id),6); Sprite_SetMatrix(w->target,&position); Sprite_SetAnimCtrlSeq(w->target,45);
 if(w->flag) w->flag=0;
 w->f4680=0; w->id=-1; w->position.x=0; w->position.y=0; w->position.z=0; w->f4694=0; w->f4698=0; w->sprites=0; w->mode=0;
 Sprite_SetDrawFlag(w->sprite,0);
}
