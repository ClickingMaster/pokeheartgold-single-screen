#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
extern const int ulaw_segment[];
void vct_encode_g711_ulaw(u8*,const s16*,u32);
static inline int segment(int sample){const int *p=ulaw_segment;int i;for(i=0;i<8;i++)if(sample<=*p++)return i;return 8;}
static inline u8 enc(int sample){int mask,seg;sample>>=2;if(sample<0){mask=127;sample=-sample;}else mask=255;if(sample>8159)sample=8159;seg=segment(sample+33);if(seg>=8)return mask^127;return ((((sample+33)>>(seg+1))&15)|(seg<<4))^mask;}
static inline u8 enc2(s16 pcm){int mask,seg,sample;sample=pcm>>2;if(sample<0){mask=127;sample=-sample;}else mask=255;if(sample>8159)sample=8159;seg=segment(sample+33);if(seg>=8)return (u8)(mask^127);return (u8)((((sample+33)>>(seg+1))&15)|(seg<<4))^mask;}
static inline int enc3(int sample){int mask,seg;sample>>=2;if(sample<0){mask=127;sample=-sample;}else mask=255;if(sample>8159)sample=8159;seg=segment(sample+33);if(seg>=8)return mask^127;seg=(((sample+33)>>(seg+1))&15)|(seg<<4);return seg^mask;}
// @fn vct_encode_g711_ulaw
void vct_encode_g711_ulaw(u8 *out,const s16 *in,u32 count){
 int i;
 for(i=0;i<count;i++){int s=in[i];out[i]=enc3(s);}
}
