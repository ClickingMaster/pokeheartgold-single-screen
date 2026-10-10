#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define G       (*(u8 **)0x0380FFF4)
#define H(p, n) (*(u16 *)((u8 *)(p) + (n)))
#define V(n)    (*(volatile u16 *)(n))
#define P(p, n) (*(u8 **)((u8 *)(p) + (n)))
void WUpdateCounter(void);
void DMA_Write(void *, const void *, u32);
void DMA_WriteHeaderData(void *, const void *, const void *, u32);
void DMA_WepWriteHeaderData(void *, const void *, const void *, u32);
void CopyTxFrmToMacBuf(void *dest, void *source);

typedef struct {
    u8 pad[12];
    u16 owner;
    u8 pad2[8];
    u16 length;
    u8 header[10];
    u16 total, control;
    u8 pad3[22];
} W40781TxFrame;

void CopyTxFrmToMacBuf(void *dest, void *source) {
    W40781TxFrame *f = source;
    u8 *d = dest, *a = source, *t, *g;
    u32 y, x;
    u8 **owner = (u8 **)0x0380fff4;
    if (f->control & 0x4000) {
        if (H(G, 0x350) == 3) {
            WUpdateCounter();
        }
        if (f->owner == 0xffff) {
            DMA_WepWriteHeaderData(d, a + 0x18, a + 0x3c, f->length);
        } else {
            DMA_WepWriteHeaderData(d, a + 0x18, *(u8 **)(a + 0x3c), f->length);
        }
        x = V(0x04808044);
        y = V(0x04808044);
        H(d, 0x24) = (u16)(x + (y << 8));
        g = *owner;
        x = V(0x04808044) & 255;
        H(d, 0x26) = (H(g, 0x336) << 14) | x;
        if (H(G, 0x690) & 8) {
            t = (u8 *)(((u32)(d + 12 + f->total) - 7) & ~1u);
            H(t, 0) = 0;
            H(t, 2) = 0;
        }
    } else {
        if (f->owner == 0xffff) {
            DMA_Write(d, a + 0x18, f->length + 0x24);
        } else {
            DMA_WriteHeaderData(d, a + 0x18, *(u8 **)(a + 0x3c), f->length);
        }
    }
    if (H(G, 0x690) & 4) {
        t = (u8 *)(((u32)(d + 12 + f->total) - 1) & ~3u);
        H(t, 0) = 0xb6b8;
        H(t, 2) = 0x1d46;
    }
}
