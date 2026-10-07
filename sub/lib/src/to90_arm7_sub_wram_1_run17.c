#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct { u8 id,type,state,flags;u8 p04[12];s32 env;u8 p14[8];u8 attack,sustain;u16 decay,release;u8 p22[22];u32 wave[3];u32 data;void *cb,*arg;u8 p50[4]; } Ch35175;
extern void StartExChannel(Ch35175 *,int);
int SND_StartExChannelNoise(Ch35175 *p,int v);
// @fn SND_StartExChannelNoise
int SND_StartExChannelNoise(Ch35175 *p,int v){if(p->id<14)return 0;if(p->id>15)return 0;p->type=2;*(u16 *)((u8 *)p+60)=8006;StartExChannel(p,v);return 1;}
