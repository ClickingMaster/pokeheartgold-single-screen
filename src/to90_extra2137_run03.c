#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct {u32 unused[2];u8 *out;} SpiWork;
extern u8 _021E4088[];
void CARDi_SpiWaitReadByte(SpiWork *work);
void CARDi_SpiWaitReadByte(SpiWork *work){u64 start=OS_GetTick();while((OS_GetTick()-start)*64000/33514 < _021E4088[2]){} *(volatile u16*)0x040001a2=0;while(*(volatile u16*)0x040001a0&0x80){} *work->out=*(volatile u16*)0x040001a2;work->out++;}
