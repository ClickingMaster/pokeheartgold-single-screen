#include "global.h"

typedef BOOL (*Ov75StateFn)(void *);
extern Ov75StateFn ov75_02249B30[];
void ov75_02247838(void *p);
void ov75_0224741C(void *p);
void sub_02021238(void);
void WirelessStatusIcon_Destroy(void);
void *OverlayManager_GetData(void *manager);
void OverlayManager_FreeData(void *manager);
BOOL IsPaletteFadeFinished(void);
void SysTask_Destroy(void *task);
void DestroyMsgData(void *data);
void MessageFormat_Delete(void *format);
void String_Delete(void *string);
void Heap_Free(void *ptr);
void Main_SetVBlankIntrCB(void *cb, void *arg);
void HBlankInterruptDisable(void);
void GF_DestroyVramTransferManager(void);
void TextFlags_SetCanABSpeedUpPrint(BOOL enabled);
void TextFlags_SetAutoScrollParam(int value);
void TextFlags_SetCanTouchSpeedUpPrint(BOOL enabled);
void Heap_Destroy(int heapId);
void GfGfx_SwapDisplay(void);
extern u8 gSystem[];

BOOL ov75_02247118(void *manager, int *step);
BOOL ov75_02247180(void *manager);

BOOL ov75_02247118(void *manager, int *step) {
    u8 *state = OverlayManager_GetData(manager);
    int oldState;
    BOOL result;

    switch (*step) {
    case 0:
        if (IsPaletteFadeFinished() == TRUE) {
            *step = 1;
        }
        break;
    case 1:
        oldState = *(u32 *)(state + 8);
        result = ov75_02249B30[oldState](state);
        if (oldState != *(u32 *)(state + 8)) {
            *(u32 *)(state + 0x94) = 0;
            *(u32 *)(state + 0x9C) = 0;
            *(u32 *)(state + 0x98) = 0;
        }
        if (result == TRUE) {
            *step = 2;
        }
        break;
    case 2:
        if (IsPaletteFadeFinished() == TRUE) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL ov75_02247180(void *manager) {
    u8 *state = OverlayManager_GetData(manager);

    if (*(void **)(state + 0xEC) != NULL) {
        SysTask_Destroy(*(void **)(state + 0xEC));
        *(void **)(state + 0xEC) = NULL;
        *(void **)(state + 0xF0) = NULL;
    }
    DestroyMsgData(*(void **)(state + 0x34));
    DestroyMsgData(*(void **)(state + 0x28));
    DestroyMsgData(*(void **)(state + 0x30));
    DestroyMsgData(*(void **)(state + 0x2C));
    DestroyMsgData(*(void **)(state + 0x24));
    MessageFormat_Delete(*(void **)(state + 0x20));
    String_Delete(*(void **)(state + 0x3C));
    String_Delete(*(void **)(state + 0x40));
    String_Delete(*(void **)(state + 0x38));
    ov75_02247838(state);
    Heap_Free(*(void **)(state + 4));
    ov75_0224741C(*(void **)(state + 4));
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GF_DestroyVramTransferManager();
    sub_02021238();
    TextFlags_SetCanABSpeedUpPrint(FALSE);
    TextFlags_SetAutoScrollParam(0);
    TextFlags_SetCanTouchSpeedUpPrint(FALSE);
    WirelessStatusIcon_Destroy();
    OverlayManager_FreeData(manager);
    Heap_Destroy(0x74);
    (gSystem + 0x60)[9] = FALSE;
    GfGfx_SwapDisplay();
    return TRUE;
}
