#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
extern void SNDi_LockMutex(void),SNDi_UnlockMutex(void);
void *SND_GetWaveDataAddress(u8 *bank,int i);
// @fn SND_GetWaveDataAddress
void *SND_GetWaveDataAddress(u8 *bank,int i){u8 *addr;SNDi_LockMutex();addr=((u8 **)(bank+0x3c))[i];if(addr){if((u32)addr<0x02000000)addr=bank+(u32)addr;}else addr=0;SNDi_UnlockMutex();return (void*)addr;}
