#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
typedef struct Ch35185 Ch35185;

struct Ch35185 {
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
    void (*cb)(Ch35185 *, int, void *);
    void *arg;
    u32 last;
};

extern Ch35185 SNDi_Work[16];
extern u32 sWeakLockChannel[2];
extern void SND_StopChannel(int, int);
extern void SND_FreeExChannel(Ch35185 *);
void SND_StopUnlockedChannel(u32 bits, u32 unusedCommandWord);

void SND_StopUnlockedChannel(u32 bits, u32 unusedCommandWord) {
    Ch35185 *p;
    int i = 0;
    while (i < 16 && bits) {
        if (bits & 1) {
            u32 locked = sWeakLockChannel[1];
            p = &SNDi_Work[i];
            if (!(locked & (1u << i))) {
                if (p->cb) {
                    p->cb(p, 0, p->arg);
                }
                SND_StopChannel(i, 0);
                p->priority = 0;
                SND_FreeExChannel(p);
                p->updates = 0;
                p->active = 0;
            }
        }
        i++;
        bits >>= 1;
    }
}
