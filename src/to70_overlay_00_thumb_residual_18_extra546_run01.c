#pragma force_active on
#include "global.h"
typedef struct {u8 p[0x880];void *buf1;u32 z;void *buf2;u8 q[0x198c-0x88c];void (*callback)(void);u8 r[0x19f4-0x1990];u32 heap;u8 stream[4];} Work;
extern struct {u32 x;Work *work;} _0221A684;
extern void MIC_StopAutoSampling(void);extern void NNS_SndStrmStop(void*);extern void NNS_SndStrmFreeChannel(void*);extern void VCT_Cleanup(void);extern void Heap_FreeExplicit(u32,void*);extern void VCT_GetVADInfo(void*);
void ov00_021E7A30(void);int ov00_021E7AA4(void);
void ov00_021E7A30(void){void (*cb)(void);if(_0221A684.work){cb=_0221A684.work->callback;MIC_StopAutoSampling();NNS_SndStrmStop(_0221A684.work->stream);NNS_SndStrmFreeChannel(_0221A684.work->stream);VCT_Cleanup();Heap_FreeExplicit(_0221A684.work->heap,_0221A684.work->buf2);Heap_FreeExplicit(_0221A684.work->heap,_0221A684.work->buf1);_0221A684.work=0;if(cb)cb();}}
int ov00_021E7AA4(void){int info[4];if(_0221A684.work){VCT_GetVADInfo(info);if(info[1]>2)return info[0];}return 0;}
