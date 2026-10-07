#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
#define H(a) (*(volatile u16 *)(a))
#define W(a) (*(volatile u32 *)(a))
extern void MicTimerHandler(void);
void MIC_TimerHandler(void);
// @fn MIC_TimerHandler
void MIC_TimerHandler(void){MicTimerHandler();W(0x0380fff8)|=64;W(0x04000214)=64;}
