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
extern int __VENEER_OS_ReceiveMessage(void*,void**,int);
extern void __VENEER_OS_ExitThread(void);
extern void (*WmspRequestFuncTable[])(void*);
void WMSP_RequestThread(void *a);
#pragma opt_treetransformation off
// @fn WMSP_RequestThread
void WMSP_RequestThread(void *a){u8*w=wmspW;u8*s=*(u8**)(w+0x1550);u16*p;(void)a;for(;;){__VENEER_OS_ReceiveMessage(w+0x88,(void**)&p,1);if(!p){__VENEER_OS_ExitThread();break;}{u16 n=p[0];if(n&0x8000)n&=~0x8000;if(n<46){*(u32*)(s+4)=1;*(u16*)(s+2)=n;WmspRequestFuncTable[n](p);*(u32*)(s+4)=0;}p[0]=n|0x8000;}}}
