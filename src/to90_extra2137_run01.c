#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct {u8 a,b,c,pad;u32 active;} Card;
extern Card _021E4088;extern u16 _02110FBC;void CARD_SpiWaitInit(void);
void CARD_SpiWaitInit(void){_021E4088.a=_021E4088.b=_021E4088.c=50;_021E4088.active=1;if(_02110FBC==0xfffd)_02110FBC=OS_GetLockID();}
