#include "global.h"
extern int (*ov112_021FF924[])(void *);
int ov112_021EF67C(void *);
int ov112_021EF67C(void *s) {
    int result = ov112_021FF924[*(int *)((u8 *)s + 8)](s);
    *(int *)((u8 *)s + 8) = result;
    if (result == 37) {
        *(int *)((u8 *)s + 8) = 0;
        return 3;
    }
    return 2;
}
extern void FillBgTilemapRect(void *, int, int, int, int, int, int, int);
extern void BgCommitTilemapBufferToVram(void *, int);
extern void PokewalkerApp_RemoveWindowBank(void *, int);
extern void PokewalkerApp_AddWindowBank(void *, int);
int ov112_021EF6A0(void *);
int ov112_021EF6A0(void *s) {
    FillBgTilemapRect(*(void **)((u8 *)s + 0x18), 1, 0, 0, 0, 0x20, 0x18, 0x10);
    FillBgTilemapRect(*(void **)((u8 *)s + 0x18), 2, 0, 0, 0, 0x20, 0x18, 0x10);
    BgCommitTilemapBufferToVram(*(void **)((u8 *)s + 0x18), 1);
    BgCommitTilemapBufferToVram(*(void **)((u8 *)s + 0x18), 2);
    PokewalkerApp_RemoveWindowBank(s, 6);
    PokewalkerApp_AddWindowBank(s, 1);
    return 1;
}
