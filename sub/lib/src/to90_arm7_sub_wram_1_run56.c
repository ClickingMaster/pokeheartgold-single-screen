#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct W35224Track {
    u8 active : 1;
    u8 flag1 : 1;
    u8 mute : 1;
    u8 other : 5;
    u8 mod;
    u8 b2, b3;
    u8 volume, volume2;
    s8 bend;
    u8 range;
    s8 pan, pan2;
    s16 extvol, pitch;
    u8 rest[10];
    u16 mod1, mod2, mod3;
    u8 rest2[30];
    struct W35224Channel *channels;
} W35224Track;

typedef struct W35224Player {
    u8 active : 1;
    u8 flags : 7;
    u8 number, b2, b3, b4, volume;
    s16 extvol;
    u8 tracks[16];
} W35224Player;

typedef struct W35224Channel {
    u8 b0, b1, type, b3, mod, key;
    s16 extvol;
    u8 note, velocity;
    s8 pan, pan2;
    s16 volume, pitch;
    u8 rest[18];
    u8 release;
    u8 rest2[5];
    u16 mod1, mod2, mod3;
    u8 rest3[6];
    void *callback;
    u8 rest4[24];
    struct W35224Channel *next;
} W35224Channel;

extern u8 SNDi_Work[];
extern void ReleaseTrackChannelAll(W35224Track *, W35224Player *, int);
extern void FreeTrackChannelAll(W35224Track *);
void SetTrackMute(W35224Track *t, W35224Player *p, int mode);

void SetTrackMute(W35224Track *t, W35224Player *p, int mode) {
    switch (mode) {
    case 0:
        t->mute = 0;
        break;
    case 1:
        t->mute = 1;
        break;
    case 2:
        t->mute = 1;
        ReleaseTrackChannelAll(t, p, -1);
        break;
    case 3:
        t->mute = 1;
        ReleaseTrackChannelAll(t, p, 127);
        FreeTrackChannelAll(t);
        break;
    }
}
