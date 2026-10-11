#include "overlay_14_sol_partial_internal.h"

void PCBox_SetPendingUpdateFunc(void *, void *);
void PCBox_SetUpdateFunc(void *, void *);
void PCBox_StartPendingUpdateFunc(void *);
void PCBox_SetGfxBanks(void);
void PCBox_FreeMonInfo(void *);
u32 PCBox_GetWallpaperGfxIndex(void *);
void ov14_021E7AD4(void *, u32, u32, u32);
u32 ov14_021E7B8C(void *);
u32 ov14_021E7B98(void *);
void PCBox_FreePanels(void *);
void PCBox_LoadPanelTilemap(void *, u32, u32);
void ov14_021E81A8(void *, u32);
void ov14_021E954C(void *);
void ov14_021EBDE0(void *);
u32 ov14_021EBF8C(void *);
u32 ov14_021EC344(void *);
u32 ov14_021ED350(void *);
u32 ov14_021ED620(void *);
u32 ov14_021ED6C4(void *);
u32 ov14_021ED960(void *);
u32 ov14_021EDA3C(void *);
u32 ov14_021EDF90(void *);
u32 ov14_021EE328(void *);
u32 ov14_021EE4D8(void *);
u32 PCBox_WaitForPendingUpdateFunc(void *, u32);
u32 ov14_021F1534(void *);
void ov14_021F1F38(void *);
u32 ov14_021F2610(void *);
void ov14_021F261C(void *);
void ov14_021F2624(void *);
void ov14_021F262C(void *);
void ov14_021F2634(void *);
u32 ov14_021F2728(void *);
u32 ov14_021F2760(void *);
u32 ov14_021F276C(void *);
void ov14_021F27CC(void *);
void ov14_021F2810(void *);
void ov14_021F2890(void *);
void PCBox_UpdateSlotIconItemTranslucency(void *, u32);
void PCBox_SetHeldItemIconPosition(void *, s32, s32);
u32 PCBox_HideSelectedIconOutline(void *);
void PCBox_DrawChangeBoxButton(void *);
void ov14_021F63A8(void *);
void ov14_021F63B8(void *);
void *PCBox_LoadBoxMessages(void);
void ov14_021F6678(void *, u32);
void ov14_021F6688(void *, u32);
s32 PCBox_FindTouchedBoxSlot(void);
s32 PCBox_FindTouchedPartySlot(void);
s32 ov14_021F6A34(void);
void ov14_021F6B7C(void *);
void PCBox_FreePaletteData(void *);
void PCBox_CreateYesNoPrompt(void *);
void PCBox_DestroyYesNoPrompt(void *);
void PCBox_FreeReleaseMoveCheck(void *);
void PCBox_HidePartyButton(void *);
void ov14_021E827C(void *);
void PCBox_UpdateMarkingMenuSlide(void *);
u32 ov14_021E9518(void *);
void ov14_021E95B4(void *);
void ov14_021E9604(void *);
u32 PCBox_State_WaitPaletteBuffers(void *);
u32 PCBox_State_WaitPaletteFlash(void *);
u32 ov14_021EBDCC(void *);
u32 ov14_021ED2C8(void *);
u32 ov14_021ED60C(void *);
void ov14_021F1F24(void *);
u32 PCBox_IsSpriteAnimated(void *, u32);
void PCBox_SetSpritePriority(void *, u32, u32);
void PCBox_DeleteAllSprites(void *);
void PCBox_EndReleaseShrink(void *);
void PCBox_SetSlotIconTranslucent(void *, u32, u32);
void PCBox_GetTrayBoxPosition(void *, u32, s32 *, s32 *);
void ov14_021F62CC(void *);
void ov14_021F62E4(void *);
void ov14_021F62FC(void *);
void ov14_021F63F0(void *);
void PCBox_PrintMenuPrompt(void *, u32, u32);
void PCBox_FreeGridInput(void *);
void PCBox_ClearMonInfoAndSelection(void *);
void PCBox_DrawMarkingMenu(void *);
void ov14_021F2700(void *);
void ov14_021F28FC(void *);
void ov14_021F29AC(void *);
void ov14_021EDE20(void *);
void PCBox_DrawAllBoxThumbnails(void *);

#define CALL_196E8(name, a, b, c) \
    void name(void *p) {          \
        BgCellManager_DrawCellAt(p, a, b, c); \
    }

#define FREE_RETURN(name, value)                 \
    u32 name(void *p) {                          \
        Heap_Free(PTR_AT(PTR_AT(p, 0x34), 0xc)); \
        return value;                            \
    }

#define TWO_CALLS(name, second) \
    void name(void *p) {        \
        PCBox_ClearMonInfoAndSelection(p);       \
        second(p);              \
    }

#define CALL_5950(name, a, b)      \
    void name(void *p, u32 v) {    \
        PCBox_DrawButtonWindow(p, a, b, v); \
    }

#define CALL_6698(name, id)      \
    void name(void *p, u32 v) {  \
        PCBox_PrintMessage(p, id, v); \
    }

#define CALL_198FC(name, a, b, c, d) \
    void name(void *p) {             \
        BgCellManager_StartCellMove(p, a, b, c, d); \
    }

#define CALL_67B0(name, a, next)   \
    u32 name(void *p) {            \
        PCBox_PrintReleaseMessage(p, a, 0x25); \
        U32_AT(p, 0x30) = next;    \
        return 6;                  \
    }

#define CALL_5BD8(name, id)               \
    void name(void *p) {                  \
        ov14_021F5BD8(p, 0x29, id, 0, 3); \
    }

#define PREP_196E8(name, a, b, c) \
    void name(void *p) {          \
        BgCellManager_EraseCell(p, a);       \
        BgCellManager_DrawCellAt(p, a, b, c); \
    }

#define RESET_7AC4(name)                               \
    void name(void *p) {                               \
        PCBox_StartCursorSlide(PTR_AT(p, 0x34));                \
        PCBox_SetPendingUpdateFunc(PTR_AT(p, 0x34), PCBox_UpdateCursorSlide); \
    }

void PCBox_SetHeldItemIconPosition(void *p, s32 x, s32 y) {
    ManagedSprite_SetPositionXY(PTR_AT(p, 0x328), x, y);
}
