#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct W35219Track {
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
    struct W35219Channel *channels;
} W35219Track;

typedef struct W35219Player {
    u8 active : 1;
    u8 flags : 7;
    u8 number, b2, b3, b4, volume;
    s16 extvol;
    u8 tracks[16];
} W35219Player;

typedef struct W35219Channel {
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
    struct W35219Channel *next;
} W35219Channel;

extern u8 SNDi_Work[];
extern void ClosePlayerTrack(W35219Player *, int);
void FinishPlayer(W35219Player *p);

void FinishPlayer(W35219Player *p) {
    int i;
    for (i = 0; i < 16; i++) {
        ClosePlayerTrack(p, i);
    }
    p->active = 0;
}
