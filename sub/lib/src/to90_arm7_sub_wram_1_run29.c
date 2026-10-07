#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct {u8 a,speed,depth,range;u16 delay,count;volatile u16 phase;} Lfo35190;
void SND_InitLfoParam(Lfo35190 *p);
// @fn SND_InitLfoParam
void SND_InitLfoParam(Lfo35190 *p){p->a=0;p->depth=0;p->range=1;p->speed=16;p->delay=0;}
