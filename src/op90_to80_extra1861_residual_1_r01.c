#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct State{s16 sample;u8 index;} State;
extern s8 cAdpcmIndexTable3[];extern s16 cAdpcmStepSizeTable[];
static inline s16 Decode3(int code,State*s){s32 previous=s->sample,index=s->index,step=cAdpcmStepSizeTable[index],diff=step>>2;
if(code&2)diff+=step;if(code&1)diff+=step>>1;if(code&4){previous-=diff;if(previous<-32768)previous=-32768;}else{previous+=diff;if(previous>32767)previous=32767;}index+=cAdpcmIndexTable3[code];if(index<0)index=0;else if(index>88)index=88;s->sample=(s16)previous;s->index=index;return (s16)previous;}
void vct_decode_adpcm_24(u8*,s16*,u32,State*);
// @fn vct_decode_adpcm_24
void vct_decode_adpcm_24(u8*in,s16*out,u32 size,State*s){u32 i;for(i=0;i<size/3;i++){u8 a=in[0],b=in[1],c=in[2];
out[0]=Decode3(a>>5,s);out[1]=Decode3((a>>2)&7,s);out[2]=Decode3(((a<<1)&7)|(b>>7),s);out[3]=Decode3((b>>4)&7,s);out[4]=Decode3((b>>1)&7,s);out[5]=Decode3(((b<<2)&7)|(c>>6),s);out[6]=Decode3((c>>3)&7,s);out[7]=Decode3(c&7,s);in+=3;out+=8;}}
