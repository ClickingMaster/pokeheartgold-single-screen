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

void NNSi_SndFaderInit(Fader *fader) {
    fader->target = 0;
    fader->start = 0;
    fader->duration = 0;
    fader->elapsed = 0;
}

void NNSi_SndFaderSet(Fader *fader, s32 target, s32 duration) {
    fader->start = NNSi_SndFaderGet(fader);
    fader->target = target;
    fader->duration = duration;
    fader->elapsed = 0;
}

s32 NNSi_SndFaderGet(Fader *fader) {
    if (fader->elapsed >= fader->duration) {
        return fader->target;
    }
    return fader->start + (fader->target - fader->start) * fader->elapsed / fader->duration;
}

void NNSi_SndFaderUpdate(Fader *fader) {
    if (fader->elapsed < fader->duration) {
        fader->elapsed++;
    }
}

int NNSi_SndFaderIsFinished(Fader *fader) {
    return fader->elapsed >= fader->duration;
}
