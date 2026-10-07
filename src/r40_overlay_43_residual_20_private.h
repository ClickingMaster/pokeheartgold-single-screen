#ifndef SOL_R40_NIGHT_OVERLAY43_RESIDUAL20_PRIVATE_H
#define SOL_R40_NIGHT_OVERLAY43_RESIDUAL20_PRIVATE_H
#include "global.h"
#include "unk_0202C034.h"
typedef struct {
    s16 x, y;
    u8 w, h, palette, a7, a8, a9;
} Ov43Rect;
extern const Ov43Rect ov43_0222EFA0[];
void LoadRectToBgTilemapRect(void *, int, void *, int, int, int, int);
void ScheduleBgTilemapBufferTransfer(void *, int);
void ov43_0222AD74(void *, int, int);
void Sprite_SetPositionXY(void *, int, int);
void BgTilemapRectChangePalette(void *, int, int, int, int, int, int);
void ov43_0222CA30(void *, void *);
void ClearWindowTilemapAndScheduleTransfer(void *);
void FillBgTilemapRect(void *, int, int, int, int, int, int, int);
void ov43_0222C7F8(s16 *, void *, void *);
void ov43_0222C844(void *, void *, int);
void ov43_0222C890(s16 *, void *, int, int);
void ov43_0222C9A4(s16 *, void *);
void FillWindowPixelBuffer(void *, u8);
void ov43_0222CA50(s16 *, void *, void *, void *, int, void *, int, int);
void ov43_0222CB34(s16 *, void *, void *, int);
void ScheduleWindowCopyToVram(void *);
void ov43_0222C918(void *, void *, void *, void *, void *, u8 *, int, int);
#endif
