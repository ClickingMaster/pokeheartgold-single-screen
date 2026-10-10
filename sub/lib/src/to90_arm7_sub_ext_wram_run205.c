#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define H(a)      (*(volatile u16 *)(a))
#define WL        (*(u8 **)0x0380fff4)
#define R16(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define R32(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define P(p, o)   (*(u8 **)((u8 *)(p) + (o)))
extern void TxqPri(int);
void SetParentTbttTxqTask(void);

void SetParentTbttTxqTask(void) {
    u8 *work = WL;
    u8 *queue = work + 0x17c;
    if (H(0x04808088) == 0 && R16(queue, 0x38)) {
        H(0x048080ae) = 8;
        TxqPri(2);
    } else {
        if (!(R16(work, 0x52e) & ~R16(work, 0x532))) {
            H(0x048080ae) = 8;
            if (R16(queue, 0x38)) {
                TxqPri(2);
            }
        }
        H(0x048080ae) = 5;
        if (R16(queue, 0x2c)) {
            TxqPri(1);
        }
        if (R16(queue, 0x20)) {
            TxqPri(0);
        }
    }
}
