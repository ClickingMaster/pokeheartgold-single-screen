#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 p[0x110c];u8 buffer[0x880];u8 p2[0x6c];u8 stream[0x80];u32 a,b,c,d,e;u16 values[16];u32 f;} Work;
typedef struct {u32 value;Work*work;} State;
extern State _0221A684;
extern void MIC_Init(void),PM_Init(void),NNS_SndInit(void);
extern void VCT_EnableVAD(int),VCT_EnableEchoCancel(int);
extern void NNS_SndStrmInit(void*);

void ov00_021E733C(void);
void ov00_021E733C(void){int i;MIC_Init();PM_Init();PM_SetAmp((PMAmpSwitch)1);PM_SetAmpGain((PMAmpGain)3);_0221A684.work->a=0;_0221A684.work->d=0;_0221A684.work->b=0;_0221A684.work->c=0;_0221A684.work->e=0;for(i=0;i<16;i++)_0221A684.work->values[i]=0;_0221A684.work->f=0;VCT_EnableVAD(1);NNS_SndInit();NNS_SndStrmInit(_0221A684.work->stream);MIi_CpuClearFast(0,(u32*)_0221A684.work->buffer,0x880);_0221A684.value=0;VCT_EnableEchoCancel(1);}
