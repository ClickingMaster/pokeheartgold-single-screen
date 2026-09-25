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

u32 NNSi_SndArcLoadWaveArc(u32 id, u32 flags, void *heap, u32 load, void **out) {
    WaveWord *info = NNS_SndArcGetWaveArcInfo(id);
    if (!info) {
        return 5;
    }
    u32 packed = *(u32 *)info;
    void *address;
    if (flags & 4) {
        if (info->flag & 1) {
            address = LoadWaveArcTable(info->fileId, heap, load);
        } else {
            address = LoadWaveArc(info->fileId, heap, load);
        }
        if (!address) {
            return 9;
        }
    } else {
        address = NNS_SndArcGetFileAddress(info->fileId);
    }
    if (out) {
        *out = address;
    }
    return 0;
}

void *NNSi_SndArcLoadFile(u32 id, void *callback, void *arg, u32 tag, void *heap) {
    u32 size = NNS_SndArcGetFileSize(id);
    if (!size) {
        return 0;
    }
    if (!heap) {
        return 0;
    }
    void *data = NNS_SndHeapAlloc(heap, size + 0x20, callback, arg, tag);
    if (!data) {
        return 0;
    }
    if (NNS_SndArcReadFile(id, data, size, 0) != size) {
        return 0;
    }
    DC_StoreRange(data, size);
    return data;
}
