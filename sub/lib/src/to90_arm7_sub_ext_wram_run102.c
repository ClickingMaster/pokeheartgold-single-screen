#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct HB HB;struct HB{HB*prev,*next;u16 owner,magic;};typedef struct{HB*first,*last;volatile u16 count;u16 id;} HM;extern u32 __VENEER_OS_DisableIrqMask(u32);extern void __VENEER_OS_EnableIrqMask(u32);
extern int NewHeapBuf(HM*,HB*);extern void AddTask(u32,u32);
void ExecuteMessage(HB**message);
// @fn ExecuteMessage
void ExecuteMessage(HB**message){if(*message){NewHeapBuf((HM*)(*(u8*volatile*)0x380fff4+0x200),*message);AddTask(2,11);}}
