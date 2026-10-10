#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define B(p, o) (*(u8 *)((u8 *)(p) + (o)))
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define W(p, o) (*(u32 *)((u8 *)(p) + (o)))

typedef struct {
    u8 f0 : 1, f1 : 1, f2 : 1, f3 : 1, f4 : 1, f5 : 1, f6 : 1, f7 : 1;
} A35196_Flags;

typedef struct {
    A35196_Flags flags;
    u8 id;
    u8 pad[22];
    u16 tempo, mult, acc, pad2;
    void *bank;
} A35196_Player;

typedef struct A35196_Channel {
    u8 pad[34];
    u8 marker;
    u8 pad2[45];
    struct A35196_Channel *next;
} A35196_Channel;

typedef struct {
    A35196_Flags flags;
    u8 priority;
    u16 wait;
    u8 pad[32];
    const u8 *base, *pos;
    u8 tail[12];
    u32 time;
    A35196_Channel *channel;
} A35196_Track;

typedef struct {
    u8 pad[0x540];
    A35196_Player players[16];
    A35196_Track tracks[32];
} A35196_Work;

extern A35196_Work SNDi_Work;
extern u8 *SNDi_SharedWork;
extern u8 seqCache[];

typedef struct {
    u32 enable;
    const u8 *start, *end;
    u32 words[4];
} A35196_Cache;

extern A35196_Cache sMmlPrintEnable;
extern void FinishPlayer(A35196_Player *);
extern void UpdateTrackChannel(A35196_Track *, A35196_Player *, int);
extern void ReleaseTrackChannelAll(A35196_Track *, A35196_Player *, int);
extern void FreeTrackChannelAll(A35196_Track *);
extern void SetTrackMute(A35196_Track *, A35196_Player *, int);
extern A35196_Track *GetPlayerTrack(A35196_Player *, int);
extern int PlayerSeqMain(A35196_Player *, int);
extern int AllocTrack(void);
extern int ReadByte(A35196_Track *);
extern void InitTrack(A35196_Track *);
extern void InitCache(const u8 *);
extern void SND_PrepareSeq(int, const u8 *, u32, void *);
extern void SND_StopIntervalTimer(void);
extern void SND_StartIntervalTimer(void);
extern s16 *GetVariablePtr(A35196_Player *, int);
extern u16 SND_CalcRandom(void);
extern void SND_InitLfoParam(void *);
extern void SND_FreeExChannel(A35196_Channel *);
extern void SND_ReleaseExChannel(A35196_Channel *);
extern void SND_SetExChannelRelease(A35196_Channel *, u8);
extern int SND_IsExChannelActive(A35196_Channel *);
void SND_SeqMain(int advance);

typedef struct {
    u8 pad[64];
    u32 ticks;
} A35196_Shared;

static inline void Tempo(A35196_Player *q) {
    q->acc += (q->mult * q->tempo) >> 8;
}

void SND_SeqMain(int advance) {
    A35196_Player *p;
    int i;
    u32 active = 0;
    int count, ticks;
    for (i = 0; i < 16; i++) {
        p = SNDi_Work.players + i;
        if (!p->flags.f0) {
            continue;
        }
        if (p->flags.f1) {
            if (advance && !p->flags.f2) {
                ticks = 0;
                while (p->acc >= 240) {
                    p->acc -= 240;
                    ticks++;
                }
                for (count = 0; count < ticks; count++) {
                    if (PlayerSeqMain(p, 1)) {
                        FinishPlayer(p);
                        break;
                    }
                }
                if (SNDi_SharedWork) {
                    ((A35196_Shared *)(SNDi_SharedWork + p->id * 36))->ticks += count;
                }
                {
                    int t = p->tempo;
                    t *= p->mult;
                    t >>= 8;
                    t += p->acc;
                    p->acc = t;
                }
            }
            for (ticks = 0; ticks < 16; ticks++) {
                A35196_Track *t = GetPlayerTrack(p, ticks);
                if (t) {
                    UpdateTrackChannel(t, p, 1);
                }
            }
        }
        if (p->flags.f0) {
            active |= 1u << i;
        }
    }
    if (SNDi_SharedWork) {
        W(SNDi_SharedWork, 4) = active;
    }
}
