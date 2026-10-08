// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct Channel40224 Channel40224;
typedef struct Track40224 {u8 flags,pad;u16 program;u8 volume,expression;s8 bend;u8 range;s8 pan;u8 gap[10];s8 transpose;u8 gap2[40];Channel40224 *channels;} Track40224;
struct Channel40224 {u8 id;u8 gap[79];Channel40224 *next;};
typedef struct {u8 prefix[8],tracks[16],tail[12];} Player;
typedef struct {Channel40224 channels[16];Player players[16];Track40224 tracks[32];u8 tail[0x240];const u8 *origin;u8 final[0x1c];} Snapshot;
typedef char SnapshotSize[(sizeof(Snapshot)==0x11e0)?1:-1];
typedef char PlayerOffset[(offsetof(Snapshot,players)==0x540)?1:-1];
typedef char TrackOffset[(offsetof(Snapshot,tracks)==0x780)?1:-1];
typedef char OriginOffset[(offsetof(Snapshot,origin)==0x11c0)?1:-1];
static inline const Channel40224 *Resolve(const Snapshot *base,const Channel40224 *ptr){const u8 *origin=base->origin;return !ptr?NULL:(const Channel40224*)((u32)base+((u32)ptr-(u32)origin));}
BOOL SND_ReadTrackInfo(const void*,int,int,SNDTrackInfo*);
// @fn SND_ReadTrackInfo
BOOL SND_ReadTrackInfo(const void *input,int player,int track,SNDTrackInfo*out){
    const Snapshot *base=input;
    const Track40224 *info;
    const Channel40224 *channel;
    u8 index;
    if(player<0||player>15)return FALSE;
    if(track<0||track>15)return FALSE;
    index=base->players[player].tracks[track];
    if(index==255)return FALSE;
    info=base->tracks+index;
    out->prgNo=info->program;
    out->volume=info->volume;
    out->volume2=info->expression;
    out->pitchBend=info->bend;
    out->bendRange=info->range;
    out->pan=info->pan+64;
    out->transpose=info->transpose;
    channel=Resolve(base,info->channels);
    out->chCount=0;
    while(channel){out->channel[out->chCount]=channel->id;out->chCount++;{const u8*o=base->origin;const Channel40224*n=channel->next;if(!n)channel=NULL;else channel=(const Channel40224*)((u32)base+((u32)n-(u32)o));}}
    return TRUE;
}
