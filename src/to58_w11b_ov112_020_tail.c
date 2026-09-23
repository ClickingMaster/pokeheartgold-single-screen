#include "global.h"
extern void *OverlayManager_GetData(void *);
extern void OverlayManager_FreeData(void *);
extern void TextFlags_SetCanABSpeedUpPrint(int);
extern void TextFlags_SetCanTouchSpeedUpPrint(int);
extern void ov112_021E77E4(void *);
extern void Main_SetVBlankIntrCB(void *, void *);
extern void Heap_Destroy(int);
extern void sub_0201A738(int);
extern void RegisterMainOverlay(void *, const void *);
extern u8 SDK_OVERLAY_intro_title_ID[];
extern const u8 gApplication_TitleScreen[];
int ov112_021E7910(void *manager);
int ov112_021E7910(void *manager) {
    void *data = OverlayManager_GetData(manager);
    TextFlags_SetCanABSpeedUpPrint(0);
    TextFlags_SetCanTouchSpeedUpPrint(0);
    ov112_021E77E4(data);
    OverlayManager_FreeData(manager);
    Main_SetVBlankIntrCB(0, 0);
    Heap_Destroy(0x9a);
    sub_0201A738(0x10);
    RegisterMainOverlay(SDK_OVERLAY_intro_title_ID, gApplication_TitleScreen);
    return 1;
}
