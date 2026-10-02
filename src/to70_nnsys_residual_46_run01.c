#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
typedef struct Wave {int channel,frequency,active;} Wave;
void SND_SetChannelTimer(u32,u32);void NNS_SndWaveOutSetSpeed(Wave*,int);
void NNS_SndWaveOutSetSpeed(Wave *wave,int speed){if(wave->active){u64 timer=0x7fd87f8000ULL/wave->frequency/speed;if(timer<16)timer=16;else if(timer>65535)timer=65535;SND_SetChannelTimer(1<<wave->channel,(u32)timer);}}
