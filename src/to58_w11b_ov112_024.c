#include "global.h"
extern const u8 ov112_021F3684[];
extern const u8 ov112_021F3704[];
extern const u8 ov112_021F3720[];
extern const u8 ov112_021F373C[];
extern const u8 ov112_021F3758[];
extern const u8 ov112_021F3774[];
extern const u8 ov112_021F3790[];
extern void SetBothScreensModesAndDisable(void *);
extern void InitBgFromTemplate(void *, int, const void *, int);
extern void BgClearTilemapBufferAndCommit(void *, int);
extern void BG_ClearCharDataRange(int, int, int, int);
typedef struct FourWords { u32 v[4]; } FourWords;
void ov112_021E79F0(void *bg);
void ov112_021E79F0(void *bg) {
    FourWords modes = *(const FourWords *)ov112_021F3684;
    SetBothScreensModesAndDisable(&modes);
    InitBgFromTemplate(bg, 1, ov112_021F3704, 0);
    InitBgFromTemplate(bg, 2, ov112_021F3720, 0);
    InitBgFromTemplate(bg, 3, ov112_021F373C, 0);
    BgClearTilemapBufferAndCommit(bg, 1);
    BgClearTilemapBufferAndCommit(bg, 2);
    BgClearTilemapBufferAndCommit(bg, 3);
    InitBgFromTemplate(bg, 4, ov112_021F3758, 0);
    InitBgFromTemplate(bg, 5, ov112_021F3774, 0);
    InitBgFromTemplate(bg, 6, ov112_021F3790, 0);
    BgClearTilemapBufferAndCommit(bg, 4);
    BgClearTilemapBufferAndCommit(bg, 5);
    BgClearTilemapBufferAndCommit(bg, 6);
    BG_ClearCharDataRange(4, 0x20, 0, 0x9a);
    BG_ClearCharDataRange(5, 0x20, 0, 0x9a);
    BG_ClearCharDataRange(6, 0x20, 0, 0x9a);
}
