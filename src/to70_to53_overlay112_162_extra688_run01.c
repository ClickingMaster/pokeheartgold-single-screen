#pragma force_active on
#include "global.h"
#include "sprite.h"
typedef struct {SpriteList*list;u32 heap;u8 p[0x130-8];GF_2DGfxResMan*man[4];}Work;
extern void ov112_021F0C30(SpriteTemplate*,u32,SpriteResourcesHeader*,int,SpriteList*);
extern void Sprite_SetAnimActiveFlag(Sprite*,BOOL),Sprite_SetPriority(Sprite*,u8);
Sprite*ov112_021F0B9C(Work*,int,int,int,int,int,int,int);
Sprite*ov112_021F0B9C(Work*w,int id,int priority,int transfer,int x,int y,int anim,int draw){SpriteTemplate t;SpriteResourcesHeader h;Sprite*s;CreateSpriteResourcesHeader(&h,id,id,id,id,-1,-1,0,transfer,w->man[0],w->man[1],w->man[2],w->man[3],0,0);ov112_021F0C30(&t,w->heap,&h,2,w->list);t.drawPriority=priority;t.position.x=x<<12;t.position.y=(y<<12)+(1<<20);s=Sprite_CreateAffine(&t);Sprite_SetAnimActiveFlag(s,1);Sprite_SetAnimCtrlSeq(s,anim);Sprite_SetPriority(s,1);Sprite_SetDrawFlag(s,draw);return s;}
