#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define H(a)      (*(volatile u16 *)(a))
#define WL        (*(u8 **)0x0380fff4)
#define R16(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define R32(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define P(p, o)   (*(u8 **)((u8 *)(p) + (o)))
extern int CheckFrameTimeout(void *);

typedef struct {
    u16 state, pad, count, reserved;
    u8 *frame, *out;
    void (*done)(void *, int);
} W35522Slot;

void WlIntrTxEndTask(void);

void WlIntrTxEndTask(void) {
    u8 *base = WL;
    W35522Slot *slots = (W35522Slot *)(base + 0x42c);
    u8 *stats = base + 0x344;
    W35522Slot *entry;
    int i;
    for (i = 2; i >= 0; i--) {
        entry = &slots[i];
        if (H(0x048080a0 + i * 4) & 0x8000) {
            continue;
        }
        if (!entry->state) {
            continue;
        }
        if (entry->out) {
            R16(entry->out, 8) = R16(entry->frame, 0);
            R16(entry->out, 0x14) = R16(entry->frame, 0xc);
            if (R16(entry->frame, 0xc) & 0x4000) {
                u8 *frame = entry->frame;
                if (R16(WL, 0x690) & 8) {
                    u16 *tail = (u16 *)((u32)((u32)(frame + 0xc) + R16(frame, 0xa) - 7) & ~1u);
                    if (tail[0] == 0 && tail[1] == 0) {
                        H(0x04808032) = 0;
                        H(0x04808032) = 0x8000;
                        R16(stats, 0xba)
                        ++;
                        if (CheckFrameTimeout(entry->out)) {
                            R16(entry->out, 8) = 2;
                            entry->count++;
                            R16(slots, 0xae)
                            ++;
                            entry->done(entry->out, 1);
                        } else {
                            R16(entry->out, 0xc) = 0;
                            H(0x048080a0 + i * 4) |= 0x8000;
                        }
                        continue;
                    }
                }
                R16(entry->out, 0xc) = (u16)(R16(entry->out, 0xc) + (R16(frame, 4) & 255));
            } else {
                R16(entry->out, 0xc) = R16(entry->frame, 4) & 255;
            }
            entry->count++;
            entry->done(entry->out, 2);
        } else {
            entry->count++;
            entry->done(entry->frame, 3);
        }
    }
}
