#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct HB HB;struct HB{HB*prev,*next;u16 owner,magic;};typedef struct{HB*first,*last;volatile u16 count;u16 id;} HM;extern u32 __VENEER_OS_DisableIrqMask(u32);extern void __VENEER_OS_EnableIrqMask(u32);

int AddHeapBuf(HM*m,HB*b);
// @fn AddHeapBuf
int AddHeapBuf(HM*m,HB*b){u32 irq;if(b->magic!=0xbf1d)return 1;if(b->owner)return 2;irq=__VENEER_OS_DisableIrqMask(0x1000000);if(m->count==0){b->prev=(HB*)0xffffffff;m->first=b;}else{HB*last=m->last;b->prev=last;last->next=b;}b->next=(HB*)0xffffffff;b->owner=m->id;m->last=b;m->count++;__VENEER_OS_EnableIrqMask(irq);return 0;}
