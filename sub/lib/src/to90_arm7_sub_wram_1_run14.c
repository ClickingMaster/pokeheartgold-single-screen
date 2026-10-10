#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
typedef struct Ch35172 Ch35172;

struct Ch35172 {
    u8 id, type, state;

    union {
        u8 flags;

        struct {
            u8 active : 1, started : 1, reserved : 1, updates : 5;
        };
    };

    u8 vol, key;
    u16 tune;
    u8 key2, vel, a, b;
    u16 c, d;
    s32 env, e, f;
    u8 attack, sustain;
    u16 decay, release;
    u8 priority, pan;
    u16 volume, timer;
    u8 lfo[10];
    s16 pitch;
    u32 next;

    struct {
        u8 format, loop;
        u16 rate, timer, start;
        u32 length;
    } wave;

    u32 data;
    void (*cb)(Ch35172 *, int, void *);
    void *arg;
    u32 last;
};

extern Ch35172 SNDi_Work[16];
extern u32 sWeakLockChannel[2];
extern void SND_StopChannel(int, int);
extern void SND_SetupChannelPcm(int, u32, int, int, int, u32, int, int, int, int);
extern void SND_SetupChannelPsg(int, u32, int, int, int, int);
extern void SND_SetupChannelNoise(int, int, int, int, int);
extern void SND_SetChannelTimer(int, int);
extern void SND_SetChannelVolume(int, int, int);
extern void SND_SetChannelPan(int, int);
void SND_UpdateExChannel(void);

void SND_UpdateExChannel(void) {
    Ch35172 *p;
    int i;
    for (i = 0; i < 16; i++) {
        p = &SNDi_Work[i];
        u32 flags = p->updates;
        if (!flags) {
            continue;
        }
        if (flags & 2) {
            SND_StopChannel(i, 0);
        }
        flags = p->updates;
        if (flags & 1) {
            switch (p->type) {
            case 0:
                SND_SetupChannelPcm(i, p->data, p->wave.format, p->wave.loop ? 1 : 2, p->wave.start, p->wave.length, p->volume & 255, p->volume >> 8, p->timer, p->pan);
                break;
            case 1:
                SND_SetupChannelPsg(i, p->data, p->volume & 255, p->volume >> 8, p->timer, p->pan);
                break;
            case 2:
                SND_SetupChannelNoise(i, p->volume & 255, p->volume >> 8, p->timer, p->pan);
                break;
            }
        } else {
            if (flags & 4) {
                SND_SetChannelTimer(i, p->timer);
            }
            if ((p->updates) & 8) {
                SND_SetChannelVolume(i, p->volume & 255, p->volume >> 8);
            }
            if ((p->updates) & 16) {
                SND_SetChannelPan(i, p->pan);
            }
        }
    }
    for (i = 0; i < 16; i++) {
        p = &SNDi_Work[i];
        u32 flags = p->updates;
        if (flags) {
            if (flags & 1) {
                *(volatile u8 *)(0x04000403 + i * 16) |= 128;
            }
            p->updates = 0;
        }
    }
}
