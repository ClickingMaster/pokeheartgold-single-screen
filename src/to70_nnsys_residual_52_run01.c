#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
typedef struct Group {u8 pad[0x1c];u32 channels;} Group;
typedef struct Player {void *handle;Group *group;u8 pad[0x14];u8 fader[16];u8 state,x2d,x2e,prepared;u32 tag;u8 p34[8];u8 id;} Player;
void SND_PrepareSeq(u32,void*,u32,void*);void SND_SetTrackAllocatableChannel(u32,u32,u32);u32 SND_GetCurrentCommandTag(void);
void InitPlayer(Player*);void ForceStopSeq(Player*);void NNSi_SndFaderSet(void*,int,int);void SetPlayerPriority(Player*,int);
void NNSi_SndPlayerStartSeq(Player*,void*,u32,void*);void NNSi_SndPlayerStopSeq(Player*,int);
void NNSi_SndPlayerStartSeq(Player*p,void *data,u32 offset,void *bank){Group *group=p->group;SND_PrepareSeq(p->id,data,offset,bank);if(group->channels)SND_SetTrackAllocatableChannel(p->id,0xffff,group->channels);InitPlayer(p);p->tag=SND_GetCurrentCommandTag();p->prepared=1;p->state=1;}
void NNSi_SndPlayerStopSeq(Player*p,int frames){if(!p || !p->state)return;if(!frames){ForceStopSeq(p);return;}NNSi_SndFaderSet(p->fader,0,frames);SetPlayerPriority(p,0);p->state=2;}
