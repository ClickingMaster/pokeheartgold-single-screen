#include "overlay15_bag_exit_private.h"

BOOL Bag_Exit(OverlayManager *manager, int *state) {
    u8 *work;

    work = OverlayManager_GetData(manager);
    BagApp_Free3D(work);
    BagApp_FreeSprites(work);
    Heap_Free(*(void **)(work + 0x68C));
    Heap_Free(*(void **)(work + 0x690));
    ov15_021FA0D8(work);
    BagApp_SaveCursorPositions(work);
    BagApp_RemoveWindows(work);
    BagApp_FreeBgLayers(*(void **)work);
    GF_TouchpadStopSampling();
    GF_DestroyVramTransferManager();
    BagApp_FreeItemMenuStrings(work);
    ov15_021FE504(work);
    ov15_021FE8A4(work);
    BagApp_FreeItemNameStrings(work);
    String_Delete(*(String **)(work + 0x5E4));
    DestroyMsgData(*(MsgData **)(work + 0x2FC));
    DestroyMsgData(*(MsgData **)(work + 0x2F8));
    DestroyMsgData(*(MsgData **)(work + 0x2F0));
    MessagePrinter_Delete(*(MessagePrinter **)(work + 0x2EC));
    MessageFormat_Delete(*(MessageFormat **)(work + 0x2F4));
    NARC_Delete(*(NARC **)(work + 0x244));
    OverlayManager_FreeData(manager);
    sub_02004B10();
    Main_SetVBlankIntrCB(NULL, NULL);
    Heap_Destroy(HEAP_ID_6);
    return TRUE;
}
