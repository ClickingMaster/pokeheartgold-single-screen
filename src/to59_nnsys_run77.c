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

#define DEFINE_ARC_LOAD(name, callback)                                          \
    void *name(int fileId, u32 loadFlag, int cache) {                            \
        void *data = NNS_SndArcGetFileAddress(fileId);                           \
        if (data == 0) {                                                         \
            void *arc = cache ? NNS_SndArcGetCurrent() : 0;                      \
            data = NNSi_SndArcLoadFile(fileId, callback, arc, fileId, loadFlag); \
            if (cache && data != 0)                                              \
                NNS_SndArcSetFileAddress(fileId, data);                          \
        }                                                                        \
        return data;                                                             \
    }

DEFINE_ARC_LOAD(LoadSeq, SeqDisposeCallback)
DEFINE_ARC_LOAD(LoadSeqArc, SeqDisposeCallback)
DEFINE_ARC_LOAD(LoadBank, BankDisposeCallback)
DEFINE_ARC_LOAD(LoadWaveArc, WaveArcDisposeCallback)
