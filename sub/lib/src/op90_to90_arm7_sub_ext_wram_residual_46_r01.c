#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern void *AllocateHeapBuf(void *heap, u32 size);
extern void DMA_Read(void *dst, const void *src, u32 size);

typedef struct {
    u8 heapHeader[0x10];
    u8 unk10[6];
    u16 length; // 0x16
    u8 unk18[6];
    u16 unk1E;
    u16 unk20;
    u16 unk22;
} RxFrameBuf;

void *TakeoutRxFrame(void *src, u32 size);

void *TakeoutRxFrame(void *src, u32 size) {
    RxFrameBuf *buf = AllocateHeapBuf(*(u8 **)0x0380fff4 + 0x188, size + 0x22);
    if (buf == NULL) {
        return NULL;
    }
    DMA_Read(buf->unk18, src, size + 12);
    buf->length = size - 24;
    buf->unk1E = (buf->unk1E & 0xff) | ((u16)(buf->unk22 & 0xff) << 8);
    return buf->unk10;
}
