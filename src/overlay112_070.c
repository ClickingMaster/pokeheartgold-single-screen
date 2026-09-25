#include "global.h"
#include "system.h"
extern void ov112_021E9C10(void *, int);
extern void ov112_021E9A78(void *, int);
extern void FillBgTilemapRect(void *, int, int, int, int, int, int, int);
extern void BgCommitTilemapBufferToVram(void *, int);
extern void ov112_021EA570(void *);
void ov112_021EB90C(u8 *, int);
void ov112_021EB90C(u8 *work, int arg1) {
    *(u32 *)(work + 4) = 2;
    ov112_021E9C10(work, arg1);
    ov112_021E9A78(work, 1);
    FillBgTilemapRect(*(void **)(work + 0x18), 1, 0, 0, 0, 0x20, 0x18, 0x10);
    BgCommitTilemapBufferToVram(*(void **)(work + 0x18), 1);
    ov112_021EA570(work);
}

extern void sub_02032688(void *, u16 *, void *);
extern void ov112_021E9C10(void *, int);
extern void ov112_021E9A78(void *, int);
extern void FillBgTilemapRect(void *, int, int, int, int, int, int, int);
extern void BgCommitTilemapBufferToVram(void *, int);
extern void ov112_021EA570(void *);
int ov112_021EB950(u8 *, int);
int ov112_021EB950(u8 *work, int arg1) {
    u8 result[4];
    sub_02032688(*(void **)(work + 0x1E440), (u16 *)(result + 2), result);
    if (*(u16 *)(result + 2) != 2) return 0;
    *(u32 *)(work + 4) = 3;
    *(u32 *)(work + 0x10) = 4;
    ov112_021E9C10(work, arg1);
    ov112_021E9A78(work, 1);
    FillBgTilemapRect(*(void **)(work + 0x18), 1, 0, 0, 0, 0x20, 0x18, 0x10);
    BgCommitTilemapBufferToVram(*(void **)(work + 0x18), 1);
    ov112_021EA570(work);
    return 1;
}

int ov112_021EB9C0(void);
int ov112_021EB9E4(void);
int ov112_021EB9C0(void) {
    u32 keys = gSystem.heldKeys;
    return (keys & 0x80) && (keys & 0x400) && (keys & 0x200);
}
int ov112_021EB9E4(void) {
    u32 keys = gSystem.heldKeys;
    return (keys & 0x40) && (keys & 4) && (keys & 0x100);
}
