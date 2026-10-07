#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
#define H(a) (*(volatile u16 *)(a))
#define WL (*(u8 **)0x0380fff4)
#define R16(p,o) (*(u16 *)((u8 *)(p)+(o)))
#define R32(p,o) (*(u32 *)((u8 *)(p)+(o)))
#define P(p,o) (*(u8 **)((u8 *)(p)+(o)))
extern u32 __VENEER_OS_DisableIrqMask(u32),__VENEER_OS_EnableIrqMask(u32);extern void CAM_SetTIMElementBitmap(u32),MoveHeapBuf(void*,void*,void*);
void CAM_AddBcFrame(void *from,void *frame);
// @fn CAM_AddBcFrame
void CAM_AddBcFrame(void *from,void *frame){u8 *queue=WL+0x1ac;u32 mask=__VENEER_OS_DisableIrqMask(0x1000000);if(R16(queue,8)==0)CAM_SetTIMElementBitmap(0);MoveHeapBuf(from,queue,frame);__VENEER_OS_EnableIrqMask(mask);}
