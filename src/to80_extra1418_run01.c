#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern int ov13_0224F548;
extern void *ov13_0224F4F8;
extern const u8 ov13_022454CC[];
extern const u16 ov13_0224646C[];
extern const u16 ov13_022454DE[];
extern const u16 ov13_022454DC[];
extern void ov13_0222B948(int, void *);
extern void WifiSetupText_DrawMonospace(void *, int, int, int, int, const u16 *, int);
extern void WifiSetupText_DrawInRect(void *, int, int, int, int, int, void *, void *);
extern void WifiSetupText_MarkDirty(void *);
extern int ov13_0222B970(void);
extern void *WifiSetupText_InitBgCanvas(int, int);
extern void *ov13_0222BADC(void *, int);
extern void *WifiSetup_GetTextFlags(void);
extern int swprintf(u16 *, int, const u16 *, ...);
void ov13_0223287C(void);

void ov13_0223287C(void) {
    int out, code;
    void *display, *msg;
    u16 text[8];
    int error;
    ov13_0222B948(0, &out);
    error = ov13_0224F548;
    if (error >= -20099) {
        code = 0;
    } else if (error >= -20100) {
        code = 2;
    } else if (error >= -20101) {
        code = 19;
    } else if (error >= -20107) {
        code = 2;
    } else if (error >= -20108) {
        code = 18;
    } else if (error >= -20109) {
        code = 2;
    } else if (error >= -20110) {
        code = 3;
    } else if (error >= -20999) {
        code = 2;
    } else if (error >= -22999) {
        code = 0;
    } else if (error >= -23999) {
        code = 19;
    } else if (error >= -49999) {
        code = 0;
    } else if (error >= -50002) {
        code = 7;
    } else if (error >= -50003) {
        code = 5;
    } else if (error >= -50098) {
        code = 0;
    } else if (error >= -50099) {
        code = out == 2 ? 5 : 7;
    } else if (error >= -51098) {
        code = 0;
    } else if (error >= -51099) {
        code = out == 2 ? 5 : 8;
    } else if (error >= -51102) {
        code = 9;
    } else if (error >= -51103) {
        code = 5;
    } else if (error >= -51199) {
        code = 0;
    } else if (error >= -51299) {
        code = 20;
    } else if (error >= -51302) {
        code = 21;
    } else if (error >= -51303) {
        code = 4;
    } else if (error >= -51999) {
        code = 0;
    } else if (error >= -52002) {
        code = 10;
    } else if (error >= -52003) {
        code = 22;
    } else if (error >= -52099) {
        code = 0;
    } else if (error >= -52103) {
        code = 11;
    } else if (error >= -52199) {
        code = 0;
    } else if (error >= -52203) {
        code = 11;
    } else if (error >= -52299) {
        code = 0;
    } else if (error >= -52399) {
        code = 2;
    } else if (error >= -52999) {
        code = 0;
    } else if (error >= -53299) {
        code = 2;
    } else {
        code = 0;
    }
    display = WifiSetupText_InitBgCanvas(0, ov13_022454CC[ov13_0222B970()]);
    msg = ov13_0222BADC(ov13_0224F4F8, code);
    swprintf(text, 8, ov13_0224646C, -ov13_0224F548);
    {
        int y = ov13_022454DE[ov13_0222B970() * 2];
        {
            int x = ov13_022454DC[ov13_0222B970() * 2];
            WifiSetupText_DrawMonospace(display, x, y, 2, 10, text, 0);
        }
    }
    WifiSetupText_DrawInRect(display, 13, 60, 230, 94, 2, WifiSetup_GetTextFlags(), msg);
    WifiSetupText_MarkDirty(display);
}
