#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
extern u8 wmspW[];
extern u32 __VENEER_OS_DisableInterrupts(void);
extern u32 __VENEER_OS_RestoreInterrupts(u32);
extern void __VENEER_OS_FreeToHeap(int,int,void*);
extern void* __VENEER_OS_AllocFromHeap(int,int,u32);
extern u32* WMSP_GetInternalRequestBuf(void);
extern int __VENEER_OS_SendMessage(void*,void*,int);
extern u16* __VENEER_WMSP_GetBuffer4Callback2Wm9(void);
extern void __VENEER_WMSP_ReturnResult2Wm9(void*);
void WmspFreeBufOfWL(void *p);
// @fn WmspFreeBufOfWL
void WmspFreeBufOfWL(void *p){u32 irq=__VENEER_OS_DisableInterrupts();__VENEER_OS_FreeToHeap(*(int*)(wmspW+0x1544),*(int*)(wmspW+0x1548),p);__VENEER_OS_RestoreInterrupts(irq);}
