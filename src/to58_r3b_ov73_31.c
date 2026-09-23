#include "global.h"

extern void DrawFrameAndWindow1(void *, u32, u32, u32);
extern void ov73_021EA0F0(void *, void *, u32, u32);
extern void ov73_021EA134(void *, u32, u32);
extern void CopyWindowToVram(void *);
int ov73_021E98AC(void *);

int ov73_021E98AC(void *work) {
    *(u32 *)((u8 *)work + 0x9C) = 1;
    DrawFrameAndWindow1((u8 *)work + 0xDF8, 0, 31, 14);
    ov73_021EA0F0((u8 *)work + 0xDF8, *(void **)((u8 *)work + 0xBE8), *(u32 *)((u8 *)work + 0x9C), 3);
    ov73_021EA134(work, 0, 1);
    CopyWindowToVram((u8 *)work + 0xDF8);
    *(u32 *)((u8 *)work + 0x1c) = 34;
    return 3;
}
