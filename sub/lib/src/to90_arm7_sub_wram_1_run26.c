#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct { u8 id,type,state,flags;u8 p04[12];s32 env;u8 p14[8];u8 attack,sustain;u16 decay,release;u8 p22[22];u32 wave[3];u32 data;void *cb,*arg;u8 p50[4]; } Ch35187;
extern u32 sWeakLockChannel[2];
void SND_UnlockChannel(u32 mask,u32 v);
// @fn SND_UnlockChannel
void SND_UnlockChannel(u32 mask,u32 v){if(v&1)sWeakLockChannel[0]&=~mask;else sWeakLockChannel[1]&=~mask;}
