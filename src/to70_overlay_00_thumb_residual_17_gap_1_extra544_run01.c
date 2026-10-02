#pragma force_active on
#include "global.h"
typedef struct {u8 pad[0x110c];u8 audio[0x8e4];int muted;u8 p19f4[100];u8 state,restart;u8 gap[2];u8 params[32];} Audio;
typedef struct{u32 pad;Audio *audio;} Root;
extern Root _0221A684;
extern void ov00_021E73E4(int,void *);
extern void MIC_StartAutoSamplingAsync(void *,void (*)(int,void *),void *);extern u8 *MIC_GetLastSamplingAddress(void);extern void VCT_SendAudio(void *,u32);extern void VCT_ReceiveAudio(void *,u32,int);
void ov00_021E73E8(int,u32,void **,u32,int,u8 *);
void ov00_021E73E8(int enabled,u32 count,void **buffers,u32 size,int unused,u8 *address){u32 i;if(!enabled){for(i=0;i<count;i++)MI_CpuFill8(buffers[i],0,size);return;}if(_0221A684.audio->restart){MIC_StartAutoSamplingAsync(_0221A684.audio->params,ov00_021E73E4,0);_0221A684.audio->restart=0;}if((u32)(MIC_GetLastSamplingAddress()-address)<size)address+=size;if((*(volatile u16*)0x027fffa8&0x8000)>>15)address=_0221A684.audio->audio;if(!_0221A684.audio->muted)VCT_SendAudio(address,size);for(i=0;i<count;i++)VCT_ReceiveAudio(buffers[i],size,0);_0221A684.audio->state=2;}
