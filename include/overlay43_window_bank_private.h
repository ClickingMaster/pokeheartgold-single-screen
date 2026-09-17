#ifndef TO46_OV43_R29_PREFIX_PRIVATE_H
#define TO46_OV43_R29_PREFIX_PRIVATE_H

#include "global.h"

typedef struct Window Window;
typedef struct BgConfig BgConfig;

typedef struct Ov43WindowBank {
    s16 xOffset;
    s16 yOffset;
    u32 pad04;
    u32 baseTile;
    Window *windows[8];
    u32 counts[8];
} Ov43WindowBank;

typedef struct Ov43WindowSpec {
    u8 x;
    u8 y;
    u8 width;
    u8 height;
} Ov43WindowSpec;

typedef void (*Ov43PageDrawFunc)(void *, void *, void *, void *);

extern Ov43PageDrawFunc _0222F0E0[];
extern Ov43PageDrawFunc ov43_0222F12C[];

Window *AllocWindows(u32 heapId, s32 count);
void InitWindow(void *window);
void AddWindowParameterized(void *bgConfig, void *window, u8 bgId, u8 x, u8 y, u8 width, u8 height, u8 palette, u32 baseTile);
void FillWindowPixelBuffer(void *window, u8 fillValue);
void WindowArray_Delete(void *windows, u8 count);
void ScheduleWindowCopyToVram(void *window);
void ClearWindowTilemapAndScheduleTransfer(void *window);
void ov43_0222DF90(void *work, void *coords);

void ov43_0222DE14(Ov43WindowBank *bank, BgConfig **bgConfig, u32 bankIndex, u32 count, const Ov43WindowSpec *specs, u32 heapId);
void ov43_0222DED0(Ov43WindowBank *bank, u32 bankIndex);
void ov43_0222DEF8(Ov43WindowBank *bank, u32 bankIndex);
void ov43_0222DF1C(Ov43WindowBank *bank, u32 bankIndex);
void ov43_0222DF4C(void *arg0, void *arg1, void *arg2, u32 page, u32 unused, BOOL alternate, void *arg6);

#endif
