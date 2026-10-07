#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct {u8 a,speed,depth,range;u16 delay,count;volatile u16 phase;} Lfo35191;
void SND_UpdateLfo(Lfo35191 *p);
// @fn SND_UpdateLfo
void SND_UpdateLfo(Lfo35191 *p){u32 q;if(p->count<p->delay){p->count++;return;}q=p->phase;q=(q+((u32)p->speed<<6))>>8;while(q>=128)q-=128;p->phase+=p->speed<<6;p->phase&=255;p->phase|=q<<8;}
