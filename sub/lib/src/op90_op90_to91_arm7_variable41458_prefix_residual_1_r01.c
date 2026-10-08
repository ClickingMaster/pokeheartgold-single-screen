#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
typedef struct SNDExChannel {
    u8 x0;
    u8 x1;
    u8 type;
    u8 x3;
    u8 pan;
    u8 x5;
    s16 pitch;
    u8 x8[3];
    s8 x0b;
    s16 volume;
    s16 x0e;
    u8 x10[0x22 - 0x10];
    u8 x22;
    u8 x23[0x28 - 0x23];
    u16 x28;
    u16 x2a;
    u16 x2c;
    u8 x2e[0x34 - 0x2e];
    void *x34;
    u8 x38[0x50 - 0x38];
    struct SNDExChannel *next;
} SNDExChannel;
typedef struct {
    u8 x0;
    u8 pan;
    u8 x2;
    u8 x3;
    u8 volume;
    u8 volume2;
    s8 bend;
    u8 bendRange;
    s8 panRange;
    s8 initPan;
    s16 pitch;
    s16 x0c;
    u8 x0e[0x18 - 0x0e];
    u16 x18;
    u16 x1a;
    u16 x1c;
    u8 x1e[0x3c - 0x1e];
    SNDExChannel *chList;
} SNDTrack;
typedef struct {
    u8 x0[5];
    u8 volume;
    s16 pitch;
} SNDPlayer;
extern s16 SNDi_DecibelTable[];
extern void SND_ReleaseExChannel(SNDExChannel *);
void UpdateTrackChannel(SNDTrack *track, SNDPlayer *player, BOOL release);
// @fn UpdateTrackChannel
void UpdateTrackChannel(SNDTrack *track, SNDPlayer *player, BOOL release)
{
/*DECL*/
    SNDExChannel *ch;
    int vol;
    int pitch;
    int pan;
    int fader;
    /*ENDDECL*/
    vol = SNDi_DecibelTable[track->volume] + SNDi_DecibelTable[track->volume2] + SNDi_DecibelTable[player->volume];
    pitch = track->pitch + player->pitch;
    fader = track->bend;
    fader *= track->bendRange << 6;
    fader >>= 7;
    fader += track->x0c;
    pan = track->panRange;
    if (track->pan != 0x7f)
        pan = (pan * track->pan + 0x40) >> 7;
    pan += track->initPan;
    if (vol < -0x8000)
        vol = -0x8000;
    if (pitch < -0x8000)
        pitch = -0x8000;
    if (pan < -0x80)
        pan = -0x80;
    else if (pan > 0x7f)
        pan = 0x7f;
    for (ch = track->chList; ch != NULL; ch = ch->next) {
        ch->pitch = pitch;
        if (ch->type == 3)
            continue;
        ch->volume = vol;
        ch->x0e = fader;
        ch->x0b = pan;
        ch->pan = track->pan;
        ch->x28 = track->x18;
        ch->x2a = track->x1a;
        ch->x2c = track->x1c;
        if (ch->x34 == NULL && release) {
            ch->x22 = 1;
            SND_ReleaseExChannel(ch);
        }
    }
}
