#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct { u8 id,type,state,flags;u8 p04[12];s32 env;u8 p14[8];u8 attack,sustain;u16 decay,release;u8 p22[22];u32 wave[3];u32 data;void *cb,*arg;u8 p50[4]; } Ch35184;
void SND_FreeExChannel(Ch35184 *p);
// @fn SND_FreeExChannel
void SND_FreeExChannel(Ch35184 *p){if(p){p->cb=0;p->arg=0;}}
