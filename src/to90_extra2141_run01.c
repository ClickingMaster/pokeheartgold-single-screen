#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
extern OSMessageQueue _021E4228;
void *WmGetCommandBuffer4Arm7(void);
void *WmGetCommandBuffer4Arm7(void){OSMessage message;if(!OS_ReceiveMessage(&_021E4228,&message,0))return 0;DC_InvalidateRange(message,2);if(*(u16*)message&0x8000)return message;OS_JamMessage(&_021E4228,message,1);return 0;}
