#include "global.h"

extern int FontID_String_GetWidth(int, const void *, int);
extern void FillWindowPixelBuffer(void *, u32);
extern void AddTextPrinterParameterizedWithColor(void *, int, const void *, int, int, int, u32, void *);
void ov73_021E63BC(void *, const void *, int);

void ov73_021E63BC(void *window, const void *string, int speed) {
    int width = FontID_String_GetWidth(1, string, 0);
    FillWindowPixelBuffer(window, 0);
    AddTextPrinterParameterizedWithColor(window, 1, string, (208 - width) / 2, 1, speed, 0x10400, NULL);
}
