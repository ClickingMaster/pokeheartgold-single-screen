#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
extern void *WmGetCommandBuffer4Arm7(void);extern OSMessageQueue _021E4228;int WMi_SendCommandDirect(const void*,u32);
int WMi_SendCommandDirect(const void*source,u32 size){void*buffer=WmGetCommandBuffer4Arm7();int result;if(!buffer)return 8;MI_CpuCopy8(source,buffer,size);DC_StoreRange(buffer,size);result=PXI_SendWordByFifo((PXIFifoTag)10,(u32)buffer,0);OS_SendMessage(&_021E4228,buffer,OS_MESSAGE_BLOCK);return result<0?8:2;}
