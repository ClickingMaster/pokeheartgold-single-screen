#include "canonical-api.h"
#pragma force_active on
// clang-format off
#include <nitro/types.h>
#include <nitro/code32.h>
// clang-format on
#define B(p,o) (*(u8*)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define W(p,o) (*(u32*)((u8*)(p)+(o)))
typedef struct {u8 f0:1,f1:1,f2:1,f3:1,f4:1,f5:1,f6:1,f7:1;} A35208_Flags;
typedef struct {A35208_Flags flags;u8 id;u8 pad[22];u16 tempo,mult,acc,pad2;void*bank;} A35208_Player;
typedef struct A35208_Channel {u8 pad[34];u8 marker;u8 pad2[45];struct A35208_Channel*next;} A35208_Channel;
typedef struct {A35208_Flags flags;u8 priority;u16 wait;u8 pad[32];const u8*base,*pos;u8 tail[12];u32 time;A35208_Channel*channel;} A35208_Track;
extern u8 SNDi_Work[];extern u8*SNDi_SharedWork;
extern u8 seqCache[];
typedef struct {u32 enable;const u8*start,*end;u32 words[4];} A35208_Cache;
extern A35208_Cache sMmlPrintEnable;
extern void FinishPlayer(A35208_Player*),UpdateTrackChannel(A35208_Track*,A35208_Player*,int),ReleaseTrackChannelAll(A35208_Track*,A35208_Player*,int),FreeTrackChannelAll(A35208_Track*),SetTrackMute(A35208_Track*,A35208_Player*,int);
extern A35208_Track*GetPlayerTrack(A35208_Player*,int);
extern int PlayerSeqMain(A35208_Player*,int),AllocTrack(void),ReadByte(A35208_Track*);
extern void InitTrack(A35208_Track*),InitCache(const u8*),SND_PrepareSeq(int,const u8*,u32,void*);
extern void SND_StopIntervalTimer(void),SND_StartIntervalTimer(void);
extern u16 SND_CalcRandom(void);
extern void SND_InitLfoParam(void*),SND_FreeExChannel(A35208_Channel*),SND_ReleaseExChannel(A35208_Channel*),SND_SetExChannelRelease(A35208_Channel*,u8);extern int SND_IsExChannelActive(A35208_Channel*);
void SNDi_SetPlayerParam(int player,u32 offset,u32 value,int size);
void SNDi_SetPlayerParam(int player,u32 offset,u32 value,int size){u8*p=SNDi_Work+0x540+player*36;switch(size){case 1:*(u8*)(p+offset)=value;break;case 2:*(u16*)(p+offset)=value;break;case 4:*(u32*)(p+offset)=value;break;}}
