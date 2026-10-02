#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
struct Stream { u8 pad[32]; int format; struct { signed int enabled:1; signed int running:1; signed int rest:30; } flags; u32 bytes,blocks; void (*callback)(int,int,void **,u32,int,void *); void *user; int current; u32 pad3c; int alarm; u32 channels; int count; u8 ids[16]; };
struct Buffer { u8 *pointer; u32 pad; };
extern struct Buffer _021E0294[];
extern void *_021E0254[];
extern u8 _021E0314[],_021E035C[];
extern void SND_StopTimer(u32,u32,u32,u32);
extern void SND_StartTimer(u32,u32,u32,u32);
extern u32 SND_GetCurrentCommandTag(void);
extern BOOL SND_FlushCommand(u32);
extern void SND_WaitForCommandProc(u32);
extern void NNSi_SndCaptureStop(void);
extern void NNSi_SndFaderSet(void *,int,int);
void StrmCallback(struct Stream *,int);
void BeginSleep__Strm(struct Stream *);
void EndSleep__Strm(struct Stream *);
void NNS_SndCaptureStopReverb(int);
extern void PM_PrependPreSleepCallback(void *);
extern void PM_AppendPostSleepCallback(void *);
extern void NNS_SndStrmStop(struct Stream *);
extern int NNS_SndAllocAlarm(void);
extern void NNS_FndAppendListObject(void *,void *);
extern void SND_SetupChannelPcm(int,int,const void *,int,u32,u32,int,int,int,int);
extern void SND_SetupAlarm(int,u32,u32,void (*)(void *),void *);
extern void AlarmCallback(void *);
extern u8 _021E0248[];
BOOL NNS_SndStrmSetup(struct Stream *,int,void *,u32,u32,u32,void (*)(int,int,void **,u32,int,void *),void *);
void NNS_SndStrmStart(struct Stream *);
BOOL NNS_SndStrmSetup(struct Stream *stream,int format,void *buffer,u32 bytes,u32 timer,u32 blocks,void (*callback)(int,int,void **,u32,int,void *),void *user) { u32 alarm,rounded; u32 savedBlocks=blocks; int i; OSIntrMode mode; if(stream->flags.enabled) NNS_SndStrmStop(stream); rounded=bytes/(stream->count*(savedBlocks<<5)); rounded*=savedBlocks; rounded<<=5; stream->bytes=rounded; alarm=(timer*(format==1?(rounded>>1):rounded))/savedBlocks; stream->alarm=NNS_SndAllocAlarm(); if(stream->alarm<0)return FALSE; for(i=0;i<stream->count;i++) { int id=stream->ids[i]; _021E0294[id].pointer=(u8 *)buffer+stream->bytes*i; _021E0294[id].pad=0; SND_SetupChannelPcm(id,format,_021E0294[id].pointer,1,0,stream->bytes>>2,127,0,timer<<5,64); } SND_SetupAlarm(stream->alarm,alarm,alarm,AlarmCallback,stream); NNS_FndAppendListObject(_021E0248,stream); stream->format=format; stream->blocks=blocks; stream->callback=callback; stream->user=user; stream->current=0; stream->pad3c=0; stream->flags.enabled=1; mode=OS_DisableInterrupts(); stream->blocks=1; StrmCallback(stream,0); stream->blocks=blocks; OS_RestoreInterrupts(mode); return TRUE; }
void NNS_SndStrmStart(struct Stream *stream) { SND_StartTimer(stream->channels,0,1<<stream->alarm,0); if(stream->flags.running) return; PM_PrependPreSleepCallback((void *)((u8 *)stream+8)); PM_AppendPostSleepCallback((void *)((u8 *)stream+0x14)); stream->flags.running=1; }
