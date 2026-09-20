#ifndef W19_PRIVATE_H
#define W19_PRIVATE_H
#include "global.h"

typedef struct W19Work W19Work;
typedef struct W19Window {
    u8 data[0x10];
} W19Window;
typedef struct W19Record {
    u8 pad_00[0x3C];
    u16 item[2];
    u8 pad_40[0x50];
} W19Record;
extern void WindowArray_Delete(void *, int);
extern void RemoveWindow(void *);
extern void ov08_022239B4(W19Work *);
extern void ov08_02223AA0(W19Work *);
extern void *NewString_ReadMsgData(void *, u32);
extern u32 FontID_String_GetWidth(u32, void *, u32);
extern u32 GetWindowWidth(void *);
extern u8 AddTextPrinterParameterizedWithColor(void *, u32, void *, u32, u32, u32, u32, void *);
extern void String_Delete(void *);
extern void ScheduleWindowCopyToVram(void *);
extern void FillWindowPixelBuffer(void *, u8);
extern void BufferItemName(void *, u32, u16);
extern void StringExpandPlaceholders(void *, void *, void *);
extern void BufferIntegerAsString(void *, u32, s32, u32, u32, u32);
extern void FillBgTilemapRect(void *, u8, u16, u8, u8, u8, u8, u8);
extern const u32 ov08_02225BE0[];
extern const u32 ov08_02225BE4[];
void ov08_02223464(W19Work *);
void ov08_02223480(W19Work *, int);
void ov08_022234A4(W19Work *, u32, u32, u32, u32, u32);
void ov08_022234FC(W19Work *);
void ov08_022235D4(W19Work *, u32, u32, u32, u32, u32);
void ov08_02223678(W19Work *, u32, u32, u32, u32, u32, u32);
void ov08_0222370C(W19Work *, u32);
void ov08_0222377C(W19Work *);
void ov08_022237C4(W19Work *);
void ov08_022238F4(W19Work *);
#endif
