#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {void **objects;} Collection;
typedef struct {int heap,mode;u8 pad[6];u16 vram;u8 pad2[0x144-0x10];void *chars,*pltt,*cells,*anims;Collection *charList,*plttList,*cellList,*animList;} State;
extern void *AddCharResObjFromNarc(void *,int,int,int,int,int,int),*AddCellOrAnimResObjFromNarc(void *,int,int,int,int,int,int),*AddPlttResObjFromNarc(void *,int,int,int,int,int,int,int);
extern void GF_AssertFail(void),SpriteTransfer_CreateCharTransferTask_AllocAtEnd(void *),SpriteTransfer_CreateCharTransferTask_UpdateMappingTypeFromHW_AllocAtEnd(void *),SpriteTransfer_CreateCharTransferTask_UpdateMappingTypeFromHW(void *),SpriteTransfer_CreatePlttTransferTask(void *);
void ov108_021E8758(State *);
void ov108_021E8758(State *s){int i;Collection *list=s->charList;for(i=0;i<12;i++){list->objects[i]=AddCharResObjFromNarc(s->chars,166,i+18,0,i+0xe000,s->vram,s->heap);if(list->objects[i]==0)GF_AssertFail();switch(s->mode){case 0:SpriteTransfer_CreateCharTransferTask_AllocAtEnd(list->objects[i]);break;case 1:SpriteTransfer_CreateCharTransferTask_UpdateMappingTypeFromHW_AllocAtEnd(list->objects[i]);break;case 2:default:SpriteTransfer_CreateCharTransferTask_UpdateMappingTypeFromHW(list->objects[i]);break;}}for(i=0;i<2;i++){Collection *item=(&s->cellList)[i];item->objects[0]=AddCellOrAnimResObjFromNarc((&s->cells)[i],166,i+16,0,0xe000,i+2,s->heap);if(item->objects[0]==0)GF_AssertFail();}list=s->plttList;list->objects[0]=AddPlttResObjFromNarc(s->pltt,166,15,0,0xe000,s->vram,4,s->heap);if(list->objects[0]==0)GF_AssertFail();SpriteTransfer_CreatePlttTransferTask(list->objects[0]);}
