#include "gf_gfx_planes.h"

#include "global.h"

#include "constants/sndseq.h"

#include "sound_playback.h"
#include "system.h"

#ifdef SINGLE_SCREEN
const volatile u16 gSingleScreenDispSelectMask = 0;

// Lets the native recompilation (ssrecomp) recognise this build and show only the visible screen.
// The byte at offset 32 from the tag is read every frame: nonzero means the scene wants both
// screens (the intro movie), zero means the visible screen alone.
struct SingleScreenBuildInfo {
    char tag[32];
    volatile u8 twoScreenScene;
    volatile u8 battleMenu;      // battle command panel shown: 0 none, else SS_MENU_* kind
    volatile u8 battleSelection; // its selected item
    volatile u8 subEngineShown;  // the sub engine is on the visible screen (battle bag, party)
};
struct SingleScreenBuildInfo gSingleScreenBuildInfo = { "SSRECOMP-SINGLE-SCREEN-1", 0, 0, 0, 0 };
#define gSingleScreenBuildTag gSingleScreenBuildInfo.tag

void SingleScreen_SetTwoScreenScene(BOOL twoScreen) {
    gSingleScreenBuildInfo.twoScreenScene = twoScreen != FALSE;
    SingleScreen_ShowSubEngine(twoScreen);
}
void SingleScreen_SetBattleMenu(int kind, int selection);
void SingleScreen_SetBattleMenu(int kind, int selection) {
    gSingleScreenBuildInfo.battleMenu = kind;
    gSingleScreenBuildInfo.battleSelection = selection;
}
static volatile u8 sSingleScreenTagSink;

void SingleScreen_ShowSubEngine(BOOL show) {
    gSingleScreenBuildInfo.subEngineShown = show != FALSE;
    u32 dsel = (u32)(show ? GX_DISP_SELECT_MAIN_SUB : GX_DISP_SELECT_SUB_MAIN) << REG_GX_POWCNT_DSEL_SHIFT;

    reg_GX_POWCNT = (u16)((reg_GX_POWCNT & ~(1 << REG_GX_POWCNT_DSEL_SHIFT)) | dsel);
}

BOOL SingleScreen_IsSubEngineShown(void) {
    return (reg_GX_POWCNT >> REG_GX_POWCNT_DSEL_SHIFT) & 1;
}

BOOL SingleScreen_HandleSwapKey(void) {
    if (!(gSystem.newKeys & PAD_BUTTON_SELECT)) {
        return FALSE;
    }
    sSingleScreenTagSink = gSingleScreenBuildTag[gSystem.newKeys & 3]; // a real use, so the linker keeps the tag
    PlaySE(SEQ_SE_DP_SELECT);
    SingleScreen_ShowSubEngine(!SingleScreen_IsSubEngineShown());
    return TRUE;
}
#endif

static u32 sEngineBPlanes = 0;
static u32 sEngineAPlanes = 0;

void GfGfx_SetBanks(const GraphicsBanks *banks) {
    GX_ResetBankForBG();
    GX_ResetBankForBGExtPltt();
    GX_ResetBankForSubBG();
    GX_ResetBankForSubBGExtPltt();
    GX_ResetBankForOBJ();
    GX_ResetBankForOBJExtPltt();
    GX_ResetBankForSubOBJ();
    GX_ResetBankForSubOBJExtPltt();
    GX_ResetBankForTex();
    GX_ResetBankForTexPltt();

    GX_SetBankForBG(banks->bg);
    GX_SetBankForBGExtPltt(banks->bgextpltt);
    GX_SetBankForSubBG(banks->subbg);
    GX_SetBankForSubBGExtPltt(banks->subbgextpltt);
    GX_SetBankForOBJ(banks->obj);
    GX_SetBankForOBJExtPltt(banks->objextpltt);
    GX_SetBankForSubOBJ(banks->subobj);
    GX_SetBankForSubOBJExtPltt(banks->subobjextpltt);
    GX_SetBankForTex(banks->tex);
    GX_SetBankForTexPltt(banks->texpltt);
}

void GfGfx_DisableEngineAPlanes(void) {
    (void)sEngineBPlanes;
    sEngineAPlanes = 0;
}

void GfGfx_EngineATogglePlanes(u8 planeMask, u8 enable) {
    if (enable == GF_PLANE_TOGGLE_ON) {
        if (sEngineAPlanes & planeMask) {
            return;
        }
    } else {
        if (!(sEngineAPlanes & planeMask)) {
            return;
        }
    }
    sEngineAPlanes ^= planeMask;
    GX_SetVisiblePlane(sEngineAPlanes);
}

void GfGfx_EngineASetPlanes(u32 planes) {
    sEngineAPlanes = planes;
    GX_SetVisiblePlane(sEngineAPlanes);
}

void GfGfx_DisableEngineBPlanes(void) {
    sEngineBPlanes = 0;
}

void GfGfx_EngineBTogglePlanes(u8 planeMask, u8 enable) {
    if (enable == GF_PLANE_TOGGLE_ON) {
        if (sEngineBPlanes & planeMask) {
            return;
        }
    } else {
        if (!(sEngineBPlanes & planeMask)) {
            return;
        }
    }
    sEngineBPlanes ^= planeMask;
    GXS_SetVisiblePlane(sEngineBPlanes);
}

void GfGfx_EngineBSetPlanes(u32 planes) {
    sEngineBPlanes = planes;
    GXS_SetVisiblePlane(sEngineBPlanes);
}

void GfGfx_BothDispOn(void) {
    GX_DispOn();
    GXS_DispOn();
}

void GfGfx_SwapDisplay(void) {
    if (gSystem.screensFlipped == 0) {
        GX_SetDispSelect(GX_DISP_SELECT_MAIN_SUB);
    } else {
        GX_SetDispSelect(GX_DISP_SELECT_SUB_MAIN);
    }
}

u32 GfGfx_EngineAGetPlanes(void) {
    return sEngineAPlanes;
}
