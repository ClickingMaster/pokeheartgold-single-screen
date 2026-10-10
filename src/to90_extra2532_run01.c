#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct {
    u8 prefix[0x20];
    u8 volume;
} Owner;

typedef struct {
    u8 bytes[16];
} Fader;

struct Player {
    void *previous;
    Owner *owner;
    u8 pad8[0x14];
    Fader fader;
    u8 fadeState, finished, pad2e, prepared;
    u32 tag;
    u8 pad34[8];
    u8 index, pad3d;
    s16 volume;
    u8 mix, offset, pad42[2];
};

struct SeqList {
    u8 data[0x24];
};

extern u8 _021DF978[];
extern u8 _021DF96C[];
extern struct Player _021DF984[];
extern struct SeqList _021DFDC4[];
extern void NNS_FndInitList(void *, int);
extern void NNS_FndAppendListObject(void *, void *);
extern void *NNS_FndGetNextListObject(void *, void *);

extern void ForceStopSeq(void *);
extern void *AllocSeqPlayer(int);
extern void InsertPlayerList(void *, void *);
extern void NNSi_SndPlayerInit(void);
extern void *NNSi_SndPlayerAllocSeqPlayer(void **, int, int);
extern const s16 _0210D750[];
extern u32 SND_GetPlayerStatus(void);
extern BOOL SND_IsFinishedCommandTag(u32);
extern void SND_SetPlayerVolume(int, int);
extern void SND_StartPreparedSeq(int);
extern void ShutdownPlayer(void *);
extern void NNSi_SndFaderUpdate(void *);
extern int NNSi_SndFaderGet(void *);
extern BOOL NNSi_SndFaderIsFinished(void *);
extern void NNSi_SndPlayerMain(void);
#pragma force_active off

static inline int combineVolume(int owner, int mix, int offset, int fade) {
    return fade + (owner + (mix + offset));
}

#pragma force_active on

/* Assembly initialization assigns the immutable player index from0 through15. */
void NNSi_SndPlayerMain(void) {
    u32 status = SND_GetPlayerStatus();
    struct Player *p = NNS_FndGetNextListObject(_021DF978, NULL);
    while (p) {
        struct Player *next = NNS_FndGetNextListObject(_021DF978, p);
        int volume;
        if (!p->finished) {
            if (SND_IsFinishedCommandTag(p->tag)) {
                p->finished = 1;
            }
        }
        if (p->finished && !(status & (1U << p->index))) {
            ShutdownPlayer(p);
        } else {
            s32 c, b, a;
            NNSi_SndFaderUpdate(&p->fader);
            c = _0210D750[p->offset];
            b = _0210D750[p->mix];
            a = _0210D750[p->owner->volume];
            volume = combineVolume(a, b, c, _0210D750[NNSi_SndFaderGet(&p->fader) >> 8]);
            if (volume < -32768) {
                volume = -32768;
            } else if (volume > 32767) {
                volume = 32767;
            }
            if (volume != p->volume) {
                SND_SetPlayerVolume(p->index, volume);
                p->volume = volume;
            }
            if (p->fadeState == 2 && NNSi_SndFaderIsFinished(&p->fader)) {
                ForceStopSeq(p);
            }
            if (p->prepared) {
                SND_StartPreparedSeq(p->index);
                p->prepared = 0;
            }
        }
        p = next;
    }
}
