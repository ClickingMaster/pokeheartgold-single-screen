#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/spi/ARM9/tp.h>
#include <nitro/code32.h>
// clang-format on
#define REG_DIVCNT (*(vu16 *)0x4000280)
#define REG_DIV_NUMER (*(vu32 *)0x4000290)
#define REG_DIV_DENOM (*(vu64 *)0x4000298)
#define REG_DIV_RESULT (*(vs32 *)0x40002a0)
int TP_CalcCalibrateParam(TPCalibrateParam *calibrate, u16 raw_x1, u16 raw_y1, u16 dx1, u16 dy1, u16 raw_x2, u16 raw_y2, u16 dx2, u16 dy2);
static inline void SetDiv(s32 numer, s32 denom)
{
    REG_DIVCNT = 0;
    REG_DIV_NUMER = numer;
    *(REGType64 *)0x4000298 = (u32)denom;
}
static inline void WaitDiv(void)
{
    while (REG_DIVCNT & 0x8000) { }
}
// @fn TP_CalcCalibrateParam
int TP_CalcCalibrateParam(TPCalibrateParam *calibrate, u16 raw_x1, u16 raw_y1, u16 dx1, u16 dy1, u16 raw_x2, u16 raw_y2, u16 dx2, u16 dy2)
{
    s32 rx_width, dx_width, ry_width, dy_width;
    s32 tmp32;
    s32 y0;
    OSIntrMode enabled;

    if (raw_x1 >= 4096 || raw_y1 >= 4096 || raw_x2 >= 4096 || raw_y2 >= 4096) {
        return 1;
    }
    if (dx1 >= 256 || dx2 >= 256 || dy1 >= 192 || dy2 >= 192) {
        return 1;
    }
    if (dx1 == dx2 || dy1 == dy2 || raw_x1 == raw_x2 || raw_y1 == raw_y2) {
        return 1;
    }
    enabled = OS_DisableInterrupts();
    rx_width = raw_x1 - raw_x2;
    dx_width = dx1 - dx2;
    ry_width = raw_y1 - raw_y2;
    dy_width = dy1 - dy2;
    SetDiv(rx_width << 8, dx_width);
    WaitDiv();
    tmp32 = REG_DIV_RESULT;
    SetDiv(ry_width << 8, dy_width);
    if (tmp32 >= 0x8000 || tmp32 < -0x8000) {
        OS_RestoreInterrupts(enabled);
        return 1;
    }
    calibrate->xDotSize = (s16)tmp32;
    tmp32 = (s16)((((raw_x1 + raw_x2) << 8) - (dx1 + dx2) * calibrate->xDotSize) >> 7);
    if (tmp32 >= 0x8000 || tmp32 < -0x8000) {
        OS_RestoreInterrupts(enabled);
        return 1;
    }
    calibrate->x0 = (s16)tmp32;
    WaitDiv();
    tmp32 = REG_DIV_RESULT;
    OS_RestoreInterrupts(enabled);
    if (tmp32 >= 0x8000 || tmp32 < -0x8000) {
        return 1;
    }
    calibrate->yDotSize = (s16)tmp32;
    y0 = (s16)((((raw_y1 + raw_y2) << 8) - (dy1 + dy2) * calibrate->yDotSize) >> 7);
    if (y0 >= 0x8000 || y0 < -0x8000) {
        return 1;
    }
    calibrate->y0 = (s16)y0;
    return 0;
}
