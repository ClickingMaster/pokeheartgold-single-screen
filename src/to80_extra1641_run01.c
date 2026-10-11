#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    void *first, *pad, *obj, *objs[2], *pad2, *items[7], *extra, *pad4, *last;
    u8 mode;
} Inner;

typedef struct {
    u8 pad, flag;
    u16 pad2, value, pad3;
    Inner *inner;
} Root;

extern Root ov13_0224F54C;
extern u32 ov13_0224F554[];
extern u8 ov13_022464CC[];
extern int WifiSetupFade_IsActive(int);
extern int DWCi_BACKUPlConvMaskCidr(void *);
extern u8 *ov13_02239FF0(void);
extern void WifiSetupTask_Destroy(int, void *), WifiSetupOam_Release(void *), ov13_02239514(void), WifiSetupText_FreeBgCanvas(int), WifiSetupCursor_Hide(void), WifiSetupMessage_Hide(int), ov13_02239288(void), WifiSetupFile_Free(void *), WifiSetup_LoadGraphics(void *, void (*)(const void *, u32, u32)), WifiSetupDisplay_HidePlanes(int, int), ov13_0222B934(int, int), ov13_0222B8F8(int, int), WifiSetupApp_SetStateFunc(void (*)(void)), ov13_0223A414(int), WifiSetupHeap_FreeAndClear(void *);
extern void WifiSetupTextEntry_Init(void);
extern void ov13_02231B18(void);
extern void ov13_02237DD0(void);
extern void ov13_022352D0(void);
extern void ov13_022368D8(void);
extern void ov13_02237618(void);
void ov13_02233AD8(void);

void ov13_02233AD8(void) {
    if (WifiSetupFade_IsActive(1)) {
        return;
    }
    if (WifiSetupFade_IsActive(0)) {
        return;
    }
    WifiSetupTask_Destroy(0, ov13_0224F54C.inner->first);
    WifiSetupTask_Destroy(1, ov13_0224F54C.inner->last);
    int i;
    for (i = 0; i < 7; i++) {
        WifiSetupOam_Release(ov13_0224F54C.inner->items[i]);
    }
    if (ov13_0224F54C.inner->extra) {
        WifiSetupOam_Release(ov13_0224F54C.inner->extra);
    }
    ov13_02239514();
    WifiSetupText_FreeBgCanvas(0);
    WifiSetupCursor_Hide();
    if (ov13_0224F54C.inner->mode != 12) {
        WifiSetupMessage_Hide(ov13_0224F54C.inner->mode);
    }
    ov13_02239288();
    WifiSetupFile_Free(ov13_0224F54C.inner->obj);
    for (i = 0; i < 2; i++) {
        WifiSetupFile_Free(ov13_0224F54C.inner->objs[i]);
    }
    WifiSetup_LoadGraphics(ov13_022464CC, GX_LoadOBJPltt);
    WifiSetupDisplay_HidePlanes(1, 1);
    WifiSetupDisplay_HidePlanes(0, 29);
    *(vu32 *)0x04000010 = 0;
    *(vu32 *)0x04000018 = 0;
    *(vu16 *)0x0400000c = (*(vu16 *)0x0400000c & 0x43) | 0xe10;
    int mode = ov13_0224F54C.inner->mode;
    switch (mode) {
    case 0:
    case 1:
        ov13_0222B934(mode, 0);
        ov13_0222B8F8(2, 0);
        WifiSetupApp_SetStateFunc(WifiSetupTextEntry_Init);
        break;
    case 4:
    case 5:
    case 6:
    case 9:
    case 10: {
        int value = mode - 4;
        if ((u32)mode >= 9) {
            value -= 2;
        }
        ov13_0222B8F8(2, 0);
        ov13_0222B934(value, 0);
        WifiSetupApp_SetStateFunc(ov13_02231B18);
        break;
    }
    case 11: {
        u8 *data = ov13_02239FF0();
        data[0xd0] = DWCi_BACKUPlConvMaskCidr(data + 0xf0);
        if (data[0xf5]) {
            MI_CpuFill8(data + 0xc0, 0, 4);
            MI_CpuFill8(data + 0xc4, 0, 4);
            MI_CpuFill8(data + 0xf0, 0, 4);
            data[0xd0] = 0;
        }
        if (data[0xf6]) {
            MI_CpuFill8(data + 0xc8, 0, 8);
        }
        ov13_0222B8F8(2, 0);
        ov13_0222B934(0, 0);
        WifiSetupApp_SetStateFunc(ov13_02237DD0);
        break;
    }
    case 12:
        ov13_0222B8F8(0, 0);
        WifiSetupApp_SetStateFunc(ov13_022352D0);
        break;
    case 13:
        if (ov13_0224F54C.flag == 0) {
            ov13_0222B8F8(2, 1);
            WifiSetupApp_SetStateFunc(ov13_022368D8);
        } else {
            ov13_0223A414(ov13_02239FF0()[0xf4]);
            ov13_0222B8F8(0, 1);
            WifiSetupApp_SetStateFunc(ov13_02237618);
        }
        break;
    }
    WifiSetupHeap_FreeAndClear(ov13_0224F554);
}
