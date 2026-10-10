#pragma force_active on
#pragma dollar_identifiers on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/SND_work_shared.h>
#include <nitro/code32.h>
extern u32 sWeakLockChannel[2];
extern void SND_StopChannel(int, int);
extern const u8 shift$4005[];
extern const u8 channel_order$3829[];
extern void SND_InitLfoParam(struct SNDLfoParam *);
struct SNDExChannel *SND_AllocExChannel(u32 mask, int priority, int weak, void (*cb)(struct SNDExChannel *, int, void *), void *arg);

static inline int CmpVol(int a, int b) {
    int bv = (b & 255) << 4;
    int av = (a & 255) << 4;
    bv >>= shift$4005[b >> 8];
    av >>= shift$4005[a >> 8];
    if (av != bv) {
        if (av < bv) {
            return 1;
        } else {
            return -1;
        }
    }
    return 0;
}

struct SNDExChannel *SND_AllocExChannel(u32 mask, int priority, int weak, void (*cb)(struct SNDExChannel *, int, void *), void *arg) {
    SNDExChannelCallback callback;
    struct SNDExChannel *best;
    int i;
    struct SNDExChannel *p;
    int ch;
    callback = cb;
    mask &= ~sWeakLockChannel[1];
    if (!weak) {
        mask &= ~sWeakLockChannel[0];
    }
    best = 0;
    for (i = 0; i < 16; i++) {
        ch = channel_order$3829[i];
        if (mask & (1u << ch)) {
            p = &SNDi_Work.channels[ch];
            if (!best) {
                best = p;
            } else if (p->priority <= best->priority) {
                if (p->priority == best->priority && CmpVol(best->volume, p->volume) >= 0) {
                    continue;
                }
                best = p;
            }
        }
    }
    if (!best) {
        return 0;
    }
    if (priority < best->priority) {
        return 0;
    }
    if (best->callback) {
        best->callback(best, 0, best->callbackUserData);
    }
    best->flags.syncFlag = 2;
    best->flags.active = 0;
    best->channelLLNext = 0;
    best->callback = callback;
    best->callbackUserData = arg;
    best->length = 0;
    best->priority = priority;
    best->volume = 127;
    best->flags.start = 0;
    best->flags.autoSweep = 1;
    best->midiKey = 60;
    best->rootMidiKey = 60;
    best->velocity = 127;
    best->initPan = 0;
    best->userDecay = 0;
    best->userDecay2 = 0;
    best->userPitch = 0;
    best->userPan = 0;
    best->panRange = 127;
    best->sweepPitch = 0;
    best->sweepLength = 0;
    best->sweepCounter = 0;
    best->envAttack = 0;
    best->envDecay = 65535;
    best->envSustain = 127;
    best->envRelease = 65535;
    SND_InitLfoParam(&best->lfo.param);
    return best;
}
