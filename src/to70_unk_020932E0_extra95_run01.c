#pragma force_active on
#include "global.h"
#include "system.h"
#pragma force_active on
#define WORD(w,o) (*(u32*)((w)+(o)))
#define PTR(w,o) (*(void**)((w)+(o)))
extern void *sub_02095CE0(int,u8*);
extern void sub_02095D2C(void*),sub_0209515C(void*),sub_020950F8(void*,int),RemoveWindow(void*),sub_020950D4(void*),Heap_Free(void*),SpriteTransfer_DeleteCharTransferTask(void*),SpriteTransfer_DeletePlttTransferTask(void*),Destroy2DGfxResObjMan(void*),SpriteList_Delete(void*),YesNoPrompt_Destroy(void*),FontID_Release(int),GfGfx_SwapDisplay(void),FreeBgTilemapBuffer(void*,int);
u8 *sub_020932E0(enum HeapID,int,int);void sub_02093354(u8*);
u8 *sub_020932E0(enum HeapID heap,int count,int flag){u8*w=Heap_Alloc(heap,0x46e8);memset(w,0,0x46e8);GF_ASSERT(count!=0);WORD(w,4)=heap;w[0xd]=count;w[0x10]=flag;if(w[0xd]>3){GF_ASSERT(FALSE);w[0xd]=3;}PTR(w,0x8d0)=Heap_Alloc((enum HeapID)WORD(w,4),w[0xd]*12);PTR(w,0x7e4)=Heap_Alloc((enum HeapID)WORD(w,4),(w[0xd]+34)*4);PTR(w,0x46b8)=sub_02095CE0(WORD(w,4),w+0xf);return w;}
