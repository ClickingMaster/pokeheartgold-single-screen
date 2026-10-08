#pragma force_active on
#include "global.h"
#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "overlay_manager.h"
#include "message_format.h"
#include "msgdata.h"
#include "font.h"
#include "render_text.h"
#include "sound_02004A44.h"
#include "system.h"
#include "unk_02034B0C.h"
#include "unk_02037C94.h"
#include "nnsys/fnd/expheap.h"
typedef struct Wfc72 {
    u8 unk0000[4];
    BgConfig *bgConfig;
    u8 unk0008[0x1C];
    void *heapMem;
    NNSFndHeapHandle expHeap;
    u8 unk002C[0xBA4];
    MessageFormat *msgFormat;
    MsgData *msgData0;
    MsgData *msgData1;
    MsgData *msgData2;
    u8 unkBE0[0x7C4];
} Wfc72;
typedef struct WfcGlobal72 {
    u8 unk00[4];
    Wfc72 *work;
} WfcGlobal72;
extern WfcGlobal72 ov72_0223B92C;
extern const GraphicsModes ov72_0223B354;
BOOL LoadDwcOverlay(void);
BOOL LoadOVY38(void);
void ov72_02238144(Wfc72 *work, OverlayManager *man);
void ov72_022387D8(Wfc72 *work);
BOOL NintendoWfc_Init(OverlayManager *man, int *state);
// @fn NintendoWfc_Init
BOOL NintendoWfc_Init(OverlayManager *man, int *state) {
    Wfc72 *work;
    switch (*state) {
    case 0: {
        GraphicsModes modes;
        Main_SetVBlankIntrCB(NULL, NULL);
        HBlankInterruptDisable();
        GfGfx_DisableEngineAPlanes();
        GfGfx_DisableEngineBPlanes();
        reg_GX_DISPCNT &= ~REG_GX_DISPCNT_DISPLAY_MASK;
        reg_GXS_DB_DISPCNT &= ~REG_GX_DISPCNT_DISPLAY_MASK;
        Heap_Create(HEAP_ID_3, (enum HeapID)0x43, 0x50000);
        LoadDwcOverlay();
        LoadOVY38();
        sub_02039FD8((enum HeapID)0x43);
        work = OverlayManager_CreateAndGetData(man, sizeof(Wfc72), (enum HeapID)0x43);
        memset(work, 0, sizeof(Wfc72));
        work->bgConfig = BgConfig_Alloc((enum HeapID)0x43);
        ov72_0223B92C.work = work;
        modes = ov72_0223B354;
        SetBothScreensModesAndDisable(&modes);
        work->msgFormat = MessageFormat_New_Custom(11, 32, (enum HeapID)0x43);
        work->msgData0 = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 774, (enum HeapID)0x43);
        work->msgData1 = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 778, (enum HeapID)0x43);
        work->msgData2 = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 800, (enum HeapID)0x43);
        SetKeyRepeatTimers(4, 8);
        ov72_02238144(work, man);
        ov72_022387D8(work);
        Sound_SetSceneAndPlayBGM(52, 0, 0);
        work->heapMem = Heap_Alloc((enum HeapID)0x43, 0x20020);
        work->expHeap = NNS_FndCreateExpHeapEx((void *)(((u32)work->heapMem + 31) & ~31), 0x20000, 0);
        Sound_SetSceneAndPlayBGM(11, 1149, 1);
        TextFlags_SetCanTouchSpeedUpPrint(TRUE);
        reg_GX_POWCNT |= REG_GX_POWCNT_DSEL_MASK;
        *state = 1;
        break;
    }
    case 1:
        sub_02034D8C();
        FontID_Alloc(4, (enum HeapID)0x43);
        *state = 0;
        return TRUE;
    }
    return FALSE;
}
