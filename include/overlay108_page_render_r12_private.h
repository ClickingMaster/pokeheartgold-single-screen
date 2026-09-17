#ifndef TO50_OV108_R12_H
#define TO50_OV108_R12_H
#include "global.h"
typedef struct Ov108ScreenData12 {
    u16 width;
    u16 height;
    u8 pad4[8];
    u8 data[1];
} Ov108ScreenData12;
typedef struct Ov108Window12 {
    u8 raw[0x10];
} Ov108Window12;
typedef struct Ov108Work12 {
    u8 pad0[0x304];
    void *msgData;
    void *format;
    void *string;
    void *strings2[5];
    void *strings01[5];
    u8 pad338[0x340 - 0x338];
    void *bgConfig;
    u8 pad344[0x354 - 0x344];
    void *sprites[18];
    u8 pad39c[0x3b4 - 0x39c];
    Ov108Window12 windows[15];
    u8 pad4a4[0x4d8 - 0x4a4];
    Ov108ScreenData12 *screen;
    u8 pad4dc[0x184de - 0x4dc];
    u8 page;
} Ov108Work12;
void FillBgTilemapRect(void *, u8, u16, u8, u8, u8, u8, u8);
void CopyToBgTilemapRect(void *, u8, u8, u8, u8, u8, const void *, u8, u8, u8, u8);
void ScheduleBgTilemapBufferTransfer(void *, u8);
u32 GetWindowWidth(void *);
u32 GetWindowHeight(void *);
void FillWindowPixelBuffer(void *, u8);
void FillWindowPixelRect(void *, u8, u16, u16, u16, u16);
u32 FontID_String_GetWidth(u32, void *, u32);
void AddTextPrinterParameterizedWithColor(void *, u32, void *, u32, u32, u32, u32, u32);
void BufferIntegerAsString(void *, u32, s32, u32, u32, u32);
void StringExpandPlaceholders(void *, void *, void *);
void Sprite_SetAnimActiveFlag(void *, BOOL);
void Sprite_SetAnimCtrlSeq(void *, int);
void ov108_021E767C(Ov108Work12 *, u32);
void ov108_021E7700(Ov108Work12 *, u32, u32, u32);
void ov108_021E77D4(Ov108Work12 *);
#endif
