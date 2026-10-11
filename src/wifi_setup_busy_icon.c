#include "global.h"

#include "wifi_setup_ui.h"
#include "wifi_setup_ui_parts.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#pragma force_active on

static inline void SetOamPriority(GXOamAttr *oam, int priority) {
    oam->attr2 = (oam->attr2 & ~0xc00) | (priority << 10);
}

static inline void SetOamModeAndPalette(GXOamAttr *oam, int mode, int palette) {
    oam->attr01 = (oam->attr01 & ~0xc00) | (mode << 10);
    oam->attr2 = (oam->attr2 & ~0xf000) | (palette << 12);
}

static inline void SetOamPosition(GXOamAttr *oam, int x, int y) {
    oam->attr01 = (oam->attr01 & 0xFE00FF00) | ((u32)(y & 0xff)) | ((u32)(x & 0x1ff) << 16);
}

void WifiSetupBusyIcon_Create(u8 style) {
    gWifiSetupBusyIcon = WifiSetupHeap_AllocZeroed(sizeof(WifiSetupBusyIcon), 4);
    gWifiSetupBusyIcon->style = style;
    gWifiSetupBusyIcon->oam = WifiSetupOamTemplate_AllocOam(0, 0x47);
    SetOamPriority(gWifiSetupBusyIcon->oam, 1);
    SetOamModeAndPalette(gWifiSetupBusyIcon->oam, 0, ov13_02244EB0[style]);
    SetOamPosition(gWifiSetupBusyIcon->oam, 230, 139);
    gWifiSetupBusyIcon->task = WifiSetupTask_Create(1, (WifiSetupTaskFunc)WifiSetupBusyIcon_Task, NULL, 120);
}

WifiSetupBusyIcon *gWifiSetupBusyIcon;

void WifiSetupBusyIcon_Destroy(void) {
    WifiSetupTask_Destroy(1, gWifiSetupBusyIcon->task);
    WifiSetupOam_Release(gWifiSetupBusyIcon->oam);
    WifiSetupHeap_FreeAndClear(&gWifiSetupBusyIcon);
}
