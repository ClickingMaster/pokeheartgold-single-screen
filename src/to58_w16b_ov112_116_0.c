#include "global.h"
extern int TextPrinterCheckActive(int);
extern void FillBgTilemapRect(void *, int, int, int, int, int, int, int);
extern void BgCommitTilemapBufferToVram(void *, int);
int ov112_021EDE1C(void *);
int ov112_021EDE1C(void *s) {
    int next;
    if (!TextPrinterCheckActive(*(u32 *)((u8 *)s + 0x1e524) & 0xff)) {
        *(int *)((u8 *)s + 4) = 0;
        FillBgTilemapRect(*(void **)((u8 *)s + 0x18), 2, 0, 0, 0, 0x20, 0x18, 0x10);
        BgCommitTilemapBufferToVram(*(void **)((u8 *)s + 0x18), 2);
        next = *(int *)((u8 *)s + 0xc);
        *(int *)((u8 *)s + 8) = next;
        return next;
    }
    return 0x22;
}
extern void ov112_021E7464(void *);
extern int ov112_021ED640(void *);
extern void BufferBoxMonNickname(void *, int, void *);
extern int ov112_021EA08C(void *, int, int);
extern void PlayFanfare(int);
int ov112_021EDE68(void *);
int ov112_021EDE68(void *s) {
    void *box;
    ov112_021E7464(s);
    if (!ov112_021ED640(s)) {
        MIi_CpuClearFast(0, (u32 *)((u8 *)s + 0xad00), 0x2fc);
        *(u8 *)((u8 *)s + 0xaabc) &= ~0x20;
    }
    box = *(void **)((u8 *)s + 0x1e42c);
    if (box) {
        BufferBoxMonNickname(*(void **)((u8 *)s + 0x1e448), 1, box);
    } else {
        BufferBoxMonNickname(*(void **)((u8 *)s + 0x1e448), 1, *(void **)((u8 *)s + 0x1e430));
    }
    FillBgTilemapRect(*(void **)((u8 *)s + 0x18), 2, 0, 0, 0, 0x20, 0x18, 0x10);
    BgCommitTilemapBufferToVram(*(void **)((u8 *)s + 0x18), 2);
    *(int *)((u8 *)s + 0x1e524) = ov112_021EA08C(s, 2, 0x18);
    PlayFanfare(0x4a2);
    return 0x24;
}
