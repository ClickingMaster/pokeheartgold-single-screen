#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u8 id, type, state, flags;
    u8 p04[12];
    s32 env;
    u8 p14[8];
    u8 attack, sustain;
    u16 decay, release;
    u8 p22[22];
    u32 wave[3];
    u32 data;
    void *cb, *arg;
    u8 p50[4];
} Ch35174;

extern void StartExChannel(Ch35174 *, int);
int SND_StartExChannelPsg(Ch35174 *p, u32 data, int v);

int SND_StartExChannelPsg(Ch35174 *p, u32 data, int v) {
    if (p->id < 8) {
        return 0;
    }
    if (p->id > 13) {
        return 0;
    }
    p->type = 1;
    p->data = data;
    *(u16 *)((u8 *)p + 60) = 8006;
    StartExChannel(p, v);
    return 1;
}
