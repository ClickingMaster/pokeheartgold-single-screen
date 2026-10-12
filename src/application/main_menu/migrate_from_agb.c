#include "application/main_menu/migrate_from_agb.h"

#include "global.h"

#include "constants/sndseq.h"

#include "application/main_menu/agb_cartridge.h"
#include "application/main_menu/main_menu_shared.h"
#include "msgdata/msg.naix"

#include "bg_window.h"
#include "config.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "item.h"
#include "main.h"
#include "message_format.h"
#include "msgdata.h"
#include "oam_manager.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "overlay_manager.h"
#include "palPark_migration.h"
#include "player_data.h"
#include "pm_string.h"
#include "pokemon.h"
#include "pokemon_icon_idx.h"
#include "render_window.h"
#include "save.h"
#include "save_arrays.h"
#include "save_data_read_error.h"
#include "screen_fade.h"
#include "sound_playback.h"
#include "sound_system.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "system.h"
#include "title_screen.h"
#include "touchscreen.h"
#include "unk_02009D48.h"
#include "sprite_resource.h"
#include "vram_transfer_manager.h"
#include "yes_no_prompt.h"

// Declared here rather than through text.h / render_text.h: the text parameters keep the font id in an int that is
// passed unconverted (text.h types it as the u8 FontID; matching).
extern u8 AddTextPrinterParameterizedWithColor(Window *window, u32 fontId, String *string, u32 x, u32 y, u32 textSpeed, u32 color, void *callback);
extern void ResetAllTextPrinters(void);
extern u8 TextPrinterCheckActive(u8 printerId);
extern void TextFlags_SetCanTouchSpeedUpPrint(BOOL enable);

// Overlay helpers (not in a header yet)
BOOL CTRDG_IsExisting(void);
BOOL CTRDG_IsPulledOut(void);
void CTRDG_ReadAgbFlash(u16 sector, u32 offset, void *dest, u32 size);
BOOL NNS_G2dGetUnpackedBGCharacterData(void *file, NNSG2dCharacterData **charData);
BOOL OS_IsTickAvailable(void);
void OS_InitTick(void);
u64 OS_GetTick(void);
void Sys_SetSoftResetDisableFlag(int a0);
void Sys_ClearSoftResetDisableFlag(int a0);

extern int _02111864;
extern const ObjCharTransferTemplate _0223B4E8;
extern const GraphicsModes _0223B4F8;
extern const BgTemplate _0223B508;
extern const BgTemplate _0223B524;
extern const BgTemplate _0223B540;
extern const BgTemplate _0223B55C;
extern const GraphicsBanks _0223B578;
extern const u8 ov74_0223C960[];
extern const int ov74_0223C968[];
extern const int ov74_0223C980[];
extern const u32 sMigrateFromAgbHMMoves[];
extern const u16 ov74_0223C9A8[];
extern const int ov74_0223C9B8[];
extern const u16 sMigrateFromAgbBlockedItems[];
extern MigrateFromAgbIcon *sMigrateFromAgbIcons;

BOOL MigrateFromAgb_IsSpeciesUnavailable(int species);
void MigrateFromAgb_GetAgbVersion(int *version);
void MigrateFromAgb_SaveMigrationRecord(MigrateFromAgbApp *app);
void MigrateFromAgb_MovePicksToPalPark(MigrateFromAgbApp *app);
int MigrateFromAgb_GetTextX(MigrateFromAgbText *text, String *string, int flags);
void MigrateFromAgb_InitObjTransfer(void);
void MigrateFromAgb_InitSpriteResources(MigrateFromAgbApp *app);
void MigrateFromAgb_InitSpriteHeader(MigrateFromAgbApp *app);
void MigrateFromAgb_InitSpriteTemplate(SpriteTemplate *template, MigrateFromAgbApp *app, int unused, NNS_G2D_VRAM_TYPE whichScreen);
u32 MigrateFromAgb_GetBoxMonSpecies(MigrateFromAgbApp *app, int box, int slot);
u32 MigrateFromAgb_GetBoxMonIsEgg(MigrateFromAgbApp *app, int box, int slot);
u32 MigrateFromAgb_GetBoxMonPersonality(MigrateFromAgbApp *app, int box, int slot);
void MigrateFromAgb_SetHitbox(TouchscreenHitbox *hitbox, int x, int y, int width, int height);
void *MigrateFromAgb_LoadCharData(NarcId narcId, int fileId, NNSG2dCharacterData **charData, enum HeapID heapId);
BOOL MigrateFromAgb_ReadCharData(int fileId, NNSG2dCharacterData **charData, void *file, NARC *narc);
int MigrateFromAgb_GetMonForm(int species, u32 personality, int agbVersion);
void MigrateFromAgb_LoadMonIcon(int species, int isEgg, u32 personality, int agbVersion, int index, Sprite *sprite);
void MigrateFromAgb_PrepareBoxIcon(int species, int isEgg, int form, int index, Sprite *sprite, void *file, NARC *narc);
void MigrateFromAgb_UploadBoxIcons(void);
void MigrateFromAgb_ShowBox(MigrateFromAgbApp *app);
void MigrateFromAgb_CreateBoxSprites(MigrateFromAgbApp *app);
Sprite *MigrateFromAgb_CreateSprite(MigrateFromAgbApp *app, int x, int y, int anim, BOOL draw);
void MigrateFromAgb_CreateButtons(MigrateFromAgbApp *app);
BOOL MigrateFromAgb_IsEgg(MigrateFromAgbApp *app, int slot);
BOOL MigrateFromAgb_KnowsHM(MigrateFromAgbApp *app, int slot);
BOOL MigrateFromAgb_HoldsBlockedItem(MigrateFromAgbApp *app, int slot);
BOOL MigrateFromAgb_IsBoxMonSpeciesUnavailable(MigrateFromAgbApp *app, int slot);
int MigrateFromAgb_TogglePick(MigrateFromAgbApp *app, int slot);
void MigrateFromAgb_ShowBoxPicks(MigrateFromAgbApp *app);
void MigrateFromAgb_PrintBoxName(MigrateFromAgbApp *app, int box);
void MigrateFromAgb_ShowMarkings(MigrateFromAgbApp *app, AgbBoxMon *mon);
void MigrateFromAgb_PrintMonInfo(MigrateFromAgbApp *app, AgbBoxMon *mon);
void MigrateFromAgb_StartFadeBlack(MigrateFromAgbApp *app, enum FadeType fadeType, int nextState, int *state);
void MigrateFromAgb_StartFadeWhite(MigrateFromAgbApp *app, enum FadeType fadeType, int nextState, int *state);
void MigrateFromAgb_InitBgs(BgConfig *bgConfig);
void MigrateFromAgb_InitGraphics(MigrateFromAgbApp *app);
void MigrateFromAgb_LoadBoxBg(MigrateFromAgbApp *app);
void MigrateFromAgb_InitMsgBox(MigrateFromAgbApp *app);
void MigrateFromAgb_ShowConfirmScreen(MigrateFromAgbApp *app);
void MigrateFromAgb_RemoveWindow(Window *window);
void MigrateFromAgb_CloseConfirmScreen(MigrateFromAgbApp *app);
void MigrateFromAgb_FreeGraphics(MigrateFromAgbApp *app);
void MigrateFromAgb_ShowYesNo(MigrateFromAgbApp *app);
void MigrateFromAgb_PrintIntro(MigrateFromAgbApp *app);
void MigrateFromAgb_VBlankCB(MigrateFromAgbApp *app);
int MigrateFromAgb_CheckCanMigrate(MigrateFromAgbApp *app);

BOOL MigrateFromAgb_IsSpeciesUnavailable(int species);
void MigrateFromAgb_GetAgbVersion(int *version);
int MigrateFromAgb_RunTransfer(MigrateFromAgbApp *app);
void MigrateFromAgb_SaveMigrationRecord(MigrateFromAgbApp *app);
void MigrateFromAgb_MovePicksToPalPark(MigrateFromAgbApp *app);
int MigrateFromAgb_GetTextX(MigrateFromAgbText *text, String *string, int flags);
void MigrateFromAgb_PrintText(MigrateFromAgbApp *app, MigrateFromAgbText *text, int flags);
void MigrateFromAgb_InitObjTransfer(void);
void MigrateFromAgb_InitSpriteResources(MigrateFromAgbApp *app);
void MigrateFromAgb_InitSpriteHeader(MigrateFromAgbApp *app);
void MigrateFromAgb_InitSpriteTemplate(SpriteTemplate *template, MigrateFromAgbApp *app, int unused, NNS_G2D_VRAM_TYPE whichScreen);
u32 MigrateFromAgb_GetBoxMonSpecies(MigrateFromAgbApp *app, int box, int slot);
u32 MigrateFromAgb_GetBoxMonIsEgg(MigrateFromAgbApp *app, int box, int slot);
u32 MigrateFromAgb_GetBoxMonPersonality(MigrateFromAgbApp *app, int box, int slot);
void MigrateFromAgb_SetHitbox(TouchscreenHitbox *hitbox, int x, int y, int width, int height);
void *MigrateFromAgb_LoadCharData(NarcId narcId, int fileId, NNSG2dCharacterData **charData, enum HeapID heapId);
BOOL MigrateFromAgb_ReadCharData(int fileId, NNSG2dCharacterData **charData, void *file, NARC *narc);
int MigrateFromAgb_GetMonForm(int species, u32 personality, int agbVersion);
void MigrateFromAgb_LoadMonIcon(int species, int isEgg, u32 personality, int agbVersion, int index, Sprite *sprite);
void MigrateFromAgb_PrepareBoxIcon(int species, int isEgg, int form, int index, Sprite *sprite, void *file, NARC *narc);
void MigrateFromAgb_UploadBoxIcons(void);
void MigrateFromAgb_ShowBox(MigrateFromAgbApp *app);
void MigrateFromAgb_CreateBoxSprites(MigrateFromAgbApp *app);
Sprite *MigrateFromAgb_CreateSprite(MigrateFromAgbApp *app, int x, int y, int anim, BOOL draw);
void MigrateFromAgb_CreateButtons(MigrateFromAgbApp *app);
BOOL MigrateFromAgb_IsEgg(MigrateFromAgbApp *app, int slot);
BOOL MigrateFromAgb_KnowsHM(MigrateFromAgbApp *app, int slot);
BOOL MigrateFromAgb_HoldsBlockedItem(MigrateFromAgbApp *app, int slot);
BOOL MigrateFromAgb_IsBoxMonSpeciesUnavailable(MigrateFromAgbApp *app, int slot);
int MigrateFromAgb_TogglePick(MigrateFromAgbApp *app, int slot);
void MigrateFromAgb_ShowBoxPicks(MigrateFromAgbApp *app);
void MigrateFromAgb_PrintBoxName(MigrateFromAgbApp *app, int box);
void MigrateFromAgb_ShowMarkings(MigrateFromAgbApp *app, AgbBoxMon *mon);
void MigrateFromAgb_PrintMonInfo(MigrateFromAgbApp *app, AgbBoxMon *mon);
void MigrateFromAgb_StartFadeBlack(MigrateFromAgbApp *app, enum FadeType fadeType, int nextState, int *state);
void MigrateFromAgb_StartFadeWhite(MigrateFromAgbApp *app, enum FadeType fadeType, int nextState, int *state);
void MigrateFromAgb_InitBgs(BgConfig *bgConfig);
void MigrateFromAgb_InitGraphics(MigrateFromAgbApp *app);
void MigrateFromAgb_LoadBoxBg(MigrateFromAgbApp *app);
void MigrateFromAgb_InitMsgBox(MigrateFromAgbApp *app);
void MigrateFromAgb_ShowConfirmScreen(MigrateFromAgbApp *app);
void MigrateFromAgb_RemoveWindow(Window *window);
void MigrateFromAgb_CloseConfirmScreen(MigrateFromAgbApp *app);
void MigrateFromAgb_FreeGraphics(MigrateFromAgbApp *app);
void MigrateFromAgb_ShowYesNo(MigrateFromAgbApp *app);
void MigrateFromAgb_PrintIntro(MigrateFromAgbApp *app);
void MigrateFromAgb_VBlankCB(MigrateFromAgbApp *app);
int MigrateFromAgb_CheckCanMigrate(MigrateFromAgbApp *app);
BOOL MigrateFromAgb_PrintResultMsg(MigrateFromAgbApp *app, int index);
BOOL MigrateFromAgb_PrintResultMsgWithYesNo(MigrateFromAgbApp *app, int index);
BOOL MigrateFromAgb_PrintPendingMsg(MigrateFromAgbApp *app, int textSpeed);

#pragma force_active on

// Species the DP games can't take (none, the unused GBA indexes, out of range)
BOOL MigrateFromAgb_IsSpeciesUnavailable(int species) {
    if (species <= 0) {
        return TRUE;
    }
    if (species <= 251) {
        return FALSE;
    }
    if (species < 277) {
        return TRUE;
    }
    if (species > 411) {
        return TRUE;
    }
    return FALSE;
}

void MigrateFromAgb_GetAgbVersion(int *version) {
    switch (PmAgbCartridge_GetVersionInternal()) {
    case 0:
        *version = VERSION_RUBY;
        break;
    case 1:
        *version = VERSION_SAPPHIRE;
        break;
    case 2:
        *version = VERSION_LEAFGREEN;
        break;
    case 3:
        *version = VERSION_FIRERED;
        break;
    case 4:
        *version = VERSION_EMERALD;
        break;
    default:
        *version = -1;
        break;
    }
}

// Sends the picked Pokemon to the Pal Park, saves, then erases them from the cartridge: returns 10 while busy, 11
// when done, 12 on failure
int MigrateFromAgb_RunTransfer(MigrateFromAgbApp *app) {
    MigrateFromAgbSave *save = &app->save;
    u8 buffer[16];
    int result;
    switch (save->state) {
    case 0: {
        MigratedPokemon *migrated;
        MigrateFromAgb_MovePicksToPalPark(app);
        migrated = Save_MigratedPokemon_Get(app->saveData);
        MigratedPokemon_RecordMigration(migrated, PmAgbCartridge_GetTrainerId());
        save->state++;
        break;
    }
    case 1:
        save->delay = OS_GetTick() % 120 + 1;
        save->state++;
        break;
    case 2:
        if (--save->delay == 0) {
            save->state++;
        }
        break;
    case 3:
        Save_PrepareForAsyncWrite(app->saveData, 2);
        save->state++;
        break;
    case 4:
        result = Save_WriteFileAsync(app->saveData);
        if (result == 3) {
            return 12;
        }
        if (result == 1) {
            save->state++;
        }
        break;
    case 5:
        CTRDG_ReadAgbFlash(0, 0, buffer, 16);
        save->state++;
        break;
    case 6:
        if (!PmAgbCartridge_StartSave()) {
            Sys_ClearSleepDisableFlag(1);
            return 12;
        }
        save->state++;
        break;
    case 7:
        if (PmAgbCartridge_GetSaveStatus() == PM_AGB_RESULT_WRITE_STARTING) {
            break;
        }
        if (PmAgbCartridge_GetSaveStatus() == PM_AGB_RESULT_WRITE_LAST) {
            save->state++;
            break;
        }
        if (PmAgbCartridge_ContinueSave() == PM_AGB_RESULT_WRITE_FAILED) {
            Sys_ClearSleepDisableFlag(1);
            return 12;
        }
        break;
    case 8:
        do {
            result = Save_WriteFileAsync(app->saveData);
            if (result == 3) {
                return 12;
            }
        } while (result != 2);
        PmAgbCartridge_ContinueSave();
        Sys_SetSleepDisableFlag(1);
        save->state++;
        break;
    case 9:
        result = PmAgbCartridge_ContinueSave();
        if (result == PM_AGB_RESULT_WRITE_FAILED) {
            Sys_ClearSleepDisableFlag(1);
            return 12;
        }
        if (result == PM_AGB_RESULT_OK) {
            Sys_ClearSleepDisableFlag(1);
            return 11;
        }
        break;
    }
    return 10;
}

#pragma force_active reset

void MigrateFromAgb_SaveMigrationRecord(MigrateFromAgbApp *app) {
    MigratedPokemon *migrated = Save_MigratedPokemon_Get(app->saveData);
    u32 trainerId = PmAgbCartridge_GetTrainerId();
    MigratedPokemon_RecordMigration(migrated, trainerId);
    Sys_SetSoftResetDisableFlag(4);
    SaveGameNormal(app->saveData);
    Sys_ClearSoftResetDisableFlag(4);
}

#pragma force_active on

void MigrateFromAgb_MovePicksToPalPark(MigrateFromAgbApp *app) {
    Pokemon mon;
    u16 noSpecies;
    MigratedPokemon *migrated = Save_MigratedPokemon_Get(app->saveData);
    BoxPokemon *boxMon = Mon_GetBoxMon(&mon);
    int i;
    for (i = 0; i < MIGRATE_FROM_AGB_MON_COUNT; i++) {
        MigrateBoxMon(&app->storage->boxes[app->picks[i].box][app->picks[i].slot], boxMon);
        MigratedPokemon_CopyBoxPokemonToSlot(migrated, boxMon, i);
    }
    noSpecies = 0;
    for (i = 0; i < MIGRATE_FROM_AGB_MON_COUNT; i++) {
        int slot = app->picks[i].slot, box = app->picks[i].box;
        if (slot != -1 && box != AGB_BOX_COUNT) {
            AGB_SetBoxMonData(&app->storage->boxes[box][slot], AGB_MON_DATA_SPECIES, &noSpecies);
        }
    }
}

#pragma force_active reset

int MigrateFromAgb_GetTextX(MigrateFromAgbText *text, String *string, int flags) {
    if (flags & 1) {
        int width = FontID_String_GetWidth(1, string, 0);
        return (text->width * 8 - width) / 2;
    }
    return text->printX;
}

#pragma force_active on

// flags: 1 center, 2 keep the window contents, 4 don't copy to VRAM, 8 draw the frame (16: the message box frame)
void MigrateFromAgb_PrintText(MigrateFromAgbApp *app, MigrateFromAgbText *text, int flags) {
    String *string;
    int textSpeed = text->textSpeed;
    if (!textSpeed) {
        textSpeed = 0xFF;
    }
    if (text->window->bgConfig == NULL) {
        AddWindowParameterized(app->bgConfig, text->window, text->bgId, text->x, text->y, text->width, text->height, 15, text->baseTile);
    }
    if (!(flags & 2)) {
        FillWindowPixelBuffer(text->window, text->fillColor);
    }
    if (text->msgId != -1) {
        MsgData *msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0301_bin, HEAP_ID_76);
        MessageFormat *msgFormat = text->msgFormat;
        String *raw;
        if (!msgFormat) {
            msgFormat = MessageFormat_New(HEAP_ID_76);
        }
        String_SetEmpty(app->string);
        string = app->string;
        raw = NewString_ReadMsgData(msgData, text->msgId);
        StringExpandPlaceholders(msgFormat, app->string, raw);
        String_Delete(raw);
        text->printerId = AddTextPrinterParameterizedWithColor(text->window, text->fontId, string, MigrateFromAgb_GetTextX(text, string, flags), text->printY, textSpeed, text->color, NULL);
        if (text->msgFormat == NULL) {
            MessageFormat_Delete(msgFormat);
        }
        DestroyMsgData(msgData);
        text->msgId = -1;
    }
    if (text->chars) {
        string = app->rawString;
        CopyU16ArrayToStringN(string, text->chars, 64);
        text->printerId = AddTextPrinterParameterizedWithColor(text->window, text->fontId, string, MigrateFromAgb_GetTextX(text, string, flags), text->printY, textSpeed, text->color, NULL);
        text->chars = NULL;
    }
    if (text->string) {
        text->printerId = AddTextPrinterParameterizedWithColor(text->window, text->fontId, text->string, MigrateFromAgb_GetTextX(text, text->string, flags), text->printY, textSpeed, text->color, NULL);
        text->string = NULL;
    }
    if (!(flags & 4)) {
        CopyWindowToVram(text->window);
    }
    if (flags & 8) {
        if (flags & 16) {
            DrawFrameAndWindow2(text->window, FALSE, 0x3D2, 13);
        } else {
            DrawFrameAndWindow1(text->window, FALSE, 0x3F0, 14);
        }
    }
}

#pragma force_active reset

void MigrateFromAgb_InitObjTransfer(void) {
    ObjCharTransferTemplate template = _0223B4E8;
    ObjCharTransfer_Init(&template);
    ObjPlttTransfer_Init(20, HEAP_ID_76);
    ObjCharTransfer_ClearBuffers();
    ObjPlttTransfer_Reset();
}

void MigrateFromAgb_InitSpriteResources(MigrateFromAgbApp *app) {
    int i;
    MigrateFromAgbApp *cursor;
    NNS_G2dInitOamManagerModule();
    OamManager_Create(0, 126, 0, 32, 0, 126, 0, 32, HEAP_ID_76);
    app->spriteList = G2dRenderer_Init(80, &app->renderer, HEAP_ID_76);
    G2dRenderer_SetSubSurfaceCoords(&app->renderer, 0, FX32_CONST(256));
    for (i = 0, cursor = app; i < 6; i++, cursor = (MigrateFromAgbApp *)((u8 *)cursor + 4)) {
        cursor->resMan[0] = Create2DGfxResObjMan(3, (GfGfxResType)i, HEAP_ID_76);
    }
    app->resources[GF_GFX_RES_TYPE_CHAR] = AddCharResObjFromNarc(app->resMan[GF_GFX_RES_TYPE_CHAR], NARC_a_1_1_3, 30, TRUE, 0, NNS_G2D_VRAM_TYPE_2DMAIN, HEAP_ID_76);
    app->resources[GF_GFX_RES_TYPE_PLTT] = AddPlttResObjFromNarc(app->resMan[GF_GFX_RES_TYPE_PLTT], NARC_a_1_1_3, 27, FALSE, 0, NNS_G2D_VRAM_TYPE_2DMAIN, 4, HEAP_ID_76);
    app->resources[GF_GFX_RES_TYPE_CELL] = AddCellOrAnimResObjFromNarc(app->resMan[GF_GFX_RES_TYPE_CELL], NARC_a_1_1_3, 29, TRUE, 0, GF_GFX_RES_TYPE_CELL, HEAP_ID_76);
    app->resources[GF_GFX_RES_TYPE_ANIM] = AddCellOrAnimResObjFromNarc(app->resMan[GF_GFX_RES_TYPE_ANIM], NARC_a_1_1_3, 28, TRUE, 0, GF_GFX_RES_TYPE_ANIM, HEAP_ID_76);
    SpriteTransfer_CreateCharTransferTask(app->resources[GF_GFX_RES_TYPE_CHAR]);
    SpriteTransfer_CreateExtPlttTransferTask(app->resources[GF_GFX_RES_TYPE_PLTT]);
    GfGfxLoader_GXLoadPal(NARC_poketool_icongra_poke_icon, GetMonIconPlttNaix(), GF_PAL_LOCATION_MAIN_OBJ, GF_PAL_SLOT_8_OFFSET, 0, HEAP_ID_76);
}

void MigrateFromAgb_InitSpriteHeader(MigrateFromAgbApp *app) {
    CreateSpriteResourcesHeader(&app->resHeader, 0, 0, 0, 0, -1, -1, FALSE, 0, app->resMan[GF_GFX_RES_TYPE_CHAR], app->resMan[GF_GFX_RES_TYPE_PLTT], app->resMan[GF_GFX_RES_TYPE_CELL], app->resMan[GF_GFX_RES_TYPE_ANIM], NULL, NULL);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
}

void MigrateFromAgb_InitSpriteTemplate(SpriteTemplate *template, MigrateFromAgbApp *app, int unused, NNS_G2D_VRAM_TYPE whichScreen) {
    template->spriteList = app->spriteList;
    template->header = &app->resHeader;
    template->position.z = 0;
    template->scale.x = FX32_ONE;
    template->scale.y = FX32_ONE;
    template->scale.z = FX32_ONE;
    template->rotation = 0;
    template->drawPriority = 20;
    template->whichScreen = whichScreen;
    template->heapID = HEAP_ID_76;
}

u32 MigrateFromAgb_GetBoxMonSpecies(MigrateFromAgbApp *app, int box, int slot) {
    AgbPokemonStorage *storage = app->storage;
    return AGB_GetBoxMonData(&storage->boxes[box][slot], AGB_MON_DATA_SPECIES, NULL);
}

u32 MigrateFromAgb_GetBoxMonIsEgg(MigrateFromAgbApp *app, int box, int slot) {
    AgbPokemonStorage *storage = app->storage;
    return AGB_GetBoxMonData(&storage->boxes[box][slot], AGB_MON_DATA_IS_EGG, NULL);
}

u32 MigrateFromAgb_GetBoxMonPersonality(MigrateFromAgbApp *app, int box, int slot) {
    AgbPokemonStorage *storage = app->storage;
    return AGB_GetBoxMonData(&storage->boxes[box][slot], AGB_MON_DATA_PERSONALITY, NULL);
}

void MigrateFromAgb_SetHitbox(TouchscreenHitbox *hitbox, int x, int y, int width, int height) {
    int halfHeight = height / 2;
    int halfWidth;
    hitbox->rect.top = y - halfHeight;
    halfWidth = width / 2;
    hitbox->rect.left = x - halfWidth;
    hitbox->rect.bottom = y + halfHeight;
    hitbox->rect.right = x + halfWidth;
}

void *MigrateFromAgb_LoadCharData(NarcId narcId, int fileId, NNSG2dCharacterData **charData, enum HeapID heapId) {
    void *file = Heap_AllocAtEnd(heapId, 0x1000);
    if (file != NULL) {
        ReadWholeNarcMemberByIdPair(file, narcId, fileId);
        if (!NNS_G2dGetUnpackedBGCharacterData(file, charData)) {
            Heap_Free(file);
            return NULL;
        }
    }
    return file;
}

BOOL MigrateFromAgb_ReadCharData(int fileId, NNSG2dCharacterData **charData, void *file, NARC *narc) {
    NARC_ReadWholeMember(narc, fileId, file);
    return NNS_G2dGetUnpackedBGCharacterData(file, charData);
}

#pragma force_active on

// Unown letter from the personality, Deoxys form from the game
int MigrateFromAgb_GetMonForm(int species, u32 personality, int agbVersion) {
    u8 form = 0;
    switch (species) {
    case SPECIES_UNOWN:
        form = (u8)((((personality & 0x03000000) >> 18) | ((personality & 0x00030000) >> 12) | ((personality & 0x00000300) >> 6) | (personality & 3)) % 28);
        break;
    case SPECIES_DEOXYS:
        switch (agbVersion) {
        default:
        case 0:
        case 1:
        case 2:
            form = 0;
            break;
        case 4:
            form = 1;
            break;
        case 5:
            form = 2;
            break;
        case 3:
            form = 3;
            break;
        }
        break;
    }
    return form;
}

void MigrateFromAgb_LoadMonIcon(int species, int isEgg, u32 personality, int agbVersion, int index, Sprite *sprite) {
    NNSG2dCharacterData *charData;
    int dpSpecies = TranslateAgbSpecies(species);
    int form = MigrateFromAgb_GetMonForm(dpSpecies, personality, agbVersion);
    void *file = MigrateFromAgb_LoadCharData(NARC_poketool_icongra_poke_icon, GetMonIconNaixEx(dpSpecies, isEgg, form), &charData, HEAP_ID_76);
    DC_FlushRange(charData->pRawData, 0x200);
    GX_LoadOBJ(charData->pRawData, ((index << 4) + 100) << 5, 0x200);
    Sprite_SetPaletteOverride(sprite, GetMonIconPaletteEx(dpSpecies, form, isEgg) + 8);
    Heap_Free(file);
}

#pragma force_active reset

#pragma force_active on

// Loads a box slot's icon into sMigrateFromAgbIcons[index] for the VBlank upload (MigrateFromAgb_UploadBoxIcons)
void MigrateFromAgb_PrepareBoxIcon(int species, int isEgg, int form, int index, Sprite *sprite, void *file, NARC *narc) {
    MigrateFromAgbIcon *icon = sMigrateFromAgbIcons + index;
    NNSG2dCharacterData *charData;
    if (sprite) {
        if (MigrateFromAgb_IsSpeciesUnavailable(species) == FALSE) {
            species = TranslateAgbSpecies(species);
        } else {
            species = SPECIES_NONE;
        }
        MigrateFromAgb_ReadCharData(GetMonIconNaixEx(species, isEgg, form), &charData, file, narc);
        MIi_CpuCopyFast(charData->pRawData, icon->charData, 0x200);
        icon->vramOffset = (index * 16 + 100) << 5;
        icon->sprite = sprite;
        icon->palette = GetMonIconPaletteEx(species, form, isEgg) + 8;
    } else {
        icon->sprite = NULL;
    }
}

#pragma force_active reset

void MigrateFromAgb_UploadBoxIcons(void) {
    int i;
    MigrateFromAgbIcon *icon = sMigrateFromAgbIcons;
    for (i = 0; i < AGB_BOX_MON_COUNT; i++, icon++) {
        if (icon->sprite != NULL) {
            DC_FlushRange(icon->charData, 0x200);
            GX_LoadOBJ(icon->charData, icon->vramOffset, 0x200);
            Sprite_SetPaletteOverride(icon->sprite, icon->palette);
        }
    }
}

#pragma force_active on

// Shows the current box
void MigrateFromAgb_ShowBox(MigrateFromAgbApp *app) {
    int species, isEgg, form;
    void *file;
    NARC *narc;
    int i;
    narc = NARC_New(NARC_poketool_icongra_poke_icon, HEAP_ID_76);
    file = Heap_AllocAtEnd(HEAP_ID_76, 0x1000);
    for (i = 0; i < AGB_BOX_MON_COUNT; i++) {
        if (AGB_GetBoxMonData(&app->storage->boxes[app->box][i], AGB_MON_DATA_HAS_SPECIES, NULL)) {
            species = MigrateFromAgb_GetBoxMonSpecies(app, app->box, i);
            isEgg = MigrateFromAgb_GetBoxMonIsEgg(app, app->box, i);
            form = MigrateFromAgb_GetBoxMonPersonality(app, app->box, i);
            form = MigrateFromAgb_GetMonForm(TranslateAgbSpecies(species), form, gSystem.agbCartridgeVersion);
            MigrateFromAgb_PrepareBoxIcon(species, isEgg, form, i, app->slots[i].mon, file, narc);
            Sprite_SetDrawFlag(app->slots[i].mon, TRUE);
            if (AGB_GetBoxMonData(&app->storage->boxes[app->box][i], AGB_MON_DATA_HELD_ITEM, NULL)) {
                Sprite_SetDrawFlag(app->slots[i].item, TRUE);
            } else {
                Sprite_SetDrawFlag(app->slots[i].item, FALSE);
            }
        } else {
            MigrateFromAgb_PrepareBoxIcon(species, isEgg, form, i, NULL, file, narc);
            Sprite_SetDrawFlag(app->slots[i].mon, FALSE);
            Sprite_SetDrawFlag(app->slots[i].item, FALSE);
        }
    }
    Heap_Free(file);
    NARC_Delete(narc);
    app->vblankCallback = MigrateFromAgb_UploadBoxIcons;
    MigrateFromAgb_PrintBoxName(app, app->box);
    MigrateFromAgb_ShowBoxPicks(app);
}

#pragma push
#pragma opt_strength_reduction off

// The 6x5 box grid: a mon icon and a held item icon per slot
void MigrateFromAgb_CreateBoxSprites(MigrateFromAgbApp *app) {
    SpriteTemplate template;

    volatile struct {
        int y;
        int row;
        int col;
    } loops;

    int x, index, yPos;
    TouchscreenHitbox *hitbox;
    MigrateFromAgb_InitSpriteTemplate(&template, app, (int)&app->resHeader, NNS_G2D_VRAM_TYPE_2DMAIN);
    loops.y = 40;
    index = 0;
    loops.row = 0;
    hitbox = app->hitboxes;
    for (;;) {
        loops.col = 0;
        yPos = loops.y << FX32_SHIFT;
        x = 28;
        for (;;) {
            MigrateFromAgb_SetHitbox(hitbox, x, loops.y, 28, 28);
            template.position.x = x << FX32_SHIFT;
            template.position.y = yPos;
            if (app->slots[0].mon == NULL) {
                app->slots[0].mon = Sprite_CreateAffine(&template);
            }
            Sprite_SetAnimActiveFlag(app->slots[0].mon, TRUE);
            Sprite_SetAnimCtrlSeq(app->slots[0].mon, index + 10);
            Sprite_SetPriority(app->slots[0].mon, 1);
            Sprite_SetDrawFlag(app->slots[0].mon, TRUE);
            Sprite_SetDrawPriority(app->slots[0].mon, index + 100);
            template.position.x += FX32_CONST(6);
            template.position.y += FX32_CONST(12);
            if (app->slots[0].item == NULL) {
                app->slots[0].item = Sprite_CreateAffine(&template);
            }
            Sprite_SetAnimActiveFlag(app->slots[0].item, TRUE);
            Sprite_SetAnimCtrlSeq(app->slots[0].item, 40);
            Sprite_SetPriority(app->slots[0].item, 1);
            Sprite_SetDrawFlag(app->slots[0].item, FALSE);
            Sprite_SetDrawPriority(app->slots[0].item, index);
            index++;
            hitbox++;
            // walks the slots by advancing the work pointer
            app = (MigrateFromAgbApp *)((u8 *)app + sizeof(MigrateFromAgbSlotSprites));
            if (index == AGB_BOX_MON_COUNT) {
                goto end;
            }
            {
                int next = loops.col + 1;
                x += 40;
                loops.col = next;
                if (next >= 6) {
                    break;
                }
            }
        }
        loops.y += 24;
        {
            int next = loops.row + 1;
            loops.row = next;
            if (next >= 5) {
                break;
            }
        }
    }
end:;
}

#pragma pop
#pragma force_active reset

#pragma force_active on

Sprite *MigrateFromAgb_CreateSprite(MigrateFromAgbApp *app, int x, int y, int anim, BOOL draw) {
    SpriteTemplate template;
    MigrateFromAgb_InitSpriteTemplate(&template, app, (int)&app->resHeader, NNS_G2D_VRAM_TYPE_2DMAIN);
    template.position.x = x << FX32_SHIFT;
    template.position.y = y << FX32_SHIFT;
    template.drawPriority = 10;
    Sprite *sprite = Sprite_CreateAffine(&template);
    Sprite_SetAnimActiveFlag(sprite, TRUE);
    Sprite_SetAnimCtrlSeq(sprite, anim);
    Sprite_SetPriority(sprite, 1);
    Sprite_SetDrawFlag(sprite, draw);
    return sprite;
}

// The exit / next box / previous box buttons and the picked mon icons
void MigrateFromAgb_CreateButtons(MigrateFromAgbApp *app) {
    MigrateFromAgb_SetHitbox(&app->hitboxes[30], 228, 176, 50, 32);
    app->buttons[0].sprite = MigrateFromAgb_CreateSprite(app, 228, 176, 6, TRUE);
    MigrateFromAgb_SetHitbox(&app->hitboxes[32], 176, 176, 32, 32);
    app->buttons[1].sprite = MigrateFromAgb_CreateSprite(app, 176, 176, 3, TRUE);
    MigrateFromAgb_SetHitbox(&app->hitboxes[31], 24, 176, 32, 32);
    app->buttons[2].sprite = MigrateFromAgb_CreateSprite(app, 24, 176, 0, TRUE);
    app->hitboxes[33].rect.top = TOUCHSCREEN_RECTLIST_END;
    int i = 0;
    int x = 0;
    MigrateFromAgbApp *cursor = app;
    for (; i < MIGRATE_FROM_AGB_MON_COUNT; i++, x += 36, cursor = (MigrateFromAgbApp *)((u8 *)cursor + sizeof(MigrateFromAgbPick))) {
        cursor->picks[0].sprite = MigrateFromAgb_CreateSprite(app, x, 64, 9, FALSE);
        cursor->picks[0].slot = -1;
        cursor->picks[0].box = AGB_BOX_COUNT;
    }
    app->pickCount = 0;
}

#pragma force_active reset

// Pokemon that can't be migrated: eggs, Pokemon knowing an HM (sMigrateFromAgbHMMoves), holding a GBA-only item
// (sMigrateFromAgbBlockedItems), species the DP games lack
BOOL MigrateFromAgb_IsEgg(MigrateFromAgbApp *app, int slot) {
    AgbPokemonStorage *storage = app->storage;
    int box = app->box;
    return AGB_GetBoxMonData(&storage->boxes[box][slot], AGB_MON_DATA_SPECIES_OR_EGG, NULL) == 412;
}

BOOL MigrateFromAgb_KnowsHM(MigrateFromAgbApp *app, int slot) {
    AgbPokemonStorage *storage = app->storage;
    int box = app->box;
    AgbBoxMon *mon = &storage->boxes[box][slot];
    int i;
    u32 j;
    for (i = 0; i < 4; i++) {
        u32 move = AGB_GetBoxMonData(mon, i + AGB_MON_DATA_MOVE1, NULL);
        for (j = 0; j < 8; j++) {
            if (move == sMigrateFromAgbHMMoves[j]) {
                return TRUE;
            }
        }
    }
    return FALSE;
}

BOOL MigrateFromAgb_HoldsBlockedItem(MigrateFromAgbApp *app, int slot) {
    AgbPokemonStorage *storage = app->storage;
    int box = app->box;
    u32 item = AGB_GetBoxMonData(&storage->boxes[box][slot], AGB_MON_DATA_HELD_ITEM, NULL);
    if (ov74_0223C9A8[24] != 0) {
        const u16 *items = sMigrateFromAgbBlockedItems;
        do {
            if (item == *items) {
                return TRUE;
            }
            items++;
        } while (*items != 0);
    }
    return FALSE;
}

BOOL MigrateFromAgb_IsBoxMonSpeciesUnavailable(MigrateFromAgbApp *app, int slot) {
    AgbPokemonStorage *storage = app->storage;
    int box = app->box;
    return MigrateFromAgb_IsSpeciesUnavailable(AGB_GetBoxMonData(&storage->boxes[box][slot], AGB_MON_DATA_SPECIES, NULL));
}

#pragma force_active on

// Picks or unpicks a box slot: 0 nothing (6 picked), 1 picked, 2 unpicked, 3 empty, 4 egg, 5 forbidden move,
// 6 forbidden item, 7 not a DP species
int MigrateFromAgb_TogglePick(MigrateFromAgbApp *app, int slot) {
    int found;
    int i;
    VecFx32 pos;
    if (app->slots[slot].mon != NULL && !Sprite_GetDrawFlag(app->slots[slot].mon)) {
        return 3;
    }
    found = -1;
    for (i = 0; found == -1 && i < MIGRATE_FROM_AGB_MON_COUNT; i++) {
        if (app->box == app->picks[i].box && slot == app->picks[i].slot) {
            found = i;
        }
    }
    if (MigrateFromAgb_IsEgg(app, slot) == TRUE) {
        return 4;
    }
    if (MigrateFromAgb_KnowsHM(app, slot) == TRUE) {
        return 5;
    }
    if (MigrateFromAgb_HoldsBlockedItem(app, slot) == TRUE) {
        return 6;
    }
    if (MigrateFromAgb_IsBoxMonSpeciesUnavailable(app, slot) == TRUE) {
        return 7;
    }
    if (found == -1) {
        if (app->pickCount == MIGRATE_FROM_AGB_MON_COUNT) {
            return 0;
        }
        for (i = 0; i < MIGRATE_FROM_AGB_MON_COUNT; i++) {
            if (app->picks[i].slot == -1) {
                VecFx32 *matrix = Sprite_GetMatrixPtr(app->slots[slot].mon);
                Sprite_SetMatrix(app->picks[i].sprite, matrix);
                pos = *Sprite_GetMatrixPtr(app->picks[i].sprite);
                pos.x -= FX32_CONST(8);
                pos.y -= FX32_CONST(4);
                Sprite_SetMatrix(app->picks[i].sprite, &pos);
                Sprite_SetDrawFlag(app->picks[i].sprite, TRUE);
                app->picks[i].slot = slot;
                app->picks[i].box = app->box;
                app->pickCount++;
                return 1;
            }
        }
    } else {
        Sprite_SetDrawFlag(app->picks[found].sprite, FALSE);
        app->picks[found].slot = -1;
        app->pickCount--;
        return 2;
    }
    return 0;
}

#pragma force_active reset

// Shows the picks of the current box
void MigrateFromAgb_ShowBoxPicks(MigrateFromAgbApp *app) {
    int i;
    MigrateFromAgbApp *cursor = app;
    for (i = 0; i < MIGRATE_FROM_AGB_MON_COUNT; i++, cursor = (MigrateFromAgbApp *)((u8 *)cursor + sizeof(MigrateFromAgbPick))) {
        if (cursor->picks[0].slot != -1 && app->box == cursor->picks[0].box) {
            Sprite_SetDrawFlag(cursor->picks[0].sprite, TRUE);
        } else {
            Sprite_SetDrawFlag(cursor->picks[0].sprite, FALSE);
        }
    }
}

#pragma force_active on

void MigrateFromAgb_PrintBoxName(MigrateFromAgbApp *app, int box) {
    MigrateFromAgbText text = { 0 };
    u16 name[10];
    text.window = &app->boxNameWindow;
    text.x = 6;
    text.y = 21;
    text.width = 13;
    text.height = 2;
    text.msgId = -1;
    text.printX = 0;
    text.printY = 0;
    text.bgId = 1;
    text.fontId = 1;
    text.color = 0x010200;
    text.baseTile = 160;
    int language = PmAgbCartridge_GetLanguage();
    ConvertRSStringToDPStringInternational(app->storage->boxNames[box], name, 9, language);
    text.chars = name;
    MigrateFromAgb_PrintText(app, &text, 1);
}

void MigrateFromAgb_ShowMarkings(MigrateFromAgbApp *app, AgbBoxMon *mon) {
    u16 *tilemap = GetBgTilemapBuffer(app->bgConfig, GF_BG_LYR_MAIN_2);
    u8 markings;
    int i;
    if (mon != NULL) {
        markings = AGB_GetBoxMonData(mon, AGB_MON_DATA_MARKINGS, NULL);
    } else {
        markings = 0;
    }
    for (i = 0; i < 4; i++) {
        if (markings & 1) {
            tilemap[0x6B + i] = 0x80 + i;
        } else {
            tilemap[0x6B + i] = 0x60 + i;
        }
        markings >>= 1;
    }
    BgCommitTilemapBufferToVram(app->bgConfig, GF_BG_LYR_MAIN_2);
}

// Nickname, species, held item, level and markings of the touched Pokemon (NULL: clears)
void MigrateFromAgb_PrintMonInfo(MigrateFromAgbApp *app, AgbBoxMon *mon) {
    MigrateFromAgbText text = { 0 };

    struct {
        u16 name[11];
        u8 agbName[11];
    } names;

    int species;
    String *string;
    MsgData *msgData;

    text.window = &app->monInfoWindow;
    text.x = 0;
    text.y = 0;
    text.height = 4;
    text.width = 32;
    text.bgId = 1;
    text.fontId = 1;
    text.baseTile = 188;
    text.color = 0x0F0200;
    text.msgId = 44;
    text.printX = 144;
    text.chars = NULL;
    text.printY = 0;
    MigrateFromAgb_PrintText(app, &text, 4);
    text.msgId = 43;
    text.printX = 80;
    text.printY = 8;
    MigrateFromAgb_PrintText(app, &text, 6);
    MigrateFromAgb_ShowMarkings(app, mon);
    if (!mon) {
        CopyWindowToVram(text.window);
        return;
    }
    AGB_GetBoxMonData(mon, AGB_MON_DATA_NICKNAME, names.agbName);
    ConvertRSStringToDPStringInternational(names.agbName, names.name, 11, AGB_GetBoxMonData(mon, AGB_MON_DATA_LANGUAGE, NULL));
    text.chars = names.name;
    text.msgId = -1;
    text.printX = 8;
    text.printY = 0;
    MigrateFromAgb_PrintText(app, &text, 6);
    string = String_New(64, HEAP_ID_76);
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0237_bin, HEAP_ID_76);
    species = TranslateAgbSpecies(AGB_GetBoxMonData(mon, AGB_MON_DATA_SPECIES, NULL));
    ReadMsgDataIntoString(msgData, species, string);
    text.msgId = -1;
    text.printX = 16;
    text.printY = 16;
    text.string = string;
    MigrateFromAgb_PrintText(app, &text, 6);
    DestroyMsgData(msgData);
    String_Delete(string);
    {
        int item = AGB_GetBoxMonData(mon, AGB_MON_DATA_HELD_ITEM, NULL);
        if (item) {
            int dpItem = UpConvertItemId_Gen3to4((u16)item);
            string = String_New(64, HEAP_ID_76);
            GetItemNameIntoString(string, (u16)dpItem, HEAP_ID_76);
            text.msgId = -1;
            text.printX = 152;
            text.printY = 16;
            text.string = string;
            MigrateFromAgb_PrintText(app, &text, 6);
            String_Delete(string);
        }
    }
    {
        int level = AGB_GetBoxMonLevel(mon);
        string = String_New(10, HEAP_ID_76);
        String16_FormatInteger(string, level, 3, PRINTING_MODE_RIGHT_ALIGN, TRUE);
        text.msgId = -1;
        text.printX = 100;
        text.printY = 8;
        text.string = string;
        MigrateFromAgb_PrintText(app, &text, 2);
        String_Delete(string);
    }
    PlayCry((u16)species, 0);
}

#pragma force_active reset

void MigrateFromAgb_StartFadeBlack(MigrateFromAgbApp *app, enum FadeType fadeType, int nextState, int *state) {
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, fadeType, fadeType, RGB_BLACK, 6, 1, HEAP_ID_76);
    if (state != NULL) {
        *state = 23;
    }
    app->nextState = nextState;
}

void MigrateFromAgb_StartFadeWhite(MigrateFromAgbApp *app, enum FadeType fadeType, int nextState, int *state) {
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, fadeType, fadeType, RGB_WHITE, 6, 1, HEAP_ID_76);
    if (state != NULL) {
        *state = 23;
    }
    app->nextState = nextState;
}

#pragma force_active on

void MigrateFromAgb_InitBgs(BgConfig *bgConfig) {
    GraphicsModes modes = _0223B4F8;
    SetBothScreensModesAndDisable(&modes);
    BgTemplate bg0 = _0223B508;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_0, &bg0, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_0);
    BgTemplate bg1 = _0223B524;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_1, &bg1, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_1);
    BgTemplate bg2 = _0223B540;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_2, &bg2, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_2);
    BgTemplate bg3 = _0223B55C;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_3, &bg3, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_3);
}

void MigrateFromAgb_InitGraphics(MigrateFromAgbApp *app) {
    GraphicsBanks banks = _0223B578;
    GfGfx_SetBanks(&banks);
    MigrateFromAgb_InitBgs(app->bgConfig);
    gSystem.screensFlipped = TRUE;
    GfGfx_SwapDisplay();
    ResetAllTextPrinters();
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_15_OFFSET, HEAP_ID_76);
    GfGfxLoader_GXLoadPal(NARC_a_1_1_3, 23, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0xC0, HEAP_ID_76);
    GfGfxLoader_LoadCharData(NARC_a_1_1_3, 26, app->bgConfig, GF_BG_LYR_MAIN_2, 0, 0x1400, TRUE, HEAP_ID_76);
    FontID_Alloc(2, HEAP_ID_76);
}

// The box screen background, in the colour of the game
void MigrateFromAgb_LoadBoxBg(MigrateFromAgbApp *app) {
    GfGfxLoader_LoadScrnData(NARC_a_1_1_3, 24, app->bgConfig, GF_BG_LYR_MAIN_2, 0, 0x600, TRUE, HEAP_ID_76);
    BgTilemapRectChangePalette(app->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, 32, 24, ov74_0223C960[app->agbVersion]);
    BgCommitTilemapBufferToVram(app->bgConfig, GF_BG_LYR_MAIN_2);
}

void MigrateFromAgb_InitMsgBox(MigrateFromAgbApp *app) {
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_14_OFFSET, HEAP_ID_76);
    LoadUserFrameGfx1(app->bgConfig, GF_BG_LYR_MAIN_0, 0x3F0, 14, 0, HEAP_ID_76);
    LoadUserFrameGfx2(app->bgConfig, GF_BG_LYR_MAIN_0, 0x3D2, 13, app->frame, HEAP_ID_76);
    memset(&app->msgBox, 0, sizeof(MigrateFromAgbText));
    app->msgBox.window = &app->msgWindow;
    app->msgBox.x = 2;
    app->msgBox.y = 1;
    app->msgBox.width = 27;
    app->msgBox.height = 4;
    app->msgBox.bgId = 0;
    app->msgBox.fontId = 1;
    app->msgBox.baseTile = 1;
    app->msgBox.color = 0x01020F;
    app->msgBox.fillColor = 0x0F020F; // only the low byte (15) is used
    app->msgBox.chars = NULL;
    app->msgBox.string = NULL;
}

// The confirmation screen with the six picked Pokemon
void MigrateFromAgb_ShowConfirmScreen(MigrateFromAgbApp *app) {
    int i;
    int species, isEgg;
    SpriteTemplate template;
    for (i = 0; i < AGB_BOX_MON_COUNT; i++) {
        Sprite_SetDrawFlag(app->slots[i].mon, FALSE);
        Sprite_SetDrawFlag(app->slots[i].item, FALSE);
    }
    for (i = 0; i < MIGRATE_FROM_AGB_MON_COUNT; i++) {
        Sprite_SetDrawFlag(app->picks[i].sprite, FALSE);
    }
    Sprite_SetDrawFlag(app->buttons[0].sprite, FALSE);
    Sprite_SetDrawFlag(app->buttons[2].sprite, FALSE);
    Sprite_SetDrawFlag(app->buttons[1].sprite, FALSE);
    MigrateFromAgb_InitSpriteTemplate(&template, app, (int)&app->resHeader, NNS_G2D_VRAM_TYPE_2DMAIN);
    for (i = 0; i < MIGRATE_FROM_AGB_MON_COUNT; i++) {
        template.position.x = (28 + i * 40) << FX32_SHIFT;
        template.position.y = 142 << FX32_SHIFT;
        app->pickIcons[i] = Sprite_CreateAffine(&template);
        Sprite_SetAnimActiveFlag(app->pickIcons[i], TRUE);
        Sprite_SetAnimCtrlSeq(app->pickIcons[i], i + 10);
        Sprite_SetPriority(app->pickIcons[i], 1);
        Sprite_SetDrawFlag(app->pickIcons[i], TRUE);
        species = MigrateFromAgb_GetBoxMonSpecies(app, app->picks[i].box, app->picks[i].slot);
        isEgg = MigrateFromAgb_GetBoxMonIsEgg(app, app->picks[i].box, app->picks[i].slot);
        MigrateFromAgb_LoadMonIcon(species, isEgg, MigrateFromAgb_GetBoxMonPersonality(app, app->picks[i].box, app->picks[i].slot), gSystem.agbCartridgeVersion, i, app->pickIcons[i]);
    }
    GfGfxLoader_LoadScrnData(NARC_a_1_1_3, 25, app->bgConfig, GF_BG_LYR_MAIN_2, 0, 0x600, TRUE, HEAP_ID_76);
    BgTilemapRectChangePalette(app->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, 32, 24, ov74_0223C960[app->agbVersion]);
    BgCommitTilemapBufferToVram(app->bgConfig, GF_BG_LYR_MAIN_2);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_OFF);
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_14_OFFSET, HEAP_ID_76);
    LoadUserFrameGfx1(app->bgConfig, GF_BG_LYR_MAIN_0, 0x3F0, 14, 0, HEAP_ID_76);
    LoadUserFrameGfx2(app->bgConfig, GF_BG_LYR_MAIN_0, 0x3D2, 13, app->frame, HEAP_ID_76);
    MigrateFromAgb_InitMsgBox(app);
    app->msgBox.msgId = 10;
    MigrateFromAgb_PrintText(app, &app->msgBox, 24);
}

#pragma force_active reset

void MigrateFromAgb_RemoveWindow(Window *window) {
    if (window->bgConfig != NULL) {
        ClearFrameAndWindow2(window, FALSE);
        ClearWindowTilemapAndCopyToVram(window);
        RemoveWindow(window);
    }
}

// Back from the confirmation screen to the boxes
void MigrateFromAgb_CloseConfirmScreen(MigrateFromAgbApp *app) {
    int i = 0;
    MigrateFromAgbApp *cursor = app;
    for (; i < MIGRATE_FROM_AGB_MON_COUNT; i++, cursor = (MigrateFromAgbApp *)((u8 *)cursor + sizeof(Sprite *))) {
        Sprite_Delete(cursor->pickIcons[0]);
    }
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_ON);
    MigrateFromAgb_RemoveWindow(&app->msgWindow);
    Sprite_SetDrawFlag(app->buttons[0].sprite, TRUE);
    Sprite_SetDrawFlag(app->buttons[2].sprite, TRUE);
    Sprite_SetDrawFlag(app->buttons[1].sprite, TRUE);
    MigrateFromAgbApp *cursor2 = app;
    int j = 0;
    for (; j < MIGRATE_FROM_AGB_MON_COUNT; j++, cursor2 = (MigrateFromAgbApp *)((u8 *)cursor2 + sizeof(MigrateFromAgbPick))) {
        cursor2->picks[0].slot = -1;
        cursor2->picks[0].box = AGB_BOX_COUNT;
    }
    app->pickCount = 0;
}

#pragma force_active on

void MigrateFromAgb_FreeGraphics(MigrateFromAgbApp *app) {
    int i;
    Main_SetVBlankIntrCB(NULL, NULL);
    for (i = 0; i < AGB_BOX_MON_COUNT; i++) {
        if (app->slots[i].mon) {
            Sprite_Delete(app->slots[i].mon);
        }
        if (app->slots[i].item) {
            Sprite_Delete(app->slots[i].item);
        }
    }
    for (i = 0; i < MIGRATE_FROM_AGB_MON_COUNT; i++) {
        if (app->picks[i].sprite) {
            Sprite_Delete(app->picks[i].sprite);
        }
    }
    if (app->buttons[0].sprite) {
        Sprite_Delete(app->buttons[0].sprite);
    }
    if (app->buttons[2].sprite) {
        Sprite_Delete(app->buttons[2].sprite);
    }
    if (app->buttons[1].sprite) {
        Sprite_Delete(app->buttons[1].sprite);
    }
    MigrateFromAgb_RemoveWindow(&app->boxNameWindow);
    MigrateFromAgb_RemoveWindow(&app->monInfoWindow);
    MigrateFromAgb_RemoveWindow(&app->msgWindow);
    FontID_Release(2);
    SpriteTransfer_DeleteCharTransferTask(app->resources[GF_GFX_RES_TYPE_CHAR]);
    SpriteTransfer_DeletePlttTransferTask(app->resources[GF_GFX_RES_TYPE_PLTT]);
    for (i = 0; i < 6; i++) {
        Destroy2DGfxResObjMan(app->resMan[i]);
    }
    SpriteList_Delete(app->spriteList);
    app->spriteList = NULL;
    OamManager_Free();
    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
    YesNoPrompt_Destroy(app->yesNoPrompt);
    gSystem.screensFlipped = FALSE;
    GfGfx_SwapDisplay();
    FreeBgTilemapBuffer(app->bgConfig, GF_BG_LYR_MAIN_0);
    FreeBgTilemapBuffer(app->bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(app->bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(app->bgConfig, GF_BG_LYR_MAIN_3);
}

#pragma force_active reset

void MigrateFromAgb_ShowYesNo(MigrateFromAgbApp *app) {
    YesNoPromptTemplate template;
    template.bgConfig = app->bgConfig;
    template.bgId = GF_BG_LYR_MAIN_0;
    template.tileStart = 0x200;
    template.plttSlot = 8;
    template.x = 25;
    template.y = 7;
    template.shapeParam = 0;
    template.initialCursorPos = 1;
    YesNoPrompt_InitFromTemplate(app->yesNoPrompt, &template);
}

#pragma force_active on

// "Migrate from <player>'s game?" (message per GBA game)
void MigrateFromAgb_PrintIntro(MigrateFromAgbApp *app) {
    u16 name[8];
    MessageFormat *msgFormat;
    String *string;
    void *playerName = PmAgbCartridge_GetPlayerName(app);
    ConvertRSStringToDPStringInternational(playerName, name, 8, PmAgbCartridge_GetLanguage());
    msgFormat = MessageFormat_New(HEAP_ID_76);
    string = String_New(8, HEAP_ID_76);
    CopyU16ArrayToString(string, name);
    BufferString(msgFormat, 1, string, 0, 1, 2);
    MigrateFromAgb_InitMsgBox(app);
    app->msgBox.msgId = ov74_0223C980[app->agbVersion];
    app->msgBox.msgFormat = msgFormat;
    MigrateFromAgb_PrintText(app, &app->msgBox, 24);
    String_Delete(string);
    MessageFormat_Delete(msgFormat);
    MigrateFromAgb_ShowYesNo(app);
}

void MigrateFromAgb_VBlankCB(MigrateFromAgbApp *app) {
    if (app->vblankCallback) {
        app->vblankCallback();
        app->vblankCallback = NULL;
    }
    GF_RunVramTransferTasks();
    OamManager_ApplyAndResetBuffers();
    DoScheduledBgGpuUpdates(app->bgConfig);
    {
        // OS_SetIrqCheckFlag(OS_IE_V_BLANK), with the address of the check flag spelled out (matching)
        volatile u32 *dtcm = (volatile u32 *)0x027E0000;
        dtcm[0x3FF8 / 4] |= OS_IE_V_BLANK;
    }
}

#pragma force_active reset

// The checks before the boxes are shown: 0 OK, 5 fewer than six Pokemon, 6 the Pal Park is not empty
int MigrateFromAgb_CheckCanMigrate(MigrateFromAgbApp *app) {
    MigratedPokemon *migrated = Save_MigratedPokemon_Get(app->saveData);
    int count = 0;
    int box;
    for (box = 0; box < AGB_BOX_COUNT; box++) {
        int slot;
        for (slot = 0; slot < AGB_BOX_MON_COUNT; slot++) {
            AgbPokemonStorage *storage = app->storage;
            if (AGB_GetBoxMonData(&storage->boxes[box][slot], AGB_MON_DATA_HAS_SPECIES, NULL) != 0) {
                count++;
            }
        }
    }
    if (count < MIGRATE_FROM_AGB_MON_COUNT) {
        return 5;
    }
    if (MigratedPokemon_CountPokemon(migrated) != 0) {
        return 6;
    }
    return 0;
}

// Prints message ov74_0223C9B8[index] (once) and waits for it: TRUE when done
BOOL MigrateFromAgb_PrintResultMsg(MigrateFromAgbApp *app, int index) {
    if (app->pendingMsg != 0) {
        MigrateFromAgb_InitMsgBox(app);
        TextFlags_SetCanTouchSpeedUpPrint(TRUE);
        app->msgBox.msgId = ov74_0223C9B8[index];
        app->msgBox.textSpeed = 1;
        MigrateFromAgb_PrintText(app, &app->msgBox, 24);
        app->pendingMsg = 0;
    } else if (!TextPrinterCheckActive((u8)app->msgBox.printerId)) {
        TextFlags_SetCanTouchSpeedUpPrint(FALSE);
        return TRUE;
    }
    return FALSE;
}

// The same, then shows the yes/no prompt
BOOL MigrateFromAgb_PrintResultMsgWithYesNo(MigrateFromAgbApp *app, int index) {
    if (app->pendingMsg != 0) {
        MigrateFromAgb_InitMsgBox(app);
        TextFlags_SetCanTouchSpeedUpPrint(TRUE);
        app->msgBox.msgId = ov74_0223C9B8[index];
        app->msgBox.textSpeed = 1;
        MigrateFromAgb_PrintText(app, &app->msgBox, 24);
        app->pendingMsg = 0;
    } else if (!TextPrinterCheckActive((u8)app->msgBox.printerId)) {
        MigrateFromAgb_ShowYesNo(app);
        TextFlags_SetCanTouchSpeedUpPrint(FALSE);
        return TRUE;
    }
    return FALSE;
}

// Prints app->pendingMsgId: TRUE when printed (textSpeed != 0) or, if printed instantly, when a key or the screen is pressed
BOOL MigrateFromAgb_PrintPendingMsg(MigrateFromAgbApp *app, int textSpeed) {
    if (app->pendingMsgId != -1) {
        MigrateFromAgb_InitMsgBox(app);
        TextFlags_SetCanTouchSpeedUpPrint(TRUE);
        app->msgBox.msgId = app->pendingMsgId;
        app->msgBox.textSpeed = textSpeed;
        MigrateFromAgb_PrintText(app, &app->msgBox, 24);
        app->pendingMsgId = -1;
        PlaySE(SEQ_SE_DP_SELECT);
    } else if (textSpeed != 0) {
        if (!TextPrinterCheckActive((u8)app->msgBox.printerId)) {
            TextFlags_SetCanTouchSpeedUpPrint(FALSE);
            return TRUE;
        }
    } else if (gSystem.touchNew != 0 || gSystem.newKeys != 0) {
        TextFlags_SetCanTouchSpeedUpPrint(FALSE);
        PlaySE(SEQ_SE_DP_SELECT);
        return TRUE;
    }
    return FALSE;
}

#pragma force_active on

BOOL MigrateFromAgb_Init(OverlayManager *man, int *state) {
    MigrateFromAgbApp *app;
    Heap_Create(HEAP_ID_3, HEAP_ID_76, 0x38000);
    app = OverlayManager_CreateAndGetData(man, sizeof(MigrateFromAgbApp), HEAP_ID_76);
    memset(app, 0, sizeof(MigrateFromAgbApp));
    app->bgConfig = BgConfig_Alloc(HEAP_ID_76);
    app->yesNoPrompt = YesNoPrompt_Create(HEAP_ID_76);
    SetMasterBrightnessByColor(PM_LCD_TOP, RGB_BLACK);
    SetMasterBrightnessByColor(PM_LCD_BOTTOM, RGB_BLACK);
    app->saveData = ((SaveData **)OverlayManager_GetArgs(man))[2];
    app->profile = Save_PlayerData_GetProfile(app->saveData);
    app->options = Save_PlayerData_GetOptionsAddr(app->saveData);
    app->frame = Options_GetFrame(app->options);
    app->string = String_New(384, HEAP_ID_76);
    app->rawString = String_New(384, HEAP_ID_76);
    Sound_SetSceneAndPlayBGM(9, SEQ_GS_WIFI_PRESENT, 1);
    if (OS_IsTickAvailable() == FALSE) {
        OS_InitTick();
    }
    MainMenuUtil_IgnoreCartridgeIrq();
    sMigrateFromAgbIcons = app->icons;
    return TRUE;
}

#pragma force_active reset

#pragma force_active on

BOOL MigrateFromAgb_Main(OverlayManager *man, int *state) {
    MigrateFromAgbApp *app = OverlayManager_GetData(man);
    CTRDG_IsExisting();
    if (CTRDG_IsPulledOut() == TRUE || _02111864) {
        if (app->save.state == 3 || app->save.state == 4) {
            Save_Cancel(app->saveData);
        }
        ShowGBACartRemovedError(HEAP_ID_76);
    }
    app->frameCount++;
    switch (*state) {
    case 0:
        PmAgbCartridgeUnlinkSpec();
        app->cartridgeResult = PmAgbCartridgeGetOffsets(app->agbSave);
        if (!app->cartridgeResult) {
            app->loadResult = PmAgbCartridge_LoadSave();
            if (!app->loadResult) {
                app->storage = PmAgbCartridge_GetPCStorage();
                app->box = *(u8 *)app->storage;
            } else {
                app->pendingMsg = 1;
            }
        }
        MigrateFromAgb_GetAgbVersion(&app->agbVersion);
        *state = 1;
        break;
    case 1:
        MigrateFromAgb_InitGraphics(app);
        MigrateFromAgb_InitObjTransfer();
        MigrateFromAgb_InitSpriteResources(app);
        MigrateFromAgb_InitSpriteHeader(app);
        MigrateFromAgb_CreateButtons(app);
        Main_SetVBlankIntrCB((GFIntrCB)MigrateFromAgb_VBlankCB, app);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_OFF);
        if (app->pendingMsg == 1) {
            MigrateFromAgb_StartFadeBlack(app, FADE_TYPE_BRIGHTNESS_IN, 11, state);
        } else {
            *state = 2;
        }
        break;
    case 2:
        MigrateFromAgb_PrintIntro(app);
        MigrateFromAgb_StartFadeBlack(app, FADE_TYPE_BRIGHTNESS_IN, 3, state);
        break;
    case 3:
        switch (YesNoPrompt_HandleInput(app->yesNoPrompt)) {
        case YESNORESPONSE_YES:
            YesNoPrompt_Reset(app->yesNoPrompt);
#ifdef HEARTGOLD
            app->pendingMsgId = 34;
#else
            app->pendingMsgId = 35;
#endif
            *state = 4;
            break;
        case YESNORESPONSE_NO:
            YesNoPrompt_Reset(app->yesNoPrompt);
            MigrateFromAgb_StartFadeWhite(app, FADE_TYPE_BRIGHTNESS_OUT, 24, state);
            break;
        }
        break;
    case 4:
        if (MigrateFromAgb_PrintPendingMsg(app, 1)) {
            MigrateFromAgb_ShowYesNo(app);
            *state = 5;
        }
        break;
    case 5:
        switch (YesNoPrompt_HandleInput(app->yesNoPrompt)) {
        case YESNORESPONSE_YES:
            YesNoPrompt_Reset(app->yesNoPrompt);
            app->pendingMsg = MigrateFromAgb_CheckCanMigrate(app);
            if (app->pendingMsg) {
                if (app->pendingMsg == 3 || app->pendingMsg == 4) {
                    *state = 8;
                } else {
                    *state = 11;
                }
            } else {
                app->pendingMsgId = 6;
                *state = 6;
            }
            break;
        case YESNORESPONSE_NO:
            YesNoPrompt_Reset(app->yesNoPrompt);
            MigrateFromAgb_StartFadeWhite(app, FADE_TYPE_BRIGHTNESS_OUT, 24, state);
            break;
        }
        break;
    case 6:
        if (MigrateFromAgb_PrintPendingMsg(app, 1)) {
            *state = 7;
        }
        break;
    case 7:
        if (gSystem.touchNew || gSystem.newKeys) {
            MigrateFromAgb_StartFadeBlack(app, FADE_TYPE_BRIGHTNESS_OUT, 12, state);
        }
        break;
    case 8:
        if (MigrateFromAgb_PrintResultMsgWithYesNo(app, app->pendingMsg)) {
            *state = 9;
        }
        break;
    case 9:
        switch (YesNoPrompt_HandleInput(app->yesNoPrompt)) {
        case YESNORESPONSE_YES:
            YesNoPrompt_Reset(app->yesNoPrompt);
            app->msgBox.textSpeed = 0;
            app->msgBox.msgId = 40;
            MigrateFromAgb_PrintText(app, &app->msgBox, 0);
            app->waitingIcon = WaitingIcon_New(&app->msgWindow, 0x3D2);
            *state = 10;
            break;
        case YESNORESPONSE_NO:
            YesNoPrompt_Reset(app->yesNoPrompt);
            MigrateFromAgb_StartFadeWhite(app, FADE_TYPE_BRIGHTNESS_OUT, 24, state);
            break;
        }
        break;
    case 10:
        MigrateFromAgb_SaveMigrationRecord(app);
        WaitingIcon_Delete(app->waitingIcon);
        app->pendingMsg = 7;
        *state = 11;
        break;
    case 11:
        if (MigrateFromAgb_PrintResultMsg(app, app->pendingMsg)) {
            *state = 22;
        }
        break;
    case 12:
        MigrateFromAgb_RemoveWindow(&app->msgWindow);
        MigrateFromAgb_LoadBoxBg(app);
        MigrateFromAgb_CreateBoxSprites(app);
        MigrateFromAgb_ShowBox(app);
        Main_SetVBlankIntrCB((GFIntrCB)MigrateFromAgb_VBlankCB, app);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
        MigrateFromAgb_PrintMonInfo(app, NULL);
        MigrateFromAgb_StartFadeBlack(app, FADE_TYPE_BRIGHTNESS_IN, 13, state);
        break;
    case 13: {
        int hit = TouchscreenHitbox_FindRectAtTouchNew(app->hitboxes);
        if (hit != -1) {
            if (hit < AGB_BOX_MON_COUNT) {
                int result = MigrateFromAgb_TogglePick(app, hit);
                if (result == 1) {
                    MigrateFromAgb_PrintMonInfo(app, &app->storage->boxes[app->box][hit]);
                    if (app->pickCount == MIGRATE_FROM_AGB_MON_COUNT) {
                        app->pickDoneTimer = 45;
                        *state = 15;
                    }
                } else if (result == 2) {
                    MigrateFromAgb_PrintMonInfo(app, NULL);
                } else if (result == 4) {
                    app->pendingMsgId = 8;
                    *state = 14;
                } else if (result == 5) {
                    app->pendingMsgId = 9;
                    *state = 14;
                } else if (result == 6) {
                    app->pendingMsgId = 38;
                    *state = 14;
                } else if (result == 7) {
                    app->pendingMsgId = 38;
                    *state = 14;
                }
            } else {
                switch (hit) {
                case 31:
                    if (app->box == 0) {
                        app->box = AGB_BOX_COUNT - 1;
                    } else {
                        app->box--;
                    }
                    MigrateFromAgb_ShowBox(app);
                    PlaySE(SEQ_SE_DP_SELECT);
                    break;
                case 32:
                    app->box++;
                    if (app->box == AGB_BOX_COUNT) {
                        app->box = 0;
                    }
                    MigrateFromAgb_ShowBox(app);
                    PlaySE(SEQ_SE_DP_SELECT);
                    break;
                case 30:
                    MigrateFromAgb_StartFadeWhite(app, FADE_TYPE_BRIGHTNESS_OUT, 24, state);
                    PlaySE(SEQ_SE_DP_SELECT);
                    break;
                }
            }
        }
        break;
    }
    case 14:
        if (MigrateFromAgb_PrintPendingMsg(app, 0)) {
            MigrateFromAgb_RemoveWindow(&app->msgWindow);
            *state = 13;
        }
        break;
    case 15:
        app->pickDoneTimer--;
        if (!app->pickDoneTimer) {
            MigrateFromAgb_StartFadeBlack(app, FADE_TYPE_BRIGHTNESS_OUT, 16, state);
        }
        break;
    case 16:
        MigrateFromAgb_ShowConfirmScreen(app);
        MigrateFromAgb_StartFadeBlack(app, FADE_TYPE_BRIGHTNESS_IN, 17, state);
        MigrateFromAgb_ShowYesNo(app);
        break;
    case 17:
        switch (YesNoPrompt_HandleInput(app->yesNoPrompt)) {
        case YESNORESPONSE_YES:
            YesNoPrompt_Reset(app->yesNoPrompt);
            *state = 18;
            break;
        case YESNORESPONSE_NO:
            YesNoPrompt_Reset(app->yesNoPrompt);
            MigrateFromAgb_StartFadeBlack(app, FADE_TYPE_BRIGHTNESS_OUT, 20, state);
            break;
        }
        break;
    case 18:
        app->msgBox.msgId = 11;
        MigrateFromAgb_PrintText(app, &app->msgBox, 0);
        MigrateFromAgb_ShowYesNo(app);
        PlaySE(SEQ_SE_DP_SELECT);
        *state = 19;
        break;
    case 19:
        switch (YesNoPrompt_HandleInput(app->yesNoPrompt)) {
        case YESNORESPONSE_YES:
            app->msgBox.msgId = ov74_0223C968[app->agbVersion];
            MigrateFromAgb_PrintText(app, &app->msgBox, 0);
            YesNoPrompt_Reset(app->yesNoPrompt);
            app->save.state = 0;
            app->waitingIcon = WaitingIcon_New(&app->msgWindow, 0x3D2);
            *state = 21;
            Sys_SetSoftResetDisableFlag(4);
            break;
        case YESNORESPONSE_NO:
            YesNoPrompt_Reset(app->yesNoPrompt);
            MigrateFromAgb_StartFadeBlack(app, FADE_TYPE_BRIGHTNESS_OUT, 20, state);
            break;
        }
        break;
    case 20:
        MigrateFromAgb_CloseConfirmScreen(app);
        *state = 12;
        break;
    case 21: {
        int result = MigrateFromAgb_RunTransfer(app);
        if (result != 10) {
            WaitingIcon_Delete(app->waitingIcon);
            PlaySE(SEQ_SE_DP_SAVE);
            if (result == 11) {
#ifdef HEARTGOLD
                app->msgBox.msgId = 29;
#else
                app->msgBox.msgId = 30;
#endif
            } else {
                app->msgBox.msgId = 36;
            }
            MigrateFromAgb_PrintText(app, &app->msgBox, 0);
            *state = 22;
            Sys_ClearSoftResetDisableFlag(4);
        }
        break;
    }
    case 22:
        if (gSystem.touchNew || gSystem.newKeys) {
            MigrateFromAgb_StartFadeWhite(app, FADE_TYPE_BRIGHTNESS_OUT, 24, state);
            PlaySE(SEQ_SE_DP_SELECT);
        }
        break;
    case 23:
        if (IsPaletteFadeFinished()) {
            *state = app->nextState;
        }
        break;
    case 24:
        MigrateFromAgb_FreeGraphics(app);
        return TRUE;
    }
    if (app->spriteList) {
        SpriteList_RenderAndAnimateSprites(app->spriteList);
    }
    return FALSE;
}

#pragma force_active reset

BOOL MigrateFromAgb_Exit(OverlayManager *man, int *state) {
    MigrateFromAgbApp *app;
    app = OverlayManager_GetData(man);
    String_Delete(app->string);
    String_Delete(app->rawString);
    Heap_Free(app->bgConfig);
    RegisterMainOverlay(FS_OVERLAY_ID(intro_title), &gApplication_TitleScreen);
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_76);
    MainMenuUtil_EnableCartridgePullOutCheck(0);
    return TRUE;
}
