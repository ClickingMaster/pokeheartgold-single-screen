#pragma force_active on
#include "global.h"
typedef struct {void *a,*list;u8 p[0x128];void *res[6];void *charTask,*palTask;u8 q[0x34];struct {void *a,*b,*c;int d;} sprite[12];} W;
extern void Sprite_Delete(void*);extern void SpriteTransfer_DeleteCharTransferTask(void*);extern void SpriteTransfer_DeletePlttTransferTask(void*);extern void Destroy2DGfxResObjMan(void*);extern void SpriteList_Delete(void*);extern void Heap_Free(void*);
void ov96_021FC1A8(W*);
void ov96_021FC1A8(W*w) {int i;W*p;i=0;p=w;for(;i<12;i++,p=(W*)((u8*)p+16)){Sprite_Delete(p->sprite[0].a);Sprite_Delete(p->sprite[0].b);Sprite_Delete(p->sprite[0].c);}SpriteTransfer_DeleteCharTransferTask(w->charTask);SpriteTransfer_DeletePlttTransferTask(w->palTask);{W *cursor=w;int j;for(j=0;j<6;j++,cursor=(W*)((u8*)cursor+4))Destroy2DGfxResObjMan(cursor->res[0]);}SpriteList_Delete(w->list);Heap_Free(w);}
