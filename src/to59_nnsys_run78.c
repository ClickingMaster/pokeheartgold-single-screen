#include <nitro/types.h>

typedef struct Fader {
    s32 start;
    s32 target;
    s32 elapsed;
    s32 duration;
} Fader;

void NNSi_SndFaderInit(Fader *);
void NNSi_SndFaderSet(Fader *, s32, s32);
s32 NNSi_SndFaderGet(Fader *);
void NNSi_SndFaderUpdate(Fader *);
int NNSi_SndFaderIsFinished(Fader *);
u32 OS_DisableInterrupts(void);
void OS_RestoreInterrupts(u32);
void *NNS_SndArcSetCurrent(void *);
void *NNS_SndArcGetFileAddress(int);
void *NNS_SndArcGetCurrent(void);
void *NNSi_SndArcLoadFile(int, void (*)(void *, u32, void *, int), void *, int, u32);
void NNS_SndArcSetFileAddress(int, void *);
void SND_InvalidateSeqData(void *, void *);
void SND_InvalidateBankData(void *, void *);
void SND_DestroyBank(void *);
void SND_InvalidateWaveData(void *, void *);
void SND_DestroyWaveArc(void *);
void *SND_GetWaveDataAddress(void *, int);
void SND_SetWaveDataAddress(void *, int, void *);
void DisposeCallback(void *, void *, int);
void SeqDisposeCallback(void *, u32, void *, int);
void BankDisposeCallback(void *, u32, void *, int);
void WaveArcDisposeCallback(void *, u32, void *, int);
void WaveArcTableDisposeCallback(void *, u32, void *, int);
void SingleWaveDisposeCallback(void *, u32, void *, int);
void *LoadSeq(int, u32, int);
void *LoadSeqArc(int, u32, int);
void *LoadBank(int, u32, int);
void *LoadWaveArc(int, u32, int);
void NNS_SndStrmFreeChannel(void *);
void FreeChannel(void *);
void FreePlayer(void *);
void NNS_FndAppendListObject(void *, void *);
extern char _021E095C;
void FreeCommandBuffer(void *);
void *NNSi_SndSeqArcGetSeqInfo(void *, int);

void DisposeCallback(void *data, void *arc, int fileId) {
    void *prior;
    u32 interrupts;
    if (arc == 0) {
        return;
    }
    interrupts = OS_DisableInterrupts();
    prior = NNS_SndArcSetCurrent(arc);
    if (data == NNS_SndArcGetFileAddress(fileId)) {
        NNS_SndArcSetFileAddress(fileId, 0);
    }
    NNS_SndArcSetCurrent(prior);
    OS_RestoreInterrupts(interrupts);
}

void SeqDisposeCallback(void *data, u32 size, void *arc, int fileId) {
    DisposeCallback(data, arc, fileId);
    SND_InvalidateSeqData(data, (u8 *)data + size);
}

void BankDisposeCallback(void *data, u32 size, void *arc, int fileId) {
    DisposeCallback(data, arc, fileId);
    SND_InvalidateBankData(data, (u8 *)data + size);
    SND_DestroyBank(data);
}

void WaveArcDisposeCallback(void *data, u32 size, void *arc, int fileId) {
    DisposeCallback(data, arc, fileId);
    SND_InvalidateWaveData(data, (u8 *)data + size);
    SND_DestroyWaveArc(data);
}

void WaveArcTableDisposeCallback(void *data, u32 size, void *arc, int fileId) {
    DisposeCallback(data, arc, fileId);
    SND_DestroyWaveArc(data);
}

void SingleWaveDisposeCallback(void *data, u32 size, void *waveArc, int waveIndex) {
    if (data == SND_GetWaveDataAddress(waveArc, waveIndex)) {
        SND_SetWaveDataAddress(waveArc, waveIndex, 0);
    }
    SND_InvalidateWaveData(data, (u8 *)data + size);
}
