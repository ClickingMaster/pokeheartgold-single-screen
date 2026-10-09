#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct State{s16 sample;u8 index;} State;
extern s16 cAdpcmStepSizeTable[];extern s8 cAdpcmIndexTable4[];
void vct_encode_adpcm_32(u8*out,const s16*in,u32 count,State*s);
static inline u8 Encode4(const s16 *in, State *s) {
 int index; int code; int diff; int step; int delta; int smp;
 delta=*in-s->sample;index=s->index;if(delta<0){code=8;delta=-delta;}else code=0;step=cAdpcmStepSizeTable[index];if(delta>=step){code|=4;delta-=step;}if(delta>=(step>>1)){code|=2;delta-=step>>1;}if(delta>=(step>>2))code|=1;diff=step>>3;if(code&4)diff+=step;if(code&2)diff+=step>>1;if(code&1)diff+=step>>2;if(code&8)diff=-diff;smp=s->sample+diff;if(smp<-32768)smp=-32768;if(smp>32767)smp=32767;index+=cAdpcmIndexTable4[code];if(index<0)index=0;else if(index>88)index=88;s->sample=smp;s->index=index;
 return code;
}
void vct_encode_adpcm_32(u8*out,const s16*in,u32 count,State*s);
// @fn vct_encode_adpcm_32
void vct_encode_adpcm_32(u8*out,const s16*in,u32 count,State*s){
 u32 i;
 for(i=0;i<count>>1;i++){
  u8 c0,c1;c0=Encode4(in++,s);c1=Encode4(in++,s);
  *out++=c0|(c1<<4);
 }
}
