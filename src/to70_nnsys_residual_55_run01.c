#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
#pragma force_active on
typedef struct {u8 p[8];u8 pre[12],post[12],pad20[4];union {u32 flags;struct {u32 init:1; s32 active:1;u32 other:30;} bits;} state;u8 pad[20];int volume,alarm;u32 channels;int count;u8 ids[16];} Strm;
extern int SND_CalcChannelVolume(int);
extern void SND_SetChannelVolume(u32,int,int),SND_StopTimer(u32,u32,u32,u32),PM_DeletePreSleepCallback(void *),PM_DeletePostSleepCallback(void *),SND_WaitForCommandProc(u32);
extern u32 SND_GetCurrentCommandTag(void);
extern BOOL SND_FlushCommand(BOOL);
extern void NNS_FndRemoveListObject(NNSFndList *,void *);
extern s32 _021E0294[][2];extern NNSFndList _021E0248;
extern void NNS_SndFreeAlarm(int);
void NNS_SndStrmSetVolume(Strm *,int),ForceStopStrm(Strm *),ShutdownStrm(Strm *);
void ForceStopStrm(Strm *s){u32 tag;if(s->state.bits.active){SND_StopTimer(s->channels,0,1<<s->alarm,0);PM_DeletePreSleepCallback(&s->pre);PM_DeletePostSleepCallback(&s->post);s->state.flags&=~2;tag=SND_GetCurrentCommandTag();SND_FlushCommand(1);SND_WaitForCommandProc(tag);}ShutdownStrm(s);}
void ShutdownStrm(Strm *s){NNS_SndFreeAlarm(s->alarm);NNS_FndRemoveListObject(&_021E0248,s);s->state.flags&=~1;}
