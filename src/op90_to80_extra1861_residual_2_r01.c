#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct State{s16 sample;u8 index;} State;
extern s16 cAdpcmStepSizeTable[];extern s8 cAdpcmIndexTable2[];
void vct_encode_adpcm_16(u8*out,const s16*in,u32 count,State*s);
static inline u8 Encode2(const s16 *in, State *s) {
 int index; int diff; int delta; int code; int smp; int step;
 delta=*in-s->sample;index=s->index;if(delta<0){code=2;delta=-delta;}else code=0;step=cAdpcmStepSizeTable[index];if(delta>=step)code|=1;if(code&1)diff=step;else diff=0;if(code&2)diff=-diff;smp=s->sample+diff;if(smp<-32768)smp=-32768;if(smp>32767)smp=32767;index+=cAdpcmIndexTable2[code];if(index<0)index=0;else if(index>88)index=88;s->sample=smp;s->index=index;
 return code;
}
// @fn vct_encode_adpcm_16
void vct_encode_adpcm_16(u8*out,const s16*in,u32 count,State*s){
 u32 i;
 for(i=0;i<count>>2;i++){
  u8 c;c=Encode2(in++,s);c|=Encode2(in++,s)<<2;c|=Encode2(in++,s)<<4;c|=Encode2(in++,s)<<6;
  *out++=c;
 }
}
