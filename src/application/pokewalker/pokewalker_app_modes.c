// Pokewalker connection app, modes 6 to 10 (the mode table is in pokewalker_app.c).
// Mode 6 saves the game after a link (showing the Pokemon that came back first), then goes to mode 8 or 10.
// Mode 7 lets the player pick the Pokemon for a stroll from the party or the PC boxes with the box selector.
// Mode 8 runs the Pokewalker event log app over the log read from the Pokewalker.
// Mode 9 waits for Down, then goes to the link mode. Mode 10 is the stroll report (states in sStrollReportStates).

#include "application/pokewalker/pokewalker_modes2.h"

#include "global.h"

#include "constants/pokemon.h"
#include "constants/sndseq.h"

#include "msgdata/msg/msg_0274.h"

#include "gf_gfx_planes.h"
#include "heap.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "party.h"
#include "save.h"
#include "save_arrays.h"
#include "screen_fade.h"
#include "sound.h"
#include "sound_playback.h"
#include "string_util.h"
#include "system.h"
#include "text.h"
#include "oam_manager.h"

extern int (*const sLinkSaveStates[])(PokewalkerApp *app);
extern int (*const sStrollReportStates[])(PokewalkerApp *app);
extern const GraphicsBanks sSelectMonGraphicsBanks;
extern const GraphicsModes sSelectMonGraphicsModes;
extern const ObjCharTransferTemplate sSelectMonObjCharTransferTemplate;
extern const OverlayManagerTemplate sEventLogAppTemplate;

void ov112_021EE7A8(PokewalkerApp *app);
void ov112_021EE920(PokewalkerApp *app);
void ov112_021EEA7C(PokewalkerApp *app);
void ov112_021EEAF0(PokewalkerApp *app, int a1);
void ov112_021ED0C8(PokewalkerApp *app);
BOOL sub_02006B84(void);
void NNS_G2dInitOamManagerModule(void);

#pragma force_active on
int PokewalkerApp_LinkSave_Enter(PokewalkerApp *app) {
    app->strollMon = NULL;
    if (app->unk_10 == 1) {
        ov112_021EE7A8(app);
        ov112_021EEAF0(app, 1);
        ov112_021EEA7C(app);
        ov112_021EE920(app);
        if (app->peerIdentity.unk_5B_2 == 0) {
            sub_020326A4(app->pokewalker, 1, 0);
        }
        if (app->strollMon != NULL) {
            PokewalkerApp_LoadMonPicture(app->strollMon, app->monSpriteBuffer, &app->monPicTemplate, 0);
            ov112_021ED0C8(app);
            ov112_021EAA10(app);
            app->unk_1F2C2 = 0;
            PlayFanfare(SEQ_ME_KEYITEM);
        } else {
            ov112_021EAA10(app);
            app->unk_1F2D6 = 31;
        }
        PokewalkerApp_UnlockNextCourse(app);
        app->state = 0;
    } else if (app->unk_10 == 2) {
        ov112_021EEA7C(app);
        ov112_021EEAF0(app, 0);
        app->state = 5;
    } else if (app->unk_10 == 4) {
        ov112_021EE7A8(app);
        PokewalkerApp_LoadMonPicture(app->strollMon, app->monSpriteBuffer, &app->monPicTemplate, 0);
        ov112_021ED0C8(app);
        app->state = 0;
        sub_020326A4(app->pokewalker, 1, 0);
    }
    app->peerIdentity.unk_5B_2 = 0;
    return 2;
}
#pragma force_active reset

int PokewalkerApp_LinkSave_Update(PokewalkerApp *app) {
    int state = sLinkSaveStates[app->state](app);
    app->state = state;
    if (state == 11) {
        app->state = 0;
        return 3;
    }
    return 2;
}

int PokewalkerApp_LinkSave_Exit(PokewalkerApp *app) {
    PokewalkerApp_HideSprite(app, 8);
    return 1;
}

int PokewalkerApp_LinkSave_PrintSaving(PokewalkerApp *app) {
    app->textPrinterId = PokewalkerApp_StartMessageBox(app, 2, msg_0274_00013);
    ov112_021EA5A4(app, 0);
    return 1;
}

int PokewalkerApp_LinkSave_WaitSaving(PokewalkerApp *app) {
    if (!TextPrinterCheckActive(app->textPrinterId) && app->unk_1F2D6 == 31 && !sub_02006B84()) {
        IsFanfarePlaying();
        PokewalkerApp_CreateWaitingIcon(app);
        return 2;
    }
    return 1;
}

int PokewalkerApp_LinkSave_Save(PokewalkerApp *app) {
    SaveGameNormal(app->saveData);
    Save_ClearStatusFlags(app->saveData);
    return 3;
}

int PokewalkerApp_LinkSave_EndSave(PokewalkerApp *app) {
    PokewalkerApp_DeleteWaitingIcon(app);
    return 4;
}

int PokewalkerApp_LinkSave_SetNextMode(PokewalkerApp *app) {
    app->nextMode = app->unk_10 == 4 ? 0 : 8;
    return 10;
}

int PokewalkerApp_LinkSave_PrintSaving2(PokewalkerApp *app) {
    app->textPrinterId = PokewalkerApp_StartMessageBox(app, 2, msg_0274_00013);
    ov112_021EA5A4(app, 0);
    return 6;
}

int PokewalkerApp_LinkSave_WaitSaving2(PokewalkerApp *app) {
    if (!TextPrinterCheckActive(app->textPrinterId)) {
        PokewalkerApp_CreateWaitingIcon(app);
        return 7;
    }
    return 6;
}

int PokewalkerApp_LinkSave_Save2(PokewalkerApp *app) {
    SaveGameNormal(app->saveData);
    Save_ClearStatusFlags(app->saveData);
    return 8;
}

int PokewalkerApp_LinkSave_EndSave2(PokewalkerApp *app) {
    PokewalkerApp_DeleteWaitingIcon(app);
    return 9;
}

int PokewalkerApp_LinkSave_SetNextMode2(PokewalkerApp *app) {
    app->nextMode = 10;
    return 10;
}

int PokewalkerApp_LinkSave_End(PokewalkerApp *app) {
    FillBgTilemapRect(app->bgConfig, 1, 0, 0, 0, 32, 24, 16);
    BgCommitTilemapBufferToVram(app->bgConfig, 1);
    PokewalkerApp_HideSprite(app, 6);
    return 11;
}

// Addresses the mode's work as byte offsets from PokewalkerApp 0x1D764 (selectMonState); member accesses
// compile differently here.
#define SELECT_MON_WORK(app, member) ((u8 *)(app) + 0x1D764 + offsetof(PokewalkerSelectMonWork, member))

#pragma force_active on
int PokewalkerApp_SelectMon_Enter(PokewalkerApp *app) {
    PCStorage *storage;
    Party *party;

    switch (*(int *)SELECT_MON_WORK(app, state)) {
    case 0:
        PokewalkerApp_Teardown(app);
        *(int *)SELECT_MON_WORK(app, state) = 1;
        break;
    case 1:
        app->bgConfig = BgConfig_Alloc(HEAP_ID_154);
        PokewalkerApp_SelectMon_SetGxBanks();
        *(void **)SELECT_MON_WORK(app, boxSelector) = sub_020932E0(HEAP_ID_154, 1, 18);
        *(void **)SELECT_MON_WORK(app, boxDisplay) = ov112_021F039C(HEAP_ID_154);
        PokewalkerApp_SelectMon_SetGraphicsModes(app->bgConfig);
        PokewalkerApp_SelectMon_InitObjTransfer();
        PokewalkerApp_SelectMon_InitOam((PokewalkerSelectMonWork *)&app->selectMonState);
        app->selectMonState = 0;
        storage = SaveArray_PCStorage_Get(app->saveData);
        party = SaveArray_Party_Get(app->saveData);
        sub_02093440(*(void **)SELECT_MON_WORK(app, boxSelector), app->bgConfig, storage, party, 0, 0, TRUE, 0, PokewalkerApp_SelectMon_GetBoxMonIcon, PokewalkerApp_SelectMon_GetBoxName, SELECT_MON_WORK(app, box), PokewalkerApp_SelectMon_ShowMonInfo, app);
        ov112_021F03BC(*(void **)SELECT_MON_WORK(app, boxDisplay), app->bgConfig);
        Main_SetVBlankIntrCB((GFIntrCB)PokewalkerApp_SelectMon_VBlankCB, (PokewalkerSelectMonWork *)&app->selectMonState);
        *(int *)SELECT_MON_WORK(app, state) = 0;
        return 2;
    }
    return 1;
}

#undef SELECT_MON_WORK
#pragma force_active reset

int PokewalkerApp_SelectMon_Update(PokewalkerApp *app) {
    PokewalkerSelectMonWork *work = (PokewalkerSelectMonWork *)&app->selectMonState;

    switch (work->state) {
    case 0:
        work->state = 1;
        break;
    case 1:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_BLACK, 6, 1, HEAP_ID_154);
        work->state = 2;
        break;
    case 2:
        if (IsPaletteFadeFinished()) {
            work->state = 3;
        }
        break;
    case 3: {
        int result = sub_020935E0(work->boxSelector);
        if (result == 2) {
            work->cancelled = FALSE;
            work->state = 4;
        } else if (result == 3) {
            work->cancelled = TRUE;
            work->state = 4;
        }
        break;
    }
    case 4:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, HEAP_ID_154);
        work->state = 5;
        break;
    case 5:
        if (IsPaletteFadeFinished()) {
            PokewalkerApp_SelectMon_StoreSelection(work, app);
            return 3;
        }
        break;
    }

    sub_02093A40(work->boxSelector);
    ov112_021F050C(work->boxDisplay);
    return 2;
}

int PokewalkerApp_SelectMon_Exit(PokewalkerApp *app) {
    app->selectMonState = 0;
    Main_SetVBlankIntrCB(NULL, NULL);
    sub_02093354(app->boxSelector);
    ov112_021F051C(app->boxDisplay);
    OamManager_Free();
    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
    Heap_Free(app->bgConfig);
    PokewalkerApp_Setup(app);
    app->nextMode = app->selectMonCancelled == TRUE ? 0 : 3;
    return 1;
}

void PokewalkerApp_SelectMon_SetGxBanks(void) {
    GraphicsBanks banks = sSelectMonGraphicsBanks;
    GfGfx_SetBanks(&banks);
}

void PokewalkerApp_SelectMon_SetGraphicsModes(BgConfig *bgConfig) {
    GraphicsModes modes = sSelectMonGraphicsModes;
    SetBothScreensModesAndDisable(&modes);
}

void PokewalkerApp_SelectMon_InitObjTransfer(void) {
    ObjCharTransferTemplate template = sSelectMonObjCharTransferTemplate;
    ObjCharTransfer_Init(&template);
    ObjPlttTransfer_Init(20, HEAP_ID_154);
    ObjCharTransfer_ClearBuffers();
    ObjPlttTransfer_Reset();
}

void PokewalkerApp_SelectMon_InitOam(PokewalkerSelectMonWork *work) {
    NNS_G2dInitOamManagerModule();
    OamManager_Create(0, 126, 0, 32, 0, 126, 0, 32, HEAP_ID_154);
}

BOOL PokewalkerApp_SelectMon_GetBoxMon(PCStorage *storage, u32 box, u32 slot, PokewalkerBoxMon *mon) {
    BoxPokemon *boxMon = PCStorage_GetMonByIndexPair(storage, box, slot);
    if (GetBoxMonData(boxMon, MON_DATA_SPECIES_EXISTS, NULL)) {
        mon->species = GetBoxMonData(boxMon, MON_DATA_SPECIES, NULL);
        mon->personality = GetBoxMonData(boxMon, MON_DATA_PERSONALITY, NULL);
        mon->isEgg = GetBoxMonData(boxMon, MON_DATA_IS_EGG, NULL);
        mon->form = GetBoxMonData(boxMon, MON_DATA_FORM, NULL);
        mon->heldItem = GetBoxMonData(boxMon, MON_DATA_HELD_ITEM, NULL);
        mon->unk_0E = 0;
        GetBoxMonData(boxMon, MON_DATA_NICKNAME, mon->nickname);
        mon->isShiny = BoxMonIsShiny(boxMon);
        mon->gender = GetBoxMonData(boxMon, MON_DATA_GENDER, NULL);
        mon->level = GetBoxMonData(boxMon, MON_DATA_LEVEL, NULL);
        mon->markings = GetBoxMonData(boxMon, MON_DATA_MARKINGS, NULL);
        return TRUE;
    }
    mon->species = 0;
    mon->personality = 0;
    mon->isEgg = 0;
    mon->form = 0;
    mon->heldItem = 0;
    mon->unk_0E = 0;
    mon->isShiny = 0;
    mon->gender = 0;
    return FALSE;
}

// Only what the box icons need.
BOOL PokewalkerApp_SelectMon_GetBoxMonIcon(PCStorage *storage, u32 box, u32 slot, PokewalkerBoxMon *mon) {
    BoxPokemon *boxMon = PCStorage_GetMonByIndexPair(storage, box, slot);
    if (GetBoxMonData(boxMon, MON_DATA_SPECIES_EXISTS, NULL)) {
        mon->species = GetBoxMonData(boxMon, MON_DATA_SPECIES, NULL);
        mon->personality = 0;
        mon->isEgg = GetBoxMonData(boxMon, MON_DATA_IS_EGG, NULL);
        mon->form = GetBoxMonData(boxMon, MON_DATA_FORM, NULL);
        mon->heldItem = 0;
        mon->unk_0E = 0;
        mon->isShiny = 0;
        mon->gender = 0;
        mon->level = 0;
        mon->markings = 0;
        return TRUE;
    }
    mon->species = 0;
    mon->personality = 0;
    mon->isEgg = 0;
    mon->form = 0;
    mon->heldItem = 0;
    mon->unk_0E = 0;
    mon->isShiny = 0;
    mon->gender = 0;
    mon->level = 0;
    mon->markings = 0;
    return FALSE;
}

void PokewalkerApp_SelectMon_GetBoxName(String *dest, PCStorage *storage, u32 box) {
    PCStorage_GetBoxName(storage, box, dest);
}

void PokewalkerApp_SelectMon_VBlankCB(PokewalkerSelectMonWork *work) {
    sub_02093594(work->boxSelector);
}

void PokewalkerApp_SelectMon_StoreSelection(PokewalkerSelectMonWork *work, PokewalkerApp *app) {
    if (work->cancelled == FALSE) {
        u32 box = work->box;
        if (box == 18) {
            Pokemon *mon = Party_GetMonByIndex(SaveArray_Party_Get(app->saveData), work->slot);
            work->species = GetMonData(mon, MON_DATA_SPECIES, NULL);
            work->form = GetMonData(mon, MON_DATA_FORM, NULL);
            GetMonData(mon, MON_DATA_NICKNAME, work->nickname);
            work->isShiny = MonIsShiny(mon);
            work->gender = GetMonData(mon, MON_DATA_GENDER, NULL);
            app->unk_1E42C = mon;
            app->strollMon = NULL;
        } else {
            PCStorage *storage = SaveArray_PCStorage_Get(app->saveData);
            BoxPokemon *boxMon = PCStorage_GetMonByIndexPair(storage, box, work->slot);
            PokewalkerBoxMon boxMonInfo;
            if (GetBoxMonData(boxMon, MON_DATA_SPECIES_EXISTS, NULL) != 0) {
                app->strollMon = boxMon;
                app->unk_1E42C = NULL;
            }
            storage = SaveArray_PCStorage_Get(app->saveData);
            PokewalkerApp_SelectMon_GetBoxMon(storage, work->box, work->slot, &boxMonInfo);
            work->species = boxMonInfo.species;
            work->form = boxMonInfo.form;
            CopyU16StringArrayN(work->nickname, boxMonInfo.nickname, POKEMON_NAME_LENGTH + 1);
            work->isShiny = boxMonInfo.isShiny;
            work->gender = boxMonInfo.gender;
        }
    }
}

void PokewalkerApp_SelectMon_ShowMonInfo(int box, int slot, PokewalkerApp *app) {
    PokewalkerSelectMonWork *work = (PokewalkerSelectMonWork *)&app->selectMonState;
    PokewalkerSelectMonInfo info;

    if (box == -1 || slot == -1) {
        goto noMon;
    }

    info.state = 2;
    if (box == 18) {
        Pokemon *mon = Party_GetMonByIndex(SaveArray_Party_Get(app->saveData), slot);
        u8 i;
        s32 markings;
        info.personality = GetMonData(mon, MON_DATA_PERSONALITY, NULL);
        info.species = GetMonData(mon, MON_DATA_SPECIES, NULL);
        info.form = GetMonData(mon, MON_DATA_FORM, NULL);
        GetMonData(mon, MON_DATA_NICKNAME, info.nickname);
        info.isShiny = MonIsShiny(mon);
        info.gender = GetMonData(mon, MON_DATA_GENDER, NULL);
        markings = GetMonData(mon, MON_DATA_MARKINGS, NULL);
        for (i = 0; i < 6; i++) {
            if ((markings >> i) & 1) {
                info.markings[i] = 1;
            } else {
                info.markings[i] = 0;
            }
        }
        info.heldItem = GetMonData(mon, MON_DATA_HELD_ITEM, NULL);
        info.level = GetMonData(mon, MON_DATA_LEVEL, NULL);
        ov112_021F04DC(work->boxDisplay, &info);
        return;
    } else {
        s32 markings;
        u8 i;
        PokewalkerBoxMon boxMonInfo;
        PokewalkerApp_SelectMon_GetBoxMon(SaveArray_PCStorage_Get(app->saveData), box, slot, &boxMonInfo);
        info.personality = boxMonInfo.personality;
        info.species = boxMonInfo.species;
        info.form = boxMonInfo.form;
        CopyU16StringArrayN(info.nickname, boxMonInfo.nickname, POKEMON_NAME_LENGTH + 1);
        info.isShiny = boxMonInfo.isShiny;
        info.gender = boxMonInfo.gender;
        markings = boxMonInfo.markings;
        for (i = 0; i < 6; i++) {
            if ((markings >> i) & 1) {
                info.markings[i] = 1;
            } else {
                info.markings[i] = 0;
            }
        }
        info.heldItem = boxMonInfo.heldItem;
        info.level = boxMonInfo.level;
        ov112_021F04DC(work->boxDisplay, &info);
        return;
    }

noMon:
    info.state = 0;
    ov112_021F04DC(work->boxDisplay, &info);
}

int PokewalkerApp_EventLog_Enter(PokewalkerApp *app) {
    app->eventLogSaveData = app->saveData;
    app->eventLogRecords = app->irStatus.records;
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_WHITE, 6, 1, HEAP_ID_154);
    GF_SndStartFadeOutBGM(0, 16);
    return 2;
}

int PokewalkerApp_EventLog_Update(PokewalkerApp *app) {
    switch (app->state) {
    case 0:
        if (IsPaletteFadeFinished() == TRUE) {
            PokewalkerApp_Teardown(app);
            if (PokewalkerApp_IsEventLogInvalid(app->irStatus.records)) {
                return 3;
            }
            app->state = 1;
        }
        break;
    case 1:
        app->childApp = OverlayManager_New(&sEventLogAppTemplate, &app->eventLogSaveData, HEAP_ID_154);
        app->state = 2;
        break;
    case 2:
        if (OverlayManager_Run(app->childApp)) {
            OverlayManager_Delete(app->childApp);
            app->state = 3;
        }
        break;
    case 3:
        return 3;
    }
    return 2;
}

int PokewalkerApp_EventLog_Exit(PokewalkerApp *app) {
    PokewalkerApp_Setup(app);
    app->state = 0;
    app->nextMode = 10;
    return 1;
}

int PokewalkerApp_WaitKey_Enter(PokewalkerApp *app) {
    return 2;
}

int PokewalkerApp_WaitKey_Update(PokewalkerApp *app) {
    if (gSystem.newKeys & PAD_KEY_DOWN) {
        app->nextMode = 3;
        return 3;
    }
    return 2;
}

int PokewalkerApp_WaitKey_Exit(PokewalkerApp *app) {
    return 1;
}

int PokewalkerApp_StrollReport_Enter(PokewalkerApp *app) {
    PokewalkerApp_RemoveWindowBank(app, 1);
    PokewalkerApp_AddWindowBank(app, 4);
    return 2;
}

int PokewalkerApp_StrollReport_Update(PokewalkerApp *app) {
    int state = sStrollReportStates[app->state](app);
    app->state = state;
    if (state == 37) {
        app->state = 0;
        return 3;
    }
    return 2;
}

int PokewalkerApp_StrollReport_Exit(PokewalkerApp *app) {
    FillBgTilemapRect(app->bgConfig, 1, 0, 0, 0, 32, 24, 16);
    FillBgTilemapRect(app->bgConfig, 2, 0, 0, 0, 32, 24, 16);
    BgCommitTilemapBufferToVram(app->bgConfig, 1);
    BgCommitTilemapBufferToVram(app->bgConfig, 2);
    PokewalkerApp_RemoveWindowBank(app, 6);
    PokewalkerApp_AddWindowBank(app, 1);
    return 1;
}
