#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u32 heap,flags,other;void *man[4],*chars[12];u8 pad[0x60];void *pal,*extraPal;} Work;
extern void SpriteTransfer_DeleteCharTransferTask(void*),SpriteTransfer_DeletePlttTransferTask(void*),Destroy2DGfxResObjMan(void*),Heap_Free(void*);
void ov96_021E9C0C(Work*);
void ov96_021E9C0C(Work*w){int count;
if(w->flags&1)SpriteTransfer_DeleteCharTransferTask(w->chars[0]);
if(w->flags&4)SpriteTransfer_DeleteCharTransferTask(w->chars[1]);
if(w->flags&2)SpriteTransfer_DeleteCharTransferTask(w->chars[2]);
if(w->flags&8)SpriteTransfer_DeleteCharTransferTask(w->chars[3]);
if(w->flags&16)SpriteTransfer_DeleteCharTransferTask(w->chars[4]);
if(w->flags&32)SpriteTransfer_DeleteCharTransferTask(w->chars[5]);
if(w->flags&64)SpriteTransfer_DeleteCharTransferTask(w->chars[6]);
if(w->flags&128)SpriteTransfer_DeleteCharTransferTask(w->chars[7]);
if(w->flags&256)SpriteTransfer_DeleteCharTransferTask(w->chars[8]);
if(w->flags&512)SpriteTransfer_DeleteCharTransferTask(w->chars[9]);
if(w->flags&1024)SpriteTransfer_DeleteCharTransferTask(w->chars[10]);
if(w->flags&2048)SpriteTransfer_DeleteCharTransferTask(w->chars[11]);
SpriteTransfer_DeletePlttTransferTask(w->pal);if(w->extraPal)SpriteTransfer_DeletePlttTransferTask(w->extraPal);for(count=0;count<4;count++)Destroy2DGfxResObjMan(w->man[count]);Heap_Free(w);}
