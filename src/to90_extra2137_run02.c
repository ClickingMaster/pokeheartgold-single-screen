#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct{u32 pad;const u8*ptr;}Card;extern u8 _021E4088[];
u8 CARDi_SpiWaitWriteByte(Card*);
u8 CARDi_SpiWaitWriteByte(Card*c){u64 start=OS_GetTick();volatile u16 read;while((OS_GetTick()-start)*64000/33514<_021E4088[1]){}*(volatile u16*)0x040001a2=*c->ptr;c->ptr++;while(*(volatile u16*)0x040001a0&128){}read=*(volatile u16*)0x040001a2;return read&255;}
