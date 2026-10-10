#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
#include <string.h>
typedef void (*W17HeapCallback)(void *, u32, void *, int);
#define W(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define B(p, o) (*(u8 *)((u8 *)(p) + (o)))
extern void *SND_GetWaveDataAddress(void *, u32);
extern u32 SND_GetWaveDataCount(void *);
extern void *NNS_SndHeapAlloc(void *, u32, W17HeapCallback, void *, int);
extern u32 NNS_SndArcReadFile(int, void *, u32, u32);
extern void SND_SetWaveDataAddress(void *, u32, void *);
extern void SingleWaveDisposeCallback(void *, u32, void *, int);
int LoadSingleWave(u8 *, int, int, u32);

int LoadSingleWave(u8 *arc, int index, int file, u32 heap) {
    u32 start, end, size, count;
    u8 *data;
    start = (u32)SND_GetWaveDataAddress((void *)arc, index);
    if (start) {
        return 1;
    }
    count = SND_GetWaveDataCount((void *)arc);
    start = W(arc + (W(arc, 56) + index) * 4, 60);
    end = index < count - 1 ? W(arc + (W(arc, 56) + index) * 4, 64) : W(arc, 8);
    size = end - start;
    if (!heap) {
        return 0;
    }
    data = NNS_SndHeapAlloc((void *)heap, size + 32, SingleWaveDisposeCallback, arc, index);
    if (!data) {
        return 0;
    }
    if (NNS_SndArcReadFile(file, data, size, start) != size) {
        return 0;
    }
    DC_StoreRange(data, size);
    SND_SetWaveDataAddress((void *)arc, index, data);
    return 1;
}
