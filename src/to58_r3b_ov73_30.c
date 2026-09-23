#include "global.h"

extern void DrawFrameAndWindow1(void *, u32, u32, u32);
extern void ov73_021EA0F0(void *, void *, u32, u32);
extern void ov73_021EA134(void *, u32, u32);
extern void CopyWindowToVram(void *);
int ov73_021E9758(void *);

int ov73_021E9758(void *work) {
    *(u32 *)((u8 *)work + 0x94) = 1;
    DrawFrameAndWindow1((u8 *)work + 0xDE8, 0, 31, 14);
    ov73_021EA0F0((u8 *)work + 0xDE8, *(void **)((u8 *)work + 0xBEC), *(u32 *)((u8 *)work + 0x94), 2);
    ov73_021EA134(work, 1, 1);
    CopyWindowToVram((u8 *)work + 0xDE8);
    *(u32 *)((u8 *)work + 0x1c) = 31;
    return 3;
}
