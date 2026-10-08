#pragma force_active on
#include "global.h"
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct {u8 pad[5];u8 owner;u16 gap;u32 members;s32 state;} Session;
typedef struct {u16 tick;u8 pad2[2];u32 members;u8 pad8[8];u16 adpcm;u8 index,pad13;u32 reset;u8 pad18[8];u32 active,size;u8 pad28[0xed8];u32 f00,f04,f08,f0c,f10,f14;} State;
typedef struct {u32 words[37];} Stream;
extern State _021D6040;extern Stream sStream[];extern Session*sAudioSession[3];
extern void vct_init_stream(u32*,u32,u32),vct_init_audio_queue(u32),VCT_ResetVAD(void);
int VCT_StartStreaming(Session*);
#ifndef NELEMS
#define NELEMS(a) (sizeof(a)/sizeof((a)[0]))
#endif
#pragma push
#pragma optimization_level 2
// @fn VCT_StartStreaming
int VCT_StartStreaming(Session*s){u32 i; u32 n;if(!s)return 0;if(_021D6040.active==3)return 0;if(s->state!=2&&s->state!=3)return 0;
 n=sizeof(sAudioSession)/sizeof(sAudioSession[0]);
 for(i=0;i<n;i++)if(sAudioSession[i]==s)return 1;for(i=0;i<n;i++)if(sAudioSession[i]==0){sAudioSession[i]=s;break;}
 _021D6040.active++;_021D6040.members|=s->members;vct_init_stream(sStream[i].words,i,s->owner);vct_init_audio_queue(i);
 if(s->state!=2)return 1;if(_021D6040.active==1){VCT_ResetVAD();_021D6040.tick=OS_GetTickLo();_021D6040.adpcm=0;_021D6040.index=0;_021D6040.reset=1;_021D6040.f00=0;_021D6040.f04=0;_021D6040.f0c=0;_021D6040.f08=0;_021D6040.f10=0;_021D6040.f14=0;}return 1;}
#pragma pop
