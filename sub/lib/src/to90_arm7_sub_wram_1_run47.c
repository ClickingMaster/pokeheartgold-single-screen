#include "canonical-api.h"
#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define B(p, o) (*(u8 *)((u8 *)(p) + (o)))
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define W(p, o) (*(u32 *)((u8 *)(p) + (o)))

typedef struct {
    u8 f0 : 1, f1 : 1, f2 : 1, f3 : 1, f4 : 1, f5 : 1, f6 : 1, f7 : 1;
} A35213_Flags;

typedef struct {
    A35213_Flags flags;
    u8 id;
    u8 pad[22];
    u16 tempo, mult, acc, pad2;
    void *bank;
} A35213_Player;

typedef struct A35213_Channel {
    u8 pad[34];
    u8 marker;
    u8 pad2[45];
    struct A35213_Channel *next;
} A35213_Channel;

typedef struct {
    A35213_Flags flags;
    u8 priority;
    u16 wait;
    u8 pad[32];
    const u8 *base, *pos;
    u8 tail[12];
    u32 time;
    A35213_Channel *channel;
} A35213_Track;

extern u8 SNDi_Work[];
extern u8 *SNDi_SharedWork;
extern u8 seqCache[];

typedef struct {
    u32 enable;
    const u8 *start, *end;
    u32 words[4];
} A35213_Cache;

extern A35213_Cache sMmlPrintEnable;
extern void FinishPlayer(A35213_Player *);
extern void UpdateTrackChannel(A35213_Track *, A35213_Player *, int);
extern void ReleaseTrackChannelAll(A35213_Track *, A35213_Player *, int);
extern void FreeTrackChannelAll(A35213_Track *);
extern void SetTrackMute(A35213_Track *, A35213_Player *, int);
extern A35213_Track *GetPlayerTrack(A35213_Player *, int);
extern int PlayerSeqMain(A35213_Player *, int);
extern int AllocTrack(void);
extern int ReadByte(A35213_Track *);
extern void InitTrack(A35213_Track *);
extern void InitCache(const u8 *);
extern void SND_PrepareSeq(int, const u8 *, u32, void *);
extern void SND_StopIntervalTimer(void);
extern void SND_StartIntervalTimer(void);
extern u16 SND_CalcRandom(void);
extern void SND_InitLfoParam(void *);
extern void SND_FreeExChannel(A35213_Channel *);
extern void SND_ReleaseExChannel(A35213_Channel *);
extern void SND_SetExChannelRelease(A35213_Channel *, u8);
extern int SND_IsExChannelActive(A35213_Channel *);
void InitTrack(A35213_Track *t);

void InitTrack(A35213_Track *t) {
    t->base = 0;
    t->pos = 0;
    t->flags.f1 = 1;
    t->flags.f2 = 0;
    t->flags.f3 = 0;
    t->flags.f4 = 0;
    t->flags.f5 = 0;
    t->flags.f6 = 1;
    t->flags.f7 = 0;
    B(t, 59) = 0;
    H(t, 2) = 0;
    B(t, 18) = 64;
    B(t, 4) = 127;
    B(t, 5) = 127;
    H(t, 10) = 0;
    B(t, 8) = 0;
    B(t, 9) = 0;
    B(t, 6) = 0;
    H(t, 12) = 0;
    B(t, 14) = 255;
    B(t, 15) = 255;
    B(t, 16) = 255;
    B(t, 17) = 255;
    B(t, 1) = 127;
    B(t, 7) = 2;
    B(t, 20) = 60;
    B(t, 21) = 0;
    H(t, 22) = 0;
    B(t, 19) = 0;
    H(t, 30) = 65535;
    SND_InitLfoParam((u8 *)t + 24);
    W(t, 32) = 0;
    t->channel = 0;
}
