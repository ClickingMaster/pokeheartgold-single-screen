#include "global.h"

#include "overlay_37_helpers_internal.h"

typedef void String;
extern String *String_New(u32, u32);
extern void String_Delete(String *);
extern void ReadMsgDataIntoString(void *, u32, String *);
extern void StringExpandPlaceholders(void *, void *, String *);
extern void FillWindowPixelBuffer(void *, u32);
extern void DrawFrameAndWindow2(void *, u32, u32, u32);
extern u8 AddTextPrinterParameterized(void *, u32, void *, u32, u32, u32, void *);

void ov37_021E762C(Ov37Work *work, u32 msgNo, u32 speed) {
    String *str;

    str = String_New(0x50, 0x27);
    ReadMsgDataIntoString(*(void **)((u8 *)work + 0x10), msgNo, str);
    StringExpandPlaceholders(*(void **)((u8 *)work + 0xC), *(void **)((u8 *)work + 0x2C), str);
    String_Delete(str);
    FillWindowPixelBuffer((u8 *)work + 0x2D8, 0xF);
    DrawFrameAndWindow2((u8 *)work + 0x2D8, 0, 1, 0xA);
    if (speed == 0) {
        speed = 0;
    }
    *(u32 *)((u8 *)work + 0x30) = AddTextPrinterParameterized((u8 *)work + 0x2D8, 1, *(void **)((u8 *)work + 0x2C), 0, 0, speed, NULL);
    if (speed == 0) {
        *(u32 *)((u8 *)work + 0x30) = 0xFF;
    }
}
