#pragma force_active on
#include "global.h"
#pragma force_active on
#include "sprite.h"
typedef struct {void *list;void *resources;u8 pad[0x24];u32 heap;} Template;
typedef struct {u8 pad[4];void *list;u8 pad2[0x80];u8 resources[0x1f4-0x88];Sprite *sprites[3];} Work;
extern Template ov43_0222F14C[];extern u8 ov43_0222F160[];extern Template ov43_0222F1AC;
void ov43_0222AC28(Work *,u32);
void ov43_0222AC28(Work *w,u32 heap){void *resources;int i,seq;Template *t;Template *create;Work *cursor;i=0;cursor=w;t=ov43_0222F14C;create=t;seq=5;resources=w->resources;for(;i<2;i++){t->list=w->list;t->resources=resources;t->heap=heap;cursor->sprites[0]=Sprite_CreateAffine((SpriteTemplate *)create);Sprite_SetAnimActiveFlag(cursor->sprites[0],1);Sprite_SetDrawFlag(cursor->sprites[0],0);Sprite_SetAnimCtrlSeq(cursor->sprites[0],seq);t++;create++;cursor=(Work *)((u8 *)cursor+4);seq+=2;}*(void **)(ov43_0222F160+0x4c)=w->list;*(void **)(ov43_0222F160+0x50)=w->resources;*(u32 *)(ov43_0222F160+0x78)=heap;w->sprites[2]=Sprite_CreateAffine((SpriteTemplate *)&ov43_0222F1AC);Sprite_SetDrawFlag(w->sprites[2],0);Sprite_SetAnimCtrlSeq(w->sprites[2],1);}
