#include "application/pokedex/pokedex_internal.h"
#include "application/pokedex/pokedex_internal_constants.h"
#include "graphic/zukan_gra.naix"

#include "font.h"
#include "gf_gfx_loader.h"
#include "list_menu_items.h"
#include "pokedex_util.h"
#include "screen_fade.h"
#include "sound_system.h"
#include "sound_chatot.h"
#include "sound_playback.h"
#include "touchpad.h"
#include "unk_0208805C.h"

#define PLTTBUF_EXCEPT_MAIN_BG_F (PLTTBUF_SUB_BG_F | PLTTBUF_MAIN_OBJ_F | PLTTBUF_SUB_OBJ_F)
#define PLTTFADE_OPAQUE_ALL_F    (0xFFFF)

static int PokedexApp_MainSeq_00(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_01(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_02(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_03(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_04(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_05(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_06(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_07(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_08(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_09(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_10(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_11(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_12(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_13(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_14(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_15(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_16(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_17(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_18(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_21(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_22(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_23(PokedexAppData *pokedexApp);
static void PokedexApp_SetCellTilemapFromBg(PokedexAppData *pokedexApp, int bgId, int a2);
static int PokedexApp_MainSeq_24(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_25(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_19(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_20(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_26(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_27(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_28(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_29(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_30(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_31(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_32(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_33(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_34(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_35(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_36(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_37(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_38(PokedexAppData *pokedexApp);
static void PokedexApp_StepSliderValue(u32 *pOut, int hmin, int hmax, u32 *pStep, int direction);
static void PokedexApp_UpdateSizeSlider(PokedexAppData *pokedexApp, int a1, int a2, int a3, int a4);
static int PokedexApp_MainSeq_39(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_40(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_41(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_42(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_43(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_44(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_45(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_46(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_47(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_48(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_49(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_50(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_51(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_52(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_53(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_54(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_55(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_56(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_57(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_58(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_59(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_60(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_61(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_62(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_63(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_64(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_65(PokedexAppData *pokedexApp);
static int PokedexApp_SwitchEntryTab(PokedexAppData *pokedexApp, u8 a1, u8 a2);
static int PokedexApp_MainSeq_66(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_67(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_68(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_69(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_70(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_71(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_72(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_73(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_74(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_75(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_76(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_77(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_78(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_79(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_80(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_81(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_82(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_83(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_84(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_85(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_86(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_87(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_88(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_89(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_90(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_91(PokedexAppData *pokedexApp);
static int PokedexApp_MainSeq_92(PokedexAppData *pokedexApp);
static int PokedexApp_FadeOutTo(PokedexAppData *pokedexApp, int a1);
static int PokedexApp_FlashButton(PokedexAppData *pokedexApp, u8 a1, u8 a2, u8 a3, u8 a4, u8 a5, u8 a6, u8 a7, int a8);
static int PokedexApp_FlashBottomButton(PokedexAppData *pokedexApp, u8 a1, u8 a2, u8 a3);
static int PokedexApp_FlashSearchButton(PokedexAppData *pokedexApp, u8 a1, u8 a2, u8 a3);
static int PokedexApp_FlashSearchMenuOption(PokedexAppData *pokedexApp, u8 a1, u8 a2, u8 a3, u8 a4, int a5);
static int PokedexApp_FlashEntryTab(PokedexAppData *pokedexApp, u8 a1, int a2);
static int ov18_021EDC50(PokedexAppData *pokedexApp, u8 a1, u8 a2, u8 a3, u8 a4, u8 a5, int a6);
static void PokedexApp_SetDexMode(PokedexAppData *pokedexApp, int a1);
static void PokedexApp_RefreshList(PokedexAppData *pokedexApp);
static void PokedexApp_SwitchDexMode(PokedexAppData *pokedexApp, int a1);
static void PokedexApp_RestoreListCursor(PokedexAppData *pokedexApp);
static BOOL PokedexApp_PlayCursorCry(PokedexAppData *pokedexApp);
static void PokedexApp_PrepareSearchPageChange(PokedexAppData *pokedexApp, int a1, int a2, int a3);
static void PokedexApp_UpdateEntryButtonsEnabled(PokedexAppData *pokedexApp);
static void PokedexApp_UpdateSearchResultButtonsEnabled(PokedexAppData *pokedexApp);
static BOOL PokedexApp_CanScrollFormList(PokedexAppData *pokedexApp, int a1, int a2);
static void PokedexApp_StepIndexSkipping(s8 *a0, s8 a1, int a2, u8 a3);
static BOOL PokedexApp_CycleLeftCompareForm(PokedexAppData *pokedexApp, int a1);
static BOOL PokedexApp_CycleRightCompareForm(PokedexAppData *pokedexApp, int a1);
static void PokedexApp_RunWeightCheck(PokedexAppData *pokedexApp);
static void PokedexApp_EndWeightCheck(PokedexAppData *pokedexApp);
static BOOL PokedexApp_CanScrollAreaList(PokedexAppData *pokedexApp, int a1, int a2);
static BOOL PokedexApp_SetAreaTimeOfDay(PokedexAppData *pokedexApp, int a1);
static BOOL PokedexApp_SetAreaRegion(PokedexAppData *pokedexApp, int a1);
static void PokedexApp_DrawAreaRegionTabs(PokedexAppData *pokedexApp);
static void PokedexApp_ScrollAreaMapToJohto(PokedexAppData *pokedexApp);
static void PokedexApp_ResetAreaMapScroll(PokedexAppData *pokedexApp);
static BOOL PokedexApp_ScrollAreaMap(PokedexAppData *pokedexApp);
static void PokedexApp_UpdateAreaMarkerBlink(PokedexAppData *pokedexApp);
static void PokedexApp_ResetAreaMarkerBlink(PokedexAppData *pokedexApp);

static int (*const sMainSeqFuncs[POKEDEXAPP_MAINSEQ_MAX])(PokedexAppData *pokedexApp) = {
    [POKEDEXAPP_MAINSEQ_00] = PokedexApp_MainSeq_00,
    [POKEDEXAPP_MAINSEQ_01] = PokedexApp_MainSeq_01,
    [POKEDEXAPP_MAINSEQ_02] = PokedexApp_MainSeq_02,
    [POKEDEXAPP_MAINSEQ_03] = PokedexApp_MainSeq_03,
    [POKEDEXAPP_MAINSEQ_04] = PokedexApp_MainSeq_04,
    [POKEDEXAPP_MAINSEQ_05] = PokedexApp_MainSeq_05,
    [POKEDEXAPP_MAINSEQ_06] = PokedexApp_MainSeq_06,
    [POKEDEXAPP_MAINSEQ_07] = PokedexApp_MainSeq_07,
    [POKEDEXAPP_MAINSEQ_08] = PokedexApp_MainSeq_08,
    [POKEDEXAPP_MAINSEQ_09] = PokedexApp_MainSeq_09,
    [POKEDEXAPP_MAINSEQ_10] = PokedexApp_MainSeq_10,
    [POKEDEXAPP_MAINSEQ_11] = PokedexApp_MainSeq_11,
    [POKEDEXAPP_MAINSEQ_12] = PokedexApp_MainSeq_12,
    [POKEDEXAPP_MAINSEQ_13] = PokedexApp_MainSeq_13,
    [POKEDEXAPP_MAINSEQ_14] = PokedexApp_MainSeq_14,
    [POKEDEXAPP_MAINSEQ_15] = PokedexApp_MainSeq_15,
    [POKEDEXAPP_MAINSEQ_16] = PokedexApp_MainSeq_16,
    [POKEDEXAPP_MAINSEQ_17] = PokedexApp_MainSeq_17,
    [POKEDEXAPP_MAINSEQ_18] = PokedexApp_MainSeq_18,
    [POKEDEXAPP_MAINSEQ_19] = PokedexApp_MainSeq_19,
    [POKEDEXAPP_MAINSEQ_20] = PokedexApp_MainSeq_20,
    [POKEDEXAPP_MAINSEQ_21] = PokedexApp_MainSeq_21,
    [POKEDEXAPP_MAINSEQ_22] = PokedexApp_MainSeq_22,
    [POKEDEXAPP_MAINSEQ_23] = PokedexApp_MainSeq_23,
    [POKEDEXAPP_MAINSEQ_24] = PokedexApp_MainSeq_24,
    [POKEDEXAPP_MAINSEQ_25] = PokedexApp_MainSeq_25,
    [POKEDEXAPP_MAINSEQ_26] = PokedexApp_MainSeq_26,
    [POKEDEXAPP_MAINSEQ_27] = PokedexApp_MainSeq_27,
    [POKEDEXAPP_MAINSEQ_28] = PokedexApp_MainSeq_28,
    [POKEDEXAPP_MAINSEQ_29] = PokedexApp_MainSeq_29,
    [POKEDEXAPP_MAINSEQ_30] = PokedexApp_MainSeq_30,
    [POKEDEXAPP_MAINSEQ_31] = PokedexApp_MainSeq_31,
    [POKEDEXAPP_MAINSEQ_32] = PokedexApp_MainSeq_32,
    [POKEDEXAPP_MAINSEQ_33] = PokedexApp_MainSeq_33,
    [POKEDEXAPP_MAINSEQ_34] = PokedexApp_MainSeq_34,
    [POKEDEXAPP_MAINSEQ_35] = PokedexApp_MainSeq_35,
    [POKEDEXAPP_MAINSEQ_36] = PokedexApp_MainSeq_36,
    [POKEDEXAPP_MAINSEQ_37] = PokedexApp_MainSeq_37,
    [POKEDEXAPP_MAINSEQ_38] = PokedexApp_MainSeq_38,
    [POKEDEXAPP_MAINSEQ_39] = PokedexApp_MainSeq_39,
    [POKEDEXAPP_MAINSEQ_40] = PokedexApp_MainSeq_40,
    [POKEDEXAPP_MAINSEQ_41] = PokedexApp_MainSeq_41,
    [POKEDEXAPP_MAINSEQ_42] = PokedexApp_MainSeq_42,
    [POKEDEXAPP_MAINSEQ_43] = PokedexApp_MainSeq_43,
    [POKEDEXAPP_MAINSEQ_44] = PokedexApp_MainSeq_44,
    [POKEDEXAPP_MAINSEQ_45] = PokedexApp_MainSeq_45,
    [POKEDEXAPP_MAINSEQ_46] = PokedexApp_MainSeq_46,
    [POKEDEXAPP_MAINSEQ_47] = PokedexApp_MainSeq_47,
    [POKEDEXAPP_MAINSEQ_48] = PokedexApp_MainSeq_48,
    [POKEDEXAPP_MAINSEQ_49] = PokedexApp_MainSeq_49,
    [POKEDEXAPP_MAINSEQ_50] = PokedexApp_MainSeq_50,
    [POKEDEXAPP_MAINSEQ_51] = PokedexApp_MainSeq_51,
    [POKEDEXAPP_MAINSEQ_52] = PokedexApp_MainSeq_52,
    [POKEDEXAPP_MAINSEQ_53] = PokedexApp_MainSeq_53,
    [POKEDEXAPP_MAINSEQ_54] = PokedexApp_MainSeq_54,
    [POKEDEXAPP_MAINSEQ_55] = PokedexApp_MainSeq_55,
    [POKEDEXAPP_MAINSEQ_56] = PokedexApp_MainSeq_56,
    [POKEDEXAPP_MAINSEQ_57] = PokedexApp_MainSeq_57,
    [POKEDEXAPP_MAINSEQ_58] = PokedexApp_MainSeq_58,
    [POKEDEXAPP_MAINSEQ_59] = PokedexApp_MainSeq_59,
    [POKEDEXAPP_MAINSEQ_60] = PokedexApp_MainSeq_60,
    [POKEDEXAPP_MAINSEQ_61] = PokedexApp_MainSeq_61,
    [POKEDEXAPP_MAINSEQ_62] = PokedexApp_MainSeq_62,
    [POKEDEXAPP_MAINSEQ_63] = PokedexApp_MainSeq_63,
    [POKEDEXAPP_MAINSEQ_64] = PokedexApp_MainSeq_64,
    [POKEDEXAPP_MAINSEQ_65] = PokedexApp_MainSeq_65,
    [POKEDEXAPP_MAINSEQ_66] = PokedexApp_MainSeq_66,
    [POKEDEXAPP_MAINSEQ_67] = PokedexApp_MainSeq_67,
    [POKEDEXAPP_MAINSEQ_68] = PokedexApp_MainSeq_68,
    [POKEDEXAPP_MAINSEQ_69] = PokedexApp_MainSeq_69,
    [POKEDEXAPP_MAINSEQ_70] = PokedexApp_MainSeq_70,
    [POKEDEXAPP_MAINSEQ_71] = PokedexApp_MainSeq_71,
    [POKEDEXAPP_MAINSEQ_72] = PokedexApp_MainSeq_72,
    [POKEDEXAPP_MAINSEQ_73] = PokedexApp_MainSeq_73,
    [POKEDEXAPP_MAINSEQ_74] = PokedexApp_MainSeq_74,
    [POKEDEXAPP_MAINSEQ_75] = PokedexApp_MainSeq_75,
    [POKEDEXAPP_MAINSEQ_76] = PokedexApp_MainSeq_76,
    [POKEDEXAPP_MAINSEQ_77] = PokedexApp_MainSeq_77,
    [POKEDEXAPP_MAINSEQ_78] = PokedexApp_MainSeq_78,
    [POKEDEXAPP_MAINSEQ_79] = PokedexApp_MainSeq_79,
    [POKEDEXAPP_MAINSEQ_80] = PokedexApp_MainSeq_80,
    [POKEDEXAPP_MAINSEQ_81] = PokedexApp_MainSeq_81,
    [POKEDEXAPP_MAINSEQ_82] = PokedexApp_MainSeq_82,
    [POKEDEXAPP_MAINSEQ_83] = PokedexApp_MainSeq_83,
    [POKEDEXAPP_MAINSEQ_84] = PokedexApp_MainSeq_84,
    [POKEDEXAPP_MAINSEQ_85] = PokedexApp_MainSeq_85,
    [POKEDEXAPP_MAINSEQ_86] = PokedexApp_MainSeq_86,
    [POKEDEXAPP_MAINSEQ_87] = PokedexApp_MainSeq_87,
    [POKEDEXAPP_MAINSEQ_88] = PokedexApp_MainSeq_88,
    [POKEDEXAPP_MAINSEQ_89] = PokedexApp_MainSeq_89,
    [POKEDEXAPP_MAINSEQ_90] = PokedexApp_MainSeq_90,
    [POKEDEXAPP_MAINSEQ_91] = PokedexApp_MainSeq_91,
    [POKEDEXAPP_MAINSEQ_92] = PokedexApp_MainSeq_92,
};

BOOL PokedexApp_RunMainSeq(PokedexAppData *pokedexApp, int *pState) {
    *pState = sMainSeqFuncs[*pState](pokedexApp);
    if (*pState == POKEDEXAPP_MAINSEQ_MAX) {
        return FALSE;
    }

    BgCellManager_Update(pokedexApp->bgCellManager);
    PokedexApp_TickSprites(pokedexApp);
    SpriteSystem_DrawSprites(pokedexApp->spriteManager);
    return TRUE;
}

static int PokedexApp_MainSeq_00(PokedexAppData *pokedexApp) {
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GfGfx_EngineASetPlanes(GX_PLANEMASK_NONE);
    GfGfx_EngineBSetPlanes(GX_PLANEMASK_NONE);
    G2_BlendNone();
    G2S_BlendNone();
    GF_TouchpadResetSampling();
    GF_TouchpadStartSampling(4);
    GX_SetDispSelect(GX_DISP_SELECT_SUB_MAIN);
    SetKeyRepeatTimers(4, 8);
    PokedexApp_LoadSearchTables(pokedexApp);
    PokedexApp_LoadSizeData(pokedexApp);
    BeginAppScreenFade(0, HEAP_ID_POKEDEX_APP);
    FontID_Alloc(4, HEAP_ID_POKEDEX_APP);
    pokedexApp->gfxNarc = NARC_New(NARC_graphic_zukan_gra, HEAP_ID_POKEDEX_APP);
    PokedexApp_ResetSearchCriteria(pokedexApp);
    PokedexApp_RunSearch(pokedexApp, pokedexApp->dexMode, 0, DEX_SEARCH_LETTERS_ALL, DEX_SEARCH_TYPE_ALL, DEX_SEARCH_TYPE_ALL, 0, 152, 0, 152, 1 << DEX_SEARCH_AREA_ALL, DEX_SEARCH_BODYTYPE_ALL);
    PokedexApp_BuildListEntries(pokedexApp, 1);
    PokedexApp_RestoreListCursor(pokedexApp);
    PokedexApp_SetGraphicsBanks();
    PokedexApp_InitBgs(pokedexApp);
    PokedexApp_InitBgCells(pokedexApp);
    PokedexApp_InitPaletteData(pokedexApp);
    PokedexApp_InitMsgData(pokedexApp);
    PokedexApp_InitSprites(pokedexApp);
    PokedexApp_InitBlend();
    Sound_SetSceneAndPlayBGM(54, 0, 0);
    Main_SetVBlankIntrCB(PokedexApp_VBlankCallback, pokedexApp);
    return pokedexApp->nextSeq;
}

static int PokedexApp_MainSeq_01(PokedexAppData *pokedexApp) {
    Main_SetVBlankIntrCB(NULL, NULL);
    PokedexApp_RemoveAllWindows(pokedexApp);
    PokedexApp_FreeSprites(pokedexApp);
    PokedexApp_FreeMsgData(pokedexApp);
    PokedexApp_FreePaletteData(pokedexApp);
    PokedexApp_FreeBgCells(pokedexApp);
    PokedexApp_FreeBgs(pokedexApp);
    NARC_Delete(pokedexApp->gfxNarc);
    FontID_Release(4);
    PokedexApp_FreeSizeData(pokedexApp);
    PokedexApp_FreeSearchTables(pokedexApp);
    GF_TouchpadStopSampling();
    G2_BlendNone();
    G2S_BlendNone();
    GfGfx_EngineASetPlanes(GX_PLANEMASK_NONE);
    GfGfx_EngineBSetPlanes(GX_PLANEMASK_NONE);
    return pokedexApp->nextSeq;
}

static int PokedexApp_MainSeq_02(PokedexAppData *pokedexApp) {
    if (IsPaletteFadeFinished() == TRUE) {
        return pokedexApp->fadeNextSeq;
    }

    return POKEDEXAPP_MAINSEQ_02;
}

static int PokedexApp_MainSeq_03(PokedexAppData *pokedexApp) {
    if (PaletteData_GetSelectedBuffersBitmask(pokedexApp->paletteData) == 0) {
        return pokedexApp->nextSeq;
    }

    return POKEDEXAPP_MAINSEQ_03;
}

static int PokedexApp_MainSeq_04(PokedexAppData *pokedexApp) {
    PokedexAppData_UnkSub0868_State04 *r4 = &pokedexApp->seqState.state_04;
    switch (r4->state) {
    case 0:
        BgTilemapRectChangePalette(pokedexApp->bgConfig, r4->bgId, r4->x, r4->y, r4->width, r4->height, r4->flashPalette);
        ScheduleBgTilemapBufferTransfer(pokedexApp->bgConfig, r4->bgId);
        ++r4->state;
        break;
    case 1:
        ++r4->timer;
        if (r4->timer == 4) {
            BgTilemapRectChangePalette(pokedexApp->bgConfig, r4->bgId, r4->x, r4->y, r4->width, r4->height, r4->normalPalette);
            ScheduleBgTilemapBufferTransfer(pokedexApp->bgConfig, r4->bgId);
            r4->timer = 0;
            ++r4->state;
        }
        break;
    case 2:
        MI_CpuClear8(r4, sizeof(pokedexApp->seqState));
        return pokedexApp->nextSeq;
    }

    return POKEDEXAPP_MAINSEQ_04;
}

static int PokedexApp_MainSeq_05(PokedexAppData *pokedexApp) {
    pokedexApp->entryTab = 0;
    PokedexApp_SetupScreen(pokedexApp, 0);
    PaletteData_BeginPaletteFade(pokedexApp->paletteData, 0xF, PLTTFADE_OPAQUE_ALL_F, -127, 0, 0, RGB_BLACK);
    pokedexApp->fadeNextSeq = 6;
    return POKEDEXAPP_MAINSEQ_02;
}

static int PokedexApp_MainSeq_06(PokedexAppData *pokedexApp) {
    switch (PokedexApp_HandleListMenuInput(pokedexApp)) {
    case 0:
        PlaySE(SEQ_SE_GS_BUTTON01);
        return PokedexApp_FlashBottomButton(pokedexApp, 8, 16, POKEDEXAPP_MAINSEQ_09);
    case 1:
        PlaySE(SEQ_SE_GS_ZKN04);
        return PokedexApp_FlashBottomButton(pokedexApp, 0, 8, POKEDEXAPP_MAINSEQ_10);
    case 2:
        if (pokedexApp->keyInput == 0) {
            MenuInputStateMgr_SetState(pokedexApp->args->menuInputStatePtr, MENU_INPUT_STATE_TOUCH);
        } else {
            MenuInputStateMgr_SetState(pokedexApp->args->menuInputStatePtr, MENU_INPUT_STATE_BUTTONS);
        }
        PlaySE(SEQ_SE_GS_GEARCANCEL);
        return PokedexApp_FlashBottomButton(pokedexApp, 24, 8, POKEDEXAPP_MAINSEQ_92);
    case 3:
        PlaySE(SEQ_SE_GS_ZKN_BUTTON8);
        PokedexApp_SwitchDexMode(pokedexApp, 1);
        break;
    case 4:
        PlaySE(SEQ_SE_GS_ZKN_BUTTON8);
        PokedexApp_SwitchDexMode(pokedexApp, 0);
        break;
    case 5: {
        PokedexAppData_UnkSub0868_State06 *r5 = &pokedexApp->seqState.state_06;
        System_GetTouchNewCoords(&r5->touchX, &r5->touchY);
        r5->switchX = PokedexApp_GetDexModeSwitchX(pokedexApp);
        PlaySE(SEQ_SE_GS_BUTTON01);
        return POKEDEXAPP_MAINSEQ_08;
    }
    case 6: {
        u32 sp4;
        u32 sp0;

        System_GetTouchNewCoords(&sp4, &sp0);
        if (PokedexApp_IsTouchOnSprite0(pokedexApp, sp4, sp0) == TRUE) {
            return POKEDEXAPP_MAINSEQ_07;
        }
    } break;
    }

    return POKEDEXAPP_MAINSEQ_06;
}

static int PokedexApp_MainSeq_07(PokedexAppData *pokedexApp) {
    u32 sp4;
    u32 sp0;

    if (!System_GetTouchHeldCoords(&sp4, &sp0)) {
        PokedexApp_RefreshList(pokedexApp);
        return POKEDEXAPP_MAINSEQ_06;
    }

    if (sp4 >= 92 && sp4 < 164 && sp0 >= 112 && sp0 < 144) {
        if (sp4 < 96) {
            sp4 = 96;
        } else if (sp4 > 159) {
            sp4 = 159;
        }
        if (sp0 < 120) {
            sp0 = 120;
        } else if (sp0 > 135) {
            sp0 = 135;
        }
        if (!PokedexApp_IsTouchOnSprite0(pokedexApp, sp4, sp0)) {
            if (pokedexApp->dexMode == 1) {
                PlaySE(SEQ_SE_GS_ZKN_BUTTON8);
                PokedexApp_SetDexMode(pokedexApp, 0);
            } else {
                PlaySE(SEQ_SE_GS_ZKN_BUTTON8);
                PokedexApp_SetDexMode(pokedexApp, 1);
            }
        }
    } else {
        PokedexApp_RefreshList(pokedexApp);
        return POKEDEXAPP_MAINSEQ_06;
    }

    return POKEDEXAPP_MAINSEQ_07;
}

static int PokedexApp_MainSeq_08(PokedexAppData *pokedexApp) {
    PokedexAppData_UnkSub0868_State06 *r4 = &pokedexApp->seqState.state_06;

    switch (r4->state) {
    case 0: {
        u32 sp4;
        u32 sp0;

        if (!System_GetTouchHeldCoords(&sp4, &sp0)) {
            if (Bg_GetXpos(pokedexApp->bgConfig, GF_BG_LYR_MAIN_0) >= 64) {
                MI_CpuClear8(&pokedexApp->seqState, sizeof(pokedexApp->seqState));
                return POKEDEXAPP_MAINSEQ_09;
            }
            ++r4->state;
        } else if ((s32)(r4->touchX - sp4) < 0) {
            ScheduleSetBgPosText(pokedexApp->bgConfig, GF_BG_LYR_MAIN_0, BG_POS_OP_SET_X, 0);
            ScheduleSetBgPosText(pokedexApp->bgConfig, GF_BG_LYR_SUB_0, BG_POS_OP_SET_X, 0);
            PokedexApp_SetSpriteX(pokedexApp, 0, r4->switchX);
        } else {
            ScheduleSetBgPosText(pokedexApp->bgConfig, GF_BG_LYR_MAIN_0, BG_POS_OP_SET_X, r4->touchX - sp4);
            ScheduleSetBgPosText(pokedexApp->bgConfig, GF_BG_LYR_SUB_0, BG_POS_OP_SET_X, r4->touchX - sp4);
            PokedexApp_SetSpriteX(pokedexApp, 0, r4->switchX - (r4->touchX - sp4));
        }
    } break;
    case 1:
        ScheduleSetBgPosText(pokedexApp->bgConfig, GF_BG_LYR_MAIN_0, BG_POS_OP_SUB_X, 16);
        ScheduleSetBgPosText(pokedexApp->bgConfig, GF_BG_LYR_SUB_0, BG_POS_OP_SUB_X, 16);
        PokedexApp_MoveSpriteX(pokedexApp, 0, 16);
        if (Bg_GetXpos(pokedexApp->bgConfig, GF_BG_LYR_MAIN_0) <= 0) {
            ScheduleSetBgPosText(pokedexApp->bgConfig, GF_BG_LYR_MAIN_0, BG_POS_OP_SET_X, 0);
            ScheduleSetBgPosText(pokedexApp->bgConfig, GF_BG_LYR_SUB_0, BG_POS_OP_SET_X, 0);
            PokedexApp_SetSpriteX(pokedexApp, 0, PokedexApp_GetDexModeSwitchX(pokedexApp));
            MI_CpuClear8(&pokedexApp->seqState, sizeof(pokedexApp->seqState));
            return POKEDEXAPP_MAINSEQ_06;
        }
        break;
    }

    return POKEDEXAPP_MAINSEQ_08;
}

static int PokedexApp_MainSeq_09(PokedexAppData *pokedexApp) {
    PokedexAppData_UnkSub0868_State09 *r4 = &pokedexApp->seqState.state_09;

    switch (r4->state) {
    case 0:
        PokedexApp_SlideOutListButtons(pokedexApp);
        ++r4->state;
        // fallthrough
    case 1: {
        ScheduleSetBgPosText(pokedexApp->bgConfig, GF_BG_LYR_MAIN_0, BG_POS_OP_ADD_X, 16);
        ScheduleSetBgPosText(pokedexApp->bgConfig, GF_BG_LYR_SUB_0, BG_POS_OP_ADD_X, 16);
        PokedexApp_MoveSpriteX(pokedexApp, 0, -16);
        int r6 = Bg_GetXpos(pokedexApp->bgConfig, GF_BG_LYR_MAIN_0);
        if (r4->buttonsShown == 0 && r6 >= 192 && !BgCellManager_IsCellMoving(pokedexApp->bgCellManager, 0)) {
            r4->buttonsShown = 1;
            PokedexApp_SlideInEntryButtons(pokedexApp);
            PokedexApp_UpdateEntryButtonsEnabled(pokedexApp);
        }
        if (r6 >= 256) {
            ScheduleSetBgPosText(pokedexApp->bgConfig, GF_BG_LYR_MAIN_0, BG_POS_OP_SET_X, 256);
            ScheduleSetBgPosText(pokedexApp->bgConfig, GF_BG_LYR_SUB_0, BG_POS_OP_SET_X, 256);
            PokedexApp_SetSpriteX(pokedexApp, 0, PokedexApp_GetDexModeSwitchX(pokedexApp) - 256);
            if (r4->buttonsShown == 1) {
                ++r4->state;
            }
        }
    } break;
    case 2:
        if (!BgCellManager_IsCellMoving(pokedexApp->bgCellManager, 1)) {
            u16 species = PokedexApp_GetCursorSpecies(pokedexApp);
            PokedexApp_DrawEntryNameBar(pokedexApp);
            if (species != SPECIES_NONE) {
                PokedexApp_UpdateCurrentLanguageIcon(pokedexApp, species, 8);
                if (PokedexApp_HasForeignEntries(pokedexApp, species) == TRUE) {
                    PokedexApp_SetSpritePairVisibleIfInternationalView(pokedexApp, 6, 1);
                }
            }
            PokedexApp_DrawCenteredSpeciesName(pokedexApp, species, 6);
            MI_CpuClear8(&pokedexApp->seqState, sizeof(pokedexApp->seqState));
            return POKEDEXAPP_MAINSEQ_11;
        }
        break;
    }

    return POKEDEXAPP_MAINSEQ_09;
}

static int PokedexApp_MainSeq_10(PokedexAppData *pokedexApp) {
    switch (pokedexApp->seqState.state_10.state) {
    case 0:
        PokedexApp_SlideOutListButtons(pokedexApp);
        PaletteData_BeginPaletteFade(pokedexApp->paletteData, PLTTBUF_MAIN_BG_F, PLTTFADE_OPAQUE_ALL_F & ~(1 << 0), 0, 0, 16, RGB_BLACK);
        PaletteData_BeginPaletteFade(pokedexApp->paletteData, PLTTBUF_EXCEPT_MAIN_BG_F, PLTTFADE_OPAQUE_ALL_F, 0, 0, 16, RGB_BLACK);
        ++pokedexApp->seqState.state_10.state;
        break;
    case 1:
        if (PaletteData_GetSelectedBuffersBitmask(pokedexApp->paletteData) == 0) {
            ++pokedexApp->seqState.state_10.state;
        } else if (Bg_GetXpos(pokedexApp->bgConfig, GF_BG_LYR_MAIN_0) >= 256) {
            ScheduleSetBgPosText(pokedexApp->bgConfig, GF_BG_LYR_MAIN_0, BG_POS_OP_SET_X, 256);
            ScheduleSetBgPosText(pokedexApp->bgConfig, GF_BG_LYR_SUB_0, BG_POS_OP_SET_X, 256);
            PokedexApp_SetSpriteX(pokedexApp, 0, PokedexApp_GetDexModeSwitchX(pokedexApp) - 256);
        } else {
            ScheduleSetBgPosText(pokedexApp->bgConfig, GF_BG_LYR_MAIN_0, BG_POS_OP_ADD_X, 16);
            ScheduleSetBgPosText(pokedexApp->bgConfig, GF_BG_LYR_SUB_0, BG_POS_OP_ADD_X, 16);
            PokedexApp_MoveSpriteX(pokedexApp, 0, -16);
        }
        break;
    case 2:
        if (BgCellManager_IsCellMoving(pokedexApp->bgCellManager, 0) == 0) {
            ScheduleSetBgPosText(pokedexApp->bgConfig, GF_BG_LYR_MAIN_0, BG_POS_OP_SET_X, 0);
            ScheduleSetBgPosText(pokedexApp->bgConfig, GF_BG_LYR_SUB_0, BG_POS_OP_SET_X, 0);
            ZeroPalettesByBitmask(PLTTBUF_MAIN_BG_F | PLTTBUF_SUB_BG_F | PLTTBUF_MAIN_OBJ_F | PLTTBUF_SUB_OBJ_F, HEAP_ID_POKEDEX_APP);
            PokedexApp_RemoveAllWindows(pokedexApp);
            PokedexApp_FreeListSprites(pokedexApp);
            pokedexApp->seqState.state_10.state = 0;
            return POKEDEXAPP_MAINSEQ_21;
        }
        break;
    }

    return POKEDEXAPP_MAINSEQ_10;
}

static int PokedexApp_MainSeq_11(PokedexAppData *pokedexApp) {
    switch (PokedexApp_HandleListInput(pokedexApp, 1)) {
    case 0:
        PokedexApp_MoveSpriteToListCursor(pokedexApp, 5);
        PokedexApp_UpdateListEntry(pokedexApp);
        PokedexApp_UpdateEntryButtonsEnabled(pokedexApp);
        break;
    case 1:
        return POKEDEXAPP_MAINSEQ_12;
    case 2:
        pokedexApp->entryTab = 5;
        return PokedexApp_FlashBottomButton(pokedexApp, 0, 8, POKEDEXAPP_MAINSEQ_18);
    case 3:
        if (PokedexApp_PlayCursorCry(pokedexApp) == TRUE) {
            return PokedexApp_FlashBottomButton(pokedexApp, 8, 8, POKEDEXAPP_MAINSEQ_11);
        }
        break;
    case 4:
        if (PokedexApp_GetCursorSpecies(pokedexApp) != SPECIES_NONE) {
            PlaySE(SEQ_SE_GS_ZKN04);
            pokedexApp->entryTab = 1;
            return PokedexApp_FlashBottomButton(pokedexApp, 16, 8, POKEDEXAPP_MAINSEQ_18);
        }
        break;
    case 5:
        MenuInputStateMgr_SetState(pokedexApp->args->menuInputStatePtr, MENU_INPUT_STATE_TOUCH);
        return PokedexApp_FlashBottomButton(pokedexApp, 24, 8, POKEDEXAPP_MAINSEQ_20);
    case 6:
        MenuInputStateMgr_SetState(pokedexApp->args->menuInputStatePtr, MENU_INPUT_STATE_BUTTONS);
        return PokedexApp_FlashBottomButton(pokedexApp, 24, 8, POKEDEXAPP_MAINSEQ_20);
    case 7:
        PokedexApp_CycleListEntryLanguage(pokedexApp, PokedexApp_GetCursorSpecies(pokedexApp), -1);
        break;
    case 8:
        PokedexApp_CycleListEntryLanguage(pokedexApp, PokedexApp_GetCursorSpecies(pokedexApp), 1);
        break;
    case 9:
        if (pokedexApp->listPage != 0) {
            PlaySE(SEQ_SE_GS_ZKN04);
            MI_CpuClear8(&pokedexApp->seqState, sizeof(pokedexApp->seqState));
            return POKEDEXAPP_MAINSEQ_13;
        }
        break;
    case 10:
        if (pokedexApp->listPage != 0) {
            PlaySE(SEQ_SE_GS_ZKN04);
            MI_CpuClear8(&pokedexApp->seqState, sizeof(pokedexApp->seqState));
            pokedexApp->seqState.state_13.wrapCursor = 1;
            return POKEDEXAPP_MAINSEQ_13;
        }
        break;
    case 11:
        if (pokedexApp->listPage + 1 <= PokedexApp_GetLastListPage(pokedexApp, 1)) {
            PlaySE(SEQ_SE_GS_ZKN04);
            MI_CpuClear8(&pokedexApp->seqState, sizeof(pokedexApp->seqState));
            return POKEDEXAPP_MAINSEQ_14;
        }
        break;
    case 12:
        if (pokedexApp->listPage + 1 <= PokedexApp_GetLastListPage(pokedexApp, 1)) {
            PlaySE(SEQ_SE_GS_ZKN04);
            MI_CpuClear8(&pokedexApp->seqState, sizeof(pokedexApp->seqState));
            pokedexApp->seqState.state_13.wrapCursor = 1;
            return POKEDEXAPP_MAINSEQ_14;
        }
        break;
    case 13: {
        u32 x;
        u32 y;

        System_GetTouchHeldCoords(&x, &y);
        if (PokedexApp_IsTouchOnListScrollbar(pokedexApp, 1, x, y) == 1) {
            return POKEDEXAPP_MAINSEQ_15;
        } else {
            PokedexAppData_UnkSub0868_State16 *r1 = &pokedexApp->seqState.state_16;

            r1->unk_0 = x;
            r1->unk_2 = y;
            return POKEDEXAPP_MAINSEQ_16;
        }
    } break;
    case 14: {
        PokedexAppData_UnkSub0868_State17 *r0 = &pokedexApp->seqState.state_17;

        System_GetTouchNewCoords(&r0->touchX, &r0->touchY);
        BgCellManager_EraseCell(pokedexApp->bgCellManager, 4);
        PokedexApp_SetSpritePairVisibleIfInternationalView(pokedexApp, 6, 0);
        PokedexApp_SetSpriteVisibleIfInternationalView(pokedexApp, 8, 0);
        ClearWindowTilemapAndScheduleTransfer(&pokedexApp->windows[6]);
        return POKEDEXAPP_MAINSEQ_17;
    }
    }

    return POKEDEXAPP_MAINSEQ_11;
}

static int PokedexApp_MainSeq_12(PokedexAppData *pokedexApp) {
    PokedexAppData_UnkSub0868_State09 *r4 = &pokedexApp->seqState.state_09;

    switch (r4->state) {
    case 0:
        BgCellManager_EraseCell(pokedexApp->bgCellManager, 4);
        PokedexApp_SetSpritePairVisibleIfInternationalView(pokedexApp, 6, 0);
        PokedexApp_SetSpriteVisibleIfInternationalView(pokedexApp, 8, 0);
        ClearWindowTilemapAndScheduleTransfer(&pokedexApp->windows[6]);
        PokedexApp_SlideOutEntryButtons(pokedexApp);
        ++r4->state;
        // fallthrough
    case 1: {
        ScheduleSetBgPosText(pokedexApp->bgConfig, GF_BG_LYR_MAIN_0, BG_POS_OP_SUB_X, 16);
        ScheduleSetBgPosText(pokedexApp->bgConfig, GF_BG_LYR_SUB_0, BG_POS_OP_SUB_X, 16);
        PokedexApp_MoveSpriteX(pokedexApp, 0, 16);
        int r6 = Bg_GetXpos(pokedexApp->bgConfig, GF_BG_LYR_MAIN_0);
        if (r4->buttonsShown == 0 && r6 < 64 && !BgCellManager_IsCellMoving(pokedexApp->bgCellManager, 1)) {
            r4->buttonsShown = 1;
            PokedexApp_SlideInListButtons(pokedexApp);
        }
        if (r6 <= 0) {
            ScheduleSetBgPosText(pokedexApp->bgConfig, GF_BG_LYR_MAIN_0, BG_POS_OP_SET_X, 0);
            ScheduleSetBgPosText(pokedexApp->bgConfig, GF_BG_LYR_SUB_0, BG_POS_OP_SET_X, 0);
            PokedexApp_SetSpriteX(pokedexApp, 0, PokedexApp_GetDexModeSwitchX(pokedexApp));
            if (r4->buttonsShown == 1) {
                ++r4->state;
            }
        }
    } break;
    case 2:
        if (!BgCellManager_IsCellMoving(pokedexApp->bgCellManager, 0)) {
            MI_CpuClear8(&pokedexApp->seqState, sizeof(pokedexApp->seqState));
            return POKEDEXAPP_MAINSEQ_06;
        }
    }

    return POKEDEXAPP_MAINSEQ_12;
}

static int PokedexApp_MainSeq_13(PokedexAppData *pokedexApp) {
    PokedexAppData_UnkSub0868_State13 *r4 = &pokedexApp->seqState.state_13;

    switch (r4->state) {
    case 0:
        PokedexApp_HideListCursor(pokedexApp);
        PokedexApp_SetSpriteAnim(pokedexApp, 2, 6);
        PokedexApp_SetSpritePairVisibleIfInternationalView(pokedexApp, 6, 0);
        PokedexApp_SetSpriteVisibleIfInternationalView(pokedexApp, 8, 0);
        ClearWindowTilemapAndScheduleTransfer(&pokedexApp->windows[6]);
        r4->numberTilemap = PokedexApp_BuildListNumberTilemap(pokedexApp, 15 * (pokedexApp->listPage - 1));
        r4->scrollbarStep = PokedexApp_GetListScrollbarStepCount(pokedexApp, 1, PokedexApp_GetListScrollbarYForPage(pokedexApp, pokedexApp->listPage - 1, 1, 1), 15);
        ++r4->state;
        // fallthrough
    case 1:
        ++r4->scrollStep;
        PokedexApp_LoadNumberTilemap(pokedexApp, &r4->numberTilemap[0x1E0 - 32 * r4->scrollStep]);
        PokedexApp_ScrollListIconsBackward(pokedexApp, 24, 8, r4->scrollStep / 5);
        PokedexApp_MoveListScrollbar(pokedexApp, 1, -r4->scrollbarStep);
        if (r4->scrollStep == 15) {
            ++r4->state;
        }
        break;
    case 2:
        if (r4->wrapCursor == 1) {
            pokedexApp->listCursor += 10;
        }
        --pokedexApp->listPage;
        PokedexApp_MoveSpriteToListCursor(pokedexApp, 5);
        PokedexApp_FreeNumberTilemap(r4->numberTilemap);
        PokedexApp_UpdateListEntry(pokedexApp);
        PokedexApp_UpdateListPageArrows(pokedexApp, 2, 1);
        PokedexApp_SyncListScrollbarToPage(pokedexApp, 1, 1);
        PokedexApp_UpdateEntryButtonsEnabled(pokedexApp);
        MI_CpuClear8(&pokedexApp->seqState, sizeof(pokedexApp->seqState));
        return POKEDEXAPP_MAINSEQ_11;
    }

    return POKEDEXAPP_MAINSEQ_13;
}

static int PokedexApp_MainSeq_14(PokedexAppData *pokedexApp) {
    PokedexAppData_UnkSub0868_State13 *r4 = &pokedexApp->seqState.state_13;

    switch (r4->state) {
    case 0:
        PokedexApp_HideListCursor(pokedexApp);
        PokedexApp_SetSpriteAnim(pokedexApp, 3, 9);
        PokedexApp_SetSpritePairVisibleIfInternationalView(pokedexApp, 6, 0);
        PokedexApp_SetSpriteVisibleIfInternationalView(pokedexApp, 8, 0);
        ClearWindowTilemapAndScheduleTransfer(&pokedexApp->windows[6]);
        r4->numberTilemap = PokedexApp_BuildListNumberTilemap(pokedexApp, 15 * pokedexApp->listPage);
        r4->scrollbarStep = PokedexApp_GetListScrollbarStepCount(pokedexApp, 1, PokedexApp_GetListScrollbarYForPage(pokedexApp, pokedexApp->listPage + 1, 1, 1), 15);
        ++r4->state;
        // fallthrough
    case 1:
        ++r4->scrollStep;
        PokedexApp_LoadNumberTilemap(pokedexApp, &r4->numberTilemap[32 * r4->scrollStep]);
        PokedexApp_ScrollListIconsForward(pokedexApp, 24, -8, r4->scrollStep / 5);
        PokedexApp_MoveListScrollbar(pokedexApp, 1, r4->scrollbarStep);
        if (r4->scrollStep == 15) {
            ++r4->state;
        }
        break;
    case 2:
        if (r4->wrapCursor == 1) {
            pokedexApp->listCursor -= 10;
        }
        ++pokedexApp->listPage;
        PokedexApp_MoveSpriteToListCursor(pokedexApp, 5);
        PokedexApp_FreeNumberTilemap(r4->numberTilemap);
        PokedexApp_UpdateListEntry(pokedexApp);
        PokedexApp_UpdateListPageArrows(pokedexApp, 2, 1);
        PokedexApp_SyncListScrollbarToPage(pokedexApp, 1, 1);
        PokedexApp_UpdateEntryButtonsEnabled(pokedexApp);
        MI_CpuClear8(&pokedexApp->seqState, sizeof(pokedexApp->seqState));
        return POKEDEXAPP_MAINSEQ_11;
    }

    return POKEDEXAPP_MAINSEQ_14;
}

static int PokedexApp_MainSeq_15(PokedexAppData *pokedexApp) {
    u32 x;
    u32 y;

    if (!System_GetTouchHeldCoords(&x, &y)) {
        return POKEDEXAPP_MAINSEQ_11;
    }
    if (PokedexApp_DragListScrollbar(pokedexApp, 1, x, y, 1) == 1) {
        u16 *r4 = PokedexApp_BuildListNumberTilemap(pokedexApp, 15 * pokedexApp->listPage);
        pokedexApp->iconPage ^= 1;
        PokedexApp_LoadNumberTilemap(pokedexApp, r4);
        PokedexApp_FreeNumberTilemap(r4);
        PokedexApp_DrawListIconGrid(pokedexApp, 24);
        PokedexApp_UpdateListEntry(pokedexApp);
        PokedexApp_UpdateListPageArrows(pokedexApp, 2, 1);
        PokedexApp_UpdateEntryButtonsEnabled(pokedexApp);
        PlaySE(SEQ_SE_GS_ZKN03);
    }
    return POKEDEXAPP_MAINSEQ_15;
}

static int PokedexApp_MainSeq_16(PokedexAppData *pokedexApp) {
    PokedexAppData_UnkSub0868_State16 *r3 = &pokedexApp->seqState.state_16;
    if (PokedexApp_DragListScrollbar(pokedexApp, 1, r3->unk_0, r3->unk_2, 1) == 1) {
        u16 *r4 = PokedexApp_BuildListNumberTilemap(pokedexApp, 15 * pokedexApp->listPage);
        pokedexApp->iconPage ^= 1;
        PokedexApp_LoadNumberTilemap(pokedexApp, r4);
        PokedexApp_FreeNumberTilemap(r4);
        PokedexApp_DrawListIconGrid(pokedexApp, 24);
        PokedexApp_UpdateListEntry(pokedexApp);
        PokedexApp_UpdateListPageArrows(pokedexApp, 2, 1);
        PokedexApp_UpdateEntryButtonsEnabled(pokedexApp);
        MI_CpuClear8(&pokedexApp->seqState, sizeof(pokedexApp->seqState));
        PlaySE(SEQ_SE_GS_ZKN03);
    }
    return POKEDEXAPP_MAINSEQ_11;
}

static int PokedexApp_MainSeq_17(PokedexAppData *pokedexApp) {
    PokedexAppData_UnkSub0868_State17 *r4 = &pokedexApp->seqState.state_17;

    switch (r4->state) {
    case 0: {
        u32 x;
        u32 y;

        if (!System_GetTouchHeldCoords(&x, &y)) {
            if (Bg_GetXpos(pokedexApp->bgConfig, GF_BG_LYR_MAIN_0) < 192) {
                MI_CpuClear8(&pokedexApp->seqState, sizeof(pokedexApp->seqState));
                return POKEDEXAPP_MAINSEQ_12;
            }
            ++r4->state;
        } else {
            if ((int)(r4->touchX + 256 - x) > 256) {
                ScheduleSetBgPosText(pokedexApp->bgConfig, GF_BG_LYR_MAIN_0, BG_POS_OP_SET_X, 256);
                ScheduleSetBgPosText(pokedexApp->bgConfig, GF_BG_LYR_SUB_0, BG_POS_OP_SET_X, 256);
                PokedexApp_SetSpriteX(pokedexApp, 0, PokedexApp_GetDexModeSwitchX(pokedexApp) - 256);
            } else {
                ScheduleSetBgPosText(pokedexApp->bgConfig, GF_BG_LYR_MAIN_0, BG_POS_OP_SET_X, r4->touchX + 0x100 - x);
                ScheduleSetBgPosText(pokedexApp->bgConfig, GF_BG_LYR_SUB_0, BG_POS_OP_SET_X, r4->touchX + 0x100 - x);
                PokedexApp_SetSpriteX(pokedexApp, 0, (PokedexApp_GetDexModeSwitchX(pokedexApp) - 256) - (r4->touchX - x));
            }
        }
    } break;
    case 1:
        ScheduleSetBgPosText(pokedexApp->bgConfig, GF_BG_LYR_MAIN_0, BG_POS_OP_ADD_X, 16);
        ScheduleSetBgPosText(pokedexApp->bgConfig, GF_BG_LYR_SUB_0, BG_POS_OP_ADD_X, 16);
        PokedexApp_MoveSpriteX(pokedexApp, 0, -16);
        if (Bg_GetXpos(pokedexApp->bgConfig, GF_BG_LYR_MAIN_0) > 256) {
            u16 r4 = PokedexApp_GetCursorSpecies(pokedexApp);
            PokedexApp_DrawEntryNameBar(pokedexApp);
            if (r4 != SPECIES_NONE) {
                PokedexApp_UpdateCurrentLanguageIcon(pokedexApp, r4, 8);
                if (PokedexApp_HasForeignEntries(pokedexApp, r4) == TRUE) {
                    PokedexApp_SetSpritePairVisibleIfInternationalView(pokedexApp, 6, 1);
                }
            }
            PokedexApp_DrawCenteredSpeciesName(pokedexApp, r4, 6);
            ScheduleSetBgPosText(pokedexApp->bgConfig, GF_BG_LYR_MAIN_0, BG_POS_OP_SET_X, 256);
            ScheduleSetBgPosText(pokedexApp->bgConfig, GF_BG_LYR_SUB_0, BG_POS_OP_SET_X, 256);
            PokedexApp_SetSpriteX(pokedexApp, 0, PokedexApp_GetDexModeSwitchX(pokedexApp) - 256);
            MI_CpuClear8(&pokedexApp->seqState, sizeof(pokedexApp->seqState));
            return POKEDEXAPP_MAINSEQ_11;
        }
        break;
    }

    return POKEDEXAPP_MAINSEQ_17;
}

static int PokedexApp_MainSeq_18(PokedexAppData *pokedexApp) {
    switch (pokedexApp->seqState.state_10.state) {
    case 0:
        BgCellManager_EraseCell(pokedexApp->bgCellManager, 4);
        PokedexApp_SetSpritePairVisibleIfInternationalView(pokedexApp, 6, 0);
        PokedexApp_SetSpriteVisibleIfInternationalView(pokedexApp, 8, 0);
        ClearWindowTilemapAndScheduleTransfer(&pokedexApp->windows[6]);
        PokedexApp_SlideOutEntryButtons(pokedexApp);
        PaletteData_BeginPaletteFade(pokedexApp->paletteData, PLTTBUF_MAIN_BG_F, PLTTFADE_OPAQUE_ALL_F & ~(1 << 0), 0, 0, 16, RGB_BLACK);
        PaletteData_BeginPaletteFade(pokedexApp->paletteData, PLTTBUF_EXCEPT_MAIN_BG_F, PLTTFADE_OPAQUE_ALL_F, 0, 0, 16, RGB_BLACK);
        pokedexApp->nextSeq = POKEDEXAPP_MAINSEQ_18;
        ++pokedexApp->seqState.state_10.state;
        return POKEDEXAPP_MAINSEQ_03;
    case 1:
        if (!BgCellManager_IsCellMoving(pokedexApp->bgCellManager, 1)) {
            ScheduleSetBgPosText(pokedexApp->bgConfig, GF_BG_LYR_MAIN_0, BG_POS_OP_SET_X, 0);
            ScheduleSetBgPosText(pokedexApp->bgConfig, GF_BG_LYR_SUB_0, BG_POS_OP_SET_X, 0);
            ZeroPalettesByBitmask(PLTTBUF_MAIN_BG_F | PLTTBUF_SUB_BG_F | PLTTBUF_MAIN_OBJ_F | PLTTBUF_SUB_OBJ_F, HEAP_ID_POKEDEX_APP);
            PokedexApp_RemoveAllWindows(pokedexApp);
            PokedexApp_FreeListSprites(pokedexApp);
            pokedexApp->seqState.state_10.state = 0;
            if (pokedexApp->entryTab == 5) {
                return POKEDEXAPP_MAINSEQ_21;
            } else {
                pokedexApp->entryFromSearch = 0;
                return POKEDEXAPP_MAINSEQ_66;
            }
        }
        break;
    }

    return POKEDEXAPP_MAINSEQ_18;
}

static int PokedexApp_MainSeq_21(PokedexAppData *pokedexApp) {
    switch (pokedexApp->seqState.state_10.state) {
    case 0:
        PokedexApp_SetupScreen(pokedexApp, 5);
        PokedexApp_SlideInSearchMenuButtons(pokedexApp);
        PokedexApp_CreateSearchMenuGrid(pokedexApp, 0);
        PaletteData_BeginPaletteFade(pokedexApp->paletteData, PLTTBUF_MAIN_BG_F, PLTTFADE_OPAQUE_ALL_F & ~(1 << 4), 0, 16, 0, RGB_BLACK);
        PaletteData_BeginPaletteFade(pokedexApp->paletteData, PLTTBUF_EXCEPT_MAIN_BG_F, PLTTFADE_OPAQUE_ALL_F, 0, 16, 0, RGB_BLACK);
        ++pokedexApp->seqState.state_10.state;
        pokedexApp->nextSeq = POKEDEXAPP_MAINSEQ_21;
        return POKEDEXAPP_MAINSEQ_03;
    case 1:
        if (!BgCellManager_IsCellMoving(pokedexApp->bgCellManager, 6)) {
            pokedexApp->seqState.state_10.state = 0;
            return POKEDEXAPP_MAINSEQ_22;
        }
        break;
    }

    return POKEDEXAPP_MAINSEQ_21;
}

static int PokedexApp_MainSeq_22(PokedexAppData *pokedexApp) {
    switch (PokedexApp_HandleSearchMenuInput(pokedexApp)) {
    case 0:
        (void)MenuInputStateMgr_GetState(pokedexApp->args->menuInputStatePtr);
        PlaySE(SEQ_SE_GS_ZKN04);
        return PokedexApp_FlashSearchMenuOption(pokedexApp, 16, 1, 11, 2, POKEDEXAPP_MAINSEQ_28);
    case 1:
        (void)MenuInputStateMgr_GetState(pokedexApp->args->menuInputStatePtr);
        PlaySE(SEQ_SE_GS_ZKN04);
        return PokedexApp_FlashSearchMenuOption(pokedexApp, 7, 4, 9, 2, POKEDEXAPP_MAINSEQ_31);
    case 2:
        (void)MenuInputStateMgr_GetState(pokedexApp->args->menuInputStatePtr);
        PlaySE(SEQ_SE_GS_ZKN04);
        return PokedexApp_FlashSearchMenuOption(pokedexApp, 7, 7, 16, 2, POKEDEXAPP_MAINSEQ_34);
    case 3:
        (void)MenuInputStateMgr_GetState(pokedexApp->args->menuInputStatePtr);
        PlaySE(SEQ_SE_GS_ZKN04);
        return PokedexApp_FlashSearchMenuOption(pokedexApp, 7, 10, 14, 2, POKEDEXAPP_MAINSEQ_37);
    case 4:
        (void)MenuInputStateMgr_GetState(pokedexApp->args->menuInputStatePtr);
        PlaySE(SEQ_SE_GS_ZKN04);
        return PokedexApp_FlashSearchMenuOption(pokedexApp, 7, 13, 20, 2, POKEDEXAPP_MAINSEQ_44);
    case 5:
        (void)MenuInputStateMgr_GetState(pokedexApp->args->menuInputStatePtr);
        PlaySE(SEQ_SE_GS_ZKN04);
        return PokedexApp_FlashSearchMenuOption(pokedexApp, 7, 16, 7, 2, POKEDEXAPP_MAINSEQ_51);
    case 6:
        (void)MenuInputStateMgr_GetState(pokedexApp->args->menuInputStatePtr);
        PlaySE(SEQ_SE_GS_ZKN04);
        return PokedexApp_FlashSearchMenuOption(pokedexApp, 26, 7, 4, 4, POKEDEXAPP_MAINSEQ_54);
    case 7:
        (void)MenuInputStateMgr_GetState(pokedexApp->args->menuInputStatePtr);
        PlaySE(SEQ_SE_GS_ZKN04);
        return PokedexApp_FlashSearchButton(pokedexApp, 0, 10, POKEDEXAPP_MAINSEQ_24);
    case 8:
        (void)MenuInputStateMgr_GetState(pokedexApp->args->menuInputStatePtr);
        PlaySE(SEQ_SE_GS_ZKN04);
        return PokedexApp_FlashSearchButton(pokedexApp, 11, 10, POKEDEXAPP_MAINSEQ_23);
    case 9:
    case -2:
        PlaySE(SEQ_SE_GS_GEARCANCEL);
        return PokedexApp_FlashSearchButton(pokedexApp, 22, 10, POKEDEXAPP_MAINSEQ_25);
    case -3:
        break;
    case -1:
    case -4:
    default:
        break;
    }

    return POKEDEXAPP_MAINSEQ_22;
}

static int PokedexApp_MainSeq_23(PokedexAppData *pokedexApp) {
    PokedexAppData_UnkSub0868_State16 *r5 = &pokedexApp->seqState.state_16;

    switch (r5->unk_0) {
    case 0:
        PokedexApp_SetSpriteAnim(pokedexApp, 27, 2);
        PokedexApp_SetSpriteVisible(pokedexApp, 27, 1);
        PokedexApp_SetSpriteVisible(pokedexApp, 0, 0);
        PokedexApp_DrawSearchMessage(pokedexApp, 9);
        PokedexApp_SlideOutSearchMenuButtons(pokedexApp);
        PlaySE(SEQ_SE_DP_Z_SEARCH);
        r5->unk_0 = 1;
        break;
    case 1:
        PokedexApp_RunSearch(pokedexApp, pokedexApp->dexMode, pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_SORT_ORDER], pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_LETTER], pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_TYPE1], pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_TYPE2], pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_HEIGHT_MIN], pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_HEIGHT_MAX], pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_WEIGHT_MIN], pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_WEIGHT_MAX], 1 << pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_AREA], pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_BODY_TYPE]);
        if (pokedexApp->searchResults.caughtCount == 0 && pokedexApp->searchResults.count == 0) {
            r5->unk_0 = 15;
        } else {
            PokedexApp_BuildListEntries(pokedexApp, 0);
            pokedexApp->listCursor = 0;
            pokedexApp->listPage = 0;
            pokedexApp->language = 2;
            r5->unk_0 = 2;
        }
        break;
    case 2:
        PokedexApp_SetCellTilemapFromBg(pokedexApp, GF_BG_LYR_MAIN_0, 9);
        r5->unk_0 = 3;
        break;
    case 3:
        PokedexApp_SetCellTilemapFromBg(pokedexApp, GF_BG_LYR_MAIN_2, 10);
        r5->unk_0 = 4;
        break;
    case 4:
        PokedexApp_SetCellTilemapFromBg(pokedexApp, GF_BG_LYR_MAIN_0, 11);
        r5->unk_0 = 5;
        break;
    case 5:
        PokedexApp_SetCellTilemapFromBg(pokedexApp, GF_BG_LYR_MAIN_2, 12);
        r5->unk_0 = 6;
        break;
    case 6:
        PokedexApp_DrawSearchCriteriaSummary(pokedexApp);
        r5->unk_0 = 7;
        break;
    case 7:
        PokedexApp_DrawListIconGrid(pokedexApp, 59);
        PokedexApp_DrawSearchResultNumbers(pokedexApp);
        r5->unk_0 = 8;
        break;
    case 8:
        PokedexApp_SetSpriteVisible(pokedexApp, 18, 1);
        PokedexApp_SetSpriteVisible(pokedexApp, 19, 1);
        PokedexApp_SetSpriteVisible(pokedexApp, 20, 1);
        PokedexApp_SetSpriteVisible(pokedexApp, 21, 1);
        PokedexApp_SetSpriteVisible(pokedexApp, 25, 1);
        PokedexApp_UpdateListScrollbarSize(pokedexApp, 18, 0);
        PokedexApp_SyncListScrollbarToPage(pokedexApp, 18, 0);
        PokedexApp_UpdateListPageArrows(pokedexApp, 19, 0);
        r5->unk_0 = 9;
        break;
    case 9:
        if (!PokedexApp_IsSpriteAnimating(pokedexApp, 27)) {
            PokedexApp_SetSpriteAnim(pokedexApp, 26, 1);
            PokedexApp_SetSpriteVisible(pokedexApp, 27, 0);
            r5->unk_0 = 10;
        }
        break;
    case 10:
        ++r5->unk_2;
        if (r5->unk_2 == 32) {
            PokedexApp_DrawSearchMessage(pokedexApp, 1);
            PokedexApp_UpdateSearchBodyTypeIcon(pokedexApp);
            PokedexApp_MoveSearchSummaryIcons(pokedexApp, -8);
            PokedexApp_SetSubScreenClipWindow(TRUE);
            BgCellManager_DrawCellAt(pokedexApp->bgCellManager, 9, 0, 0);
            BgCellManager_DrawCellAt(pokedexApp->bgCellManager, 10, 0, 0);
            BgCellManager_DrawCellAt(pokedexApp->bgCellManager, 11, 0, 24);
            BgCellManager_DrawCellAt(pokedexApp->bgCellManager, 12, 0, 24);
            BgCellManager_StartCellMove(pokedexApp->bgCellManager, 9, 0, -1, 24);
            BgCellManager_StartCellMove(pokedexApp->bgCellManager, 10, 0, -1, 24);
            BgCellManager_StartCellMove(pokedexApp->bgCellManager, 11, 0, -1, 24);
            BgCellManager_StartCellMove(pokedexApp->bgCellManager, 12, 0, -1, 24);
            PlaySE(SEQ_SE_GS_ZUKAN06);
            r5->unk_2 = 0;
            r5->unk_0 = 11;
        }
        break;
    case 11:
        if (!BgCellManager_IsCellMoving(pokedexApp->bgCellManager, 9)) {
            PokedexApp_SetSubScreenClipWindow(FALSE);
            r5->unk_0 = 12;
        } else {
            PokedexApp_MoveSearchSummaryIcons(pokedexApp, -8);
        }
        break;
    case 12:
        GfGfxLoader_LoadCharDataFromOpenNarc(pokedexApp->gfxNarc, NARC_zukan_gra_zukan_gra_00000004_NCGR_lz, pokedexApp->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, TRUE, HEAP_ID_POKEDEX_APP);
        GfGfxLoader_LoadCharDataFromOpenNarc(pokedexApp->gfxNarc, NARC_zukan_gra_zukan_gra_00000019_NCGR_lz, pokedexApp->bgConfig, GF_BG_LYR_SUB_3, 0, 0, TRUE, HEAP_ID_POKEDEX_APP);
        GfGfxLoader_LoadScrnDataFromOpenNarc(pokedexApp->gfxNarc, NARC_zukan_gra_zukan_gra_00000020_NSCR_lz, pokedexApp->bgConfig, GF_BG_LYR_SUB_3, 0, 0, TRUE, HEAP_ID_POKEDEX_APP);
        PokedexApp_SlideInSearchResultButtons(pokedexApp);
        ov18_021F3448(pokedexApp);
        r5->unk_0 = 13;
        break;
    case 13:
        if (!BgCellManager_IsCellMoving(pokedexApp->bgCellManager, 8)) {
            r5->unk_0 = 14;
        }
        break;
    case 14:
        PokedexApp_FreeGridInput(pokedexApp);
        PokedexApp_SetSpriteVisible(pokedexApp, 0, 1);
        PokedexApp_MoveSpriteToListCursor(pokedexApp, 0);
        PokedexApp_SetSpriteAnim(pokedexApp, 0, 1);
        PokedexApp_CreateSearchEntrySprites(pokedexApp);
        PokedexApp_UpdateSearchResultButtonsEnabled(pokedexApp);
        PokedexApp_DrawEntryNameBar(pokedexApp);
        MI_CpuClear8(&pokedexApp->seqState, sizeof(pokedexApp->seqState));
        return POKEDEXAPP_MAINSEQ_57;
    case 15:
        if (!PokedexApp_IsSpriteAnimating(pokedexApp, 27)) {
            PokedexApp_SetSpriteAnim(pokedexApp, 26, 3);
            PokedexApp_SetSpriteVisible(pokedexApp, 27, 0);
            r5->unk_0 = 16;
        }
        break;
    case 16:
        if (!PokedexApp_IsSpriteAnimating(pokedexApp, 26)) {
            PokedexApp_DrawSearchMessage(pokedexApp, 10);
            PlaySE(SEQ_SE_DP_CUSTOM06);
            r5->unk_0 = 17;
        }
        break;
    case 17:
        ++r5->unk_2;
        if ((gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B)) || System_GetTouchNew() == TRUE || r5->unk_2 == 60) {
            PokedexApp_DrawSearchMessage(pokedexApp, 0);
            PokedexApp_SlideInSearchMenuButtons(pokedexApp);
            r5->unk_2 = 0;
            r5->unk_0 = 18;
        }
        break;
    case 18:
        if (!BgCellManager_IsCellMoving(pokedexApp->bgCellManager, 6)) {
            PokedexApp_SetSpriteVisible(pokedexApp, 0, 1);
            MI_CpuClear8(&pokedexApp->seqState, sizeof(pokedexApp->seqState));
            return POKEDEXAPP_MAINSEQ_22;
        }
        break;
    }

    return POKEDEXAPP_MAINSEQ_23;
}

static void PokedexApp_SetCellTilemapFromBg(PokedexAppData *pokedexApp, int bgId, int a2) {
    BgCellManager_SetCellTilemap(pokedexApp->bgCellManager, a2, GetBgTilemapBuffer(pokedexApp->bgConfig, bgId));
}

static int PokedexApp_MainSeq_24(PokedexAppData *pokedexApp) {
    PokedexApp_ResetSearchCriteria(pokedexApp);
    PokedexApp_DrawSearchCriteria(pokedexApp);
    PokedexApp_UpdateBodyTypeSprite(pokedexApp);
    return POKEDEXAPP_MAINSEQ_22;
}

static int PokedexApp_MainSeq_25(PokedexAppData *pokedexApp) {
    switch (pokedexApp->seqState.state_10.state) {
    case 0:
        PokedexApp_SetSpriteVisible(pokedexApp, 0, 0);
        PokedexApp_SlideOutSearchMenuButtons(pokedexApp);
        PaletteData_BeginPaletteFade(pokedexApp->paletteData, PLTTBUF_MAIN_BG_F, PLTTFADE_OPAQUE_ALL_F & ~(1 << 4), 0, 0, 16, RGB_BLACK);
        PaletteData_BeginPaletteFade(pokedexApp->paletteData, PLTTBUF_EXCEPT_MAIN_BG_F, PLTTFADE_OPAQUE_ALL_F, 0, 0, 16, RGB_BLACK);
        pokedexApp->nextSeq = POKEDEXAPP_MAINSEQ_25;
        ++pokedexApp->seqState.state_10.state;
        return POKEDEXAPP_MAINSEQ_03;
    case 1:
        ZeroPalettesByBitmask(PLTTBUF_MAIN_BG_F | PLTTBUF_SUB_BG_F | PLTTBUF_MAIN_OBJ_F | PLTTBUF_SUB_OBJ_F, HEAP_ID_POKEDEX_APP);
        PokedexApp_FreeGridInput(pokedexApp);
        PokedexApp_RemoveAllWindows(pokedexApp);
        PokedexApp_FreeSearchSprites(pokedexApp);
        ov18_021F3448(pokedexApp);
        pokedexApp->seqState.state_10.state = 0;
        return POKEDEXAPP_MAINSEQ_19;
    }

    return POKEDEXAPP_MAINSEQ_25;
}

static int PokedexApp_MainSeq_19(PokedexAppData *pokedexApp) {
    switch (pokedexApp->seqState.state_10.state) {
    case 0:
        PokedexApp_RunSearch(pokedexApp, pokedexApp->dexMode, 0, DEX_SEARCH_LETTERS_ALL, DEX_SEARCH_TYPE_ALL, DEX_SEARCH_TYPE_ALL, 0, 152, 0, 152, 1 << DEX_SEARCH_AREA_ALL, DEX_SEARCH_BODYTYPE_ALL);
        PokedexApp_BuildListEntries(pokedexApp, 1);
        ++pokedexApp->seqState.state_10.state;
        break;
    case 1:
        PokedexApp_SetupScreen(pokedexApp, 0);
        if (PokedexApp_GetListCursorIndex(pokedexApp) >= PokedexApp_GetListLength(pokedexApp, TRUE)) {
            PokedexApp_LoadEmptyEntryDetailsBg(pokedexApp->bgConfig, GF_BG_LYR_SUB_3, pokedexApp->gfxNarc, HEAP_ID_POKEDEX_APP);
            ClearWindowTilemapAndScheduleTransfer(&pokedexApp->windows[7]);
            PokedexApp_SetSpriteVisible(pokedexApp, 10, 0);
        }
        BgCellManager_EraseCell(pokedexApp->bgCellManager, 0);
        PokedexApp_SlideInEntryButtons(pokedexApp);
        ScheduleSetBgPosText(pokedexApp->bgConfig, GF_BG_LYR_MAIN_0, BG_POS_OP_SET_X, 256);
        ScheduleSetBgPosText(pokedexApp->bgConfig, GF_BG_LYR_SUB_0, BG_POS_OP_SET_X, 256);
        PokedexApp_MoveSpriteX(pokedexApp, 0, -256);
        PaletteData_BeginPaletteFade(pokedexApp->paletteData, PLTTBUF_MAIN_BG_F, PLTTFADE_OPAQUE_ALL_F & ~(1 << 0), 0, 16, 0, RGB_BLACK);
        PaletteData_BeginPaletteFade(pokedexApp->paletteData, PLTTBUF_EXCEPT_MAIN_BG_F, PLTTFADE_OPAQUE_ALL_F, 0, 16, 0, RGB_BLACK);
        pokedexApp->nextSeq = POKEDEXAPP_MAINSEQ_19;
        ++pokedexApp->seqState.state_10.state;
        return POKEDEXAPP_MAINSEQ_03;
    case 2: {
        u16 species = PokedexApp_GetCursorSpecies(pokedexApp);
        PokedexApp_DrawEntryNameBar(pokedexApp);
        if (species != SPECIES_NONE) {
            PokedexApp_SetSpriteVisibleIfInternationalView(pokedexApp, 8, 1);
        } else {
            PokedexApp_SetSpriteVisibleIfInternationalView(pokedexApp, 8, 0);
        }
        if (PokedexApp_HasForeignEntries(pokedexApp, species) == TRUE) {
            PokedexApp_SetSpritePairVisibleIfInternationalView(pokedexApp, 6, 1);
        }
        PokedexApp_DrawCenteredSpeciesName(pokedexApp, species, 6);
        pokedexApp->seqState.state_10.state = 0;
        return POKEDEXAPP_MAINSEQ_11;
    }
    }

    return POKEDEXAPP_MAINSEQ_19;
}

static int PokedexApp_MainSeq_20(PokedexAppData *pokedexApp) {
    PokedexAppData_UnkSub0868_State09 *r4 = &pokedexApp->seqState.state_09;

    switch (r4->state) {
    case 0:
        BgCellManager_EraseCell(pokedexApp->bgCellManager, 4);
        PokedexApp_SetSpritePairVisibleIfInternationalView(pokedexApp, 6, 0);
        PokedexApp_SetSpriteVisibleIfInternationalView(pokedexApp, 8, 0);
        ClearWindowTilemapAndScheduleTransfer(&pokedexApp->windows[6]);
        PokedexApp_SlideOutEntryButtons(pokedexApp);
        r4->timer = 0;
        ++r4->state;
        // fallthrough
    case 1:
        if (r4->timer == 10) {
            PokedexApp_FadeOutTo(pokedexApp, POKEDEXAPP_MAINSEQ_MAX);
        } else if (r4->timer > 10 && IsPaletteFadeFinished() == TRUE) {
            MI_CpuClear8(&pokedexApp->seqState, sizeof(pokedexApp->seqState));
            pokedexApp->nextSeq = POKEDEXAPP_MAINSEQ_MAX;
            return POKEDEXAPP_MAINSEQ_01;
        }
        ++r4->timer;
        if (Bg_GetXpos(pokedexApp->bgConfig, GF_BG_LYR_MAIN_0) == 0) {
            ScheduleSetBgPosText(pokedexApp->bgConfig, GF_BG_LYR_MAIN_0, BG_POS_OP_SET_X, 0);
            ScheduleSetBgPosText(pokedexApp->bgConfig, GF_BG_LYR_SUB_0, BG_POS_OP_SET_X, 0);
            PokedexApp_SetSpriteX(pokedexApp, 0, PokedexApp_GetDexModeSwitchX(pokedexApp));
        } else {
            ScheduleSetBgPosText(pokedexApp->bgConfig, GF_BG_LYR_MAIN_0, BG_POS_OP_SUB_X, 16);
            ScheduleSetBgPosText(pokedexApp->bgConfig, GF_BG_LYR_SUB_0, BG_POS_OP_SUB_X, 16);
            PokedexApp_MoveSpriteX(pokedexApp, 0, 16);
        }
        break;
    }

    return POKEDEXAPP_MAINSEQ_20;
}

static int PokedexApp_MainSeq_26(PokedexAppData *pokedexApp) {
    PokedexAppData_UnkSub0868_State26 *r4 = &pokedexApp->seqState.state_26;

    switch (r4->unk_2) {
    case 0:
        PokedexApp_SetSpriteVisible(pokedexApp, 0, 0);
        PokedexApp_SlideOutSearchMenuButtons(pokedexApp);
        PaletteData_BeginPaletteFade(pokedexApp->paletteData, PLTTBUF_MAIN_BG_F, PLTTFADE_OPAQUE_ALL_F & ~(1 << 4), 0, 0, 16, RGB_BLACK);
        PaletteData_BeginPaletteFade(pokedexApp->paletteData, PLTTBUF_EXCEPT_MAIN_BG_F, PLTTFADE_OPAQUE_ALL_F, 0, 0, 16, RGB_BLACK);
        pokedexApp->nextSeq = POKEDEXAPP_MAINSEQ_26;
        ++r4->unk_2;
        return POKEDEXAPP_MAINSEQ_03;
    case 1:
        PokedexApp_FreeGridInput(pokedexApp);
        PokedexApp_SlideInSearchPageButtons(pokedexApp);
        PokedexApp_SetupSearchPage(pokedexApp, r4->unk_0, r4->unk_3);
        PaletteData_BeginPaletteFade(pokedexApp->paletteData, PLTTBUF_MAIN_BG_F, PLTTFADE_OPAQUE_ALL_F & ~(1 << 4), 0, 16, 0, RGB_BLACK);
        PaletteData_BeginPaletteFade(pokedexApp->paletteData, PLTTBUF_EXCEPT_MAIN_BG_F, PLTTFADE_OPAQUE_ALL_F, 0, 16, 0, RGB_BLACK);
        pokedexApp->nextSeq = r4->unk_1;
        MI_CpuClear8(&pokedexApp->seqState, sizeof(pokedexApp->seqState));
        return POKEDEXAPP_MAINSEQ_03;
    }

    return POKEDEXAPP_MAINSEQ_26; // if we're here, we're softlocked
}

static int PokedexApp_MainSeq_27(PokedexAppData *pokedexApp) {
    PokedexAppData_UnkSub0868_State26 *r4 = &pokedexApp->seqState.state_26;

    switch (r4->unk_2) {
    case 0:
        PokedexApp_SetSpriteVisible(pokedexApp, 0, 0);
        PokedexApp_SlideOutSearchPageButtons(pokedexApp);
        PaletteData_BeginPaletteFade(pokedexApp->paletteData, PLTTBUF_MAIN_BG_F, PLTTFADE_OPAQUE_ALL_F & ~(1 << 4), 0, 0, 16, RGB_BLACK);
        PaletteData_BeginPaletteFade(pokedexApp->paletteData, PLTTBUF_EXCEPT_MAIN_BG_F, PLTTFADE_OPAQUE_ALL_F, 0, 0, 16, RGB_BLACK);
        pokedexApp->nextSeq = POKEDEXAPP_MAINSEQ_27;
        ++r4->unk_2;
        return POKEDEXAPP_MAINSEQ_03;
    case 1:
        PokedexApp_FreeGridInput(pokedexApp);
        PokedexApp_SlideInSearchMenuButtons(pokedexApp);
        PokedexApp_SetupSearchPage(pokedexApp, 0, r4->unk_3);
        PaletteData_BeginPaletteFade(pokedexApp->paletteData, PLTTBUF_MAIN_BG_F, PLTTFADE_OPAQUE_ALL_F & ~(1 << 4), 0, 16, 0, RGB_BLACK);
        PaletteData_BeginPaletteFade(pokedexApp->paletteData, PLTTBUF_EXCEPT_MAIN_BG_F, PLTTFADE_OPAQUE_ALL_F, 0, 16, 0, RGB_BLACK);
        pokedexApp->nextSeq = r4->unk_1;
        MI_CpuClear8(&pokedexApp->seqState, sizeof(pokedexApp->seqState));
        return POKEDEXAPP_MAINSEQ_03;
    }

    return POKEDEXAPP_MAINSEQ_27; // if we're here, we're softlocked
}

static int PokedexApp_MainSeq_28(PokedexAppData *pokedexApp) {
    pokedexApp->dexSearchCriteriaBak[0] = pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_SORT_ORDER];
    PokedexApp_PrepareSearchPageChange(pokedexApp, 1, 0, POKEDEXAPP_MAINSEQ_29);
    return POKEDEXAPP_MAINSEQ_26;
}

static int PokedexApp_MainSeq_29(PokedexAppData *pokedexApp) {
    switch (PokedexApp_HandleSearchOrderInput(pokedexApp)) {
    case DEX_ORDER_NATIONAL:
        (void)MenuInputStateMgr_GetState(pokedexApp->args->menuInputStatePtr);
        PlaySE(SEQ_SE_GS_ZKN04);
        PokedexApp_SelectSearchOrder(pokedexApp, 0);
        break;
    case DEX_ORDER_JOHTO:
        (void)MenuInputStateMgr_GetState(pokedexApp->args->menuInputStatePtr);
        PlaySE(SEQ_SE_GS_ZKN04);
        PokedexApp_SelectSearchOrder(pokedexApp, 1);
        break;
    case DEX_ORDER_ALPHABETICAL:
        (void)MenuInputStateMgr_GetState(pokedexApp->args->menuInputStatePtr);
        PlaySE(SEQ_SE_GS_ZKN04);
        PokedexApp_SelectSearchOrder(pokedexApp, 2);
        break;
    case DEX_ORDER_HEAVIEST:
        (void)MenuInputStateMgr_GetState(pokedexApp->args->menuInputStatePtr);
        PlaySE(SEQ_SE_GS_ZKN04);
        PokedexApp_SelectSearchOrder(pokedexApp, 3);
        break;
    case DEX_ORDER_LIGHTEST:
        (void)MenuInputStateMgr_GetState(pokedexApp->args->menuInputStatePtr);
        PlaySE(SEQ_SE_GS_ZKN04);
        PokedexApp_SelectSearchOrder(pokedexApp, 4);
        break;
    case DEX_ORDER_TALLEST:
        (void)MenuInputStateMgr_GetState(pokedexApp->args->menuInputStatePtr);
        PlaySE(SEQ_SE_GS_ZKN04);
        PokedexApp_SelectSearchOrder(pokedexApp, 5);
        break;
    case DEX_ORDER_SHORTEST:
        (void)MenuInputStateMgr_GetState(pokedexApp->args->menuInputStatePtr);
        PlaySE(SEQ_SE_GS_ZKN04);
        return PokedexApp_FlashSearchButton(pokedexApp, 0, 10, POKEDEXAPP_MAINSEQ_30);
    case DEX_ORDER_QUIT:
    case LIST_CANCEL:
        PlaySE(SEQ_SE_GS_GEARCANCEL);
        pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_SORT_ORDER] = pokedexApp->dexSearchCriteriaBak[0];
        return PokedexApp_FlashSearchButton(pokedexApp, 22, 10, POKEDEXAPP_MAINSEQ_30);
    }

    return POKEDEXAPP_MAINSEQ_29;
}

static int PokedexApp_MainSeq_30(PokedexAppData *pokedexApp) {
    PokedexApp_PrepareSearchPageChange(pokedexApp, 0, 0, POKEDEXAPP_MAINSEQ_22);
    return POKEDEXAPP_MAINSEQ_27;
}

static int PokedexApp_MainSeq_31(PokedexAppData *pokedexApp) {
    pokedexApp->dexSearchCriteriaBak[0] = pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_LETTER];
    PokedexApp_PrepareSearchPageChange(pokedexApp, 2, 0, POKEDEXAPP_MAINSEQ_32);
    return POKEDEXAPP_MAINSEQ_26;
}

static int PokedexApp_MainSeq_32(PokedexAppData *pokedexApp) {
    u32 r4 = PokedexApp_HandleSearchNameInput(pokedexApp);
    switch (r4) {
    case -4:
    case -3:
    case -1:
        break;
    case DEX_SEARCH_LETTERS_SUBMIT:
        (void)MenuInputStateMgr_GetState(pokedexApp->args->menuInputStatePtr);
        PlaySE(SEQ_SE_GS_ZKN04);
        return PokedexApp_FlashSearchButton(pokedexApp, 0, 10, POKEDEXAPP_MAINSEQ_33);
    case DEX_SEARCH_LETTERS_CANCEL:
    case -2:
        PlaySE(SEQ_SE_GS_GEARCANCEL);
        pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_LETTER] = pokedexApp->dexSearchCriteriaBak[0];
        return PokedexApp_FlashSearchButton(pokedexApp, 22, 10, POKEDEXAPP_MAINSEQ_33);
    default:
        (void)MenuInputStateMgr_GetState(pokedexApp->args->menuInputStatePtr);
        PlaySE(SEQ_SE_GS_ZKN04);
        PokedexApp_SelectSearchLetter(pokedexApp, r4);
        break;
    }

    return POKEDEXAPP_MAINSEQ_32;
}

static int PokedexApp_MainSeq_33(PokedexAppData *pokedexApp) {
    PokedexApp_PrepareSearchPageChange(pokedexApp, 0, 1, POKEDEXAPP_MAINSEQ_22);
    return POKEDEXAPP_MAINSEQ_27;
}

static int PokedexApp_MainSeq_34(PokedexAppData *pokedexApp) {
    pokedexApp->dexSearchCriteriaBak[0] = pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_TYPE1];
    pokedexApp->dexSearchCriteriaBak[1] = pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_TYPE2];
    PokedexApp_PrepareSearchPageChange(pokedexApp, 3, 0, POKEDEXAPP_MAINSEQ_35);
    return POKEDEXAPP_MAINSEQ_26;
}

static int PokedexApp_MainSeq_35(PokedexAppData *pokedexApp) {
    u32 r0 = PokedexApp_HandleSearchTypeInput(pokedexApp);
    switch (r0) {
    case -4:
    case -3:
    case -1:
        break;
    case DEX_SEARCH_TYPE_SUBMIT:
        if (pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_TYPE1] == pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_TYPE2]) {
            pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_TYPE2] = DEX_SEARCH_TYPE_ALL;
        }
        (void)MenuInputStateMgr_GetState(pokedexApp->args->menuInputStatePtr);
        PlaySE(SEQ_SE_GS_ZKN04);
        return PokedexApp_FlashSearchButton(pokedexApp, 0, 10, POKEDEXAPP_MAINSEQ_36);
    case DEX_SEARCH_TYPE_CANCEL:
    case -2:
        pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_TYPE1] = pokedexApp->dexSearchCriteriaBak[0];
        pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_TYPE2] = pokedexApp->dexSearchCriteriaBak[1];
        PlaySE(SEQ_SE_GS_GEARCANCEL);
        return PokedexApp_FlashSearchButton(pokedexApp, 22, 10, POKEDEXAPP_MAINSEQ_36);
    default:
        if (r0 == DEX_SEARCH_TYPE_ALL) {
            if (pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_TYPE2] != DEX_SEARCH_TYPE_ALL) {
                pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_TYPE2] = r0;
            } else {
                pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_TYPE1] = r0;
            }
        } else if (pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_TYPE1] == DEX_SEARCH_TYPE_ALL) {
            pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_TYPE1] = r0;
        } else if (pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_TYPE2] == DEX_SEARCH_TYPE_ALL) {
            if (pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_TYPE1] == r0) {
                break;
            }
            pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_TYPE2] = r0;
        } else {
            if (pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_TYPE2] == r0) {
                break;
            }
            pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_TYPE1] = pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_TYPE2];
            pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_TYPE2] = r0;
        }
        (void)MenuInputStateMgr_GetState(pokedexApp->args->menuInputStatePtr);
        PlaySE(SEQ_SE_GS_ZKN04);
        PokedexApp_HighlightSearchTypes(pokedexApp);
        PokedexApp_DrawSearchTypeValue(pokedexApp, pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_TYPE1], 45, 29);
        PokedexApp_DrawSearchTypeValue(pokedexApp, pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_TYPE2], 46, 35);
        ScheduleWindowCopyToVram(&pokedexApp->windows[45]);
        ScheduleWindowCopyToVram(&pokedexApp->windows[46]);
        break;
    }

    return POKEDEXAPP_MAINSEQ_35;
}

static int PokedexApp_MainSeq_36(PokedexAppData *pokedexApp) {
    PokedexApp_PrepareSearchPageChange(pokedexApp, 0, 2, POKEDEXAPP_MAINSEQ_22);
    return POKEDEXAPP_MAINSEQ_27;
}

static int PokedexApp_MainSeq_37(PokedexAppData *pokedexApp) {
    pokedexApp->dexSearchCriteriaBak[0] = pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_HEIGHT_MIN];
    pokedexApp->dexSearchCriteriaBak[1] = pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_HEIGHT_MAX];
    PokedexApp_PrepareSearchPageChange(pokedexApp, 4, 0, POKEDEXAPP_MAINSEQ_38);
    return POKEDEXAPP_MAINSEQ_26;
}

static int PokedexApp_MainSeq_38(PokedexAppData *pokedexApp) {
    PokedexAppData_UnkSub0868_State38 *r5 = &pokedexApp->seqState.state_38;

    switch (PokedexApp_HandleSearchHeightInput(pokedexApp)) {
    case 0:
        r5->prevValue = pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_HEIGHT_MAX];
        r5->holdFrames = 0;
        PokedexApp_SetHeightSliderX(pokedexApp, 0, 5, 1);
        return POKEDEXAPP_MAINSEQ_40;
    case 1:
        r5->prevValue = pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_HEIGHT_MIN];
        r5->holdFrames = 0;
        PokedexApp_SetHeightSliderX(pokedexApp, 0, 11, 1);
        return POKEDEXAPP_MAINSEQ_41;
    case 2:
        (void)MenuInputStateMgr_GetState(pokedexApp->args->menuInputStatePtr);
        PlaySE(SEQ_SE_GS_ZKN04);
        MI_CpuClear8(&pokedexApp->seqState, sizeof(pokedexApp->seqState));
        return PokedexApp_FlashSearchButton(pokedexApp, 0, 10, POKEDEXAPP_MAINSEQ_39);
    case 3:
    case -2:
        PlaySE(SEQ_SE_GS_GEARCANCEL);
        pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_HEIGHT_MIN] = pokedexApp->dexSearchCriteriaBak[0];
        pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_HEIGHT_MAX] = pokedexApp->dexSearchCriteriaBak[1];
        MI_CpuClear8(&pokedexApp->seqState, sizeof(pokedexApp->seqState));
        return PokedexApp_FlashSearchButton(pokedexApp, 22, 10, POKEDEXAPP_MAINSEQ_39);
    case 4:
        if (pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_HEIGHT_MAX] == 152) {
            break;
        }
        (void)MenuInputStateMgr_GetState(pokedexApp->args->menuInputStatePtr);
        PlaySE(SEQ_SE_GS_ZKN04);
        r5->arrowSprite = 2;
        r5->direction = 1;
        PokedexApp_SetSpriteAnim(pokedexApp, r5->arrowSprite, 54);
        return POKEDEXAPP_MAINSEQ_42;
    case 5:
        if (pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_HEIGHT_MAX] > pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_HEIGHT_MIN]) {
            (void)MenuInputStateMgr_GetState(pokedexApp->args->menuInputStatePtr);
            PlaySE(SEQ_SE_GS_ZKN04);
            r5->arrowSprite = 1;
            r5->direction = -1;
            PokedexApp_SetSpriteAnim(pokedexApp, r5->arrowSprite, 57);
            return POKEDEXAPP_MAINSEQ_42;
        }
        break;
    case 6:
        if (pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_HEIGHT_MIN] < pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_HEIGHT_MAX]) {
            (void)MenuInputStateMgr_GetState(pokedexApp->args->menuInputStatePtr);
            PlaySE(SEQ_SE_GS_ZKN04);
            r5->arrowSprite = 4;
            r5->direction = 1;
            PokedexApp_SetSpriteAnim(pokedexApp, r5->arrowSprite, 54);
            return POKEDEXAPP_MAINSEQ_43;
        }
        break;
    case 7:
        if (pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_HEIGHT_MIN] == 0) {
            break;
        }
        (void)MenuInputStateMgr_GetState(pokedexApp->args->menuInputStatePtr);
        PlaySE(SEQ_SE_GS_ZKN04);
        r5->arrowSprite = 3;
        r5->direction = -1;
        PokedexApp_SetSpriteAnim(pokedexApp, r5->arrowSprite, 57);
        return POKEDEXAPP_MAINSEQ_43;
    case -1:
        if ((gSystem.heldKeys & PAD_KEY_LEFT) || (gSystem.heldKeys & PAD_KEY_RIGHT) || System_GetTouchHeld()) {
            break;
        }
        r5->holdFrames = 0;
        break;
    }

    return POKEDEXAPP_MAINSEQ_38;
}

static void PokedexApp_StepSliderValue(u32 *pOut, int hmin, int hmax, u32 *pStep, int direction) {
    int tmp;
    int step;

    if (*pStep != 24) {
        ++(*pStep);
    }
    if (*pStep >= 24) {
        step = 32;
    } else if (*pStep >= 16) {
        step = 16;
    } else if (*pStep >= 8) {
        step = 8;
    } else {
        step = 1;
    }
    tmp = (*pOut) + step * direction;
    if (tmp < hmin) {
        tmp = hmin;
    } else if (tmp > hmax) {
        tmp = hmax;
    }
    *pOut = tmp;
}

static void PokedexApp_UpdateSizeSlider(PokedexAppData *pokedexApp, int a1, BOOL a2, int a3, int a4) {
    if (a2 == FALSE) {
        PokedexApp_SetHeightSliderX(pokedexApp, PokedexApp_SliderValueToX(pokedexApp, a1), a3, 1);
        PokedexApp_DrawHeightDigits(pokedexApp, a3 + 1, pokedexApp->sliderValues[a1].height);
        PokedexApp_DrawHeightValue(pokedexApp, pokedexApp->sliderValues[a1].height, a4);
        ScheduleWindowCopyToVram(&pokedexApp->windows[a4]);
    } else {
        PokedexApp_SetWeightSliderX(pokedexApp, PokedexApp_SliderValueToX(pokedexApp, a1), a3, 1);
        PokedexApp_DrawWeightDigits(pokedexApp, a3 + 1, pokedexApp->sliderValues[a1].weight);
        PokedexApp_DrawWeightValue(pokedexApp, pokedexApp->sliderValues[a1].weight, a4);
        ScheduleWindowCopyToVram(&pokedexApp->windows[a4]);
    }
}

static int PokedexApp_MainSeq_39(PokedexAppData *pokedexApp) {
    PokedexApp_PrepareSearchPageChange(pokedexApp, 0, 3, POKEDEXAPP_MAINSEQ_22);
    return POKEDEXAPP_MAINSEQ_27;
}

static int PokedexApp_MainSeq_40(PokedexAppData *pokedexApp) {
    u32 sp4;
    u32 r4;
    PokedexAppData_UnkSub0868_State38 *r4_2 = &pokedexApp->seqState.state_38;

    if (!PokedexApp_GetTouchHeldX(&sp4)) {
        if (pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_HEIGHT_MAX] < pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_HEIGHT_MIN]) {
            pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_HEIGHT_MAX] = r4_2->prevValue;
        }
        PokedexApp_UpdateSizeSlider(pokedexApp, pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_HEIGHT_MAX], FALSE, 5, 70);
        PokedexApp_UpdateSliderLimitSprites(pokedexApp, pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_HEIGHT_MAX], 1);
        return POKEDEXAPP_MAINSEQ_38;
    }

    r4 = pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_HEIGHT_MAX];
    PokedexApp_SetHeightSliderX(pokedexApp, sp4, 5, 1);
    pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_HEIGHT_MAX] = PokedexApp_GetSliderValue(pokedexApp, 5);
    PokedexApp_DrawHeightDigits(pokedexApp, 6, pokedexApp->sliderValues[pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_HEIGHT_MAX]].height);
    PokedexApp_DrawHeightValue(pokedexApp, pokedexApp->sliderValues[pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_HEIGHT_MAX]].height, 70);
    ScheduleWindowCopyToVram(&pokedexApp->windows[70]);
    PokedexApp_UpdateSliderLimitSprites(pokedexApp, pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_HEIGHT_MAX], 1);
    if (r4 != pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_HEIGHT_MAX]) {
        PlaySE(SEQ_SE_GS_ZKN03);
    }
    return POKEDEXAPP_MAINSEQ_40;
}

static int PokedexApp_MainSeq_41(PokedexAppData *pokedexApp) {
    u32 sp4;
    u32 r4;
    PokedexAppData_UnkSub0868_State38 *r4_2 = &pokedexApp->seqState.state_38;

    if (!PokedexApp_GetTouchHeldX(&sp4)) {
        if (pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_HEIGHT_MIN] > pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_HEIGHT_MAX]) {
            pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_HEIGHT_MIN] = r4_2->prevValue;
        }
        PokedexApp_UpdateSizeSlider(pokedexApp, pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_HEIGHT_MIN], FALSE, 11, 69);
        PokedexApp_UpdateSliderLimitSprites(pokedexApp, pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_HEIGHT_MIN], 3);
        return POKEDEXAPP_MAINSEQ_38;
    }

    r4 = pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_HEIGHT_MIN];
    PokedexApp_SetHeightSliderX(pokedexApp, sp4, 11, 1);
    pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_HEIGHT_MIN] = PokedexApp_GetSliderValue(pokedexApp, 11);
    PokedexApp_DrawHeightDigits(pokedexApp, 12, pokedexApp->sliderValues[pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_HEIGHT_MIN]].height);
    PokedexApp_DrawHeightValue(pokedexApp, pokedexApp->sliderValues[pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_HEIGHT_MIN]].height, 69);
    ScheduleWindowCopyToVram(&pokedexApp->windows[69]);
    PokedexApp_UpdateSliderLimitSprites(pokedexApp, pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_HEIGHT_MIN], 3);
    if (r4 != pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_HEIGHT_MIN]) {
        PlaySE(SEQ_SE_GS_ZKN03);
    }
    return POKEDEXAPP_MAINSEQ_41;
}

static int PokedexApp_MainSeq_42(PokedexAppData *pokedexApp) {
    PokedexAppData_UnkSub0868_State38 *r4 = &pokedexApp->seqState.state_38;

    if (!PokedexApp_IsSpriteAnimating(pokedexApp, r4->arrowSprite)) {
        PokedexApp_StepSliderValue(&pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_HEIGHT_MAX], pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_HEIGHT_MIN], 152, &r4->holdFrames, r4->direction);
        PokedexApp_UpdateSizeSlider(pokedexApp, pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_HEIGHT_MAX], FALSE, 5, 70);
        PokedexApp_UpdateSliderLimitSprites(pokedexApp, pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_HEIGHT_MAX], 1);
        return POKEDEXAPP_MAINSEQ_38;
    }

    return POKEDEXAPP_MAINSEQ_42;
}

static int PokedexApp_MainSeq_43(PokedexAppData *pokedexApp) {
    PokedexAppData_UnkSub0868_State38 *r4 = &pokedexApp->seqState.state_38;

    if (!PokedexApp_IsSpriteAnimating(pokedexApp, r4->arrowSprite)) {
        PokedexApp_StepSliderValue(&pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_HEIGHT_MIN], 0, pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_HEIGHT_MAX], &r4->holdFrames, r4->direction);
        PokedexApp_UpdateSizeSlider(pokedexApp, pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_HEIGHT_MIN], FALSE, 11, 69);
        PokedexApp_UpdateSliderLimitSprites(pokedexApp, pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_HEIGHT_MIN], 3);
        return POKEDEXAPP_MAINSEQ_38;
    }

    return POKEDEXAPP_MAINSEQ_43;
}

static int PokedexApp_MainSeq_44(PokedexAppData *pokedexApp) {
    pokedexApp->dexSearchCriteriaBak[0] = pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_WEIGHT_MIN];
    pokedexApp->dexSearchCriteriaBak[1] = pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_WEIGHT_MAX];
    PokedexApp_PrepareSearchPageChange(pokedexApp, 5, 0, POKEDEXAPP_MAINSEQ_45);
    return POKEDEXAPP_MAINSEQ_26;
}

static int PokedexApp_MainSeq_45(PokedexAppData *pokedexApp) {
    PokedexAppData_UnkSub0868_State38 *r5 = &pokedexApp->seqState.state_38;

    switch (PokedexApp_HandleSearchWeightInput(pokedexApp)) {
    case 0:
        r5->prevValue = pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_WEIGHT_MAX];
        r5->holdFrames = 0;
        PokedexApp_SetWeightSliderX(pokedexApp, 0, 5, 1);
        return POKEDEXAPP_MAINSEQ_47;
    case 1:
        r5->prevValue = pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_WEIGHT_MIN];
        r5->holdFrames = 0;
        PokedexApp_SetWeightSliderX(pokedexApp, 0, 11, 1);
        return POKEDEXAPP_MAINSEQ_48;
    case 2:
        (void)MenuInputStateMgr_GetState(pokedexApp->args->menuInputStatePtr);
        PlaySE(SEQ_SE_GS_ZKN04);
        MI_CpuClear8(&pokedexApp->seqState, sizeof(pokedexApp->seqState));
        return PokedexApp_FlashSearchButton(pokedexApp, 0, 10, POKEDEXAPP_MAINSEQ_46);
    case 3:
    case -2:
        PlaySE(SEQ_SE_GS_GEARCANCEL);
        pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_WEIGHT_MIN] = pokedexApp->dexSearchCriteriaBak[0];
        pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_WEIGHT_MAX] = pokedexApp->dexSearchCriteriaBak[1];
        MI_CpuClear8(&pokedexApp->seqState, sizeof(pokedexApp->seqState));
        return PokedexApp_FlashSearchButton(pokedexApp, 22, 10, POKEDEXAPP_MAINSEQ_46);
    case 4:
        if (pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_WEIGHT_MAX] == 152) {
            break;
        }
        (void)MenuInputStateMgr_GetState(pokedexApp->args->menuInputStatePtr);
        PlaySE(SEQ_SE_GS_ZKN04);
        r5->arrowSprite = 2;
        r5->direction = 1;
        PokedexApp_SetSpriteAnim(pokedexApp, r5->arrowSprite, 54);
        return POKEDEXAPP_MAINSEQ_49;
    case 5:
        if (pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_WEIGHT_MAX] > pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_WEIGHT_MIN]) {
            (void)MenuInputStateMgr_GetState(pokedexApp->args->menuInputStatePtr);
            PlaySE(SEQ_SE_GS_ZKN04);
            r5->arrowSprite = 1;
            r5->direction = -1;
            PokedexApp_SetSpriteAnim(pokedexApp, r5->arrowSprite, 57);
            return POKEDEXAPP_MAINSEQ_49;
        }
        break;
    case 6:
        if (pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_WEIGHT_MIN] < pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_WEIGHT_MAX]) {
            (void)MenuInputStateMgr_GetState(pokedexApp->args->menuInputStatePtr);
            PlaySE(SEQ_SE_GS_ZKN04);
            r5->arrowSprite = 4;
            r5->direction = 1;
            PokedexApp_SetSpriteAnim(pokedexApp, r5->arrowSprite, 54);
            return POKEDEXAPP_MAINSEQ_50;
        }
        break;
    case 7:
        if (pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_WEIGHT_MIN] == 0) {
            break;
        }
        (void)MenuInputStateMgr_GetState(pokedexApp->args->menuInputStatePtr);
        PlaySE(SEQ_SE_GS_ZKN04);
        r5->arrowSprite = 3;
        r5->direction = -1;
        PokedexApp_SetSpriteAnim(pokedexApp, r5->arrowSprite, 57);
        return POKEDEXAPP_MAINSEQ_50;
    case -1:
        if ((gSystem.heldKeys & PAD_KEY_LEFT) || (gSystem.heldKeys & PAD_KEY_RIGHT) || System_GetTouchHeld()) {
            break;
        }
        r5->holdFrames = 0;
        break;
    }

    return POKEDEXAPP_MAINSEQ_45;
}

static int PokedexApp_MainSeq_46(PokedexAppData *pokedexApp) {
    PokedexApp_PrepareSearchPageChange(pokedexApp, 0, 4, POKEDEXAPP_MAINSEQ_22);
    return POKEDEXAPP_MAINSEQ_27;
}

static int PokedexApp_MainSeq_47(PokedexAppData *pokedexApp) {
    u32 sp4;
    u32 r4;
    PokedexAppData_UnkSub0868_State38 *r4_2 = &pokedexApp->seqState.state_38;

    if (!PokedexApp_GetTouchHeldX(&sp4)) {
        if (pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_WEIGHT_MAX] < pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_WEIGHT_MIN]) {
            pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_WEIGHT_MAX] = r4_2->prevValue;
        }
        PokedexApp_UpdateSizeSlider(pokedexApp, pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_WEIGHT_MAX], TRUE, 5, 73);
        PokedexApp_UpdateSliderLimitSprites(pokedexApp, pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_WEIGHT_MAX], 1);
        return POKEDEXAPP_MAINSEQ_45;
    }

    r4 = pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_WEIGHT_MAX];
    PokedexApp_SetWeightSliderX(pokedexApp, sp4, 5, 1);
    pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_WEIGHT_MAX] = PokedexApp_GetSliderValue(pokedexApp, 5);
    PokedexApp_DrawWeightDigits(pokedexApp, 6, pokedexApp->sliderValues[pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_WEIGHT_MAX]].weight);
    PokedexApp_DrawWeightValue(pokedexApp, pokedexApp->sliderValues[pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_WEIGHT_MAX]].weight, 73);
    ScheduleWindowCopyToVram(&pokedexApp->windows[73]);
    PokedexApp_UpdateSliderLimitSprites(pokedexApp, pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_WEIGHT_MAX], 1);
    if (r4 != pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_WEIGHT_MAX]) {
        PlaySE(SEQ_SE_GS_ZKN03);
    }
    return POKEDEXAPP_MAINSEQ_47;
}

static int PokedexApp_MainSeq_48(PokedexAppData *pokedexApp) {
    u32 sp4;
    u32 r4;
    PokedexAppData_UnkSub0868_State38 *r4_2 = &pokedexApp->seqState.state_38;

    if (!PokedexApp_GetTouchHeldX(&sp4)) {
        if (pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_WEIGHT_MIN] > pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_WEIGHT_MAX]) {
            pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_WEIGHT_MIN] = r4_2->prevValue;
        }
        PokedexApp_UpdateSizeSlider(pokedexApp, pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_WEIGHT_MIN], TRUE, 11, 72);
        PokedexApp_UpdateSliderLimitSprites(pokedexApp, pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_WEIGHT_MIN], 3);
        return POKEDEXAPP_MAINSEQ_45;
    }

    r4 = pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_WEIGHT_MIN];
    PokedexApp_SetWeightSliderX(pokedexApp, sp4, 11, 1);
    pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_WEIGHT_MIN] = PokedexApp_GetSliderValue(pokedexApp, 11);
    PokedexApp_DrawWeightDigits(pokedexApp, 12, pokedexApp->sliderValues[pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_WEIGHT_MIN]].weight);
    PokedexApp_DrawWeightValue(pokedexApp, pokedexApp->sliderValues[pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_WEIGHT_MIN]].weight, 72);
    ScheduleWindowCopyToVram(&pokedexApp->windows[72]);
    PokedexApp_UpdateSliderLimitSprites(pokedexApp, pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_WEIGHT_MIN], 3);
    if (r4 != pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_WEIGHT_MIN]) {
        PlaySE(SEQ_SE_GS_ZKN03);
    }
    return POKEDEXAPP_MAINSEQ_48;
}

static int PokedexApp_MainSeq_49(PokedexAppData *pokedexApp) {
    PokedexAppData_UnkSub0868_State38 *r4 = &pokedexApp->seqState.state_38;

    if (!PokedexApp_IsSpriteAnimating(pokedexApp, r4->arrowSprite)) {
        PokedexApp_StepSliderValue(&pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_WEIGHT_MAX], pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_WEIGHT_MIN], 152, &r4->holdFrames, r4->direction);
        PokedexApp_UpdateSizeSlider(pokedexApp, pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_WEIGHT_MAX], TRUE, 5, 73);
        PokedexApp_UpdateSliderLimitSprites(pokedexApp, pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_WEIGHT_MAX], 1);
        return POKEDEXAPP_MAINSEQ_45;
    }

    return POKEDEXAPP_MAINSEQ_49;
}

static int PokedexApp_MainSeq_50(PokedexAppData *pokedexApp) {
    PokedexAppData_UnkSub0868_State38 *r4 = &pokedexApp->seqState.state_38;

    if (!PokedexApp_IsSpriteAnimating(pokedexApp, r4->arrowSprite)) {
        PokedexApp_StepSliderValue(&pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_WEIGHT_MIN], 0, pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_WEIGHT_MAX], &r4->holdFrames, r4->direction);
        PokedexApp_UpdateSizeSlider(pokedexApp, pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_WEIGHT_MIN], TRUE, 11, 72);
        PokedexApp_UpdateSliderLimitSprites(pokedexApp, pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_WEIGHT_MIN], 3);
        return POKEDEXAPP_MAINSEQ_45;
    }

    return POKEDEXAPP_MAINSEQ_50;
}

static int PokedexApp_MainSeq_51(PokedexAppData *pokedexApp) {
    pokedexApp->dexSearchCriteriaBak[0] = pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_AREA];
    PokedexApp_PrepareSearchPageChange(pokedexApp, 6, 0, POKEDEXAPP_MAINSEQ_52);
    return POKEDEXAPP_MAINSEQ_26;
}

static int PokedexApp_MainSeq_52(PokedexAppData *pokedexApp) {
    u32 r4 = PokedexApp_HandleSearchAreaInput(pokedexApp);

    switch (r4) {
    case DEX_SEARCH_AREA_SUBMIT:
        (void)MenuInputStateMgr_GetState(pokedexApp->args->menuInputStatePtr);
        PlaySE(SEQ_SE_GS_ZKN04);
        return PokedexApp_FlashSearchButton(pokedexApp, 0, 10, POKEDEXAPP_MAINSEQ_53);
    case DEX_SEARCH_AREA_CANCEL:
    case -2:
        PlaySE(SEQ_SE_GS_GEARCANCEL);
        pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_AREA] = pokedexApp->dexSearchCriteriaBak[0];
        return PokedexApp_FlashSearchButton(pokedexApp, 22, 10, POKEDEXAPP_MAINSEQ_53);
    default:
        (void)MenuInputStateMgr_GetState(pokedexApp->args->menuInputStatePtr);
        PlaySE(SEQ_SE_GS_ZKN04);
        PokedexApp_SelectSearchArea(pokedexApp, r4);
        break;
    case -3:
    case -1:
    case -4:
        break;
    }

    return POKEDEXAPP_MAINSEQ_52;
}

static int PokedexApp_MainSeq_53(PokedexAppData *pokedexApp) {
    PokedexApp_PrepareSearchPageChange(pokedexApp, 0, 5, POKEDEXAPP_MAINSEQ_22);
    return POKEDEXAPP_MAINSEQ_27;
}

static int PokedexApp_MainSeq_54(PokedexAppData *pokedexApp) {
    pokedexApp->dexSearchCriteriaBak[0] = pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_BODY_TYPE];
    PokedexApp_PrepareSearchPageChange(pokedexApp, 7, 0, POKEDEXAPP_MAINSEQ_55);
    return POKEDEXAPP_MAINSEQ_26;
}

static int PokedexApp_MainSeq_55(PokedexAppData *pokedexApp) {
    u32 r4 = PokedexApp_HandleSearchFormInput(pokedexApp);

    switch (r4) {
    case DEX_SEARCH_BODYTYPE_SUBMIT:
        (void)MenuInputStateMgr_GetState(pokedexApp->args->menuInputStatePtr);
        PlaySE(SEQ_SE_GS_ZKN04);
        return PokedexApp_FlashSearchButton(pokedexApp, 0, 10, POKEDEXAPP_MAINSEQ_56);
    case DEX_SEARCH_BODYTYPE_CANCEL:
    case -2:
        PlaySE(SEQ_SE_GS_GEARCANCEL);
        pokedexApp->dexSearchCriteria[DEX_SEARCH_CRITERIA_BODY_TYPE] = pokedexApp->dexSearchCriteriaBak[0];
        return PokedexApp_FlashSearchButton(pokedexApp, 22, 10, POKEDEXAPP_MAINSEQ_56);
    default:
        (void)MenuInputStateMgr_GetState(pokedexApp->args->menuInputStatePtr);
        PlaySE(SEQ_SE_GS_ZKN04);
        PokedexApp_SelectSearchBodyType(pokedexApp, r4);
        break;
    case -3:
    case -1:
    case -4:
        break;
    }

    return POKEDEXAPP_MAINSEQ_55;
}

static int PokedexApp_MainSeq_56(PokedexAppData *pokedexApp) {
    PokedexApp_PrepareSearchPageChange(pokedexApp, 0, 6, POKEDEXAPP_MAINSEQ_22);
    return POKEDEXAPP_MAINSEQ_27;
}

static int PokedexApp_MainSeq_57(PokedexAppData *pokedexApp) {
    switch (PokedexApp_HandleSearchResultListInput(pokedexApp, 0)) {
    case 0:
        PokedexApp_MoveSpriteToListCursor(pokedexApp, 0);
        PokedexApp_UpdateSearchResultEntry(pokedexApp);
        PokedexApp_UpdateSearchResultButtonsEnabled(pokedexApp);
        break;
    case 1:
        break;
    case 2:
        return PokedexApp_FlashBottomButton(pokedexApp, 0, 8, POKEDEXAPP_MAINSEQ_62);
    case 3:
        if (PokedexApp_GetListCursorIndex(pokedexApp) && PokedexApp_PlayCursorCry(pokedexApp) == 1) {
            return PokedexApp_FlashBottomButton(pokedexApp, 8, 8, POKEDEXAPP_MAINSEQ_57);
        }
        break;
    case 4:
        if (PokedexApp_GetListCursorIndex(pokedexApp) && PokedexApp_GetCursorSpecies(pokedexApp)) {
            PlaySE(SEQ_SE_GS_ZKN04);
            pokedexApp->entryTab = 1;
            return PokedexApp_FlashBottomButton(pokedexApp, 16, 8, POKEDEXAPP_MAINSEQ_64);
        }
        break;
    case 5:
        return PokedexApp_FlashBottomButton(pokedexApp, 24, 8, POKEDEXAPP_MAINSEQ_63);
    case 6:
        return PokedexApp_FlashBottomButton(pokedexApp, 24, 8, POKEDEXAPP_MAINSEQ_63);
    case 7:
        PokedexApp_CycleSearchEntryLanguage(pokedexApp, PokedexApp_GetCursorSpecies(pokedexApp), -1);
        break;
    case 8:
        PokedexApp_CycleSearchEntryLanguage(pokedexApp, PokedexApp_GetCursorSpecies(pokedexApp), 1);
        break;
    case 9:
        if (pokedexApp->listPage > 0) {
            PlaySE(SEQ_SE_GS_ZKN04);
            MI_CpuClear8(&pokedexApp->seqState, sizeof(pokedexApp->seqState));
            return POKEDEXAPP_MAINSEQ_58;
        }
        break;
    case 10:
        if (pokedexApp->listPage > 0) {
            PlaySE(SEQ_SE_GS_ZKN04);
            MI_CpuClear8(&pokedexApp->seqState, sizeof(pokedexApp->seqState));
            pokedexApp->seqState.state_13.wrapCursor = 1;
            return POKEDEXAPP_MAINSEQ_58;
        }
        break;
    case 11:
        if (pokedexApp->listPage + 1 <= PokedexApp_GetLastListPage(pokedexApp, 0)) {
            PlaySE(SEQ_SE_GS_ZKN04);
            MI_CpuClear8(&pokedexApp->seqState, sizeof(pokedexApp->seqState));
            return POKEDEXAPP_MAINSEQ_59;
        }
        break;
    case 12:
        if (pokedexApp->listPage + 1 <= PokedexApp_GetLastListPage(pokedexApp, 0)) {
            PlaySE(SEQ_SE_GS_ZKN04);
            MI_CpuClear8(&pokedexApp->seqState, sizeof(pokedexApp->seqState));
            pokedexApp->seqState.state_13.wrapCursor = 1;
            return POKEDEXAPP_MAINSEQ_59;
        }
        break;
    case 13: {
        u32 x, y;

        System_GetTouchHeldCoords(&x, &y);
        if (PokedexApp_IsTouchOnListScrollbar(pokedexApp, 18, x, y) == 1) {
            return POKEDEXAPP_MAINSEQ_60;
        } else {
            PokedexAppData_UnkSub0868_State16 *r1 = &pokedexApp->seqState.state_16;

            r1->unk_0 = x;
            r1->unk_2 = y;
            return POKEDEXAPP_MAINSEQ_61;
        }
    } break;
    }

    return POKEDEXAPP_MAINSEQ_57;
}

static int PokedexApp_MainSeq_58(PokedexAppData *pokedexApp) {
    PokedexAppData_UnkSub0868_State13 *r4 = &pokedexApp->seqState.state_13;

    switch (r4->state) {
    case 0:
        PokedexApp_SetSpriteVisible(pokedexApp, 0, 0);
        PokedexApp_SetSpriteAnim(pokedexApp, 19, 6);
        PokedexApp_SetSpritePairVisibleIfInternationalView(pokedexApp, 22, 0);
        PokedexApp_SetSpriteVisibleIfInternationalView(pokedexApp, 24, 0);
        ClearWindowTilemapAndScheduleTransfer(&pokedexApp->windows[94]);
        r4->numberTilemap = PokedexApp_BuildSearchResultNumberTilemap(pokedexApp, 15 * (pokedexApp->listPage - 1));
        r4->scrollbarStep = PokedexApp_GetListScrollbarStepCount(pokedexApp, 18, PokedexApp_GetListScrollbarYForPage(pokedexApp, pokedexApp->listPage - 1, 18, 0), 15);
        if (pokedexApp->listPage == 1) {
            ov18_021F3C30(pokedexApp);
        }
        ++r4->state;
        // fallthrough
    case 1:
        ++r4->scrollStep;
        PokedexApp_LoadNumberTilemap(pokedexApp, &r4->numberTilemap[0x1E0 - 32 * r4->scrollStep]);
        PokedexApp_ScrollListIconsBackward(pokedexApp, 59, 8, r4->scrollStep / 5);
        PokedexApp_MoveListScrollbar(pokedexApp, 18, -r4->scrollbarStep);
        if (pokedexApp->listPage == 1) {
            ov18_021F3C54(pokedexApp, 8);
        }
        if (r4->scrollStep == 15) {
            ++r4->state;
        }
        break;
    case 2:
        if (r4->wrapCursor == 1) {
            pokedexApp->listCursor += 10;
        }
        --pokedexApp->listPage;
        PokedexApp_MoveSpriteToListCursor(pokedexApp, 0);
        PokedexApp_FreeNumberTilemap(r4->numberTilemap);
        PokedexApp_UpdateSearchResultEntry(pokedexApp);
        PokedexApp_UpdateSearchResultButtonsEnabled(pokedexApp);
        PokedexApp_UpdateListPageArrows(pokedexApp, 19, 0);
        PokedexApp_SyncListScrollbarToPage(pokedexApp, 18, 0);
        MI_CpuClear8(&pokedexApp->seqState, sizeof(pokedexApp->seqState));
        return POKEDEXAPP_MAINSEQ_57;
    }

    return POKEDEXAPP_MAINSEQ_58;
}

static int PokedexApp_MainSeq_59(PokedexAppData *pokedexApp) {
    PokedexAppData_UnkSub0868_State13 *r4 = &pokedexApp->seqState.state_13;

    switch (r4->state) {
    case 0:
        PokedexApp_SetSpriteVisible(pokedexApp, 0, 0);
        PokedexApp_SetSpriteAnim(pokedexApp, 20, 9);
        PokedexApp_SetSpritePairVisibleIfInternationalView(pokedexApp, 22, 0);
        PokedexApp_SetSpriteVisibleIfInternationalView(pokedexApp, 24, 0);
        ClearWindowTilemapAndScheduleTransfer(&pokedexApp->windows[94]);
        r4->numberTilemap = PokedexApp_BuildSearchResultNumberTilemap(pokedexApp, 15 * (pokedexApp->listPage));
        r4->scrollbarStep = PokedexApp_GetListScrollbarStepCount(pokedexApp, 18, PokedexApp_GetListScrollbarYForPage(pokedexApp, pokedexApp->listPage + 1, 18, 0), 15);
        ++r4->state;
        // fallthrough
    case 1:
        ++r4->scrollStep;
        PokedexApp_LoadNumberTilemap(pokedexApp, &r4->numberTilemap[32 * r4->scrollStep]);
        PokedexApp_ScrollListIconsForward(pokedexApp, 59, -8, r4->scrollStep / 5);
        PokedexApp_MoveListScrollbar(pokedexApp, 18, r4->scrollbarStep);
        if (pokedexApp->listPage == 0) {
            ov18_021F3C54(pokedexApp, -8);
        }
        if (r4->scrollStep == 15) {
            ++r4->state;
        }
        break;
    case 2:
        if (r4->wrapCursor == 1) {
            pokedexApp->listCursor -= 10;
        }
        ++pokedexApp->listPage;
        PokedexApp_MoveSpriteToListCursor(pokedexApp, 0);
        PokedexApp_FreeNumberTilemap(r4->numberTilemap);
        PokedexApp_UpdateSearchResultEntry(pokedexApp);
        PokedexApp_UpdateSearchResultButtonsEnabled(pokedexApp);
        PokedexApp_UpdateListPageArrows(pokedexApp, 19, 0);
        PokedexApp_SyncListScrollbarToPage(pokedexApp, 18, 0);
        MI_CpuClear8(&pokedexApp->seqState, sizeof(pokedexApp->seqState));
        return POKEDEXAPP_MAINSEQ_57;
    }

    return POKEDEXAPP_MAINSEQ_59;
}

static int PokedexApp_MainSeq_60(PokedexAppData *pokedexApp) {
    u32 x, y;

    if (!System_GetTouchHeldCoords(&x, &y)) {
        return POKEDEXAPP_MAINSEQ_57;
    }
    if (PokedexApp_DragListScrollbar(pokedexApp, 18, x, y, 0) == 1) {
        u16 *r4 = PokedexApp_BuildSearchResultNumberTilemap(pokedexApp, 15 * pokedexApp->listPage);
        pokedexApp->iconPage ^= 1;
        PokedexApp_LoadNumberTilemap(pokedexApp, r4);
        PokedexApp_FreeNumberTilemap(r4);
        PokedexApp_DrawListIconGrid(pokedexApp, 59);
        if (!pokedexApp->listPage) {
            ov18_021F3C88(pokedexApp);
        } else {
            PokedexApp_SetSpriteVisible(pokedexApp, 25, 0);
        }
        PokedexApp_UpdateSearchResultEntry(pokedexApp);
        PokedexApp_UpdateSearchResultButtonsEnabled(pokedexApp);
        PokedexApp_UpdateListPageArrows(pokedexApp, 19, 0);
        PlaySE(SEQ_SE_GS_ZKN03);
    }
    return POKEDEXAPP_MAINSEQ_60;
}

static int PokedexApp_MainSeq_61(PokedexAppData *pokedexApp) {
    PokedexAppData_UnkSub0868_State16 *r3 = &pokedexApp->seqState.state_16;

    if (PokedexApp_DragListScrollbar(pokedexApp, 18, r3->unk_0, r3->unk_2, 0) == 1) {
        u16 *r4 = PokedexApp_BuildSearchResultNumberTilemap(pokedexApp, 15 * pokedexApp->listPage);
        pokedexApp->iconPage ^= 1;
        PokedexApp_LoadNumberTilemap(pokedexApp, r4);
        PokedexApp_FreeNumberTilemap(r4);
        PokedexApp_DrawListIconGrid(pokedexApp, 59);
        if (!pokedexApp->listPage) {
            ov18_021F3C88(pokedexApp);
        } else {
            PokedexApp_SetSpriteVisible(pokedexApp, 25, 0);
        }
        PokedexApp_UpdateSearchResultEntry(pokedexApp);
        PokedexApp_UpdateSearchResultButtonsEnabled(pokedexApp);
        PokedexApp_UpdateListPageArrows(pokedexApp, 19, 1);
        PlaySE(SEQ_SE_GS_ZKN03);
        MI_CpuClear8(&pokedexApp->seqState, sizeof(pokedexApp->seqState));
    }
    return POKEDEXAPP_MAINSEQ_57;
}

PokedexApp_MainSeq_62(PokedexAppData *pokedexApp) {
    switch (pokedexApp->seqState.state_10.state) {
    case 0: {
        u16 r1 = PokedexApp_GetCursorSpecies(pokedexApp);
        if (r1 != 0) {
            UnkStruct_02092BB8_Set(pokedexApp->args->unk_08, r1, pokedexApp->dexMode);
        }
        PokedexApp_RunSearch(pokedexApp, pokedexApp->dexMode, DEX_ORDER_NATIONAL, DEX_SEARCH_LETTERS_ALL, DEX_SEARCH_TYPE_ALL, DEX_SEARCH_TYPE_ALL, 0, 152, 0, 152, 1 << DEX_SEARCH_AREA_ALL, DEX_SEARCH_BODYTYPE_ALL);
        PokedexApp_BuildListEntries(pokedexApp, 1);
        PokedexApp_RestoreListCursor(pokedexApp);
        PokedexApp_UpdateEntryButtonsEnabled(pokedexApp);
        BgCellManager_EraseCell(pokedexApp->bgCellManager, 4);
        PokedexApp_SetSpritePairVisibleIfInternationalView(pokedexApp, 22, 0);
        PokedexApp_SetSpriteVisibleIfInternationalView(pokedexApp, 24, 0);
        ClearWindowTilemapAndScheduleTransfer(&pokedexApp->windows[94]);
        PaletteData_BeginPaletteFade(pokedexApp->paletteData, PLTTBUF_MAIN_BG_F, PLTTFADE_OPAQUE_ALL_F & ~(1 << 0), 0, 0, 16, RGB_BLACK);
        PaletteData_BeginPaletteFade(pokedexApp->paletteData, PLTTBUF_EXCEPT_MAIN_BG_F, PLTTFADE_OPAQUE_ALL_F, 0, 0, 16, RGB_BLACK);
        pokedexApp->nextSeq = POKEDEXAPP_MAINSEQ_62;
        PokedexApp_SlideOutSearchResultButtons(pokedexApp);
        ++pokedexApp->seqState.state_10.state;
        return POKEDEXAPP_MAINSEQ_03;
    }
    case 1:
        if (!BgCellManager_IsCellMoving(pokedexApp->bgCellManager, 8)) {
            ZeroPalettesByBitmask(PLTTBUF_MAIN_BG_F | PLTTBUF_SUB_BG_F | PLTTBUF_MAIN_OBJ_F | PLTTBUF_SUB_OBJ_F, HEAP_ID_POKEDEX_APP);
            PokedexApp_RemoveAllWindows(pokedexApp);
            PokedexApp_UnloadSearchEntryResources(pokedexApp);
            PokedexApp_FreeSearchSprites(pokedexApp);
            pokedexApp->seqState.state_10.state = 0;
            return POKEDEXAPP_MAINSEQ_21;
        }
        break;
    }

    return POKEDEXAPP_MAINSEQ_62;
}

static int PokedexApp_MainSeq_63(PokedexAppData *pokedexApp) {
    switch (pokedexApp->seqState.state_10.state) {
    case 0: {
        u16 species = PokedexApp_GetCursorSpecies(pokedexApp);
        if (species != SPECIES_NONE) {
            UnkStruct_02092BB8_Set(pokedexApp->args->unk_08, species, pokedexApp->dexMode);
        }
        PokedexApp_RunSearch(pokedexApp, pokedexApp->dexMode, DEX_ORDER_NATIONAL, DEX_SEARCH_LETTERS_ALL, DEX_SEARCH_TYPE_ALL, DEX_SEARCH_TYPE_ALL, 0, 152, 0, 152, 1 << DEX_SEARCH_AREA_ALL, DEX_SEARCH_BODYTYPE_ALL);
        PokedexApp_BuildListEntries(pokedexApp, 1);
        PokedexApp_RestoreListCursor(pokedexApp);
        GfGfxLoader_LoadCharDataFromOpenNarc(pokedexApp->gfxNarc, NARC_zukan_gra_zukan_gra_00000001_NCGR_lz, pokedexApp->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, TRUE, HEAP_ID_POKEDEX_APP);
        GfGfxLoader_LoadCharDataFromOpenNarc(pokedexApp->gfxNarc, NARC_zukan_gra_zukan_gra_00000017_NCGR_lz, pokedexApp->bgConfig, GF_BG_LYR_SUB_0, 0, 0, TRUE, HEAP_ID_POKEDEX_APP);
        PokedexApp_BuildListReturnCells(pokedexApp);
        BgCellManager_DrawCellAt(pokedexApp->bgCellManager, 13, -36, 0);
        BgCellManager_DrawCellAt(pokedexApp->bgCellManager, 15, -36, 0);
        BgCellManager_DrawCellAt(pokedexApp->bgCellManager, 14, -4, 0);
        BgCellManager_StartCellMove(pokedexApp->bgCellManager, 13, 2, 0, 18);
        BgCellManager_StartCellMove(pokedexApp->bgCellManager, 15, 2, 0, 18);
        BgCellManager_StartCellMove(pokedexApp->bgCellManager, 14, 2, 0, 18);
        BgCellManager_EraseCell(pokedexApp->bgCellManager, 4);
        PokedexApp_SetSpritePairVisibleIfInternationalView(pokedexApp, 22, 0);
        PokedexApp_SetSpriteVisibleIfInternationalView(pokedexApp, 24, 0);
        ClearWindowTilemapAndScheduleTransfer(&pokedexApp->windows[94]);
        PokedexApp_SetSpriteVisible(pokedexApp, 0, 0);
        PokedexApp_ShowReturningDexModeSwitch(pokedexApp);
        PokedexApp_SlideOutSearchResultButtons(pokedexApp);
        ++pokedexApp->seqState.state_10.state;
    } // fallthrough
    case 1:
        if (!BgCellManager_IsCellMoving(pokedexApp->bgCellManager, 13)) {
            ++pokedexApp->seqState.state_10.state;
        } else {
            PokedexApp_MoveReturningDexModeSwitch(pokedexApp, 16);
        }
        break;
    case 2:
        pokedexApp->seqState.state_10.state = 0;
        PokedexApp_RemoveAllWindows(pokedexApp);
        PokedexApp_UnloadSearchEntryResources(pokedexApp);
        PokedexApp_FreeSearchSprites(pokedexApp);
        PokedexApp_SetupScreen(pokedexApp, 0);
        BgCellManager_EraseCell(pokedexApp->bgCellManager, 0);
        pokedexApp->seqState.state_09.state = 1;
        return POKEDEXAPP_MAINSEQ_09;
    }

    return POKEDEXAPP_MAINSEQ_63;
}

static int PokedexApp_MainSeq_64(PokedexAppData *pokedexApp) {
    switch (pokedexApp->seqState.state_10.state) {
    case 0: {
        u16 r1 = PokedexApp_GetCursorSpecies(pokedexApp);
        if (r1 != SPECIES_NONE) {
            UnkStruct_02092BB8_Set(pokedexApp->args->unk_08, r1, pokedexApp->dexMode);
        }
        BgCellManager_EraseCell(pokedexApp->bgCellManager, 4);
        PokedexApp_SetSpritePairVisibleIfInternationalView(pokedexApp, 22, 0);
        PokedexApp_SetSpriteVisibleIfInternationalView(pokedexApp, 24, 0);
        ClearWindowTilemapAndScheduleTransfer(&pokedexApp->windows[94]);
        PokedexApp_SetSpriteVisible(pokedexApp, 0, 0);
        PokedexApp_SlideOutSearchResultButtons(pokedexApp);
        PaletteData_BeginPaletteFade(pokedexApp->paletteData, PLTTBUF_MAIN_BG_F, PLTTFADE_OPAQUE_ALL_F & ~(1 << 0), 0, 0, 16, RGB_BLACK);
        PaletteData_BeginPaletteFade(pokedexApp->paletteData, PLTTBUF_EXCEPT_MAIN_BG_F, PLTTFADE_OPAQUE_ALL_F, 0, 0, 16, RGB_BLACK);
        pokedexApp->nextSeq = 0x40;
        ++pokedexApp->seqState.state_10.state;
        return POKEDEXAPP_MAINSEQ_03;
    }
    case 1:
        if (BgCellManager_IsCellMoving(pokedexApp->bgCellManager, 8)) {
            break;
        }
        pokedexApp->seqState.state_10.state = 0;
        PokedexApp_RemoveAllWindows(pokedexApp);
        PokedexApp_UnloadSearchEntryResources(pokedexApp);
        PokedexApp_FreeSearchSprites(pokedexApp);
        pokedexApp->entryFromSearch = 5;
        return POKEDEXAPP_MAINSEQ_66;
    }

    return POKEDEXAPP_MAINSEQ_64;
}

static int PokedexApp_MainSeq_65(PokedexAppData *pokedexApp) {
    switch (pokedexApp->seqState.state_10.state) {
    case 0:
        PokedexApp_SetupScreen(pokedexApp, 5);
        PokedexApp_SetCellTilemapFromBg(pokedexApp, GF_BG_LYR_MAIN_0, 9);
        PokedexApp_SetCellTilemapFromBg(pokedexApp, GF_BG_LYR_MAIN_2, 10);
        PokedexApp_SetCellTilemapFromBg(pokedexApp, GF_BG_LYR_MAIN_0, 11);
        PokedexApp_SetCellTilemapFromBg(pokedexApp, GF_BG_LYR_MAIN_2, 12);
        PokedexApp_DrawSearchCriteriaSummary(pokedexApp);
        PokedexApp_DrawListIconGrid(pokedexApp, 59);
        PokedexApp_DrawSearchResultNumbers(pokedexApp);
        PokedexApp_SetSpriteVisible(pokedexApp, 18, 1);
        PokedexApp_SetSpriteVisible(pokedexApp, 19, 1);
        PokedexApp_SetSpriteVisible(pokedexApp, 20, 1);
        PokedexApp_SetSpriteVisible(pokedexApp, 21, 1);
        PokedexApp_SetSpriteVisible(pokedexApp, 25, 1);
        PokedexApp_UpdateListScrollbarSize(pokedexApp, 18, 0);
        PokedexApp_SyncListScrollbarToPage(pokedexApp, 18, 0);
        PokedexApp_UpdateListPageArrows(pokedexApp, 19, 0);
        PokedexApp_DrawSearchMessage(pokedexApp, 1);
        PokedexApp_UpdateSearchBodyTypeIcon(pokedexApp);
        PokedexApp_MoveSearchSummaryIcons(pokedexApp, -192);
        BgCellManager_DrawCellAt(pokedexApp->bgCellManager, 9, 0, 0);
        BgCellManager_DrawCellAt(pokedexApp->bgCellManager, 10, 0, 0);
        BgCellManager_DrawCellAt(pokedexApp->bgCellManager, 11, 0, 0);
        BgCellManager_DrawCellAt(pokedexApp->bgCellManager, 12, 0, 0);
        BgCellManager_EraseCell(pokedexApp->bgCellManager, 9);
        BgCellManager_EraseCell(pokedexApp->bgCellManager, 10);
        GfGfxLoader_LoadCharDataFromOpenNarc(pokedexApp->gfxNarc, NARC_zukan_gra_zukan_gra_00000004_NCGR_lz, pokedexApp->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, TRUE, HEAP_ID_POKEDEX_APP);
        GfGfxLoader_LoadCharDataFromOpenNarc(pokedexApp->gfxNarc, NARC_zukan_gra_zukan_gra_00000019_NCGR_lz, pokedexApp->bgConfig, GF_BG_LYR_SUB_3, 0, 0, TRUE, HEAP_ID_POKEDEX_APP);
        GfGfxLoader_LoadScrnDataFromOpenNarc(pokedexApp->gfxNarc, NARC_zukan_gra_zukan_gra_00000020_NSCR_lz, pokedexApp->bgConfig, GF_BG_LYR_SUB_3, 0, 0, TRUE, HEAP_ID_POKEDEX_APP);
        PokedexApp_SlideInSearchResultButtons(pokedexApp);
        ov18_021F3448(pokedexApp);
        PokedexApp_SetSpriteVisible(pokedexApp, 0, 1);
        PokedexApp_MoveSpriteToListCursor(pokedexApp, 0);
        PokedexApp_SetSpriteAnim(pokedexApp, 0, 1);
        PokedexApp_CreateSearchEntrySprites(pokedexApp);
        PokedexApp_UpdateSearchResultEntry(pokedexApp);
        PokedexApp_UpdateSearchResultButtonsEnabled(pokedexApp);
        PokedexApp_DrawEntryNameBar(pokedexApp);
        if (pokedexApp->listPage) {
            ov18_021F3C54(pokedexApp, -40);
        }
        PaletteData_BeginPaletteFade(pokedexApp->paletteData, PLTTBUF_MAIN_BG_F, PLTTFADE_OPAQUE_ALL_F & ~(1 << 4), 0, 16, 0, RGB_BLACK);
        PaletteData_BeginPaletteFade(pokedexApp->paletteData, PLTTBUF_EXCEPT_MAIN_BG_F, PLTTFADE_OPAQUE_ALL_F, 0, 16, 0, RGB_BLACK);
        ++pokedexApp->seqState.state_10.state;
        pokedexApp->nextSeq = POKEDEXAPP_MAINSEQ_65;
        return POKEDEXAPP_MAINSEQ_03;
    case 1:
        if (!BgCellManager_IsCellMoving(pokedexApp->bgCellManager, 8)) {
            pokedexApp->seqState.state_10.state = 0;
            return POKEDEXAPP_MAINSEQ_57;
        }
        break;
    }

    return POKEDEXAPP_MAINSEQ_65;
}

static int PokedexApp_SwitchEntryTab(PokedexAppData *pokedexApp, u8 a1, u8 a2) {
    PokedexAppData_UnkSub0868_State69 *r2 = &pokedexApp->seqState.state_69;
    u8 r6 = 0;

    r2->state = 0;
    r2->fadeStart = a1;

    if (a2 == 0xFF) {
        PokedexApp_MoveEntryTabCursor(pokedexApp, 3);
        return PokedexApp_FlashEntryTab(pokedexApp, 24, POKEDEXAPP_MAINSEQ_67);
    }

    if (a2 == 1) {
        r6 = 0;
        PokedexApp_MoveEntryTabCursor(pokedexApp, 0);
    } else if (a2 == 2) {
        r6 = 8;
        PokedexApp_MoveEntryTabCursor(pokedexApp, 1);
    } else if (a2 == 3) {
        r6 = 16;
        PokedexApp_MoveEntryTabCursor(pokedexApp, 2);
    } else {
        r6 = 24;
        PokedexApp_MoveEntryTabCursor(pokedexApp, 3);
    }
    if (pokedexApp->entryTab == 1) {
        pokedexApp->entryTab = a2;
        return PokedexApp_FlashEntryTab(pokedexApp, r6, POKEDEXAPP_MAINSEQ_70);
    } else if (pokedexApp->entryTab == 2) {
        pokedexApp->entryTab = a2;
        return PokedexApp_FlashEntryTab(pokedexApp, r6, POKEDEXAPP_MAINSEQ_78);
    } else if (pokedexApp->entryTab == 3) {
        pokedexApp->entryTab = a2;
        return PokedexApp_FlashEntryTab(pokedexApp, r6, POKEDEXAPP_MAINSEQ_81);
    } else {
        pokedexApp->entryTab = a2;
        return PokedexApp_FlashEntryTab(pokedexApp, r6, POKEDEXAPP_MAINSEQ_91);
    }
}

static int PokedexApp_MainSeq_66(PokedexAppData *pokedexApp) {
    pokedexApp->curSpecies = PokedexApp_GetCursorSpecies(pokedexApp);
    pokedexApp->areaTimeOfDay = 0;
    pokedexApp->areaListPos = 0;
    pokedexApp->areaRegion = 0;
    PokedexApp_CollectSeenForms(pokedexApp);
    PokedexApp_DimEntryTabsIfNotCaught(pokedexApp);
    PokedexApp_SlideInEntryTabs(pokedexApp);
    PokedexApp_CreateEntryTabCursor(pokedexApp);
    return PokedexApp_MainSeq_68(pokedexApp);
}

static int PokedexApp_MainSeq_67(PokedexAppData *pokedexApp) {
    PokedexAppData_UnkSub0868_State69 *r4 = &pokedexApp->seqState.state_69;

    switch (r4->state) {
    case 0:
        PokedexApp_SetSpriteVisible(pokedexApp, 0, 0);
        PokedexApp_SlideOutEntryTabs(pokedexApp);
        PaletteData_BeginPaletteFade(pokedexApp->paletteData, PLTTBUF_MAIN_BG_F, 0xF7BE, 0, r4->fadeStart, 16, RGB_BLACK);
        PaletteData_BeginPaletteFade(pokedexApp->paletteData, PLTTBUF_EXCEPT_MAIN_BG_F, 0xFFFF, 0, r4->fadeStart, 16, RGB_BLACK);
        ++r4->state;
        pokedexApp->nextSeq = POKEDEXAPP_MAINSEQ_67;
        return POKEDEXAPP_MAINSEQ_03;
    case 1:
        if (BgCellManager_IsCellMoving(pokedexApp->bgCellManager, 16)) {
            break;
        }
        ZeroPalettesByBitmask(PLTTBUF_MAIN_BG_F | PLTTBUF_SUB_BG_F | PLTTBUF_MAIN_OBJ_F | PLTTBUF_SUB_OBJ_F, HEAP_ID_POKEDEX_APP);
        if (pokedexApp->entryTab == 3) {
            PokedexApp_FreeFormsWindows(pokedexApp);
            PokedexApp_FreeFormsSprites(pokedexApp);
        } else if (pokedexApp->entryTab == 2) {
            PokedexApp_FreeSizeCheckWindows(pokedexApp);
            PokedexApp_FreeSizeCheckSprites(pokedexApp);
            PokedexApp_SetSubBg3Mode(pokedexApp, 0);
        } else if (pokedexApp->entryTab == 1) {
            PokedexApp_FreeAreaWindows(pokedexApp);
            PokedexApp_FreeAreaSprites(pokedexApp);
            PokedexApp_FreeAreaList(pokedexApp);
            PokedexApp_FreeAreaTables(pokedexApp);
            PokedexApp_ResetAreaMapScroll(pokedexApp);
        } else {
            PokedexApp_FreeEntryMenuWindows(pokedexApp);
        }
        PokedexApp_FreeEntryTabCursor(pokedexApp);
        MI_CpuClear8(&pokedexApp->seqState, sizeof(pokedexApp->seqState));
        pokedexApp->entryTab = pokedexApp->entryFromSearch;
        if (pokedexApp->entryFromSearch == 0) {
            return POKEDEXAPP_MAINSEQ_19;
        } else {
            return POKEDEXAPP_MAINSEQ_65;
        }
    }

    return POKEDEXAPP_MAINSEQ_67;
}

static int PokedexApp_MainSeq_68(PokedexAppData *pokedexApp) {
    switch (pokedexApp->seqState.state_10.state) {
    case 0:
        pokedexApp->areaTimeOfDay = 0;
        pokedexApp->areaRegion = 0;
        pokedexApp->areaListPos = 0;
        PokedexApp_LoadAreaTables(pokedexApp);
        PokedexApp_BuildAreaList(pokedexApp, pokedexApp->areaTimeOfDay, pokedexApp->areaRegion);
        ov18_021F018C(pokedexApp);
        PokedexApp_SetupScreen(pokedexApp, 1);
        PokedexApp_DrawAreaRegionTabs(pokedexApp);
        PokedexApp_ScrollAreaMapToJohto(pokedexApp);
        PokedexApp_UpdateAreaMarkers(pokedexApp);
        PokedexApp_ResetAreaMarkerBlink(pokedexApp);
        if (pokedexApp->areaList.areas.nMaps == 1) {
            PokedexApp_ShowAreaUnknownBanner(pokedexApp);
        }
        PaletteData_BeginPaletteFade(pokedexApp->paletteData, PLTTBUF_MAIN_BG_F, 0xF7BE, -2, 16, 0, RGB_BLACK);
        PaletteData_BeginPaletteFade(pokedexApp->paletteData, PLTTBUF_MAIN_OBJ_F, 0xFFEF, -2, 16, 0, RGB_BLACK);
        PaletteData_BeginPaletteFade(pokedexApp->paletteData, PLTTBUF_SUB_BG_F | PLTTBUF_SUB_OBJ_F, PLTTFADE_OPAQUE_ALL_F, -2, 16, 0, RGB_BLACK);
        ++pokedexApp->seqState.state_10.state;
        pokedexApp->nextSeq = POKEDEXAPP_MAINSEQ_68;
        return POKEDEXAPP_MAINSEQ_03;
    case 1:
        if (BgCellManager_IsCellMoving(pokedexApp->bgCellManager, 16)) {
            break;
        }
        PokedexApp_SetSpriteVisible(pokedexApp, 0, 1);
        pokedexApp->entryTab = 1;
        pokedexApp->seqState.state_10.state = 0;
        return POKEDEXAPP_MAINSEQ_69;
    }

    return POKEDEXAPP_MAINSEQ_68;
}

static int PokedexApp_MainSeq_69(PokedexAppData *pokedexApp) {
    u32 sp8;

    PokedexApp_UpdateAreaMarkerBlink(pokedexApp);
    switch (PokedexApp_HandleAreaInput(pokedexApp, &sp8)) {
    case 0:
        if (sp8 == 1) {
            PlaySE(SEQ_SE_GS_ZKN04);
        } else {
            PlaySE(SEQ_SE_GS_ZKN03);
        }
        return PokedexApp_FlashEntryTab(pokedexApp, 0, POKEDEXAPP_MAINSEQ_69);
    case 1:
        if (sp8 == 1) {
            PlaySE(SEQ_SE_GS_ZKN04);
        } else {
            PlaySE(SEQ_SE_GS_ZKN03);
        }
        return PokedexApp_SwitchEntryTab(pokedexApp, 0, 2);
    case 2:
        if (sp8 == 1) {
            PlaySE(SEQ_SE_GS_ZKN04);
        } else {
            PlaySE(SEQ_SE_GS_ZKN03);
        }
        return PokedexApp_SwitchEntryTab(pokedexApp, 0, 3);
    case 3:
        PlaySE(SEQ_SE_GS_GEARCANCEL);
        return PokedexApp_SwitchEntryTab(pokedexApp, 0, 255);
    case 4:
        if (PokedexApp_CanScrollAreaList(pokedexApp, -2, 2) == TRUE) {
            if (sp8 == 1) {
                PlaySE(SEQ_SE_GS_ZKN04);
            } else {
                PlaySE(SEQ_SE_GS_ZKN03);
            }
            return POKEDEXAPP_MAINSEQ_71;
        } else {
            return POKEDEXAPP_MAINSEQ_69;
        }
    case 5:
    case 8:
        if (PokedexApp_CanScrollAreaList(pokedexApp, -1, 1) == TRUE) {
            if (sp8 == 1) {
                PlaySE(SEQ_SE_GS_ZKN04);
            } else {
                PlaySE(SEQ_SE_GS_ZKN03);
            }
            return POKEDEXAPP_MAINSEQ_71;
        } else {
            return POKEDEXAPP_MAINSEQ_69;
        }
    case 6:
        if (PokedexApp_CanScrollAreaList(pokedexApp, 2, 2) == TRUE) {
            if (sp8 == 1) {
                PlaySE(SEQ_SE_GS_ZKN04);
            } else {
                PlaySE(SEQ_SE_GS_ZKN03);
            }
            return POKEDEXAPP_MAINSEQ_72;
        } else {
            return POKEDEXAPP_MAINSEQ_69;
        }
    case 7:
    case 9:
        if (PokedexApp_CanScrollAreaList(pokedexApp, 1, 1) == TRUE) {
            if (sp8 == 1) {
                PlaySE(SEQ_SE_GS_ZKN04);
            } else {
                PlaySE(SEQ_SE_GS_ZKN03);
            }
            return POKEDEXAPP_MAINSEQ_72;
        } else {
            return POKEDEXAPP_MAINSEQ_69;
        }
    case 10: {
        u32 x, y;

        System_GetTouchHeldCoords(&x, &y);
        if (PokedexApp_IsTouchOnAreaScrollbar(pokedexApp, x, y) == 1) {
            return POKEDEXAPP_MAINSEQ_73;
        } else {
            PokedexAppData_UnkSub0868_State16 *r1 = &pokedexApp->seqState.state_16;

            r1->unk_0 = x;
            r1->unk_2 = y;
            return POKEDEXAPP_MAINSEQ_74;
        }
    } break;
    case 11:
        if (PokedexApp_SetAreaRegion(pokedexApp, 0) == TRUE) {
            if (sp8 == 1) {
                PlaySE(SEQ_SE_GS_ZKN04);
            } else {
                PlaySE(SEQ_SE_GS_ZKN03);
            }
            return POKEDEXAPP_MAINSEQ_75;
        }
        break;
    case 12:
        if (PokedexApp_SetAreaRegion(pokedexApp, 1) == TRUE) {
            if (sp8 == 1) {
                PlaySE(SEQ_SE_GS_ZKN04);
            } else {
                PlaySE(SEQ_SE_GS_ZKN03);
            }
            return POKEDEXAPP_MAINSEQ_75;
        }
        break;
    case 13:
        if (PokedexApp_SetAreaTimeOfDay(pokedexApp, 0) == TRUE) {
            if (sp8 == 1) {
                PlaySE(SEQ_SE_GS_ZKN04);
            } else {
                PlaySE(SEQ_SE_GS_ZKN03);
            }
            PokedexApp_UpdateTimeOfDayCursor(pokedexApp);
            return POKEDEXAPP_MAINSEQ_75;
        }
        break;
    case 14:
        if (PokedexApp_SetAreaTimeOfDay(pokedexApp, 1) == TRUE) {
            if (sp8 == 1) {
                PlaySE(SEQ_SE_GS_ZKN04);
            } else {
                PlaySE(SEQ_SE_GS_ZKN03);
            }
            PokedexApp_UpdateTimeOfDayCursor(pokedexApp);
            return POKEDEXAPP_MAINSEQ_75;
        }
        break;
    case 15:
        if (PokedexApp_SetAreaTimeOfDay(pokedexApp, 2) == TRUE) {
            if (sp8 == 1) {
                PlaySE(SEQ_SE_GS_ZKN04);
            } else {
                PlaySE(SEQ_SE_GS_ZKN03);
            }
            PokedexApp_UpdateTimeOfDayCursor(pokedexApp);
            return POKEDEXAPP_MAINSEQ_75;
        }
        break;
    case 16:
        if (sp8 == 1) {
            PlaySE(SEQ_SE_GS_ZKN04);
        } else {
            PlaySE(SEQ_SE_GS_ZKN03);
        }
        return PokedexApp_SwitchEntryTab(pokedexApp, 0, 4);
    }

    return POKEDEXAPP_MAINSEQ_69;
}

static int PokedexApp_MainSeq_70(PokedexAppData *pokedexApp) {
    PokedexAppData_UnkSub0868_State69 *r4 = &pokedexApp->seqState.state_69;

    switch (r4->state) {
    case 0:
        PaletteData_BeginPaletteFade(pokedexApp->paletteData, PLTTBUF_MAIN_BG_F, 0xF7BE, -2, r4->fadeStart, 16, RGB_BLACK);
        PaletteData_BeginPaletteFade(pokedexApp->paletteData, PLTTBUF_MAIN_OBJ_F, 0xFFEF, -2, r4->fadeStart, 16, RGB_BLACK);
        PaletteData_BeginPaletteFade(pokedexApp->paletteData, PLTTBUF_SUB_BG_F | PLTTBUF_SUB_OBJ_F, PLTTFADE_OPAQUE_ALL_F, -2, r4->fadeStart, 16, RGB_BLACK);
        ++r4->state;
        pokedexApp->nextSeq = POKEDEXAPP_MAINSEQ_70;
        return POKEDEXAPP_MAINSEQ_03;
    case 1:
        PokedexApp_FreeAreaWindows(pokedexApp);
        PokedexApp_FreeAreaSprites(pokedexApp);
        PokedexApp_FreeAreaList(pokedexApp);
        PokedexApp_FreeAreaTables(pokedexApp);
        PokedexApp_ResetAreaMapScroll(pokedexApp);
        MI_CpuClear8(&pokedexApp->seqState, sizeof(pokedexApp->seqState));
        if (pokedexApp->entryTab == 2) {
            return POKEDEXAPP_MAINSEQ_76;
        } else if (pokedexApp->entryTab == 3) {
            return POKEDEXAPP_MAINSEQ_79;
        } else {
            return POKEDEXAPP_MAINSEQ_89;
        }
    }

    return POKEDEXAPP_MAINSEQ_70;
}

static int PokedexApp_MainSeq_71(PokedexAppData *pokedexApp) {
    PokedexAppData_UnkSub0868_State26 *r6 = &pokedexApp->seqState.state_26;

    switch (r6->unk_0) {
    case 0:
        --pokedexApp->areaListPos;
        PokedexApp_SetSpriteAnim(pokedexApp, 6, 6);
        PokedexApp_ScrollAreaList(pokedexApp, 1);
        for (u32 i = 0; i < 6; ++i) {
            BgCellManager_StartCellMove(pokedexApp->bgCellManager, i + 17, 0, 1, 3);
        }
        r6->unk_3 = PokedexApp_GetScrollbarStepCount(pokedexApp, 5, PokedexApp_GetAreaScrollbarY(pokedexApp, pokedexApp->areaListPos), 3);
        ++r6->unk_0;
        // fallthrough
    case 1:
        PokedexApp_MoveScrollbar(pokedexApp, 5, -r6->unk_3);
        ++r6->unk_1;
        if (r6->unk_1 == 3) {
            r6->unk_0 = 0;
            r6->unk_1 = 0;
            --r6->unk_2;
            if (r6->unk_2 == 0) {
                PokedexApp_SyncAreaScrollbar(pokedexApp);
                PokedexApp_UpdateScrollArrows(pokedexApp, pokedexApp->areaListPos, pokedexApp->areaList.areas.nMaps, 6);
                PokedexApp_UpdateAreaMarkers(pokedexApp);
                PokedexApp_UpdateAreaMarkerPalettes(pokedexApp);
                MI_CpuClear8(&pokedexApp->seqState, sizeof(pokedexApp->seqState));
                return POKEDEXAPP_MAINSEQ_69;
            }
        }
        break;
    }

    return POKEDEXAPP_MAINSEQ_71;
}

static int PokedexApp_MainSeq_72(PokedexAppData *pokedexApp) {
    PokedexAppData_UnkSub0868_State26 *r6 = &pokedexApp->seqState.state_26;

    switch (r6->unk_0) {
    case 0:
        ++pokedexApp->areaListPos;
        PokedexApp_SetSpriteAnim(pokedexApp, 7, 9);
        PokedexApp_ScrollAreaList(pokedexApp, -1);
        for (u32 i = 0; i < 6; ++i) {
            BgCellManager_StartCellMove(pokedexApp->bgCellManager, i + 17, 0, -1, 3);
        }
        r6->unk_3 = PokedexApp_GetScrollbarStepCount(pokedexApp, 5, PokedexApp_GetAreaScrollbarY(pokedexApp, pokedexApp->areaListPos), 3);
        ++r6->unk_0;
        // fallthrough
    case 1:
        PokedexApp_MoveScrollbar(pokedexApp, 5, r6->unk_3);
        ++r6->unk_1;
        if (r6->unk_1 == 3) {
            r6->unk_0 = 0;
            r6->unk_1 = 0;
            --r6->unk_2;
            if (r6->unk_2 == 0) {
                PokedexApp_SyncAreaScrollbar(pokedexApp);
                PokedexApp_UpdateScrollArrows(pokedexApp, pokedexApp->areaListPos, pokedexApp->areaList.areas.nMaps, 6);
                PokedexApp_UpdateAreaMarkers(pokedexApp);
                PokedexApp_UpdateAreaMarkerPalettes(pokedexApp);
                MI_CpuClear8(&pokedexApp->seqState, sizeof(pokedexApp->seqState));
                return POKEDEXAPP_MAINSEQ_69;
            }
        }
        break;
    }

    return POKEDEXAPP_MAINSEQ_72;
}

static int PokedexApp_MainSeq_73(PokedexAppData *pokedexApp) {
    u32 x, y;

    PokedexApp_UpdateAreaMarkerBlink(pokedexApp);
    if (!System_GetTouchHeldCoords(&x, &y)) {
        return POKEDEXAPP_MAINSEQ_69;
    }
    if (PokedexApp_DragAreaScrollbar(pokedexApp, x, y) == TRUE) {
        PokedexApp_UpdateScrollArrows(pokedexApp, pokedexApp->areaListPos, pokedexApp->areaList.areas.nMaps, 6);
        PokedexApp_UpdateAreaMarkers(pokedexApp);
        PokedexApp_UpdateAreaMarkerPalettes(pokedexApp);
        PokedexApp_DrawAreaList(pokedexApp);
        PlaySE(SEQ_SE_GS_ZKN03);
    }
    return POKEDEXAPP_MAINSEQ_73;
}

static int PokedexApp_MainSeq_74(PokedexAppData *pokedexApp) {
    PokedexAppData_UnkSub0868_State16 *r2 = &pokedexApp->seqState.state_16;

    if (PokedexApp_DragAreaScrollbar(pokedexApp, r2->unk_0, r2->unk_2) == TRUE) {
        PokedexApp_UpdateScrollArrows(pokedexApp, pokedexApp->areaListPos, pokedexApp->areaList.areas.nMaps, 6);
        PokedexApp_UpdateAreaMarkers(pokedexApp);
        PokedexApp_UpdateAreaMarkerPalettes(pokedexApp);
        PokedexApp_DrawAreaList(pokedexApp);
        PlaySE(SEQ_SE_GS_ZKN03);
        MI_CpuClear8(&pokedexApp->seqState, sizeof(pokedexApp->seqState));
    }
    return POKEDEXAPP_MAINSEQ_69;
}

static int PokedexApp_MainSeq_75(PokedexAppData *pokedexApp) {
    PokedexAppData_UnkSub0868_State16 *r4 = &pokedexApp->seqState.state_16;
    BOOL r0 = PokedexApp_ScrollAreaMap(pokedexApp);

    switch (r4->unk_0) {
    case 0:
        PokedexApp_ShowAreaUnknownBanner(pokedexApp);
        PokedexApp_HideAreaUnknownBanner(pokedexApp);
        PokedexApp_SetSpriteVisible(pokedexApp, 2, 0);
        PokedexApp_HideAreaMarkers(pokedexApp);
        ++r4->unk_0;
        // fallthrough
    case 1:
        r4->unk_2 += 2;
        PokedexApp_SetBg2Bg3Blend(31 - r4->unk_2, r4->unk_2);
        if (r4->unk_2 == 16) {
            ++r4->unk_0;
        }
        break;
    case 2:
        PokedexApp_FreeAreaList(pokedexApp);
        PokedexApp_BuildAreaList(pokedexApp, pokedexApp->areaTimeOfDay, pokedexApp->areaRegion);
        PokedexApp_SetupAreaListScrollbar(pokedexApp);
        PokedexApp_DrawTimeOfDayLabel(pokedexApp);
        PokedexApp_DrawAreaList(pokedexApp);
        PokedexApp_DrawAreaRegionTabs(pokedexApp);
        ++r4->unk_0;
        break;
    case 3:
        r4->unk_2 -= 2;
        PokedexApp_SetBg2Bg3Blend(31 - r4->unk_2, r4->unk_2);
        if (r4->unk_2 == 0) {
            PokedexApp_InitBlend();
            ++r4->unk_0;
        }
        break;
    case 4:
        if (r0) {
            break;
        }
        PokedexApp_UpdatePlayerMarkerVisibility(pokedexApp);
        PokedexApp_UpdateAreaMarkers(pokedexApp);
        PokedexApp_ResetAreaMarkerBlink(pokedexApp);
        if (pokedexApp->areaList.areas.nMaps == 1) {
            PokedexApp_ShowAreaUnknownBanner(pokedexApp);
        }
        MI_CpuClear8(&pokedexApp->seqState, sizeof(pokedexApp->seqState));
        return POKEDEXAPP_MAINSEQ_69;
    }

    return POKEDEXAPP_MAINSEQ_75;
}

static int PokedexApp_MainSeq_76(PokedexAppData *pokedexApp) {
    switch (pokedexApp->seqState.state_10.state) {
    case 0:
        PokedexApp_SetSubBg3Mode(pokedexApp, TRUE);
        PokedexApp_SetupScreen(pokedexApp, 2);
        PaletteData_BeginPaletteFade(pokedexApp->paletteData, PLTTBUF_MAIN_BG_F, 0xF7BE, -2, 16, 0, RGB_BLACK);
        PaletteData_BeginPaletteFade(pokedexApp->paletteData, PLTTBUF_MAIN_OBJ_F, 0xFFEF, -2, 16, 0, RGB_BLACK);
        PaletteData_BeginPaletteFade(pokedexApp->paletteData, PLTTBUF_SUB_BG_F | PLTTBUF_SUB_OBJ_F, 0xFFFF, -2, 16, 0, RGB_BLACK);
        ++pokedexApp->seqState.state_10.state;
        pokedexApp->nextSeq = POKEDEXAPP_MAINSEQ_76;
        return POKEDEXAPP_MAINSEQ_03;
    case 1:
        pokedexApp->entryTab = 2;
        pokedexApp->seqState.state_10.state = 0;
        return POKEDEXAPP_MAINSEQ_77;
    }

    return POKEDEXAPP_MAINSEQ_76;
}

static int PokedexApp_MainSeq_77(PokedexAppData *pokedexApp) {
    u32 sp0;

    PokedexApp_RunWeightCheck(pokedexApp);
    switch (PokedexApp_HandleSizeCheckInput(pokedexApp, &sp0)) {
    case 0:
        if (sp0 == 1) {
            PlaySE(SEQ_SE_GS_ZKN04);
        } else {
            PlaySE(SEQ_SE_GS_ZKN03);
        }
        PokedexApp_EndWeightCheck(pokedexApp);
        return PokedexApp_SwitchEntryTab(pokedexApp, 0, 1);
    case 1:
        if (sp0 == 1) {
            PlaySE(SEQ_SE_GS_ZKN04);
        } else {
            PlaySE(SEQ_SE_GS_ZKN03);
        }
        PokedexApp_EndWeightCheck(pokedexApp);
        return PokedexApp_SwitchEntryTab(pokedexApp, 0, 2);
    case 2:
        if (sp0 == 1) {
            PlaySE(SEQ_SE_GS_ZKN04);
        } else {
            PlaySE(SEQ_SE_GS_ZKN03);
        }
        PokedexApp_EndWeightCheck(pokedexApp);
        return PokedexApp_SwitchEntryTab(pokedexApp, 0, 3);
    case 3:
        PlaySE(SEQ_SE_GS_GEARCANCEL);
        PokedexApp_EndWeightCheck(pokedexApp);
        return PokedexApp_SwitchEntryTab(pokedexApp, 0, 255);
    default:
        if (gSystem.newKeys & PAD_BUTTON_A) {
            PlaySE(SEQ_SE_GS_ZKN03);
            PokedexApp_EndWeightCheck(pokedexApp);
            return PokedexApp_SwitchEntryTab(pokedexApp, 0, 2);
        }
        if (System_GetTouchNew() == TRUE) {
            PlaySE(SEQ_SE_GS_ZKN04);
            PokedexApp_EndWeightCheck(pokedexApp);
            return PokedexApp_SwitchEntryTab(pokedexApp, 0, 2);
        }
        break;
    }

    return POKEDEXAPP_MAINSEQ_77;
}

static int PokedexApp_MainSeq_78(PokedexAppData *pokedexApp) {
    PokedexAppData_UnkSub0868_State09 *r4 = &pokedexApp->seqState.state_09;

    switch (r4->state) {
    case 0:
        PaletteData_BeginPaletteFade(pokedexApp->paletteData, PLTTBUF_MAIN_BG_F, 0xF7BE, -2, r4->timer, 16, RGB_BLACK);
        PaletteData_BeginPaletteFade(pokedexApp->paletteData, PLTTBUF_MAIN_OBJ_F, 0xFFEF, -2, r4->timer, 16, RGB_BLACK);
        PaletteData_BeginPaletteFade(pokedexApp->paletteData, PLTTBUF_SUB_BG_F | PLTTBUF_SUB_OBJ_F, 0xFFFF, -2, r4->timer, 16, RGB_BLACK);
        ++r4->state;
        pokedexApp->nextSeq = POKEDEXAPP_MAINSEQ_78;
        return POKEDEXAPP_MAINSEQ_03;
    case 1:
        PokedexApp_FreeSizeCheckWindows(pokedexApp);
        PokedexApp_FreeSizeCheckSprites(pokedexApp);
        PokedexApp_SetSubBg3Mode(pokedexApp, 0);
        MI_CpuClear8(&pokedexApp->seqState, sizeof(pokedexApp->seqState));
        if (pokedexApp->entryTab == 1) {
            return POKEDEXAPP_MAINSEQ_68;
        } else if (pokedexApp->entryTab == 2) {
            return POKEDEXAPP_MAINSEQ_76;
        } else {
            return POKEDEXAPP_MAINSEQ_79;
        }
    }

    return POKEDEXAPP_MAINSEQ_78;
}

static int PokedexApp_MainSeq_79(PokedexAppData *pokedexApp) {
    switch (pokedexApp->seqState.state_10.state) {
    case 0:
        pokedexApp->selectedForm = 0;
        ov18_021F01D4(pokedexApp);
        PokedexApp_SetupScreen(pokedexApp, 3);
        PaletteData_BeginPaletteFade(pokedexApp->paletteData, PLTTBUF_MAIN_BG_F, 0xF7BE, -2, 16, 0, RGB_BLACK);
        PaletteData_BeginPaletteFade(pokedexApp->paletteData, PLTTBUF_MAIN_OBJ_F, 0xFFEF, -2, 16, 0, RGB_BLACK);
        PaletteData_BeginPaletteFade(pokedexApp->paletteData, PLTTBUF_SUB_BG_F | PLTTBUF_SUB_OBJ_F, 0xFFFF, -2, 16, 0, RGB_BLACK);
        ++pokedexApp->seqState.state_10.state;
        pokedexApp->nextSeq = POKEDEXAPP_MAINSEQ_79;
        return POKEDEXAPP_MAINSEQ_03;
    case 1:
        pokedexApp->entryTab = 3;
        pokedexApp->seqState.state_10.state = 0;
        return POKEDEXAPP_MAINSEQ_80;
    }

    return POKEDEXAPP_MAINSEQ_79;
}

static int PokedexApp_MainSeq_80(PokedexAppData *pokedexApp) {
    u32 sp14;

    switch (PokedexApp_HandleFormsInput(pokedexApp, &sp14)) {
    case 0:
        if (sp14 == 1) {
            PlaySE(SEQ_SE_GS_ZKN04);
        } else {
            PlaySE(SEQ_SE_GS_ZKN03);
        }
        return PokedexApp_SwitchEntryTab(pokedexApp, 0, 1);
    case 1:
        if (sp14 == 1) {
            PlaySE(SEQ_SE_GS_ZKN04);
        } else {
            PlaySE(SEQ_SE_GS_ZKN03);
        }
        return PokedexApp_SwitchEntryTab(pokedexApp, 0, 2);
    case 2:
        if (sp14 == 1) {
            PlaySE(SEQ_SE_GS_ZKN04);
        } else {
            PlaySE(SEQ_SE_GS_ZKN03);
        }
        return PokedexApp_FlashEntryTab(pokedexApp, 16, 80);
    case 3:
        PlaySE(SEQ_SE_GS_GEARCANCEL);
        return PokedexApp_SwitchEntryTab(pokedexApp, 0, 255);
    case 4:
        if (PokedexApp_CanScrollFormList(pokedexApp, -2, 2) == TRUE) {
            if (sp14 == 1) {
                PlaySE(SEQ_SE_GS_ZKN04);
            } else {
                PlaySE(SEQ_SE_GS_ZKN03);
            }
            return POKEDEXAPP_MAINSEQ_82;
        } else {
            return POKEDEXAPP_MAINSEQ_80;
        }
    case 5:
    case 8:
        if (PokedexApp_CanScrollFormList(pokedexApp, -1, 1) == TRUE) {
            if (sp14 == 1) {
                PlaySE(SEQ_SE_GS_ZKN04);
            } else {
                PlaySE(SEQ_SE_GS_ZKN03);
            }
            return POKEDEXAPP_MAINSEQ_82;
        } else {
            return POKEDEXAPP_MAINSEQ_80;
        }
    case 6:
        if (PokedexApp_CanScrollFormList(pokedexApp, 2, 2) == TRUE) {
            if (sp14 == 1) {
                PlaySE(SEQ_SE_GS_ZKN04);
            } else {
                PlaySE(SEQ_SE_GS_ZKN03);
            }
            return POKEDEXAPP_MAINSEQ_83;
        } else {
            return POKEDEXAPP_MAINSEQ_80;
        }
    case 7:
    case 9:
        if (PokedexApp_CanScrollFormList(pokedexApp, 1, 1) == TRUE) {
            if (sp14 == 1) {
                PlaySE(SEQ_SE_GS_ZKN04);
            } else {
                PlaySE(SEQ_SE_GS_ZKN03);
            }
            return POKEDEXAPP_MAINSEQ_83;
        } else {
            return POKEDEXAPP_MAINSEQ_80;
        }
    case 10: {
        u32 x, y;
        System_GetTouchHeldCoords(&x, &y);
        if (PokedexApp_IsTouchOnFormScrollbar(pokedexApp, x, y) == 1) {
            return POKEDEXAPP_MAINSEQ_84;
        } else {
            PokedexAppData_UnkSub0868_State16 *r1 = &pokedexApp->seqState.state_16;

            r1->unk_0 = x;
            r1->unk_2 = y;
            return POKEDEXAPP_MAINSEQ_85;
        }
    } break;
    case 11:
        if (pokedexApp->numSeenForms != 1) {
            PlaySE(SEQ_SE_GS_ZKN04);
            return ov18_021EDC50(pokedexApp, 0, 17, 17, 15, 2, POKEDEXAPP_MAINSEQ_86);
        }
        break;
    case 12:
        if (sp14 == 1) {
            PlaySE(SEQ_SE_GS_ZKN04);
        } else {
            PlaySE(SEQ_SE_GS_ZKN03);
        }
        return PokedexApp_SwitchEntryTab(pokedexApp, 0, 4);
    case -1:
        break;
    }

    return POKEDEXAPP_MAINSEQ_80;
}

static int PokedexApp_MainSeq_81(PokedexAppData *pokedexApp) {
    PokedexAppData_UnkSub0868_State69 *r4 = &pokedexApp->seqState.state_69;

    switch (r4->state) {
    case 0:
        PaletteData_BeginPaletteFade(pokedexApp->paletteData, PLTTBUF_MAIN_BG_F, 0xF7BE, -2, r4->fadeStart, 16, RGB_BLACK);
        PaletteData_BeginPaletteFade(pokedexApp->paletteData, PLTTBUF_MAIN_OBJ_F, 0xFFEF, -2, r4->fadeStart, 16, RGB_BLACK);
        PaletteData_BeginPaletteFade(pokedexApp->paletteData, PLTTBUF_SUB_BG_F | PLTTBUF_SUB_OBJ_F, 0xFFFF, -2, r4->fadeStart, 16, RGB_BLACK);
        ++r4->state;
        pokedexApp->nextSeq = POKEDEXAPP_MAINSEQ_81;
        return POKEDEXAPP_MAINSEQ_03;
    case 1:
        PokedexApp_FreeFormsWindows(pokedexApp);
        PokedexApp_FreeFormsSprites(pokedexApp);
        r4->state = 0;
        if (pokedexApp->entryTab == 1) {
            return POKEDEXAPP_MAINSEQ_68;
        } else if (pokedexApp->entryTab == 2) {
            return POKEDEXAPP_MAINSEQ_76;
        } else {
            return POKEDEXAPP_MAINSEQ_89;
        }
    }

    return POKEDEXAPP_MAINSEQ_81;
}

static int PokedexApp_MainSeq_82(PokedexAppData *pokedexApp) {
    PokedexAppData_UnkSub0868_State26 *r6 = &pokedexApp->seqState.state_26;

    switch (r6->unk_0) {
    case 0:
        --pokedexApp->selectedForm;
        PokedexApp_ScrollFormListIcons(pokedexApp, 1);
        PokedexApp_SetSpriteAnim(pokedexApp, 6, 6);
        PokedexApp_ScrollFormList(pokedexApp, 1);
        for (u32 i = 0; i < 6; ++i) {
            BgCellManager_StartCellMove(pokedexApp->bgCellManager, i + 17, 0, 1, 3);
        }
        r6->unk_3 = PokedexApp_GetScrollbarStepCount(pokedexApp, 5, PokedexApp_GetFormScrollbarY(pokedexApp, pokedexApp->selectedForm), 3);
        ++r6->unk_0;
        // fallthrough
    case 1:
        PokedexApp_MoveFormListIcons(pokedexApp, 8);
        PokedexApp_MoveScrollbar(pokedexApp, 5, -r6->unk_3);
        ++r6->unk_1;
        if (r6->unk_1 == 3) {
            r6->unk_0 = 0;
            r6->unk_1 = 0;
            --r6->unk_2;
            if (r6->unk_2 == 0) {
                ov18_021F5EF0(pokedexApp, pokedexApp->selectedForm);
                PokedexApp_SyncFormScrollbar(pokedexApp);
                PokedexApp_UpdateScrollArrows(pokedexApp, pokedexApp->selectedForm, pokedexApp->numSeenForms, 6);
                PokedexApp_DrawSelectedFormName(pokedexApp);
                MI_CpuClear8(&pokedexApp->seqState, sizeof(pokedexApp->seqState));
                return POKEDEXAPP_MAINSEQ_80;
            }
        }
        break;
    }

    return POKEDEXAPP_MAINSEQ_82;
}

static int PokedexApp_MainSeq_83(PokedexAppData *pokedexApp) {
    PokedexAppData_UnkSub0868_State26 *r6 = &pokedexApp->seqState.state_26;

    switch (r6->unk_0) {
    case 0:
        ++pokedexApp->selectedForm;
        PokedexApp_ScrollFormListIcons(pokedexApp, -1);
        PokedexApp_SetSpriteAnim(pokedexApp, 7, 9);
        PokedexApp_ScrollFormList(pokedexApp, -1);
        for (u32 i = 0; i < 6; ++i) {
            BgCellManager_StartCellMove(pokedexApp->bgCellManager, i + 17, 0, -1, 3);
        }
        r6->unk_3 = PokedexApp_GetScrollbarStepCount(pokedexApp, 5, PokedexApp_GetFormScrollbarY(pokedexApp, pokedexApp->selectedForm), 3);
        ++r6->unk_0;
        // fallthrough
    case 1:
        PokedexApp_MoveFormListIcons(pokedexApp, -8);
        PokedexApp_MoveScrollbar(pokedexApp, 5, r6->unk_3);
        ++r6->unk_1;
        if (r6->unk_1 == 3) {
            r6->unk_0 = 0;
            r6->unk_1 = 0;
            --r6->unk_2;
            if (r6->unk_2 == 0) {
                ov18_021F5EF0(pokedexApp, pokedexApp->selectedForm);
                PokedexApp_SyncFormScrollbar(pokedexApp);
                PokedexApp_UpdateScrollArrows(pokedexApp, pokedexApp->selectedForm, pokedexApp->numSeenForms, 6);
                PokedexApp_DrawSelectedFormName(pokedexApp);
                MI_CpuClear8(&pokedexApp->seqState, sizeof(pokedexApp->seqState));
                return POKEDEXAPP_MAINSEQ_80;
            }
        }
        break;
    }

    return POKEDEXAPP_MAINSEQ_83;
}

static int PokedexApp_MainSeq_84(PokedexAppData *pokedexApp) {
    u32 x, y;

    if (!System_GetTouchHeldCoords(&x, &y)) {
        return POKEDEXAPP_MAINSEQ_80;
    }

    if (PokedexApp_DragFormScrollbar(pokedexApp, x, y) == 1) {
        PokedexApp_DrawFormListIcons(pokedexApp);
        ov18_021F5EF0(pokedexApp, pokedexApp->selectedForm);
        PokedexApp_UpdateScrollArrows(pokedexApp, pokedexApp->selectedForm, pokedexApp->numSeenForms, 6);
        PokedexApp_DrawFormList(pokedexApp);
        PokedexApp_DrawSelectedFormName(pokedexApp);
        PlaySE(SEQ_SE_GS_ZKN03);
    }

    return POKEDEXAPP_MAINSEQ_84;
}

static int PokedexApp_MainSeq_85(PokedexAppData *pokedexApp) {
    PokedexAppData_UnkSub0868_State16 *r2 = &pokedexApp->seqState.state_16;

    if (PokedexApp_DragFormScrollbar(pokedexApp, r2->unk_0, r2->unk_2) == 1) {
        PokedexApp_DrawFormListIcons(pokedexApp);
        ov18_021F5EF0(pokedexApp, pokedexApp->selectedForm);
        PokedexApp_UpdateScrollArrows(pokedexApp, pokedexApp->selectedForm, pokedexApp->numSeenForms, 6);
        PokedexApp_DrawFormList(pokedexApp);
        PokedexApp_DrawSelectedFormName(pokedexApp);
        PlaySE(SEQ_SE_GS_ZKN03);
        MI_CpuClear8(&pokedexApp->seqState, sizeof(pokedexApp->seqState));
    }

    return POKEDEXAPP_MAINSEQ_80;
}

static int PokedexApp_MainSeq_86(PokedexAppData *pokedexApp) {
    switch (pokedexApp->seqState.state_10.state) {
    case 0:
        PokedexApp_SetSpriteVisible(pokedexApp, 0, 0);
        PokedexApp_SlideOutEntryTabs(pokedexApp);
        PaletteData_BeginPaletteFade(pokedexApp->paletteData, PLTTBUF_MAIN_BG_F, 0xF7BE, 0, 0, 16, RGB_BLACK);
        PaletteData_BeginPaletteFade(pokedexApp->paletteData, PLTTBUF_MAIN_OBJ_F, 0xFFEF, 0, 0, 16, RGB_BLACK);
        PaletteData_BeginPaletteFade(pokedexApp->paletteData, PLTTBUF_SUB_BG_F | PLTTBUF_SUB_OBJ_F, 0xFFFF, 0, 0, 16, RGB_BLACK);
        ++pokedexApp->seqState.state_10.state;
        pokedexApp->nextSeq = POKEDEXAPP_MAINSEQ_86;
        return POKEDEXAPP_MAINSEQ_03;
    case 1:
        if (BgCellManager_IsCellMoving(pokedexApp->bgCellManager, 16)) {
            break;
        }
        pokedexApp->compareSide = 0;
        pokedexApp->showBackPic = 0;
        pokedexApp->compareForm = 0;
        if (pokedexApp->selectedForm == pokedexApp->compareForm) {
            PokedexApp_StepIndexSkipping(&pokedexApp->compareForm, pokedexApp->selectedForm, 1, pokedexApp->numSeenForms);
        }
        PokedexApp_LoadFormCompareGraphics(pokedexApp);
        PokedexApp_DrawFormCompareWindows(pokedexApp);
        PokedexApp_HideFormListSprites(pokedexApp);
        PokedexApp_ShowFormCompareSprites(pokedexApp);
        PaletteData_BeginPaletteFade(pokedexApp->paletteData, PLTTBUF_MAIN_BG_F, 0xF7BE, 0, 16, 0, RGB_BLACK);
        PaletteData_BeginPaletteFade(pokedexApp->paletteData, PLTTBUF_MAIN_OBJ_F, 0xFFEF, 0, 16, 0, RGB_BLACK);
        PaletteData_BeginPaletteFade(pokedexApp->paletteData, PLTTBUF_SUB_BG_F | PLTTBUF_SUB_OBJ_F, 0xFFFF, 0, 16, 0, RGB_BLACK);
        pokedexApp->seqState.state_10.state = 0;
        pokedexApp->nextSeq = POKEDEXAPP_MAINSEQ_87;
        return POKEDEXAPP_MAINSEQ_03;
    }

    return POKEDEXAPP_MAINSEQ_86;
}

static int PokedexApp_MainSeq_87(PokedexAppData *pokedexApp) {
    u32 spC;

    switch (PokedexApp_HandleFormCompareInput(pokedexApp, &spC)) {
    case 0:
        if (PokedexApp_CycleLeftCompareForm(pokedexApp, -1) == 1) {
            if (spC == 1) {
                PlaySE(SEQ_SE_GS_ZKN04);
            } else {
                PlaySE(SEQ_SE_GS_ZKN03);
            }
        }
        break;
    case 1:
        if (PokedexApp_CycleLeftCompareForm(pokedexApp, 1) == 1) {
            if (spC == 1) {
                PlaySE(SEQ_SE_GS_ZKN04);
            } else {
                PlaySE(SEQ_SE_GS_ZKN03);
            }
        }
        break;
    case 2:
        if (PokedexApp_CycleRightCompareForm(pokedexApp, -1) == 1) {
            if (spC == 1) {
                PlaySE(SEQ_SE_GS_ZKN04);
            } else {
                PlaySE(SEQ_SE_GS_ZKN03);
            }
        }
        break;
    case 3:
        if (PokedexApp_CycleRightCompareForm(pokedexApp, 1) == 1) {
            if (spC == 1) {
                PlaySE(SEQ_SE_GS_ZKN04);
            } else {
                PlaySE(SEQ_SE_GS_ZKN03);
            }
        }
        break;
    case 4:
        PlaySE(SEQ_SE_GS_ZKN04);
        pokedexApp->showBackPic ^= 1;
        ov18_021F6984(pokedexApp, 1, pokedexApp->selectedForm);
        ov18_021F6984(pokedexApp, 2, pokedexApp->compareForm);
        return ov18_021EDC50(pokedexApp, 3, 1, 20, 14, 3, POKEDEXAPP_MAINSEQ_87);
    case 5:
        PlaySE(SEQ_SE_GS_GEARCANCEL);
        return ov18_021EDC50(pokedexApp, 3, 16, 20, 15, 3, POKEDEXAPP_MAINSEQ_88);
    case 6:
        if (pokedexApp->numSeenForms < 3 || pokedexApp->compareSide == 0) {
            break;
        }
        if (spC == 1) {
            PlaySE(SEQ_SE_GS_ZKN04);
        } else {
            PlaySE(SEQ_SE_GS_ZKN03);
        }
        pokedexApp->compareSide = 0;
        PokedexApp_UpdateFormCompareSelector(pokedexApp);
        break;
    case 7:
        if (pokedexApp->numSeenForms < 3 || pokedexApp->compareSide == 1) {
            break;
        }
        if (spC == 1) {
            PlaySE(SEQ_SE_GS_ZKN04);
        } else {
            PlaySE(SEQ_SE_GS_ZKN03);
        }
        pokedexApp->compareSide = 1;
        PokedexApp_UpdateFormCompareSelector(pokedexApp);
        break;
    }

    return POKEDEXAPP_MAINSEQ_87;
}

static int PokedexApp_MainSeq_88(PokedexAppData *pokedexApp) {
    switch (pokedexApp->seqState.state_10.state) {
    case 0:
        PaletteData_BeginPaletteFade(pokedexApp->paletteData, PLTTBUF_MAIN_BG_F, 0xF7BE, 0, 0, 16, RGB_BLACK);
        PaletteData_BeginPaletteFade(pokedexApp->paletteData, PLTTBUF_MAIN_OBJ_F, 0xFFEF, 0, 0, 16, RGB_BLACK);
        PaletteData_BeginPaletteFade(pokedexApp->paletteData, PLTTBUF_SUB_BG_F | PLTTBUF_SUB_OBJ_F, 0xFFFF, 0, 0, 16, RGB_BLACK);
        ++pokedexApp->seqState.state_10.state;
        pokedexApp->nextSeq = POKEDEXAPP_MAINSEQ_88;
        return POKEDEXAPP_MAINSEQ_03;
    case 1:
        PokedexApp_HideFormCompareSprites(pokedexApp);
        PokedexApp_ClearFormCompareWindows(pokedexApp);
        PokedexApp_LoadFormsGraphics(pokedexApp);
        PokedexApp_ShowFormListSprites(pokedexApp);
        PokedexApp_DrawFormsWindows(pokedexApp);
        PokedexApp_SlideInEntryTabs(pokedexApp);
        PaletteData_BeginPaletteFade(pokedexApp->paletteData, PLTTBUF_MAIN_BG_F, 0xF7BE, 0, 16, 0, RGB_BLACK);
        PaletteData_BeginPaletteFade(pokedexApp->paletteData, PLTTBUF_MAIN_OBJ_F, 0xFFEF, 0, 16, 0, RGB_BLACK);
        PaletteData_BeginPaletteFade(pokedexApp->paletteData, PLTTBUF_SUB_BG_F | PLTTBUF_SUB_OBJ_F, 0xFFFF, 0, 16, 0, RGB_BLACK);
        ++pokedexApp->seqState.state_10.state;
        pokedexApp->nextSeq = POKEDEXAPP_MAINSEQ_88;
        return POKEDEXAPP_MAINSEQ_03;
    case 2:
        if (!BgCellManager_IsCellMoving(pokedexApp->bgCellManager, 16)) {
            PokedexApp_SetSpriteVisible(pokedexApp, 0, 1);
            pokedexApp->seqState.state_10.state = 0;
            return POKEDEXAPP_MAINSEQ_80;
        }
        break;
    }

    return POKEDEXAPP_MAINSEQ_88;
}

static int PokedexApp_MainSeq_89(PokedexAppData *pokedexApp) {
    switch (pokedexApp->seqState.state_10.state) {
    case 0:
        PokedexApp_SetupScreen(pokedexApp, 4);
        PaletteData_BeginPaletteFade(pokedexApp->paletteData, PLTTBUF_MAIN_BG_F, 0xF7BE, -2, 16, 0, RGB_BLACK);
        PaletteData_BeginPaletteFade(pokedexApp->paletteData, PLTTBUF_MAIN_OBJ_F, 0xFFEF, -2, 16, 0, RGB_BLACK);
        PaletteData_BeginPaletteFade(pokedexApp->paletteData, PLTTBUF_SUB_BG_F | PLTTBUF_SUB_OBJ_F, 0xFFFF, -2, 16, 0, RGB_BLACK);
        ++pokedexApp->seqState.state_10.state;
        pokedexApp->nextSeq = POKEDEXAPP_MAINSEQ_89;
        return POKEDEXAPP_MAINSEQ_03;
    case 1:
        pokedexApp->entryTab = 4;
        pokedexApp->seqState.state_10.state = 0;
        return POKEDEXAPP_MAINSEQ_90;
    }

    return POKEDEXAPP_MAINSEQ_89;
}

static int PokedexApp_MainSeq_90(PokedexAppData *pokedexApp) {
    u32 spC;

    switch (PokedexApp_HandleEntryMenuInput(pokedexApp, &spC)) {
    case 0:
        if (spC == 1) {
            PlaySE(SEQ_SE_GS_ZKN04);
        } else {
            PlaySE(SEQ_SE_GS_ZKN03);
        }
        return PokedexApp_SwitchEntryTab(pokedexApp, 0, 1);
    case 1:
        if (spC == 1) {
            PlaySE(SEQ_SE_GS_ZKN04);
        } else {
            PlaySE(SEQ_SE_GS_ZKN03);
        }
        return PokedexApp_SwitchEntryTab(pokedexApp, 0, 2);
    case 2:
        if (spC == 1) {
            PlaySE(SEQ_SE_GS_ZKN04);
        } else {
            PlaySE(SEQ_SE_GS_ZKN03);
        }
        return PokedexApp_SwitchEntryTab(pokedexApp, 0, 3);
    case 3:
        PlaySE(SEQ_SE_GS_GEARCANCEL);
        return PokedexApp_SwitchEntryTab(pokedexApp, 0, 255);
    case 4:
        PlaySE(SEQ_SE_GS_GEARCANCEL);
        return ov18_021EDC50(pokedexApp, 3, 6, 9, 20, 4, POKEDEXAPP_MAINSEQ_67);
    }

    return POKEDEXAPP_MAINSEQ_90;
}

static int PokedexApp_MainSeq_91(PokedexAppData *pokedexApp) {
    PokedexAppData_UnkSub0868_State69 *r4 = &pokedexApp->seqState.state_69;

    switch (r4->state) {
    case 0:
        PaletteData_BeginPaletteFade(pokedexApp->paletteData, PLTTBUF_MAIN_BG_F, 0xF7BE, -2, r4->fadeStart, 16, RGB_BLACK);
        PaletteData_BeginPaletteFade(pokedexApp->paletteData, PLTTBUF_MAIN_OBJ_F, 0xFFEF, -2, r4->fadeStart, 16, RGB_BLACK);
        PaletteData_BeginPaletteFade(pokedexApp->paletteData, PLTTBUF_SUB_BG_F | PLTTBUF_SUB_OBJ_F, 0xFFFF, -2, r4->fadeStart, 16, RGB_BLACK);
        ++r4->state;
        pokedexApp->nextSeq = POKEDEXAPP_MAINSEQ_91;
        return POKEDEXAPP_MAINSEQ_03;
    case 1:
        PokedexApp_FreeEntryMenuWindows(pokedexApp);
        MI_CpuClear8(&pokedexApp->seqState, sizeof(pokedexApp->seqState));
        if (pokedexApp->entryTab == 2) {
            return POKEDEXAPP_MAINSEQ_76;
        } else if (pokedexApp->entryTab == 3) {
            return POKEDEXAPP_MAINSEQ_79;
        } else {
            return POKEDEXAPP_MAINSEQ_68;
        }
        break;
    }

    return POKEDEXAPP_MAINSEQ_91;
}

static int PokedexApp_MainSeq_92(PokedexAppData *pokedexApp) {
    pokedexApp->nextSeq = POKEDEXAPP_MAINSEQ_MAX;
    return PokedexApp_FadeOutTo(pokedexApp, 1);
}

static int PokedexApp_FadeOutTo(PokedexAppData *pokedexApp, int a1) {
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, HEAP_ID_POKEDEX_APP);
    pokedexApp->fadeNextSeq = a1;
    return POKEDEXAPP_MAINSEQ_02;
}

static int PokedexApp_FlashButton(PokedexAppData *pokedexApp, u8 a1, u8 a2, u8 a3, u8 a4, u8 a5, u8 a6, u8 a7, int a8) {
    PokedexAppData_UnkSub0868_State04 *r0 = &pokedexApp->seqState.state_04;

    r0->x = a2;
    r0->y = a3;
    r0->width = a4;
    r0->height = a5;
    r0->bgId = a1;
    r0->state = 0;
    r0->timer = 0;
    r0->normalPalette = a6;
    r0->flashPalette = a7;
    pokedexApp->nextSeq = a8;
    return POKEDEXAPP_MAINSEQ_04;
}

static int PokedexApp_FlashBottomButton(PokedexAppData *pokedexApp, u8 a1, u8 a2, u8 a3) {
    return PokedexApp_FlashButton(pokedexApp, 2, a1, 19, a2, 5, 0, 6, a3);
}

static int PokedexApp_FlashSearchButton(PokedexAppData *pokedexApp, u8 a1, u8 a2, u8 a3) {
    return PokedexApp_FlashButton(pokedexApp, 1, a1, 20, a2, 4, 4, 5, a3);
}

static int PokedexApp_FlashSearchMenuOption(PokedexAppData *pokedexApp, u8 a1, u8 a2, u8 a3, u8 a4, int a5) {
    return PokedexApp_FlashButton(pokedexApp, 2, a1, a2, a3, a4, 2, 3, a5);
}

static int PokedexApp_FlashEntryTab(PokedexAppData *pokedexApp, u8 a1, int a2) {
    return PokedexApp_FlashButton(pokedexApp, 1, a1, 20, 8, 4, 0, 6, a2);
}

static int ov18_021EDC50(PokedexAppData *pokedexApp, u8 a1, u8 a2, u8 a3, u8 a4, u8 a5, int a6) {
    return PokedexApp_FlashButton(pokedexApp, a1, a2, a3, a4, a5, 10, 11, a6);
}

static void PokedexApp_SetDexMode(PokedexAppData *pokedexApp, int a1) {
    UnkStruct_02092BB8_Set(pokedexApp->args->unk_08, PokedexApp_GetCursorSpecies(pokedexApp), pokedexApp->dexMode);
    pokedexApp->dexMode = a1;
    PokedexApp_DrawDexModeLabel(pokedexApp);
    PokedexApp_SetDexModeSwitchPos(pokedexApp, 0);
    PokedexApp_RunSearch(pokedexApp, pokedexApp->dexMode, 0, DEX_SEARCH_LETTERS_ALL, DEX_SEARCH_TYPE_ALL, DEX_SEARCH_TYPE_ALL, 0, 152, 0, 152, 1 << DEX_SEARCH_AREA_ALL, DEX_SEARCH_BODYTYPE_ALL);
    PokedexApp_DrawSeenAndCaughtCounts(pokedexApp);
}

static void PokedexApp_RefreshList(PokedexAppData *pokedexApp) {
    PokedexApp_RestoreListCursor(pokedexApp);
    PokedexApp_BuildListEntries(pokedexApp, 1);
    u16 species = PokedexApp_GetCursorSpecies(pokedexApp);
    u32 r6 = PokedexApp_GetListCursorIndex(pokedexApp);
    if (!PokedexApp_HasCaughtInLanguage(pokedexApp, species, pokedexApp->language)) {
        pokedexApp->language = GAME_LANGUAGE;
    }
    PokedexApp_DrawListEntryHeader(pokedexApp, species, r6);
    PokedexApp_DrawListEntryDetails(pokedexApp, species, r6);
    PokedexApp_DrawListIconGrid(pokedexApp, 24);
    PokedexApp_ShowMonPic(pokedexApp, species, 11, 10);
    PokedexApp_UpdateTypeIcons(pokedexApp, species, r6, 14);
    PokedexApp_UpdateLanguageIcons(pokedexApp, species, 18);
    PokedexApp_UpdateFootprintSprite(pokedexApp, species, r6, 13);
    PokedexApp_MoveSpriteToListCursor(pokedexApp, 5);
    PokedexApp_UpdateListPageArrows(pokedexApp, 2, 1);
    PokedexApp_UpdateListScrollbarSize(pokedexApp, 1, 1);
    PokedexApp_SyncListScrollbarToPage(pokedexApp, 1, 1);
    PokedexApp_LoadEntryDetailsBg(pokedexApp->bgConfig, GF_BG_LYR_SUB_3, pokedexApp->language, pokedexApp->gfxNarc, HEAP_ID_POKEDEX_APP);
    PokedexApp_DrawListNumbers(pokedexApp);
}

static void PokedexApp_SwitchDexMode(PokedexAppData *pokedexApp, int a1) {
    PokedexApp_SetDexMode(pokedexApp, a1);
    PokedexApp_RefreshList(pokedexApp);
}

static void PokedexApp_RestoreListCursor(PokedexAppData *pokedexApp) {
    u32 r4 = Pokedex_ConvertToCurrentDexNo(pokedexApp->dexMode, PokedexApp_FindSpeciesInResults(&pokedexApp->searchResults, UnkStruct_02092BB8_GetSpecies(pokedexApp->args->unk_08))) - 1;
    pokedexApp->listCursor = r4 % 15;
    pokedexApp->listPage = r4 / 15;
}

static BOOL PokedexApp_PlayCursorCry(PokedexAppData *pokedexApp) {
    u16 species = PokedexApp_GetCursorSpecies(pokedexApp);
    if (species == SPECIES_NONE) {
        return FALSE;
    }
    u32 form = Pokedex_GetSeenFormByIdx(pokedexApp->args->pokedex, species, 0);
    if (species == SPECIES_PICHU) {
        if (form == PICHU_FORM_MAX) {
            form = PICHU_SPIKY_EAR;
        } else {
            form = PICHU_NORMAL;
        }
    }
    Chatot_SetForceDefaultCry(1);
    PlayCry(species, form);
    Chatot_SetForceDefaultCry(0);
    return TRUE;
}

static void PokedexApp_PrepareSearchPageChange(PokedexAppData *pokedexApp, int a1, int a2, int a3) {
    PokedexAppData_UnkSub0868_State26 *r4 = &pokedexApp->seqState.state_26;

    r4->unk_0 = a1;
    r4->unk_1 = a3;
    r4->unk_3 = a2;
    r4->unk_2 = 0;
}

static void PokedexApp_UpdateEntryButtonsEnabled(PokedexAppData *pokedexApp) {
    u8 r4;

    if (PokedexApp_GetCursorSpecies(pokedexApp) == SPECIES_NONE) {
        r4 = 11;
    } else {
        r4 = 0;
    }
    BgCellManager_SetCellPalette(pokedexApp->bgCellManager, 1, 8, 0, 16, 5, r4);
    BgTilemapRectChangePalette(pokedexApp->bgConfig, GF_BG_LYR_MAIN_2, 8, 19, 16, 5, r4);
    ScheduleBgTilemapBufferTransfer(pokedexApp->bgConfig, GF_BG_LYR_MAIN_2);
}

static void PokedexApp_UpdateSearchResultButtonsEnabled(PokedexAppData *pokedexApp) {
    u8 r4;

    if ((pokedexApp->listCursor == 0 && pokedexApp->listPage == 0) || PokedexApp_GetCursorSpecies(pokedexApp) == SPECIES_NONE) {
        r4 = 11;
    } else {
        r4 = 0;
    }
    BgCellManager_SetCellPalette(pokedexApp->bgCellManager, 8, 8, 0, 16, 5, r4);
    BgTilemapRectChangePalette(pokedexApp->bgConfig, GF_BG_LYR_MAIN_2, 8, 19, 16, 5, r4);
    ScheduleBgTilemapBufferTransfer(pokedexApp->bgConfig, GF_BG_LYR_MAIN_2);
}

static BOOL PokedexApp_CanScrollFormList(PokedexAppData *pokedexApp, int a1, int a2) {
    int r4 = pokedexApp->selectedForm + a1;
    if (r4 < 0 || r4 >= pokedexApp->numSeenForms) {
        return FALSE;
    }

    PokedexAppData_UnkSub0868_State26 *r1 = &pokedexApp->seqState.state_26;

    r1->unk_0 = 0;
    r1->unk_1 = 0;
    r1->unk_2 = a2;
    return TRUE;
}

static void PokedexApp_StepIndexSkipping(s8 *a0, s8 a1, int a2, u8 a3) {
    for (u32 i = 0; i < a3; ++i) {
        *a0 += a2;
        if (*a0 < 0) {
            *a0 = a3 - 1;
        } else if (*a0 >= a3) {
            *a0 = 0;
        }
        if (*a0 != a1) {
            break;
        }
    }
}

static BOOL PokedexApp_CycleLeftCompareForm(PokedexAppData *pokedexApp, int a1) {
    if (pokedexApp->numSeenForms < 3) {
        return FALSE;
    }

    pokedexApp->compareSide = 0;
    PokedexApp_UpdateFormCompareSelector(pokedexApp);
    s8 r4 = pokedexApp->selectedForm;
    PokedexApp_StepIndexSkipping(&pokedexApp->selectedForm, pokedexApp->compareForm, a1, pokedexApp->numSeenForms);
    if (pokedexApp->selectedForm == r4) {
        return FALSE;
    }
    PokedexApp_SetCompareFormIcon(pokedexApp, 14, pokedexApp->selectedForm);
    ov18_021F6984(pokedexApp, 1, pokedexApp->selectedForm);
    PokedexApp_DrawCenteredFormName(pokedexApp, 6, pokedexApp->selectedForm);
    return TRUE;
}

static BOOL PokedexApp_CycleRightCompareForm(PokedexAppData *pokedexApp, int a1) {
    if (pokedexApp->numSeenForms < 3) {
        return FALSE;
    }

    pokedexApp->compareSide = 1;
    PokedexApp_UpdateFormCompareSelector(pokedexApp);
    s8 r4 = pokedexApp->compareForm;
    PokedexApp_StepIndexSkipping(&pokedexApp->compareForm, pokedexApp->selectedForm, a1, pokedexApp->numSeenForms);
    if (pokedexApp->compareForm == r4) {
        return FALSE;
    }
    PokedexApp_SetCompareFormIcon(pokedexApp, 15, pokedexApp->compareForm);
    ov18_021F6984(pokedexApp, 2, pokedexApp->compareForm);
    PokedexApp_DrawCenteredFormName(pokedexApp, 7, pokedexApp->compareForm);
    return TRUE;
}

static void PokedexApp_RunWeightCheck(PokedexAppData *pokedexApp) {
    PokedexAppData_UnkSub0868_State77 *r4 = &pokedexApp->seqState.state_77;

    switch (r4->state) {
    case 0:
        r4->anim = Heap_Alloc(HEAP_ID_POKEDEX_APP, sizeof(PokedexAppData_UnkSub0868_State77_Sub4));
        PokedexApp_InitWeightCheckAnim(pokedexApp, r4->anim);
        PlaySE(SEQ_SE_GS_RAKKA01);
        ++r4->state;
        // fallthrough
    case 1:
        if (!PokedexApp_DropFiguresOntoPans(r4->anim)) {
            ++r4->state;
        }
        break;
    case 2:
        if (!PokedexApp_RunWeightCheckAnim(r4->anim)) {
            ++r4->state;
        }
        break;
    }
}

static void PokedexApp_EndWeightCheck(PokedexAppData *pokedexApp) {
    Heap_Free(pokedexApp->seqState.state_77.anim);
    MI_CpuClear8(&pokedexApp->seqState, sizeof(pokedexApp->seqState));
}

static BOOL PokedexApp_CanScrollAreaList(PokedexAppData *pokedexApp, int a1, int a2) {
    int r4 = pokedexApp->areaListPos + a1;
    if (r4 < 0 || r4 >= pokedexApp->areaList.areas.nMaps) {
        return FALSE;
    }

    PokedexAppData_UnkSub0868_State26 *r1 = &pokedexApp->seqState.state_26;

    r1->unk_0 = 0;
    r1->unk_1 = 0;
    r1->unk_2 = a2;
    return TRUE;
}

static BOOL PokedexApp_SetAreaTimeOfDay(PokedexAppData *pokedexApp, int a1) {
    if (pokedexApp->areaTimeOfDay == a1) {
        return FALSE;
    }
    pokedexApp->areaTimeOfDay = a1;
    pokedexApp->areaListPos = 0;
    return TRUE;
}

static BOOL PokedexApp_SetAreaRegion(PokedexAppData *pokedexApp, int a1) {
    if (pokedexApp->nationalDex == 0) {
        return FALSE;
    }
    if (pokedexApp->areaRegion == a1) {
        return FALSE;
    }
    pokedexApp->areaRegion = a1;
    pokedexApp->areaListPos = 0;
    return TRUE;
}

static void PokedexApp_DrawAreaRegionTabs(PokedexAppData *pokedexApp) {
    if (pokedexApp->nationalDex == 0) {
        return;
    }
    if (pokedexApp->areaRegion == 0) {
        BgTilemapRectChangePalette(pokedexApp->bgConfig, GF_BG_LYR_MAIN_0, 9, 1, 7, 2, 12);
        BgTilemapRectChangePalette(pokedexApp->bgConfig, GF_BG_LYR_MAIN_0, 24, 1, 7, 2, 10);
    } else {
        BgTilemapRectChangePalette(pokedexApp->bgConfig, GF_BG_LYR_MAIN_0, 9, 1, 7, 2, 10);
        BgTilemapRectChangePalette(pokedexApp->bgConfig, GF_BG_LYR_MAIN_0, 24, 1, 7, 2, 12);
    }
    ScheduleBgTilemapBufferTransfer(pokedexApp->bgConfig, GF_BG_LYR_MAIN_0);
}

static void PokedexApp_ScrollAreaMapToJohto(PokedexAppData *pokedexApp) {
    ScheduleSetBgPosText(pokedexApp->bgConfig, GF_BG_LYR_SUB_0, BG_POS_OP_SET_X, -72);
}

static void PokedexApp_ResetAreaMapScroll(PokedexAppData *pokedexApp) {
    ScheduleSetBgPosText(pokedexApp->bgConfig, GF_BG_LYR_SUB_0, BG_POS_OP_SET_X, 0);
}

static BOOL PokedexApp_ScrollAreaMap(PokedexAppData *pokedexApp) {
    if (pokedexApp->areaRegion == 0) {
        if (GetBgHOffset(pokedexApp->bgConfig, GF_BG_LYR_SUB_0) == -72) {
            return FALSE;
        }
        ScheduleSetBgPosText(pokedexApp->bgConfig, GF_BG_LYR_SUB_0, BG_POS_OP_SUB_X, 8);
    } else {
        if (GetBgHOffset(pokedexApp->bgConfig, GF_BG_LYR_SUB_0) == 104) {
            return FALSE;
        }
        ScheduleSetBgPosText(pokedexApp->bgConfig, GF_BG_LYR_SUB_0, BG_POS_OP_ADD_X, 8);
    }
    return TRUE;
}

static void PokedexApp_UpdateAreaMarkerBlink(PokedexAppData *pokedexApp) {
    switch (pokedexApp->areaBlinkFadingOut) {
    case 0:
        pokedexApp->areaBlinkAlpha += (u8)1;
        if (pokedexApp->areaBlinkAlpha == 15) {
            pokedexApp->areaBlinkFadingOut = 1;
        }
        break;
    case 1:
        pokedexApp->areaBlinkAlpha += (u8)-1;
        if (pokedexApp->areaBlinkAlpha == 0) {
            pokedexApp->areaBlinkFadingOut = 0;
            pokedexApp->areaBlinkPhase ^= 1;
            PokedexApp_UpdateAreaMarkerPalettes(pokedexApp);
        }
        break;
    }

    G2S_SetBlendAlpha(0, 1, pokedexApp->areaBlinkAlpha, 31 - pokedexApp->areaBlinkAlpha);
}

static void PokedexApp_ResetAreaMarkerBlink(PokedexAppData *pokedexApp) {
    pokedexApp->areaBlinkFadingOut = 0;
    pokedexApp->areaBlinkAlpha = 0;
    pokedexApp->areaBlinkPhase = 0;
    PokedexApp_UpdateAreaMarkerPalettes(pokedexApp);
    G2S_SetBlendAlpha(0, 1, pokedexApp->areaBlinkAlpha, 31 - pokedexApp->areaBlinkAlpha);
}
