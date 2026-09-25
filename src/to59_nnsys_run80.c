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

void FreeCommandBuffer(void *command) {
    u32 interrupts = OS_DisableInterrupts();
    NNS_FndAppendListObject(&_021E095C, command);
    OS_RestoreInterrupts(interrupts);
}

void *NNSi_SndSeqArcGetSeqInfo(void *seqArc, int index) {
    u8 *base = seqArc;
    u8 *entry;
    if (index < 0) {
        return 0;
    }
    if ((u32)index >= *(u32 *)(base + 0x1c)) {
        return 0;
    }
    entry = base + 0x20 + index * 12;
    if (*(u32 *)entry == (u32)-1) {
        return 0;
    }
    return entry;
}
