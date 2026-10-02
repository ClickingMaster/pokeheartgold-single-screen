#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
struct DriverState { s8 bank; u8 pad[3]; int first; u32 tag; };
extern struct DriverState _021DD420;
extern u8 _021DD460[];
extern void *SND_RecvCommandReply(u32);
extern BOOL SND_IsFinishedCommandTag(u32);
extern void SND_ReadDriverInfo(void *);
extern u32 SND_GetCurrentCommandTag(void);
extern BOOL SND_FlushCommand(u32);
extern BOOL SND_ReadChannelInfo(const void *,int,void *);
extern BOOL SND_ReadTrackInfo(const void *,int,int,void *);
BOOL NNS_SndUpdateDriverInfo(void);
BOOL NNS_SndReadDriverChannelInfo(int,struct SNDChannelInfo *);
BOOL NNSi_SndReadDriverTrackInfo(int,int,void *);
BOOL NNS_SndUpdateDriverInfo(void) { if(!_021DD420.first) { while(SND_RecvCommandReply(0)) {} if(!SND_IsFinishedCommandTag(_021DD420.tag)) return FALSE; if(_021DD420.bank<0) _021DD420.bank=1; SND_ReadDriverInfo(_021DD460+_021DD420.bank*0x11e0); _021DD420.tag=SND_GetCurrentCommandTag(); if(_021DD420.bank==0) _021DD420.bank=1; else _021DD420.bank=0; DC_InvalidateRange(_021DD460+_021DD420.bank*0x11e0,0x11e0); SND_FlushCommand(0); return TRUE; } else { SND_ReadDriverInfo(_021DD460); _021DD420.tag=SND_GetCurrentCommandTag(); _021DD420.first=0; return FALSE; } }
BOOL NNS_SndReadDriverChannelInfo(int channel,struct SNDChannelInfo *out) { void *info=_021DD420.bank<0?NULL:_021DD460+_021DD420.bank*0x11e0; if(info==NULL) return FALSE; return SND_ReadChannelInfo(info,channel,out); }
BOOL NNSi_SndReadDriverTrackInfo(int player,int track,void *out) { void *info=_021DD420.bank<0?NULL:_021DD460+_021DD420.bank*0x11e0; if(info==NULL) return FALSE; return SND_ReadTrackInfo(info,player,track,out); }
