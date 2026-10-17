#include "constants/gx.h"

#include "application/apricorn_box/apricorn_box_overlay_internal.h"

FS_EXTERN_OVERLAY(OVY_59);

BOOL ApricornBoxScreen_Init(OverlayManager *man, int *state);
BOOL ApricornBoxScreen_Main(OverlayManager *man, int *state);
BOOL ApricornBoxScreen_Exit(OverlayManager *man, int *state);
BOOL AprijuiceScreen_Init(OverlayManager *man, int *state);
BOOL AprijuiceScreen_Main(OverlayManager *man, int *state);
BOOL AprijuiceScreen_Exit(OverlayManager *man, int *state);

BOOL ApricornBox_Init(OverlayManager *man, int *state);
BOOL ApricornBox_Main(OverlayManager *man, int *state);
BOOL ApricornBox_Exit(OverlayManager *man, int *state);
void ApricornBox_FeedAprijuice(ApricornJuice *record, Party *party, int slot);

enum ApricornBoxState {
    APRICORN_BOX_STATE_START,
    APRICORN_BOX_STATE_OPEN_BOX_SCREEN,
    APRICORN_BOX_STATE_RUN_BOX_SCREEN,
    APRICORN_BOX_STATE_OPEN_PARTY_MENU,
    APRICORN_BOX_STATE_RUN_PARTY_MENU,
    APRICORN_BOX_STATE_OPEN_APRIJUICE_SCREEN,
    APRICORN_BOX_STATE_RUN_APRIJUICE_SCREEN,
    APRICORN_BOX_STATE_EXIT,
};

static BOOL ApricornBox_RunChildApp(OverlayManager **pMan);
static void ApricornBox_InitFromArgs(ApricornBoxApp *app);
static void ApricornBox_Cleanup(ApricornBoxApp *app);
static int ApricornBox_State_OpenBoxScreen(ApricornBoxApp *app);
static int ApricornBox_State_RunBoxScreen(ApricornBoxApp *app);
static int ApricornBox_State_OpenPartyMenu(ApricornBoxApp *app);
static int ApricornBox_State_RunPartyMenu(ApricornBoxApp *app);
static int ApricornBox_State_OpenAprijuiceScreen(ApricornBoxApp *app);
static int ApricornBox_State_RunAprijuiceScreen(ApricornBoxApp *app);
static void ApricornBox_ResetDisplay(void);

static const OverlayManagerTemplate sApricornBoxScreenTemplate = {
    ApricornBoxScreen_Init,
    ApricornBoxScreen_Main,
    ApricornBoxScreen_Exit,
    FS_OVERLAY_ID(OVY_59),
};

static const OverlayManagerTemplate sAprijuiceScreenTemplate = {
    AprijuiceScreen_Init,
    AprijuiceScreen_Main,
    AprijuiceScreen_Exit,
    FS_OVERLAY_ID(OVY_59),
};

static BOOL ApricornBox_RunChildApp(OverlayManager **pMan) {
    if (*pMan != NULL && OverlayManager_Run(*pMan)) {
        OverlayManager_Delete(*pMan);
        *pMan = NULL;
        return TRUE;
    }
    return FALSE;
}

BOOL ApricornBox_Init(OverlayManager *man, int *state) {
    ApricornBoxApp *app;

    ApricornBox_ResetDisplay();
    Heap_Create(HEAP_ID_3, HEAP_ID_133, 0x2000);
    app = OverlayManager_CreateAndGetData(man, sizeof(ApricornBoxApp), HEAP_ID_133);
    MI_CpuFill8(app, 0, sizeof(ApricornBoxApp));
    app->heapId = HEAP_ID_133;
    app->args = OverlayManager_GetArgs(man);
    ApricornBox_InitFromArgs(app);
    return TRUE;
}

BOOL ApricornBox_Main(OverlayManager *man, int *state) {
    ApricornBoxApp *app = OverlayManager_GetData(man);

    switch (*state) {
    case APRICORN_BOX_STATE_START:
        if (app->args->mode == APRICORN_BOX_LAUNCH_JUICE_STAND) {
            *state = APRICORN_BOX_STATE_OPEN_PARTY_MENU;
        } else {
            *state = APRICORN_BOX_STATE_OPEN_BOX_SCREEN;
        }
        break;
    case APRICORN_BOX_STATE_OPEN_BOX_SCREEN:
        *state = ApricornBox_State_OpenBoxScreen(app);
        break;
    case APRICORN_BOX_STATE_RUN_BOX_SCREEN:
        *state = ApricornBox_State_RunBoxScreen(app);
        break;
    case APRICORN_BOX_STATE_OPEN_PARTY_MENU:
        *state = ApricornBox_State_OpenPartyMenu(app);
        break;
    case APRICORN_BOX_STATE_RUN_PARTY_MENU:
        *state = ApricornBox_State_RunPartyMenu(app);
        break;
    case APRICORN_BOX_STATE_OPEN_APRIJUICE_SCREEN:
        *state = ApricornBox_State_OpenAprijuiceScreen(app);
        break;
    case APRICORN_BOX_STATE_RUN_APRIJUICE_SCREEN:
        *state = ApricornBox_State_RunAprijuiceScreen(app);
        break;
    case APRICORN_BOX_STATE_EXIT:
        return TRUE;
    }
    return FALSE;
}

BOOL ApricornBox_Exit(OverlayManager *man, int *state) {
    ApricornBoxApp *app = OverlayManager_GetData(man);

    ApricornBox_ResetDisplay();
    ApricornBox_Cleanup(app);
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_133);
    return TRUE;
}

void ApricornBox_FeedAprijuice(ApricornJuice *record, Party *party, int slot) {
    s8 newMods[5];
    PartyExtraSub modifiers;
    ApricornJuiceData juice;
    u32 statA;
    u32 statB;
    u32 amtA;
    u32 amtB;
    u32 total;
    u8 penalty;
    u8 level;
    int i;

    MI_CpuFill8(newMods, 0, 5);
    ApricornJuice_RankFlavors(record, &juice);
    level = record->mildness;
    if (juice.entries[0].stat >= 5) {
        statA = juice.statOverride[0];
    } else {
        statA = (u8)juice.entries[0].stat;
    }
    if (juice.entries[1].stat >= 5) {
        statB = juice.statOverride[1];
    } else {
        statB = (u8)juice.entries[1].stat;
    }
    Party_GetMonAprijuiceModifiers(party, &modifiers, slot);
    amtA = juice.entries[0].amount;
    newMods[statA] = (u8)(1.5f * (float)amtA);
    if (amtA != 0) {
        newMods[statA] += 10;
    }
    amtB = juice.entries[1].amount;
    newMods[statB] = (u8)(1.5f * (float)amtB);
    total = (u8)(amtA + amtB);
    if (level == 0xFF) {
        penalty = (u8)(0.1f * (float)total);
    } else if (level >= 0xC8) {
        penalty = (u8)(0.2f * (float)total);
    } else {
        penalty = (u8)((float)total * (1.0f - 0.1f * (float)(u8)(level / 25)));
    }
    newMods[juice.penaltyStat] -= penalty;
    for (i = 0; i < 5; i++) {
        int v = newMods[i];
        if (v < -127) {
            v = -127;
        } else if (v > 127) {
            v = 127;
        }
        modifiers.stats[i] = v;
    }
    Party_SetMonAprijuiceModifiers(party, &modifiers, slot);
    ApplyMonMoodModifier(Party_GetMonByIndex(party, slot), 7);
}

static void ApricornBox_InitFromArgs(ApricornBoxApp *app) {
    if (app->args->juiceUsedVar != NULL) {
        *app->args->juiceUsedVar = 0;
    }
    switch (app->args->mode) {
    case APRICORN_BOX_LAUNCH_VIEW:
        app->mode = APRICORN_BOX_MODE_VIEW;
        break;
    case APRICORN_BOX_LAUNCH_BLENDER:
        app->mode = APRICORN_BOX_MODE_BLENDER;
        break;
    case APRICORN_BOX_LAUNCH_KURT:
        app->mode = APRICORN_BOX_MODE_KURT;
        break;
    case APRICORN_BOX_LAUNCH_JUICE_STAND:
        app->unk_19 = 1;
        app->mode = APRICORN_BOX_MODE_VIEW;
        app->juiceIndex = app->args->juiceIndex;
        return;
    }
    app->unk_19 = 0;
    app->juiceIndex = 8;
}

static void ApricornBox_Cleanup(ApricornBoxApp *app) {
}

static int ApricornBox_State_OpenBoxScreen(ApricornBoxApp *app) {
    app->child = OverlayManager_New(&sApricornBoxScreenTemplate, app, app->heapId);
    return APRICORN_BOX_STATE_RUN_BOX_SCREEN;
}

static int ApricornBox_State_RunBoxScreen(ApricornBoxApp *app) {
    if (!ApricornBox_RunChildApp(&app->child)) {
        return APRICORN_BOX_STATE_RUN_BOX_SCREEN;
    }
    if (app->mode == APRICORN_BOX_MODE_USE_JUICE) {
        app->partySlot = 0;
        return APRICORN_BOX_STATE_OPEN_PARTY_MENU;
    }
    return APRICORN_BOX_STATE_EXIT;
}

static int ApricornBox_State_OpenPartyMenu(ApricornBoxApp *app) {
    PartyMenuArgs *args = Heap_Alloc(app->heapId, sizeof(PartyMenuArgs));

    MI_CpuFill8(args, 0, sizeof(PartyMenuArgs));
    args->party = SaveArray_Party_Get(app->args->saveData);
    args->bag = Save_Bag_Get(app->args->saveData);
    args->panelLayout = 0;
    args->context = PARTY_MENU_CONTEXT_20;
    args->options = Save_PlayerData_GetOptionsAddr(app->args->saveData);
    args->mailbox = Save_Mailbox_Get(app->args->saveData);
    args->fieldSystem = NULL;
    args->menuInputStatePtr = app->args->menuInputStatePtr;
    args->partySlot = app->partySlot;
    app->child = OverlayManager_New(&gOverlayTemplate_PartyMenu, args, app->heapId);
    app->partyMenuArgs = args;
    return APRICORN_BOX_STATE_RUN_PARTY_MENU;
}

static int ApricornBox_State_RunPartyMenu(ApricornBoxApp *app) {
    u8 slot;

    if (!ApricornBox_RunChildApp(&app->child)) {
        return APRICORN_BOX_STATE_RUN_PARTY_MENU;
    }
    slot = app->partyMenuArgs->partySlot;
    app->partySlot = slot;
    Heap_Free(app->partyMenuArgs);
    app->partyMenuArgs = NULL;
    if (slot == 7) {
        if (app->args->mode == APRICORN_BOX_LAUNCH_JUICE_STAND) {
            return APRICORN_BOX_STATE_EXIT;
        }
        return APRICORN_BOX_STATE_OPEN_BOX_SCREEN;
    }
    return APRICORN_BOX_STATE_OPEN_APRIJUICE_SCREEN;
}

static int ApricornBox_State_OpenAprijuiceScreen(ApricornBoxApp *app) {
    app->child = OverlayManager_New(&sAprijuiceScreenTemplate, app, app->heapId);
    return APRICORN_BOX_STATE_RUN_APRIJUICE_SCREEN;
}

static int ApricornBox_State_RunAprijuiceScreen(ApricornBoxApp *app) {
    if (!ApricornBox_RunChildApp(&app->child)) {
        return APRICORN_BOX_STATE_RUN_APRIJUICE_SCREEN;
    }
    if (app->mode == APRICORN_BOX_MODE_USE_JUICE) {
        return APRICORN_BOX_STATE_OPEN_PARTY_MENU;
    }
    if (app->args->mode == APRICORN_BOX_LAUNCH_JUICE_STAND) {
        return APRICORN_BOX_STATE_EXIT;
    }
    return APRICORN_BOX_STATE_OPEN_BOX_SCREEN;
}

static void ApricornBox_ResetDisplay(void) {
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    reg_GX_DISPCNT &= 0xFFFFE0FF;
    reg_GXS_DB_DISPCNT &= 0xFFFFE0FF;
    SetMasterBrightnessByColor(PM_LCD_TOP, RGB_BLACK);
    SetMasterBrightnessByColor(PM_LCD_BOTTOM, RGB_BLACK);
    ResetVisibleHardwareWindows(PM_LCD_TOP);
    ResetVisibleHardwareWindows(PM_LCD_BOTTOM);
}
