#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct HB HB;struct HB{HB*prev,*next;u16 owner,magic;};typedef struct{HB*first,*last;volatile u16 count;u16 id;} HM;extern u32 __VENEER_OS_DisableIrqMask(u32);extern void __VENEER_OS_EnableIrqMask(u32);

void InitHeapBufMan(HM*m,u32 id);
// @fn InitHeapBufMan
void InitHeapBufMan(HM*m,u32 id){m->first=(HB*)0xffffffff;m->last=(HB*)0xffffffff;m->count=0;m->id=id;}
