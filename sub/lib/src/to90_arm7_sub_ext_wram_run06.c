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
extern void WmspFreeBufOfWL(void*);
void WmspMaMultiPollAckAlarmCallback(void *a);
#pragma opt_treetransformation off
// @fn WmspMaMultiPollAckAlarmCallback
void WmspMaMultiPollAckAlarmCallback(void *a){u8*w=wmspW;u16*p=__VENEER_OS_AllocFromHeap(*(int*)(w+0x1544),*(int*)(w+0x1548),64);(void)a;p[6]=0x185;p[7]=0;if(!__VENEER_OS_SendMessage(w+0x28,p,0)){WmspFreeBufOfWL(p);if(*(int*)(w+0x154c)){u16*b=__VENEER_WMSP_GetBuffer4Callback2Wm9();b[0]=128;b[1]=8;b[2]=22;b[3]=128;__VENEER_WMSP_ReturnResult2Wm9(b);}}}
