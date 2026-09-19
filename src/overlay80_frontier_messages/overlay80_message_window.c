#include "global.h"

#define U8_AT(p, n) (*(u8 *)((u8 *)(p) + (n)))
#define PTR_AT(p, n) (*(void **)((u8 *)(p) + (n)))

extern void *FrontierSystem_GetFrontierMap(void *);
extern void AddWindowParameterized(void *, void *, int, int, int, int, int, int, int);
extern void FillWindowPixelBuffer(void *, int);
extern void DrawFrameAndWindow2(void *, int, int, int);
extern void ReadMsgDataIntoString(void *, int, void *);
extern void StringExpandPlaceholders(void *, void *, void *);
extern void TextFlags_SetCanABSpeedUpPrint(int);
extern void TextFlags_SetAutoScrollParam(int);
extern void TextFlags_SetCanTouchSpeedUpPrint(int);
extern u8 AddTextPrinterParameterized(void *, int, void *, int, int, int, int);

void ov80_0222E2B8(void *);
void ov80_0222E328(void *, void *, u16);
void ov80_0222E344(void *, u32, u32, BOOL, BOOL);

void ov80_0222E2B8(void *system)
{
    void *map = FrontierSystem_GetFrontierMap(system);

    if (U8_AT(system, 0x5a) == 0) {
        AddWindowParameterized(PTR_AT(map, 0), (u8 *)system + 0x64, 1, 2, 0x13, 0x1b, 4, 0xd, 0x36d);
        FillWindowPixelBuffer((u8 *)system + 0x64, 0xf);
        DrawFrameAndWindow2((u8 *)system + 0x64, 0, 0x3e2, 0xb);
        U8_AT(system, 0x5a) = 1;
    } else {
        FillWindowPixelBuffer((u8 *)system + 0x64, 0xf);
    }
}

void ov80_0222E328(void *system, void *msgData, u16 msgNo)
{
    ReadMsgDataIntoString(msgData, msgNo, PTR_AT(system, 0x4c));
    StringExpandPlaceholders(PTR_AT(system, 0x44), PTR_AT(system, 0x48), PTR_AT(system, 0x4c));
}

void ov80_0222E344(void *system, u32 font, u32 speed, BOOL canAB, BOOL autoScroll)
{
    TextFlags_SetCanABSpeedUpPrint(canAB);
    TextFlags_SetAutoScrollParam(autoScroll);
    if (speed == 0 || speed == 0xff) {
        TextFlags_SetCanTouchSpeedUpPrint(0);
    } else {
        TextFlags_SetCanTouchSpeedUpPrint(1);
    }
    U8_AT(system, 0x50) = AddTextPrinterParameterized((u8 *)system + 0x64, font, PTR_AT(system, 0x48), 0, 0, speed, 0);
}
