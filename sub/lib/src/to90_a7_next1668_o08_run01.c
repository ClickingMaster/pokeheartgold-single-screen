#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct {u8 f0:1,f1:1,f2:1,f3:1,f4:1,f5:1,f6:1,f7:1;} Flags;
typedef struct {Flags flags;u8 id;u8 state[6];u8 tracks[16];u16 tempo,mult,acc,pad;void *bank;} Player;
typedef struct {Flags flags;u8 priority;u16 wait;u8 before[26];u16 channels;u8 after[4];const u8 *base,*pos;u8 tail[12];u32 time;void *channel;} Track;
extern u8 SNDi_Work[];
Track *GetPlayerTrack(Player *,int);
void SND_SetTrackAllocatableChannel(int,u32,u32);
// @fn SND_SetTrackAllocatableChannel
void SND_SetTrackAllocatableChannel(int player,u32 mask,u32 channel){int i;for(i=0;i<16&&mask;i++,mask>>=1){if(mask&1){Track *t=GetPlayerTrack((Player *)(SNDi_Work+0x540)+player,i);if(t){t->channels=(u16)channel;t->flags.f7=1;}}}}
