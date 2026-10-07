#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct HB HB;struct HB{HB*prev,*next;u16 owner,magic;};typedef struct{HB*first,*last;volatile u16 count;u16 id;} HM;extern u32 __VENEER_OS_DisableIrqMask(u32);extern void __VENEER_OS_EnableIrqMask(u32);
extern void ReleaseAllHeapBuf(HM*);
void ReleaseAllWlHeapBuf(void);
// @fn ReleaseAllWlHeapBuf
void ReleaseAllWlHeapBuf(void){u8*w=*(u8*volatile*)0x380fff4;ReleaseAllHeapBuf((HM*)(w+0x194));ReleaseAllHeapBuf((HM*)(w+0x1a0));ReleaseAllHeapBuf((HM*)(w+0x1ac));ReleaseAllHeapBuf((HM*)(w+0x1b8));ReleaseAllHeapBuf((HM*)(w+0x1c4));ReleaseAllHeapBuf((HM*)(w+0x1d0));ReleaseAllHeapBuf((HM*)(w+0x1dc));ReleaseAllHeapBuf((HM*)(w+0x1e8));}
