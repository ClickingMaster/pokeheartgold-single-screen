#include "global.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "filesystem.h"
#pragma force_active on
typedef struct {u8 pad[0x3c];s32 state;SpriteResource *resources[4];Sprite *sprites[2];} Work;
typedef struct {u8 pad0[4];SpriteList *list;u8 pad8[0x130-8];GF_2DGfxResMan *managers[4];} Context;
extern const u8 ov49_022696E8[];
void ov49_0225AD20(void *,void *,u32);
void ov49_0225AD20(void *work,void *context,u32 heap){Work *w=work;Context *ctx=context;SpriteResourcesHeader header;SimpleSpriteTemplate template;int i;void*narc=NARC_New((NarcId)60,(enum HeapID)heap);w->resources[0]=AddCharResObjFromOpenNarc(ctx->managers[0],narc,4,0,5000,1,(enum HeapID)heap);w->resources[1]=AddPlttResObjFromOpenNarc(ctx->managers[1],narc,10,0,5000,1,1,(enum HeapID)heap);w->resources[2]=AddCellOrAnimResObjFromOpenNarc(ctx->managers[2],narc,5,0,5000,GF_GFX_RES_TYPE_CELL,(enum HeapID)heap);w->resources[3]=AddCellOrAnimResObjFromOpenNarc(ctx->managers[3],narc,6,0,5000,GF_GFX_RES_TYPE_ANIM,(enum HeapID)heap);NARC_Delete(narc);SpriteTransfer_CreateCharTransferTask_AllocAtEnd(w->resources[0]);SpriteTransfer_CreatePlttTransferTask(w->resources[1]);CreateSpriteResourcesHeader(&header,5000,5000,5000,5000,-1,-1,0,0,ctx->managers[0],ctx->managers[1],ctx->managers[2],ctx->managers[3],0,0);template.spriteList=ctx->list;template.header=&header;template.priority=0;template.whichScreen=NNS_G2D_VRAM_TYPE_2DMAIN;template.heapID=(enum HeapID)heap;template.position.x=0xc0000;for(i=0;i<2;i++){template.position.y=ov49_022696E8[i]<<12;w->sprites[i]=Sprite_Create(&template);Sprite_SetAnimCtrlSeq(w->sprites[i],i);Sprite_SetAnimActiveFlag(w->sprites[i],1);Sprite_SetDrawFlag(w->sprites[i],0);}w->state=0;}
