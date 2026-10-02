#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {int heap;void *p,*sys,*man,*q,*resources;u8 pad[12];void *a,*b,*sprites[3],*small[2];} State;
extern void *SpriteManager_GetSpriteList(void *),*ov96_021EA584(void *,void *,int,int),*ov96_021EA374(void *,void *,int,int),*ov96_0220D13C(void *,void *,int,int,int,int);
extern void Sprite_SetMatrix(void *,void *),Sprite_SetDrawPriority(void *,int),Sprite_SetAnimCtrlSeq(void *,int),Sprite_SetDrawFlag(void *,int);
void ov96_02209910(State *);
void ov96_02209910(State *s){VecFx32 pos={0,0,0};int i,x=32768;for(i=0;i<3;i++){pos.x=x;pos.y=11<<16;s->sprites[i]=ov96_021EA584(s->resources,SpriteManager_GetSpriteList(s->man),0,s->heap);Sprite_SetMatrix(s->sprites[i],&pos);Sprite_SetDrawPriority(s->sprites[i],2);Sprite_SetAnimCtrlSeq(s->sprites[i],1);Sprite_SetDrawFlag(s->sprites[i],1);x+=1<<16;}s->a=ov96_0220D13C(s->sys,s->man,192,184,25,2);{int x2;State *cursor;i=0;cursor=s;x2=212;for(;i<2;i++){VecFx32 pos2={0,0,0};cursor->small[0]=ov96_021EA374(s->resources,SpriteManager_GetSpriteList(s->man),2,s->heap);pos2.x=(u32)x2<<12;pos2.y=184<<12;pos2.z=0;Sprite_SetMatrix(cursor->small[0],&pos2);Sprite_SetDrawFlag(cursor->small[0],1);cursor=(State*)((u8*)cursor+4);x2+=16;}}s->b=ov96_0220D13C(s->sys,s->man,32,16,19,2);}
