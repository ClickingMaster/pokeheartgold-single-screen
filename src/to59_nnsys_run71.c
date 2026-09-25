#include <nitro/code32.h>
#include <nitro/types.h>
typedef struct {
    u32 a, b;
} FileId;
typedef struct {
    u32 fileId : 24;
    u32 flag : 8;
} WaveWord;
extern void *_021E0910;
void *NNS_SndArcSetCurrent(void *arc);
void *NNS_SndArcGetCurrent(void);
void *NNS_SndArcGetSeqParam(int index);
void *NNS_SndArcGetSeqInfo(int index);
void *NNS_SndArcGetSeqArcInfo(int index);
void *NNS_SndArcGetBankInfo(int index);
void *NNS_SndArcGetWaveArcInfo(int index);
void *NNS_SndArcGetStrmInfo(int index);
void *NNS_SndArcGetGroupInfo(int index);
u32 NNS_SndArcGetFileSize(u32 index);
void *NNS_SndArcGetFileAddress(u32 index);
void NNS_SndArcSetFileAddress(u32 index, void *address);
void InfoDisposeCallback(void *a, void *b, u8 *arc);
void FatDisposeCallback(void *a, void *b, u8 *arc);
void SymbolDisposeCallback(void *a, void *b, u8 *arc);
void *NNS_SndArcGetSeqInfo(int index);
extern u32 NNS_FndGetAllocatableSizeForFrmHeapEx(void *, u32);
extern void NNS_FndInitList(void *, u32);
extern u32 SND_GetCurrentCommandTag(void);
extern void SND_FlushCommand(u32);
extern void SND_WaitForCommandProc(u32);
extern u32 NNSi_SndArcLoadGroup(u32, void *);
extern u32 NNSi_SndArcLoadSeq(u32, u32, void *, u32, void **);
extern u32 NNSi_SndArcLoadBank(u32, u32, void *, u32, void *);
extern u32 NNSi_SndArcLoadWaveArc(u32, u32, void *, u32, void **);
u32 NNS_SndHeapGetSize(void *heap);
u32 NNS_SndHeapGetFreeSize(void *heap);
void InitHeapSection(void *section);
void EraseSync(void);
u32 NNS_SndArcLoadGroup(u32 id, void *heap);
u32 NNS_SndArcLoadSeq(u32 id, void *heap);
u32 NNS_SndArcLoadBank(u32 id, void *heap);
u32 NNS_SndArcLoadWaveArc(u32 id, void *heap);
u32 NNS_SndArcLoadSeqEx(u32 id, u32 flags, void *heap);
extern void NNS_FndDestroyFrmHeap(void *);
extern void *NNS_FndCreateFrmHeapEx(void *, u32, u32);
extern void *NNS_FndAllocFromFrmHeapEx(void *, u32, u32);
extern void NNS_FndAppendListObject(void *, void *);
void NNS_SndHeapClear(void *heap);
void NNS_SndHeapDestroy(void *heap);
u32 InitHeap(void *heap, void *frame);
u32 NewSection(void *heap);
void *NNS_SndHeapCreate(void *buf, u32 size);
extern BOOL NNS_FndRecordStateForFrmHeap(void *, u32);
extern void NNS_FndFreeByStateToFrmHeap(void *, u32);
int NNS_SndHeapSaveState(void *heap);
extern void *NNS_FndGetPrevListObject(void *, void *);
void *NNS_SndHeapAlloc(void *heap, u32 size, void *callback, void *arg, u32 tag);
extern void *LoadSeqArc(u32 id, void *heap, u32 load);
u32 NNSi_SndArcLoadSeqArc(u32 id, u32 flags, void *heap, u32 load, void **out);
extern void *LoadSeq(u32 id, void *heap, u32 load);
extern void *LoadWaveArcTable(u32 id, void *heap, u32 load);
extern void *LoadWaveArc(u32 id, void *heap, u32 load);
extern void MIi_CpuCopy32(const void *, void *, u32);
void NNS_SndArcInitOnMemory(void *arc, void *archive);
extern BOOL FS_ConvertPathToFileID(void *, const char *);
extern void FS_InitFile(void *);
extern BOOL FS_OpenFileFast(void *, FileId);
extern BOOL NNS_SndArcSetup(void *, void *, BOOL);
void NNS_SndArcInit(void *arc, const char *path, void *heap, BOOL loadSymbols);
extern int NNS_SndArcReadFile(u32 id, void *dst, u32 len, u32 offset);
extern void DC_StoreRange(void *ptr, u32 len);
void *NNSi_SndArcLoadFile(u32 id, void *callback, void *arg, u32 tag, void *heap);

typedef struct {
    u8 pad[0x8c];
    u8 *info;
} Arc;
#define INFO_GETTER(name, off)                                   \
    void *name(int index) {                                      \
        Arc *arc = _021E0910;                                    \
        u8 *info = arc->info;                                    \
        u32 tableOffset = *(u32 *)(info + off);                  \
        u8 *table = (tableOffset == 0) ? 0 : info + tableOffset; \
        if (table == 0)                                          \
            return 0;                                            \
        if (index < 0)                                           \
            return 0;                                            \
        if ((u32)index >= *(u32 *)table)                         \
            return 0;                                            \
        u32 value = *(u32 *)(table + 4 + index * 4);             \
        u8 *base = *(u8 *volatile *)((u8 *)arc + 0x8c);          \
        return (value == 0) ? 0 : base + value;                  \
    }
void NNS_SndArcInitOnMemory(void *arc, void *archive) {
    u8 *a = arc;
    u8 *data = archive;
    MIi_CpuCopy32(data, a, 0x30);
    u32 off = *(u32 *)(a + 0x18);
    *(u8 **)(a + 0x8c) = (off == 0) ? 0 : data + off;
    off = *(u32 *)(a + 0x20);
    *(u8 **)(a + 0x84) = (off == 0) ? 0 : data + off;
    off = *(u32 *)(a + 0x10);
    *(u8 **)(a + 0x88) = (off == 0) ? 0 : data + off;
    *(u32 *)(a + 0x90) = 0;
    for (u32 i = 0; i < *(u32 *)(*(u8 **)(a + 0x84) + 8); ++i) {
        u8 *entry = *(u8 **)(a + 0x84) + 0xc + 0x10 * i;
        u32 offset = *(u32 *)entry;
        *(u8 **)(entry + 8) = (offset == 0) ? 0 : data + offset;
    }
    *(u32 *)(a + 0x30) = 0;
    _021E0910 = a;
}

void *NNS_SndArcSetCurrent(void *arc) {
    void *old = _021E0910;
    _021E0910 = arc;
    return old;
}

void *NNS_SndArcGetCurrent(void) {
    return _021E0910;
}

void *NNS_SndArcGetSeqParam(int index) {
    u8 *seq = NNS_SndArcGetSeqInfo(index);
    return (seq == 0) ? 0 : seq + 4;
}

INFO_GETTER(NNS_SndArcGetSeqInfo, 0x8)

INFO_GETTER(NNS_SndArcGetSeqArcInfo, 0xc)

INFO_GETTER(NNS_SndArcGetBankInfo, 0x10)

INFO_GETTER(NNS_SndArcGetWaveArcInfo, 0x14)

INFO_GETTER(NNS_SndArcGetStrmInfo, 0x18)

INFO_GETTER(NNS_SndArcGetGroupInfo, 0x1c)

u32 NNS_SndArcGetFileSize(u32 index) {
    u8 *arc = _021E0910;
    u8 *fat = *(u8 **)(arc + 0x84);
    u32 n = *(u32 *)(fat + 8);
    if (index >= n) {
        return 0;
    }
    return *(u32 *)(fat + 0x10 + 16 * index);
}
