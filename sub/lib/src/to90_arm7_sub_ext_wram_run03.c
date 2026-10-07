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
void WMSP_RequestResumeMP(void);
// @fn WMSP_RequestResumeMP
void WMSP_RequestResumeMP(void){u8*s=*(u8**)(wmspW+0x1550);u32*p=WMSP_GetInternalRequestBuf();int ok;if(p){p[0]=45;p[1]=*(u16*)(s+0x68);ok=__VENEER_OS_SendMessage(wmspW+0x88,p,0);}else ok=0;if(ok){*(u16*)(s+0x66)=1;}else{u16*b=__VENEER_WMSP_GetBuffer4Callback2Wm9();b[0]=128;b[1]=8;b[2]=22;b[3]=45;__VENEER_WMSP_ReturnResult2Wm9(b);}}
