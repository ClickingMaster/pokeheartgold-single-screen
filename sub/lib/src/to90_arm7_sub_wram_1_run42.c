#include "canonical-api.h"
#pragma force_active on
// clang-format off
#include <nitro/types.h>
#include <nitro/code32.h>
// clang-format on
#define B(p,o) (*(u8*)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define W(p,o) (*(u32*)((u8*)(p)+(o)))
typedef struct {u8 f0:1,f1:1,f2:1,f3:1,f4:1,f5:1,f6:1,f7:1;} A35207_Flags;
typedef struct {A35207_Flags flags;u8 id;u8 pad[22];u16 tempo,mult,acc,pad2;void*bank;} A35207_Player;
typedef struct A35207_Channel {u8 pad[34];u8 marker;u8 pad2[45];struct A35207_Channel*next;} A35207_Channel;
typedef struct {A35207_Flags flags;u8 priority;u16 wait;u8 pad[32];const u8*base,*pos;u8 tail[12];u32 time;A35207_Channel*channel;} A35207_Track;
extern u8 SNDi_Work[];extern u8*SNDi_SharedWork;
extern u8 seqCache[];
typedef struct {u32 enable;const u8*start,*end;u32 words[4];} A35207_Cache;
extern A35207_Cache sMmlPrintEnable;
extern void FinishPlayer(A35207_Player*),UpdateTrackChannel(A35207_Track*,A35207_Player*,int),ReleaseTrackChannelAll(A35207_Track*,A35207_Player*,int),FreeTrackChannelAll(A35207_Track*),SetTrackMute(A35207_Track*,A35207_Player*,int);
extern A35207_Track*GetPlayerTrack(A35207_Player*,int);
extern int PlayerSeqMain(A35207_Player*,int),AllocTrack(void),ReadByte(A35207_Track*);
extern void InitTrack(A35207_Track*),InitCache(const u8*),SND_PrepareSeq(int,const u8*,u32,void*);
extern void SND_StopIntervalTimer(void),SND_StartIntervalTimer(void);
extern u16 SND_CalcRandom(void);
extern void SND_InitLfoParam(void*),SND_FreeExChannel(A35207_Channel*),SND_ReleaseExChannel(A35207_Channel*),SND_SetExChannelRelease(A35207_Channel*,u8);extern int SND_IsExChannelActive(A35207_Channel*);
void SND_InvalidateBank(const void*start,const void*end);
void SND_InvalidateBank(const void*start,const void*end){int i;for(i=0;i<16;i++){A35207_Player*p=(A35207_Player*)(SNDi_Work+0x540)+i;if(p->flags.f0&&(u32)start<=(u32)p->bank&&(u32)p->bank<=(u32)end)FinishPlayer(p);}}
