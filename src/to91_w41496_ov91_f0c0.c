#include "global.h"
#include "sprite.h"
#pragma force_active on
#include "sprite_transfer.h"
void ov91_0225F0C0(u8*,u8*,int);
#define P(w,o) (*(void**)((w)+(o)))
void ov91_0225F0C0(u8*out,u8*w,int heap){SimpleSpriteTemplate t;NARC*narc;memset(out,0,64);narc=NARC_New((NarcId)200,(enum HeapID)heap);P(out,0)=AddCharResObjFromOpenNarc(P(w,0x148),narc,21,0,120,2,(enum HeapID)heap);P(out,4)=AddPlttResObjFromOpenNarc(P(w,0x14c),narc,20,0,120,2,2,(enum HeapID)heap);P(out,8)=AddCellOrAnimResObjFromOpenNarc(P(w,0x150),narc,22,0,120,(enum GF_GraphicsResourceType)2,(enum HeapID)heap);P(out,12)=AddCellOrAnimResObjFromOpenNarc(P(w,0x154),narc,23,0,120,(enum GF_GraphicsResourceType)3,(enum HeapID)heap);if(!SpriteTransfer_CreateCharTransferTask_UpdateMappingTypeFromHW_AllocAtEnd(P(out,0)))GF_AssertFail();if(!SpriteTransfer_CreatePlttTransferTask(P(out,4)))GF_AssertFail();sub_0200A740(P(out,0));sub_0200A740(P(out,4));CreateSpriteResourcesHeader((SpriteResourcesHeader*)(out+16),120,120,120,120,-1,-1,0,0,P(w,0x148),P(w,0x14c),P(w,0x150),P(w,0x154),0,0);t.spriteList=P(w,28);t.header=(SpriteResourcesHeader*)(out+16);t.position.x=0x80000;t.position.y=0x278000;t.priority=0;t.whichScreen=(NNS_G2D_VRAM_TYPE)2;t.heapID=(enum HeapID)heap;P(out,0x34)=Sprite_Create(&t);Sprite_SetDrawFlag(P(out,0x34),0);NARC_Delete(narc);}
