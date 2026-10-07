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
void WmspKickMPChild(void *a);
#pragma opt_treetransformation off
// @fn WmspKickMPChild
void WmspKickMPChild(void *a){u32*p=WMSP_GetInternalRequestBuf();u8*w;u8*s=*(u8**)(wmspW+0x1550);int ok=0;(void)a;*(u16*)(s+0x5e)=0;*(u16*)(s+0x60)=0;*(u16*)(s+0x88)=0;w=wmspW;if(p){p[0]=44;ok=__VENEER_OS_SendMessage(w+0x88,p,0);}if(!ok&&*(int*)(w+0x154c)){u16*b=__VENEER_WMSP_GetBuffer4Callback2Wm9();b[0]=128;b[1]=8;b[2]=22;b[3]=44;__VENEER_WMSP_ReturnResult2Wm9(b);}}
