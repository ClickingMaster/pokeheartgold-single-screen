#include "canonical-api.h"
#pragma force_active on
// clang-format off
#include <nitro/types.h>
#include <nitro/code32.h>
// clang-format on
#define B(p,o) (*(u8*)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define W(p,o) (*(u32*)((u8*)(p)+(o)))
typedef struct {u8 f0:1,f1:1,f2:1,f3:1,f4:1,f5:1,f6:1,f7:1;} A35209_Flags;
typedef struct {A35209_Flags flags;u8 id;u8 pad[22];u16 tempo,mult,acc,pad2;void*bank;} A35209_Player;
typedef struct A35209_Channel {u8 pad[34];u8 marker;u8 pad2[45];struct A35209_Channel*next;} A35209_Channel;
typedef struct {A35209_Flags flags;u8 priority;u16 wait;u8 pad[32];const u8*base,*pos;u8 tail[12];u32 time;A35209_Channel*channel;} A35209_Track;
extern u8 SNDi_Work[];extern u8*SNDi_SharedWork;
extern u8 seqCache[];
typedef struct {u32 enable;const u8*start,*end;u32 words[4];} A35209_Cache;
extern A35209_Cache sMmlPrintEnable;
extern void FinishPlayer(A35209_Player*),UpdateTrackChannel(A35209_Track*,A35209_Player*,int),ReleaseTrackChannelAll(A35209_Track*,A35209_Player*,int),FreeTrackChannelAll(A35209_Track*),SetTrackMute(A35209_Track*,A35209_Player*,int);
extern A35209_Track*GetPlayerTrack(A35209_Player*,int);
extern int PlayerSeqMain(A35209_Player*,int),AllocTrack(void),ReadByte(A35209_Track*);
extern void InitTrack(A35209_Track*),InitCache(const u8*),SND_PrepareSeq(int,const u8*,u32,void*);
extern void SND_StopIntervalTimer(void),SND_StartIntervalTimer(void);
extern u16 SND_CalcRandom(void);
extern void SND_InitLfoParam(void*),SND_FreeExChannel(A35209_Channel*),SND_ReleaseExChannel(A35209_Channel*),SND_SetExChannelRelease(A35209_Channel*,u8);extern int SND_IsExChannelActive(A35209_Channel*);
void SNDi_SetTrackParam(int player,u32 mask,u32 offset,u32 value,int size);
void SNDi_SetTrackParam(int player,u32 mask,u32 offset,u32 value,int size){int i;for(i=0;i<16&&mask;i++,mask>>=1){if(mask&1){A35209_Track*t=GetPlayerTrack((A35209_Player*)(SNDi_Work+0x540)+player,i);if(t){switch(size){case 1:B(t,offset)=value;break;case 2:H(t,offset)=value;break;case 4:W(t,offset)=value;break;}}}}}
