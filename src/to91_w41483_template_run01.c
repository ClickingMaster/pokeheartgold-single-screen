#include "global.h"
#include "sprite.h"
#include "unk_02009D48.h"
#pragma force_active on
typedef struct {u8 prefix[0x44];SpriteList *list;GF_2DGfxResMan *managers[4];} Config;
typedef struct {SpriteTemplate *sprite;void *callback;u32 priority,draw;} Wrapper;
void ov41_0224A118(void*,void*);
void ov41_0224A6C4(void*,int,void*,int,int,int,int);
void ov41_0224A6C4(void*w,int id,void*opaque,int x,int y,int width,int height){Config*config=opaque;Wrapper wrap;SpriteResourcesHeader header;SpriteTemplate sprite;
CreateSpriteResourcesHeader(&header,id,0,id,id,-1,-1,0,0,config->managers[0],config->managers[1],config->managers[2],config->managers[3],0,0);
sprite.spriteList=config->list;sprite.header=&header;sprite.position.x=(u32)x<<12;sprite.position.y=(u32)y<<12;sprite.position.z=0;sprite.drawPriority=2;sprite.whichScreen=NNS_G2D_VRAM_TYPE_2DMAIN;sprite.heapID=(enum HeapID)14;
wrap.sprite=&sprite;wrap.draw=1;wrap.callback=0;wrap.priority=0;ov41_0224A118(w,&wrap);}
