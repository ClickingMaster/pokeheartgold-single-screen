#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct HB HB;struct HB{HB*prev,*next;u16 owner,magic;};typedef struct{HB*first,*last;volatile u16 count;u16 id;} HM;extern u32 __VENEER_OS_DisableIrqMask(u32);extern void __VENEER_OS_EnableIrqMask(u32);

int DeleteHeapBuf(HM*m,HB*b);
// @fn DeleteHeapBuf
int DeleteHeapBuf(HM*m,HB*b){u32 irq;if(b->magic!=0xbf1d)return 1;if(b->owner!=m->id)return 2;irq=__VENEER_OS_DisableIrqMask(0x1000000);m->count--;if(m->count==0){m->first=(HB*)0xffffffff;m->last=(HB*)0xffffffff;}else if(b==m->first){HB*next=b->next;m->first=next;next->prev=(HB*)0xffffffff;}else{HB*prev=b->prev;if(b==m->last){m->last=prev;prev->next=(HB*)0xffffffff;}else{b->next->prev=prev;b->prev->next=b->next;}}b->owner=0;__VENEER_OS_EnableIrqMask(irq);return 0;}
