#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
typedef void (*W17HeapCallback)(void *, u32, void *, int);

typedef struct Wave {
    u32 p00[2], size;
    u8 p0c[44];
    u32 index;
    u32 offset[1];
} Wave;

typedef struct SeqInfo {
    u32 data;
    u16 bank;
    u8 volume, channel, priority, player;
} SeqInfo;

typedef struct StreamInfo {
    u8 count, p01;
    u16 channel;
    u32 size;
} StreamInfo;

typedef struct InstPos {
    u32 p[2];
} InstPos;

typedef struct InstData {
    u8 type, p01;
    u16 wave, bank;
    u8 tail[4];
} InstData;

typedef struct Stream {
    u8 p00[0xe8];
    u8 fader[0x28];
    s32 active : 1, started : 1, ready : 1, stopping : 1, padFlags : 28;
    int available, readyCount;
    u8 p11c[0x30];
    void **handle;
    u32 p150;
    int initialVolume, otherVolume, currentVolume;
    u32 p160[2];
    void (*dispose)(struct Stream *);
    u32 p16c;
    void (*stop)(struct Stream *);
} Stream;

extern Stream _021E0FEC[4];
extern const s16 _0210D750[];
extern OSMutex _021E0FC8;
extern u8 _021E0FE0[];
extern void *_021E0950[];
void *SND_GetWaveDataAddress(Wave *, int);
int SND_GetWaveDataCount(Wave *);
void SND_SetWaveDataAddress(Wave *, int, void *);
InstPos SND_GetFirstInstDataPos(void);
int SND_GetNextInstData(void *, InstData *, InstPos *);
void SingleWaveDisposeCallback(void *, u32, void *, int);
void *NNS_SndHeapAlloc(void *, u32, W17HeapCallback, void *, int);
int NNS_SndArcReadFile(int, void *, u32, u32);
void *NNS_SndArcGetCurrent(void);
StreamInfo *NNS_SndArcGetStrmInfo(int);
void NNS_SndPlayerSetPlayableSeqCount(int, int);
void NNS_SndPlayerSetAllocatableChannel(int, u32);
int NNS_SndPlayerCreateHeap(int, struct NNSSndHeap *, u32);
SeqInfo *NNS_SndArcGetSeqInfo(int);
void *NNS_SndArcGetSeqArcInfo(int);
void *NNS_SndArcGetFileAddress(int);
SeqInfo *NNSi_SndSeqArcGetSeqInfo(void *, int);
void *NNSi_SndPlayerAllocSeqPlayer(void *, int, int);
void *NNSi_SndPlayerAllocHeap(int, void *);
void NNSi_SndPlayerFreeSeqPlayer(void *);
int NNSi_SndArcLoadBank(int, int, void *, int, void **);
int NNSi_SndArcLoadSeq(int, int, void *, int, void **);
void NNSi_SndPlayerStartSeq(void *, void *, u32, void *);
void NNS_SndPlayerSetInitialVolume(void *, int);
void NNS_SndPlayerSetChannelPriority(void *, int);
void NNS_SndPlayerSetSeqNo(void *, int);
void NNS_SndPlayerSetSeqArcNo(void *, int, int);
void NNS_SndStrmStart(Stream *);
void NNS_SndStrmStop(Stream *);
void NNS_SndStrmSetVolume(Stream *, int);
void NNSi_SndFaderUpdate(void *);
int NNSi_SndFaderGet(void *);
int NNSi_SndFaderIsFinished(void *);
void RemoveCommandByPlayer(void *, Stream *);
void FreeChannel(Stream *);
int LoadSingleWave(Wave *, int, int, struct NNSSndHeap *);
int LoadSingleWaves(Wave *, void *, int, int, void *);
int NNS_SndArcPlayerSetup(struct NNSSndHeap *);
int NNS_SndArcPlayerStartSeqEx(struct NNSSndHandle *, int, int, int, int);
int NNS_SndArcPlayerStartSeqArc(void *, int, int);
int StartSeq(void *, int, int, int, SeqInfo *, int);
int StartSeqArc(void *, int, int, int, SeqInfo *, u8 *, int, int);
void NNSi_SndArcStrmMain(void);
void FreePlayer(Stream *);
void ForceStopStrm__SndArc(Stream *);
void ShutdownPlayer__SndArc(Stream *);

void FreePlayer(Stream *p) {
    if (p->handle) {
        *p->handle = 0;
        p->handle = 0;
    }
    p->active = 0;
    p->ready = 0;
    p->started = 0;
}

void ForceStopStrm__SndArc(Stream *p) {
    OS_LockMutex(&_021E0FC8);
    if (_021E0950[1]) {
        OS_LockMutex((OSMutex *)((u8 *)_021E0950[1] + 0x4c8));
    }
    if (p->started) {
        NNS_SndStrmStop(p);
    }
    if (p->active) {
        p->stop(p);
    }
    ShutdownPlayer__SndArc(p);
    OS_UnlockMutex(&_021E0FC8);
    if (_021E0950[1]) {
        OS_UnlockMutex((OSMutex *)((u8 *)_021E0950[1] + 0x4c8));
    }
}

void ShutdownPlayer__SndArc(Stream *p) {
    if (!p->active) {
        return;
    }
    FreeChannel(p);
    p->dispose(p);
    RemoveCommandByPlayer(_021E0FE0, p);
    if (_021E0950[1]) {
        RemoveCommandByPlayer((u8 *)_021E0950[1] + 0x4e0, p);
    }
    FreePlayer(p);
}
