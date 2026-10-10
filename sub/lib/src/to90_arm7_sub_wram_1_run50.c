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
} A35216_Flags;

typedef struct {
    A35216_Flags flags;
    u8 id;
    u8 pad[22];
    u16 tempo, mult, acc, pad2;
    void *bank;
} A35216_Player;

typedef struct A35216_Channel {
    u8 pad[34];
    u8 marker;
    u8 pad2[45];
    struct A35216_Channel *next;
} A35216_Channel;

typedef struct {
    A35216_Flags flags;
    u8 priority;
    u16 wait;
    u8 pad[32];
    const u8 *base, *pos;
    u8 tail[12];
    u32 time;
    A35216_Channel *channel;
} A35216_Track;

extern u8 SNDi_Work[];
extern u8 *SNDi_SharedWork;
extern u8 seqCache[];

typedef struct {
    u32 enable;
    const u8 *start, *end;
    u32 words[4];
} A35216_Cache;

extern A35216_Cache sMmlPrintEnable;
extern void FinishPlayer(A35216_Player *);
extern void UpdateTrackChannel(A35216_Track *, A35216_Player *, int);
extern void ReleaseTrackChannelAll(A35216_Track *, A35216_Player *, int);
extern void FreeTrackChannelAll(A35216_Track *);
extern void SetTrackMute(A35216_Track *, A35216_Player *, int);
extern A35216_Track *GetPlayerTrack(A35216_Player *, int);
extern int PlayerSeqMain(A35216_Player *, int);
extern int AllocTrack(void);
extern int ReadByte(A35216_Track *);
extern void InitTrack(A35216_Track *);
extern void InitCache(const u8 *);
extern void SND_PrepareSeq(int, const u8 *, u32, void *);
extern void SND_StopIntervalTimer(void);
extern void SND_StartIntervalTimer(void);
extern u16 SND_CalcRandom(void);
extern void SND_InitLfoParam(void *);
extern void SND_FreeExChannel(A35216_Channel *);
extern void SND_ReleaseExChannel(A35216_Channel *);
extern void SND_SetExChannelRelease(A35216_Channel *, u8);
extern int SND_IsExChannelActive(A35216_Channel *);
void FreeTrackChannelAll(A35216_Track *t);

void FreeTrackChannelAll(A35216_Track *t) {
    A35216_Channel *c;
    for (c = t->channel; c; c = c->next) {
        SND_FreeExChannel(c);
    }
    t->channel = 0;
}
