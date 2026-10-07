#include <nitro/code32.h>
#include <nitro/types.h>
typedef void (*W17HeapCallback)(void *,u32,void *,int);
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
void InfoDisposeCallback(void *a,u32 size,void *context,int tag);
void FatDisposeCallback(void *a,u32 size,void *context,int tag);
void SymbolDisposeCallback(void *a,u32 size,void *context,int tag);
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
u32 NNS_SndHeapClear(void *heap);
void NNS_SndHeapDestroy(void *heap);
u32 InitHeap(void *heap, void *frame);
u32 NewSection(void *heap);
void *NNS_SndHeapCreate(void *buf, u32 size);
extern BOOL NNS_FndRecordStateForFrmHeap(void *, u32);
extern void NNS_FndFreeByStateToFrmHeap(void *, u32);
int NNS_SndHeapSaveState(void *heap);
extern void *NNS_FndGetPrevListObject(void *, void *);
void *NNS_SndHeapAlloc(void *heap,u32 size,W17HeapCallback callback,void *arg,int tag);
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
void *NNSi_SndArcLoadFile(u32 id,W17HeapCallback callback,void *arg,int tag,void *heap);

void *NNS_SndHeapAlloc(void *heap,u32 size,W17HeapCallback callback,void *arg,int tag) {
    u32 amount = ((size + 31) & ~31) + 32;
    u8 *block = NNS_FndAllocFromFrmHeapEx(*(void **)heap, amount, 32);
    if (!block) {
        return 0;
    }
    void *section = NNS_FndGetPrevListObject((u8 *)heap + 4, 0);
    *(u32 *)(block + 8) = size;
    *(W17HeapCallback *)(block + 0xc) = callback;
    *(void **)(block + 0x10) = arg;
    *(int *)(block + 0x14) = tag;
    NNS_FndAppendListObject(section, block);
    return block + 0x20;
}

int NNS_SndHeapSaveState(void *heap) {
    u16 level = *(u16 *)((u8 *)heap + 0xc);
    void *frame = *(void **)heap;
    if (!NNS_FndRecordStateForFrmHeap(frame, level)) {
        return -1;
    }
    if (NewSection(heap)) {
        return *(u16 *)((u8 *)heap + 0xc) - 1;
    }
    NNS_FndFreeByStateToFrmHeap(*(void **)heap, 0);
    return -1;
}
