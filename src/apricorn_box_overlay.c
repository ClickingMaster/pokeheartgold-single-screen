#include "constants/gx.h"

#include "apricorn_box_overlay_internal.h"

FS_EXTERN_OVERLAY(OVY_59);

BOOL ApricornBoxScreen_Init(OverlayManager *man, int *state);
BOOL ApricornBoxScreen_Main(OverlayManager *man, int *state);
BOOL ApricornBoxScreen_Exit(OverlayManager *man, int *state);
BOOL AprijuiceScreen_Init(OverlayManager *man, int *state);
BOOL AprijuiceScreen_Main(OverlayManager *man, int *state);
BOOL AprijuiceScreen_Exit(OverlayManager *man, int *state);

BOOL ApricornBox_RunChildApp(OverlayManager **pMan);
BOOL ApricornBox_Init(OverlayManager *man, int *state);
BOOL ApricornBox_Main(OverlayManager *man, int *state);
BOOL ApricornBox_Exit(OverlayManager *man, int *state);
void ApricornBox_FeedAprijuice(ApricornJuice *record, Party *party, int slot);
void ApricornBox_InitFromArgs(ApricornBoxApp *app);
void ApricornBox_Cleanup(ApricornBoxApp *app);
int ApricornBox_State_OpenBoxScreen(ApricornBoxApp *app);
int ApricornBox_State_RunBoxScreen(ApricornBoxApp *app);
int ApricornBox_State_OpenPartyMenu(ApricornBoxApp *app);
int ApricornBox_State_RunPartyMenu(ApricornBoxApp *app);
int ApricornBox_State_OpenAprijuiceScreen(ApricornBoxApp *app);
int ApricornBox_State_RunAprijuiceScreen(ApricornBoxApp *app);
void ApricornBox_ResetDisplay(void);

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

BOOL ApricornBox_RunChildApp(OverlayManager **pMan) {
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
    case 0:
        if (app->args->mode == 3) {
            *state = 3;
        } else {
            *state = 1;
        }
        break;
    case 1:
        *state = ApricornBox_State_OpenBoxScreen(app);
        break;
    case 2:
        *state = ApricornBox_State_RunBoxScreen(app);
        break;
    case 3:
        *state = ApricornBox_State_OpenPartyMenu(app);
        break;
    case 4:
        *state = ApricornBox_State_RunPartyMenu(app);
        break;
    case 5:
        *state = ApricornBox_State_OpenAprijuiceScreen(app);
        break;
    case 6:
        *state = ApricornBox_State_RunAprijuiceScreen(app);
        break;
    case 7:
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
    level = record->unk2;
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

void ApricornBox_InitFromArgs(ApricornBoxApp *app) {
    if (app->args->juiceUsedVar != NULL) {
        *app->args->juiceUsedVar = 0;
    }
    switch (app->args->mode) {
    case 0:
        app->mode = 0;
        break;
    case 1:
        app->mode = 1;
        break;
    case 2:
        app->mode = 2;
        break;
    case 3:
        app->unk_19 = 1;
        app->mode = 0;
        app->juiceIndex = app->args->unkC;
        return;
    }
    app->unk_19 = 0;
    app->juiceIndex = 8;
}

void ApricornBox_Cleanup(ApricornBoxApp *app) {
}

int ApricornBox_State_OpenBoxScreen(ApricornBoxApp *app) {
    app->child = OverlayManager_New(&sApricornBoxScreenTemplate, app, app->heapId);
    return 2;
}

int ApricornBox_State_RunBoxScreen(ApricornBoxApp *app) {
    if (!ApricornBox_RunChildApp(&app->child)) {
        return 2;
    }
    if (app->mode == 3) {
        app->partySlot = 0;
        return 3;
    }
    return 7;
}

int ApricornBox_State_OpenPartyMenu(ApricornBoxApp *app) {
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
    return 4;
}

int ApricornBox_State_RunPartyMenu(ApricornBoxApp *app) {
    u8 slot;

    if (!ApricornBox_RunChildApp(&app->child)) {
        return 4;
    }
    slot = app->partyMenuArgs->partySlot;
    app->partySlot = slot;
    Heap_Free(app->partyMenuArgs);
    app->partyMenuArgs = NULL;
    if (slot == 7) {
        if (app->args->mode == 3) {
            return 7;
        }
        return 1;
    }
    return 5;
}

int ApricornBox_State_OpenAprijuiceScreen(ApricornBoxApp *app) {
    app->child = OverlayManager_New(&sAprijuiceScreenTemplate, app, app->heapId);
    return 6;
}

int ApricornBox_State_RunAprijuiceScreen(ApricornBoxApp *app) {
    if (!ApricornBox_RunChildApp(&app->child)) {
        return 6;
    }
    if (app->mode == 3) {
        return 3;
    }
    if (app->args->mode == 3) {
        return 7;
    }
    return 1;
}

void ApricornBox_ResetDisplay(void) {
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
