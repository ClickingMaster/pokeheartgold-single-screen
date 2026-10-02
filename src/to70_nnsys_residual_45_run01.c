#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
#pragma warn_uninitializedvar off
typedef struct Wave {int channel,frequency,active,stopped;u32 tag;} Wave;
typedef enum Format {PCM8,PCM16,OTHER} Format;
void SND_SetupChannelPcm(int,Format,const void*,int,int,int,int,int,u32,int);void SND_StartTimer(u32,u32,u32,u32);u32 SND_GetCurrentCommandTag(void);int NNS_SndWaveOutStart(Wave*,Format,const void*,int,int,int,int,int,int,int);
int NNS_SndWaveOutStart(Wave *wave,Format format,const void *data,int loop,int loopStart,int length,int rate,int volume,int speed,int pan){int start,size;switch(format){case PCM8:start=loopStart>>2;size=(length>>2)-start;break;case PCM16:start=loopStart>>1;size=(length>>1)-start;break;}u64 timer=0x7fd87f8000ULL/rate/speed;if(timer<16)timer=16;else if(timer>65535)timer=65535;SND_SetupChannelPcm(wave->channel,format,data,loop?1:2,start,size,volume,0,(u32)timer,pan);SND_StartTimer(1<<wave->channel,0,0,0);wave->active=1;wave->stopped=0;wave->tag=SND_GetCurrentCommandTag();wave->frequency=rate;return 1;}
