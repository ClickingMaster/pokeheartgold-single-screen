#include "canonical-api.h"
#pragma force_active on
// clang-format off
#include <nitro/types.h>
#include <nitro/code32.h>
// clang-format on
#define B(p,o) (*(u8*)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define W(p,o) (*(u32*)((u8*)(p)+(o)))
typedef struct {u8 f0:1,f1:1,f2:1,f3:1,f4:1,f5:1,f6:1,f7:1;} A35214_Flags;
typedef struct {A35214_Flags flags;u8 id;u8 pad[22];u16 tempo,mult,acc,pad2;void*bank;} A35214_Player;
typedef struct A35214_Channel {u8 pad[34];u8 marker;u8 pad2[45];struct A35214_Channel*next;} A35214_Channel;
typedef struct {A35214_Flags flags;u8 priority;u16 wait;u8 pad[32];const u8*base,*pos;u8 tail[12];u32 time;A35214_Channel*channel;} A35214_Track;
extern u8 SNDi_Work[];extern u8*SNDi_SharedWork;
extern u8 seqCache[];
typedef struct {u32 enable;const u8*start,*end;u32 words[4];} A35214_Cache;
extern A35214_Cache sMmlPrintEnable;
extern void FinishPlayer(A35214_Player*),UpdateTrackChannel(A35214_Track*,A35214_Player*,int),ReleaseTrackChannelAll(A35214_Track*,A35214_Player*,int),FreeTrackChannelAll(A35214_Track*),SetTrackMute(A35214_Track*,A35214_Player*,int);
extern A35214_Track*GetPlayerTrack(A35214_Player*,int);
extern int PlayerSeqMain(A35214_Player*,int),AllocTrack(void),ReadByte(A35214_Track*);
extern void InitTrack(A35214_Track*),InitCache(const u8*),SND_PrepareSeq(int,const u8*,u32,void*);
extern void SND_StopIntervalTimer(void),SND_StartIntervalTimer(void);
extern u16 SND_CalcRandom(void);
extern void SND_InitLfoParam(void*),SND_FreeExChannel(A35214_Channel*),SND_ReleaseExChannel(A35214_Channel*),SND_SetExChannelRelease(A35214_Channel*,u8);extern int SND_IsExChannelActive(A35214_Channel*);
void StartTrack(A35214_Track*t,const u8*base,u32 offset);
void StartTrack(A35214_Track*t,const u8*base,u32 offset){t->base=base;t->pos=base+offset;}
