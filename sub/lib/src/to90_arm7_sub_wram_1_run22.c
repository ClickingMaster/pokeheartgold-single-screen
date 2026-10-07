#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct { u8 id,type,state;u8 active:1;u8 remain:7;u8 p04[12];s32 env;u8 p14[8];u8 attack,sustain;u16 decay,release;u8 p22[22];u32 wave[3];u32 data;void *cb,*arg;u8 p50[4]; } Ch35182;
int SND_IsExChannelActive(Ch35182 *p);
// @fn SND_IsExChannelActive
int SND_IsExChannelActive(Ch35182 *p){return p->active;}
