#pragma force_active on
#include "global.h"
typedef struct {u32 data[9];} Resources;
typedef struct {void *list;Resources *header;s32 pos[3],scale[3];u16 rotation;u16 pad;u32 priority;int screen,heap;} Template;
extern void *ov96_021E9D08(void *,int);extern void CreateSpriteResourcesHeader(Resources *,int,int,int,int,int,int,int,int,void *,void *,void *,void *,int,int);extern void *Sprite_CreateAffine(Template *);extern void Sprite_SetAnimActiveFlag(void *,int);extern void Sprite_SetAnimCtrlSeq(void *,int);extern void Sprite_SetDrawFlag(void *,int);
void *ov96_021EA6E4(void *,void *,int,int,int);
void *ov96_021EA6E4(void *w,void *list,int screen,int arg,int heap){Resources header;Template t;void *a,*b,*c,*d,*sprite;int resource=screen==1?23:24;a=ov96_021E9D08(w,0);b=ov96_021E9D08(w,1);c=ov96_021E9D08(w,2);d=ov96_021E9D08(w,3);CreateSpriteResourcesHeader(&header,31,resource,31,31,-1,-1,0,arg,a,b,c,d,0,0);t.list=list;t.header=&header;t.heap=heap;t.pos[0]=0;t.pos[1]=0;t.pos[2]=0;t.scale[0]=4096;t.scale[1]=4096;t.scale[2]=4096;t.rotation=0;t.priority=1;t.screen=screen;sprite=Sprite_CreateAffine(&t);if(!sprite)GF_AssertFail();Sprite_SetAnimActiveFlag(sprite,1);Sprite_SetAnimCtrlSeq(sprite,0);Sprite_SetDrawFlag(sprite,0);return sprite;}
