#pragma force_active on
#include "global.h"
#include "sprite.h"

typedef struct {SpriteList *list;u8 p[0x128];GF_2DGfxResMan *res[4];} W;
Sprite *ov87_021E7DF8(W*,int,int,int,int,u8);
Sprite *ov87_021E7DF8(W*w,int id,int seq,int z,int priority,u8 sub) {SpriteResourcesHeader header;SpriteTemplate t;Sprite*s;CreateSpriteResourcesHeader(&header,id,id,id,id,-1,-1,0,z,w->res[0],w->res[1],w->res[2],w->res[3],0,0);t.spriteList=w->list;t.header=&header;t.position.x=0;t.position.y=0;t.position.z=0;t.scale.x=4096;t.scale.y=4096;t.scale.z=4096;t.rotation=0;t.drawPriority=priority;t.heapID=(enum HeapID)122;if(sub==0)t.whichScreen=NNS_G2D_VRAM_TYPE_2DMAIN;else t.whichScreen=NNS_G2D_VRAM_TYPE_2DSUB;s=Sprite_CreateAffine(&t);Sprite_SetAnimActiveFlag(s,0);Sprite_SetAnimSpeed(s,4096);Sprite_SetAnimCtrlSeq(s,seq);return s;}
