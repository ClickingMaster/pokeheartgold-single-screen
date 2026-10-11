#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u32 first[2];VecFx32 pos;u32 middle[4];int priority;u32 tail[2];} Template;
typedef struct{u8 pad[0x122];u16 index;u8 pad2[0xd60-0x124];u8 manager[0x6c];void*cursor;u8 pad3[8];void*markers[30];void*lower[30];void*side[6];u8 pad4[0xf04-0xee0];void*other[2];u32 pad5;void*final;} State;
typedef struct{u16 x,y;} Point;
extern Point ov70_02245784[],ov70_022456E8[];
extern u16 ov70_02245786[][2];
extern void Gts_InitSpriteTemplate(Template*,State*,void*,int),Gts_SetSpritePosition(void*,int,int);
extern void*Sprite_CreateAffine(Template*);
extern void Sprite_SetAnimActiveFlag(void*,int),Sprite_SetAnimCtrlSeq(void*,int),Sprite_SetPriority(void*,int),Sprite_SetDrawFlag(void*,int);
void ov70_0223D058(State*);
void ov70_0223D058(State*s){Template t;int i;Gts_InitSpriteTemplate(&t,s,s->manager,1);t.pos.x=ov70_02245784[s->index].x<<12;t.pos.y=ov70_02245786[s->index][0]<<12;s->cursor=Sprite_CreateAffine(&t);Sprite_SetAnimActiveFlag(s->cursor,1);Sprite_SetAnimCtrlSeq(s->cursor,4);if(s->index==31||s->index<=5)Sprite_SetPriority(s->cursor,0);else Sprite_SetPriority(s->cursor,1);
for(i=0;i<30;i++){t.pos.x=ov70_02245784[i].x<<12;t.pos.y=ov70_02245784[i].y<<12;t.priority=20;s->markers[i]=Sprite_CreateAffine(&t);Sprite_SetAnimCtrlSeq(s->markers[i],i+6);Sprite_SetPriority(s->markers[i],1);}
for(i=0;i<30;i++){t.pos.x=ov70_02245784[i].x<<12;t.pos.y=(ov70_02245784[i].y+6)<<12;t.priority=10;s->lower[i]=Sprite_CreateAffine(&t);Sprite_SetAnimCtrlSeq(s->lower[i],40);Sprite_SetPriority(s->lower[i],1);}
for(i=0;i<6;i++){t.pos.x=(ov70_02245784[i].x+8)<<12;t.pos.y=(ov70_02245784[i].y+6)<<12;t.priority=10;s->side[i]=Sprite_CreateAffine(&t);Sprite_SetAnimCtrlSeq(s->side[i],42);Sprite_SetPriority(s->side[i],1);}
for(i=0;i<2;i++){t.pos.x=ov70_022456E8[i].x<<12;t.pos.y=ov70_022456E8[i].y<<12;s->other[i]=Sprite_CreateAffine(&t);Sprite_SetAnimCtrlSeq(s->other[i],i+38);Sprite_SetPriority(s->other[i],1);}
Sprite_SetDrawFlag(s->final,1);Gts_SetSpritePosition(s->final,55,424);}
