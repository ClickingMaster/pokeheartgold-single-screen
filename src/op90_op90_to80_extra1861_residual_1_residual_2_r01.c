#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct State{s16 sample;u8 index;} State;
extern s8 cAdpcmIndexTable3[];extern s16 cAdpcmStepSizeTable[];
static inline u8 Encode3(s16*in,State*s){s32 previous,step,index,diff,code,sample;
previous=s->sample;diff=*in-previous;index=s->index;
if(diff<0){code=4;diff=-diff;}else code=0;
step=cAdpcmStepSizeTable[index];
if(diff>=step){code|=2;diff-=step;}
if(diff>=(step>>1))code|=1;
diff=step>>2;if(code&2)diff+=step;if(code&1)diff+=step>>1;if(code&4)diff=-diff;
sample=previous+diff;if(sample<-32768)sample=-32768;if(sample>32767)sample=32767;
index+=cAdpcmIndexTable3[code];if(index<0)index=0;else if(index>88)index=88;
s->sample=(s16)sample;s->index=index;return code;}
void vct_encode_adpcm_24(u8*,s16*,u32,State*);
// @fn vct_encode_adpcm_24
void vct_encode_adpcm_24(u8*out,s16*in,u32 size,State*s){u32 i;for(i=0;i<size/8;i++){u8 c0,c1,c2,c3,c4,c5,c6,c7,t;
c0=Encode3(in++,s);t=c0<<5;c1=Encode3(in++,s);t|=c1<<2;c2=Encode3(in++,s);out[0]=t|(c2>>1);
c3=Encode3(in++,s);t=c3<<4;c4=Encode3(in++,s);t|=c4<<1;c5=Encode3(in++,s);out[1]=(c2<<7)|t|(c5>>2);
c6=Encode3(in++,s);t=c6<<3;c7=Encode3(in++,s);t|=c7;out[2]=(c5<<6)|t;out+=3;}}
