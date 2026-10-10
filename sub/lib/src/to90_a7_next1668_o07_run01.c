#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u8 active : 1, started : 1, paused : 1, f3 : 1, f4 : 1, f5 : 1, f6 : 1, f7 : 1;
} W40762_Flags;

typedef struct {
    W40762_Flags flags;
    u8 id;
    u8 gap[2], volume, priority;
    s16 pitch;
    u8 tracks[16];
    u16 tempo, mult, acc, pad;
    void *bank;
} W40762_Player;

typedef struct {
    u8 prefix[36];
    const u8 *base, *pos;
    u8 tail[20];
} W40762_Track;

typedef struct {
    u8 prefix[0x540];
    W40762_Player players[16];
    W40762_Track tracks[32];
} W40762_Work;

typedef struct {
    s16 vars[16];
    u32 ticks;
} W40762_SharedRow;

typedef struct {
    u32 unused, activePlayers;
    u8 gap[24];
    W40762_SharedRow players[16];
} W40762_Shared;

extern W40762_Work SNDi_Work;
extern W40762_Shared *SNDi_SharedWork;
extern void FinishPlayer(W40762_Player *);
extern void InitTrack(W40762_Track *);
extern void InitCache(const u8 *);
extern int AllocTrack(void);
extern int ReadByte(W40762_Track *);
void SND_PrepareSeq(int, const u8 *, u32, void *);

#pragma force_active off

static inline void W40762_InitPlayer(W40762_Player *p, void *bank) {
    int i;
    if (p->flags.active) {
        FinishPlayer(p);
    }
    p->flags.paused = 0;
    p->bank = bank;
    p->tempo = 120;
    p->mult = 256;
    p->acc = 240;
    p->priority = 127;
    p->pitch = 0;
    p->volume = 64;
    for (i = 0; i < 16; i++) {
        p->tracks[i] = 255;
    }
    if (SNDi_SharedWork) {
        SNDi_SharedWork->players[p->id].ticks = 0;
        for (i = 0; i < 16; i++) {
            SNDi_SharedWork->players[p->id].vars[i] = -1;
        }
    }
}

#pragma force_active on

void SND_PrepareSeq(int player, const u8 *data, u32 offset, void *bank) {
    int i, num;

    struct {
        W40762_Player *p;
        W40762_Track *t;
    } state;

    state.p = &SNDi_Work.players[player];
    W40762_InitPlayer(state.p, bank);
    num = AllocTrack();
    if (num < 0) {
        return;
    }
    state.t = &SNDi_Work.tracks[num];
    InitTrack(state.t);
    state.t->base = data;
    state.t->pos = data + offset;
    state.p->tracks[0] = num;
    InitCache(state.t->pos);
    if (ReadByte(state.t) != 254) {
        state.t->pos--;
    } else {
        u16 mask;
        int lo = ReadByte(state.t);
        mask = (u16)(lo | (ReadByte(state.t) << 8));
        mask >>= 1;
        for (i = 1; mask; i++, mask = (u16)(mask >> 1)) {
            if (mask & 1) {
                num = AllocTrack();
                if (num < 0) {
                    break;
                }
                InitTrack(&SNDi_Work.tracks[num]);
                state.p->tracks[i] = num;
            }
        }
    }
    state.p->flags.active = 1;
    state.p->flags.started = 0;
    if (SNDi_SharedWork) {
        SNDi_SharedWork->activePlayers |= 1u << player;
    }
}
