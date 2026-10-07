#include "canonical-api.h"
#pragma force_active on
// clang-format off
#include <nitro/types.h>
#include <nitro/code32.h>
// clang-format on
#define B(p,o) (*(u8*)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define W(p,o) (*(u32*)((u8*)(p)+(o)))
typedef struct {u8 f0:1,f1:1,f2:1,f3:1,f4:1,f5:1,f6:1,f7:1;} A35193_Flags;
typedef struct {A35193_Flags flags;u8 id;u8 pad[22];u16 tempo,mult,acc,pad2;void*bank;} A35193_Player;
typedef struct A35193_Channel {u8 pad[34];u8 marker;u8 pad2[45];struct A35193_Channel*next;} A35193_Channel;
typedef struct {A35193_Flags flags;u8 priority;u16 wait;u8 pad[32];const u8*base,*pos;u8 tail[12];u32 time;A35193_Channel*channel;} A35193_Track;
extern u8 SNDi_Work[];extern u8*SNDi_SharedWork;
extern u8 seqCache[];
typedef struct {u32 enable;const u8*start,*end;u32 words[4];} A35193_Cache;
extern A35193_Cache sMmlPrintEnable;
extern void FinishPlayer(A35193_Player*),UpdateTrackChannel(A35193_Track*,A35193_Player*,int),ReleaseTrackChannelAll(A35193_Track*,A35193_Player*,int),FreeTrackChannelAll(A35193_Track*),SetTrackMute(A35193_Track*,A35193_Player*,int);
extern A35193_Track*GetPlayerTrack(A35193_Player*,int);
extern int PlayerSeqMain(A35193_Player*,int),AllocTrack(void),ReadByte(A35193_Track*);
extern void InitTrack(A35193_Track*),InitCache(const u8*),SND_PrepareSeq(int,const u8*,u32,void*);
extern void SND_StopIntervalTimer(void),SND_StartIntervalTimer(void);
extern u16 SND_CalcRandom(void);
extern void SND_InitLfoParam(void*),SND_FreeExChannel(A35193_Channel*),SND_ReleaseExChannel(A35193_Channel*),SND_SetExChannelRelease(A35193_Channel*,u8);extern int SND_IsExChannelActive(A35193_Channel*);
u16 CalcRelease(int x);
u16 CalcRelease(int x){u16 result;if(x==127)result=65535;else if(x==126)result=0x3c00;else if(x<50)result=(u16)((u32)x*2+1);else result=(u16)(0x1e00/(s32)(126u-(u32)x));return result;}
