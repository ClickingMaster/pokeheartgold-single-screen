#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct State{s16 sample;u8 index;} State;
extern s16 cAdpcmStepSizeTable[];extern s8 cAdpcmIndexTable4[],cAdpcmIndexTable2[];
static inline s16 Decode2(int code,State*s){s32 previous=s->sample,index=s->index,step=cAdpcmStepSizeTable[index];if(!(code&1))step=0;if(code&2){previous-=step;if(previous<-32768)previous=-32768;}else{previous+=step;if(previous>32767)previous=32767;}index+=cAdpcmIndexTable2[code];if(index<0)index=0;else if(index>88)index=88;s->sample=(s16)previous;s->index=index;return (s16)previous;}
void vct_encode_adpcm_16(u8*,s16*,u32,State*);
void vct_encode_adpcm_32(u8*,s16*,u32,State*);void vct_decode_adpcm_16(u8*,s16*,u32,State*);

#pragma force_active on
void vct_decode_adpcm_16(u8*in,s16*out,u32 count,State*s){int i;for(i=0;i<count;i++){*out++=Decode2(*in&3,s);*out++=Decode2((*in>>2)&3,s);*out++=Decode2((*in>>4)&3,s);*out++=Decode2((*in>>6)&3,s);in++;}}
