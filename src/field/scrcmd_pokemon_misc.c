#include "global.h"

#include "constants/items.h"
#include "constants/map_sections.h"
#include "constants/moves.h"

#include "field/field_control.h"
#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0066_D23R0102.h"
#include "msgdata/msg/msg_0096_D31R0201.h"
#include "overlay_2/fashion_case_gifts.h"
#include "overlay_2/overlay_02_gear_phone.h"
#include "pokeathlon/pokeathlon_save.h"

#include "bag.h"
#include "bug_contest.h"
#include "follow_mon.h"
#include "friend_group.h"
#include "get_egg.h"
#include "gf_gfx_loader.h"
#include "launch_application.h"
#include "list_menu.h"
#include "map_header.h"
#include "map_object.h"
#include "map_section.h"
#include "math_util.h"
#include "field/map_object_billboard_motion.h"
#include "field/map_object_render.h"
#include "overlay_02.h"
#include "overlay_03.h"
#include "overlay_58.h"
#include "photo_album.h"
#include "pokedex.h"
#include "save_wifi_history.h"
#include "scrcmd.h"
#include "screen_fade.h"
#include "script_manager.h"
#include "sound_system.h"
#include "sys_vars.h"
#include "system.h"
#include "sound_playback.h"
#include "billboard_lists.h"
#include "frontier_save_access.h"
#include "apricorn_box.h"
#include "comm_state.h"
#include "frontier_stat_ids_legacy.h"
#include "field_misc_events.h"
#include "gear_phone_ring_manager.h"
#include "update_dex_received.h"
// #include "application/zukanlist/zkn_data/zukan_data.naix"

typedef struct SpeciesListMenu {
    FieldSystem *fieldSystem;
    SysTask *sysTask;
    Window window_8;
    Window *window_18;
    String *stringArr_1C[120];
    MsgData *msgdata;
    MessageFormat *msgfmt;
    u8 delay;
    u8 unk_205;
    u8 initCursorPos;
    u8 flags;
    u8 x;
    u8 y;
    u8 unk_20A;
    u8 totalItems;
    u8 unk_20C;
    u8 unk_20D;
    u8 unk_20E;
    u8 unk_20F;
    u16 *input;
    u16 *cursorPos;
    u16 *itemsAbove;
    struct ListMenuTemplate listMenuTemplate;
    struct ListMenu *listMenu_23C;
    u32 unk_240;
    ListMenuItem items[120];
    u16 msgIds[120];
    u16 lastRow;
} SpeciesListMenu;

static BOOL NativeScript_WaitSpeciesListMenu(ScriptContext *ctx);
static void *LoadZukanSpeciesList(enum HeapID heapID, s32 fileId, int *count);
static void SpeciesListMenu_SetMsgData(SpeciesListMenu *menu, MsgData *msgdata);
static void SpeciesListMenu_Init(FieldSystem *fieldSystem, SpeciesListMenu *menu, u8 x, u8 y, u8 initCursorPos, u8 a5, u16 *input, MessageFormat *msgfmt, Window *window, MsgData *msgdata, u16 *cursorPos, u16 *itemsAbove);
static SpeciesListMenu *SpeciesListMenu_New(FieldSystem *fieldSystem, u8 x, u8 y, u8 initCursorPos, u8 a4, u16 *input, MessageFormat *msgfmt, Window *window, MsgData *msgdata, u16 *cursorPos, u16 *itemsAbove);
static void SpeciesListMenu_AddItem(SpeciesListMenu *menu, int strNo, u16 msgId, u32 value);
static void SpeciesListMenu_Show(SpeciesListMenu *menu);
static void SpeciesListMenu_AddItemInternal(SpeciesListMenu *menu, int strNo, u16 msgId, u32 value);
static void SpeciesListMenu_InitTemplate(SpeciesListMenu *menu);
static void SpeciesListMenu_ItemPrintFunc(struct ListMenu *listMenu, s32 value, u8 unused);
static void SpeciesListMenu_MoveCursorFunc(struct ListMenu *listMenu, s32 unused1, u8 unused2);
static void SpeciesListMenu_Task(SysTask *sysTask, void *work);
static void SpeciesListMenu_Close(SpeciesListMenu *menu);
static void GetHiddenPowerPowerType(Pokemon *mon, s32 *power, s32 *type);
static LocalMapObject *FriendshipRoom_CreateStatueObject(MapObjectManager *mapObjectManager, u8 slot, u16 species, u16 form, u32 gender, u32 x, u32 y, u32 mapId);

// zukan_data_XXXXX
static u16 sSpeciesListZukanFiles[9] = {
    93,
    94,
    95,
    96,
    97,
    98,
    99,
    100,
    101,
};

BOOL ScrCmd_BattleHallRecordSpeciesListMenu(ScriptContext *ctx) {
    int i;
    u32 out_1;
    int numSpecies;
    u16 *input;
    u32 record;

    MsgData *msgdata2;
    SpeciesListMenu *menu;
    u32 *records;
    u16 *data;
    u16 *cursorPos;
    u16 *itemsAbove;
    Window *window;
    MessageFormat **msgfmt;
    MsgData *msgdata;
    FieldSystem *fieldSystem = ctx->fieldSystem;

    msgfmt = FieldSysGetAttrAddr(ctx->fieldSystem, SCRIPTENV_MESSAGE_FORMAT);
    u32 recordsType = ScriptGetVar(ctx);
    u32 fileIndex = ScriptGetVar(ctx);
    u16 inputVar = ScriptReadHalfword(ctx);
    u16 cursorPosVar = ScriptReadHalfword(ctx);
    u16 itemsAboveVar = ScriptReadHalfword(ctx);

    ctx->data[0] = inputVar;

    msgdata = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 237, HEAP_ID_FIELD3);

    input = GetVarPointer(fieldSystem, inputVar);

    window = FieldSysGetAttrAddr(ctx->fieldSystem, SCRIPTENV_WINDOW);

    cursorPos = GetVarPointer(fieldSystem, cursorPosVar);
    itemsAbove = GetVarPointer(fieldSystem, itemsAboveVar);

    menu = SpeciesListMenu_New(fieldSystem, 20, 1, 0, 1, input, *msgfmt, window, msgdata, cursorPos, itemsAbove);

    records = FrontierSpeciesRecords_Load(fieldSystem->saveData, 0xb, &out_1);

    if (out_1 == 1) {
        data = LoadZukanSpeciesList(HEAP_ID_FIELD3, sSpeciesListZukanFiles[fileIndex], &numSpecies);

        for (i = 0; i < numSpecies; i++) {
            record = FrontierSpeciesRecords_Get(fieldSystem->saveData, records, BattleHall_GetSpeciesRecordsType(recordsType), data[i]);
            if (record != 0) {
                SpeciesListMenu_AddItem(menu, data[i], 0xff, data[i]);
            }
        }
        Heap_Free(data);
    }
    if (records) {
        Heap_Free(records);
    }

    msgdata2 = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0191_bin, HEAP_ID_FIELD3);

    SpeciesListMenu_SetMsgData(menu, msgdata2);
    SpeciesListMenu_AddItem(menu, 0xd, 0xff, 0xfffe);
    DestroyMsgData(msgdata2);

    SpeciesListMenu_SetMsgData(menu, msgdata);
    SpeciesListMenu_Show(menu);

    SetupNativeScript(ctx, NativeScript_WaitSpeciesListMenu);

    DestroyMsgData(msgdata);

    return TRUE;
}

static BOOL NativeScript_WaitSpeciesListMenu(ScriptContext *ctx) {
    return *GetVarPointer(ctx->fieldSystem, ctx->data[0]) != 0xEEEE;
}

static void *LoadZukanSpeciesList(enum HeapID heapID, s32 fileId, int *count) {
    u32 size;
    void *data = GfGfxLoader_LoadFromNarc_GetSizeOut(NARC_application_zukanlist_zkn_data_zukan_data, fileId, FALSE, heapID, FALSE, &size);
    *count = size / 2;
    return data;
}

static void SpeciesListMenu_SetMsgData(SpeciesListMenu *menu, MsgData *msgdata) {
    menu->msgdata = msgdata;
}

static void SpeciesListMenu_Init(FieldSystem *fieldSystem, SpeciesListMenu *menu, u8 x, u8 y, u8 initCursorPos, u8 a5, u16 *input, MessageFormat *msgfmt, Window *window, MsgData *msgdata, u16 *cursorPos, u16 *itemsAbove) {
    int i;
    menu->msgdata = msgdata;
    menu->flags = menu->flags & ~0x2;
    menu->msgfmt = msgfmt;
    menu->fieldSystem = fieldSystem;
    menu->input = input;
    i = 0;
    *menu->input = 0;
    menu->cursorPos = cursorPos;
    menu->itemsAbove = itemsAbove;
    menu->flags = menu->flags & ~1 | a5 & 1;
    menu->initCursorPos = initCursorPos;
    menu->x = x;
    menu->y = y;
    menu->totalItems = 0;
    menu->window_18 = window;
    menu->delay = 3;
    menu->lastRow = initCursorPos;

    for (i = 0; i < 0x78; i++) {
        menu->items[i].text = NULL;
        menu->items[i].value = 0;
        menu->msgIds[i] = 0xff;
    }

    for (i = 0; i < 0x78; i++) {
        menu->stringArr_1C[i] = String_New(0x50, HEAP_ID_FIELD1);
    }

    *menu->input = 0xEEEE;
}

static SpeciesListMenu *SpeciesListMenu_New(FieldSystem *fieldSystem, u8 x, u8 y, u8 initCursorPos, u8 a4, u16 *input, MessageFormat *msgfmt, Window *window, MsgData *msgdata, u16 *cursorPos, u16 *itemsAbove) {
    SpeciesListMenu *menu = Heap_Alloc(HEAP_ID_FIELD1, sizeof(SpeciesListMenu));
    if (!menu) {
        return NULL;
    }
    memset(menu, 0, sizeof(SpeciesListMenu));
    SpeciesListMenu_Init(fieldSystem, menu, x, y, initCursorPos, a4, input, msgfmt, window, msgdata, cursorPos, itemsAbove);
    return menu;
}

static void SpeciesListMenu_AddItem(SpeciesListMenu *menu, int strNo, u16 msgId, u32 value) {
    SpeciesListMenu_AddItemInternal(menu, strNo, msgId, value);
}

static void SpeciesListMenu_Show(SpeciesListMenu *menu) {
    u32 numItems = menu->totalItems;
    if (numItems > 8) {
        AddWindowParameterized(menu->fieldSystem->bgConfig, &menu->window_8, 3, menu->x, menu->y, 11, 0x10, 13, 1);
    } else {
        AddWindowParameterized(menu->fieldSystem->bgConfig, &menu->window_8, 3, menu->x, menu->y, 11, 2 * numItems, 13, 1);
    }
    LoadUserFrameGfx1(menu->fieldSystem->bgConfig, GF_BG_LYR_MAIN_3, 0x3D9, 11, 0, HEAP_ID_FIELD1);
    DrawFrameAndWindow1(&menu->window_8, TRUE, 0x3D9, 11);
    SpeciesListMenu_InitTemplate(menu);
    menu->listMenu_23C = ListMenuInit(&menu->listMenuTemplate, *menu->cursorPos, *menu->itemsAbove, HEAP_ID_FIELD1);
    menu->sysTask = SysTask_CreateOnMainQueue(SpeciesListMenu_Task, menu, 0);
}

static void SpeciesListMenu_AddItemInternal(SpeciesListMenu *menu, int strNo, u16 msgId, u32 value) {
    String *str = String_New(0x50, HEAP_ID_FIELD1);
    ReadMsgDataIntoString(menu->msgdata, strNo, str);
    StringExpandPlaceholders(menu->msgfmt, menu->stringArr_1C[menu->totalItems], str);
    menu->items[menu->totalItems].text = menu->stringArr_1C[menu->totalItems];
    String_Delete(str);
    if (value == 0xfa) {
        menu->items[menu->totalItems].value = -3;
    } else {
        menu->items[menu->totalItems].value = value;
    }
    menu->msgIds[menu->totalItems] = msgId;
    menu->totalItems++;
}

static void SpeciesListMenu_InitTemplate(SpeciesListMenu *menu) {
    menu->listMenuTemplate.items = menu->items;
    menu->listMenuTemplate.moveCursorFunc = SpeciesListMenu_MoveCursorFunc;
    menu->listMenuTemplate.itemPrintFunc = SpeciesListMenu_ItemPrintFunc;
    menu->listMenuTemplate.window = &menu->window_8;
    menu->listMenuTemplate.totalItems = menu->totalItems;
    menu->listMenuTemplate.maxShowed = 8;
    menu->listMenuTemplate.header_X = 1;
    menu->listMenuTemplate.item_X = 12;
    menu->listMenuTemplate.cursor_X = 2;
    menu->listMenuTemplate.upText_Y = 1;
    menu->listMenuTemplate.cursorPal = 1;
    menu->listMenuTemplate.fillValue = 15;
    menu->listMenuTemplate.cursorShadowPal = 2;
    menu->listMenuTemplate.lettersSpacing = 0;
    menu->listMenuTemplate.itemVerticalPadding = 0;
    menu->listMenuTemplate.scrollMultiple = 1;
    menu->listMenuTemplate.fontId = 0;
    menu->listMenuTemplate.cursorKind = 0;
    menu->listMenuTemplate.data = menu;
}

static void SpeciesListMenu_ItemPrintFunc(struct ListMenu *listMenu, s32 value, u8 unused) {
    if (value == -3) {
        ListMenuOverrideSetColors(listMenu, 3, 0xf, 4);
    } else {
        ListMenuOverrideSetColors(listMenu, 1, 0xf, 2);
    }
}

static void SpeciesListMenu_MoveCursorFunc(struct ListMenu *listMenu, s32 unused1, u8 unused2) {
    u16 cursorPos = 0, itemsAbove = 0;
    SpeciesListMenu *menu = (SpeciesListMenu *)ListMenuGetTemplateField(listMenu, LISTMENUATTR_DATA);
    ListMenuGetScrollAndRow(listMenu, &cursorPos, &itemsAbove);
    if (menu->cursorPos != 0 && menu->itemsAbove != 0) {
        *menu->cursorPos = cursorPos;
        *menu->itemsAbove = itemsAbove;
    }
}

static void SpeciesListMenu_Task(SysTask *sysTask, void *work) {
    u16 prev;
    int input;
    SpeciesListMenu *menu = work;
    if (menu->delay) {
        menu->delay--;
        return;
    }
    if (IsPaletteFadeFinished()) {
        input = ListMenu_ProcessInput(menu->listMenu_23C);
        prev = menu->lastRow;
        ListMenuGetCurrentItemArrayId(menu->listMenu_23C, &menu->lastRow);
        if (prev != menu->lastRow) {
            PlaySE(SEQ_SE_DP_SELECT);
        }
        switch (input) {
        case -2:
            if (((u32)(menu->flags << 0x1f) >> 0x1f) == 1) {
                PlaySE(SEQ_SE_DP_SELECT);
                *menu->input = -2;
                SpeciesListMenu_Close(menu);
            }
        case -1:
            break;
        default:
            PlaySE(SEQ_SE_DP_SELECT);
            *menu->input = input;
            SpeciesListMenu_Close(menu);
            break;
        }
    }
}

static void SpeciesListMenu_Close(SpeciesListMenu *menu) {
    int i;
    PlaySE(SEQ_SE_DP_SELECT);
    DestroyListMenu(menu->listMenu_23C, NULL, NULL);
    EraseFrameAndWindow1(menu->listMenuTemplate.window, FALSE);
    RemoveWindow(&menu->window_8);

    for (i = 0; i < 0x78; i++) {
        String_Delete(menu->stringArr_1C[i]);
    }

    if (((u32)(menu->flags << 0x1e) >> 0x1f) == 1) {
        DestroyMsgData(menu->msgdata);
    }

    SysTask_Destroy(menu->sysTask);

    Heap_Free(menu);
}

static const u16 sStatJudgeBestStatMsgIdxs[6] = {
    msg_0096_D31R0201_00122,
    msg_0096_D31R0201_00123,
    msg_0096_D31R0201_00124,
    msg_0096_D31R0201_00127,
    msg_0096_D31R0201_00125,
    msg_0096_D31R0201_00126,
};

BOOL ScrCmd_StatJudge(ScriptContext *ctx) {
    u32 ivList[6];
    u8 i;
    u8 offset;
    u8 highestIvValue;
    FieldSystem *fieldSystem = ctx->fieldSystem;
    u32 monIndex = ScriptGetVar(ctx);
    u16 *ivTotal = ScriptGetVarPointer(ctx);
    u16 *highestIvIndex = ScriptGetVarPointer(ctx);
    u16 *highestIv = ScriptGetVarPointer(ctx);
    Pokemon *mon = Party_GetMonByIndex(SaveArray_Party_Get(fieldSystem->saveData), monIndex);
    ivList[0] = GetMonData(mon, MON_DATA_HP_IV, 0);
    ivList[1] = GetMonData(mon, MON_DATA_ATK_IV, 0);
    ivList[2] = GetMonData(mon, MON_DATA_DEF_IV, 0);
    ivList[3] = GetMonData(mon, MON_DATA_SPEED_IV, 0);
    ivList[4] = GetMonData(mon, MON_DATA_SPATK_IV, 0);
    ivList[5] = GetMonData(mon, MON_DATA_SPDEF_IV, 0);

    highestIvValue = 0;
    *ivTotal = 0;

    for (i = 0; i < 6; ++i) {
        *ivTotal += ivList[i];
        if (highestIvValue < ivList[i]) {
            highestIvValue = ivList[i];
        }
    }

    offset = fieldSystem->judgeStatPosition;
    *highestIvIndex = offset;

    for (i = 0; i < 6; ++i) {
        if (highestIvValue == ivList[offset]) {
            *highestIvIndex = offset;
            fieldSystem->judgeStatPosition = (offset + 1) % 6;
            break;
        }
        offset = (offset + 1) % 6;
    }

    *highestIv = highestIvValue;
    *highestIvIndex = sStatJudgeBestStatMsgIdxs[*highestIvIndex];

    return FALSE;
}

BOOL ScrCmd_CommSanitizeParty(ScriptContext *ctx) {
    int partyCount, i, form;
    u32 species, data;
    u32 heldItems[6];
    Pokemon *mon;
    int count = 0;
    FieldSystem *fieldSystem = ctx->fieldSystem;
    u16 *success = ScriptGetVarPointer(ctx);
    Party *party = SaveArray_Party_Get(fieldSystem->saveData);
    partyCount = Party_GetCount(party);

    *success = FALSE;

    for (i = 0; i < partyCount; i++) {
        mon = Party_GetMonByIndex(party, i);
        heldItems[i] = GetMonData(mon, MON_DATA_HELD_ITEM, 0);
        if (heldItems[i] == ITEM_GRISEOUS_ORB) {
            count++;
        }
    }
    if (count > 0) {
        if (Bag_AddItem(Save_Bag_Get(fieldSystem->saveData), ITEM_GRISEOUS_ORB, count, HEAP_ID_FIELD1) == FALSE) {
            *success = 255;
            return FALSE;
        }

        data = 0;
        for (i = 0; i < partyCount; i++) {
            if (heldItems[i] == ITEM_GRISEOUS_ORB) {
                mon = Party_GetMonByIndex(party, i);
                SetMonData(mon, MON_DATA_HELD_ITEM, &data);
            }
        }
    }

    for (i = 0; i < partyCount; i++) {
        mon = Party_GetMonByIndex(party, i);
        form = GetMonData(mon, MON_DATA_FORM, 0);
        if (form > 0) {
            species = GetMonData(mon, MON_DATA_SPECIES, 0);
            switch (species) {
            case SPECIES_GIRATINA:
                Mon_UpdateGiratinaForm(mon);
                break;
            case SPECIES_ROTOM:
                Mon_UpdateRotomForm(mon, 0, 0);
                break;
            case SPECIES_SHAYMIN:
                Mon_UpdateShayminForm(mon, 0);
                break;
            }
        }
    }
    return FALSE;
}

BOOL ScrCmd_SetMonForm(ScriptContext *ctx) {
    FieldSystem *fieldSystem = ctx->fieldSystem;
    u16 index = ScriptGetVar(ctx);
    u16 form = ScriptGetVar(ctx);
    Pokemon *mon = Party_GetMonByIndex(SaveArray_Party_Get(fieldSystem->saveData), index);
    SetMonData(mon, MON_DATA_FORM, &form);
    return FALSE;
}

BOOL ScrCmd_CountTranformedRotomsInParty(ScriptContext *ctx) {
    int i, partyCount, count;
    u32 species, form, isEgg;
    Pokemon *mon;
    Party *party;
    FieldSystem *fieldSystem = ctx->fieldSystem;
    u16 *rotomCount = ScriptGetVarPointer(ctx);
    u16 *firstIndex = ScriptGetVarPointer(ctx);

    count = 0;
    *firstIndex = 255;

    party = SaveArray_Party_Get(fieldSystem->saveData);
    partyCount = Party_GetCount(party);
    for (i = 0; i < partyCount; i++) {
        mon = Party_GetMonByIndex(party, i);
        species = GetMonData(mon, MON_DATA_SPECIES, 0);
        form = GetMonData(mon, MON_DATA_FORM, 0);
        isEgg = GetMonData(mon, MON_DATA_IS_EGG, 0);
        if (species == SPECIES_ROTOM && form && !isEgg) {
            if (*firstIndex == 255) {
                *firstIndex = i;
            }
            count++;
        }
    }
    *rotomCount = count;
    return FALSE;
}

BOOL ScrCmd_UpdateRotomForm(ScriptContext *ctx) {
    FieldSystem *fieldSystem = ctx->fieldSystem;
    int rotomIndex = ScriptGetVar(ctx);
    int defaultSlot = ScriptGetVar(ctx);
    ScriptGetVar(ctx); // unsused variable
    u32 form = ScriptGetVar(ctx);
    Pokemon *mon = Party_GetMonByIndex(SaveArray_Party_Get(fieldSystem->saveData), rotomIndex);
    Mon_UpdateRotomForm(mon, form, defaultSlot);
    Pokedex_SetMonCaughtFlag(Save_Pokedex_Get(fieldSystem->saveData), mon);
    return FALSE;
}

BOOL ScrCmd_GetHiddenPowerType(ScriptContext *ctx) {
    s32 power, type;
    FieldSystem *fieldSystem = ctx->fieldSystem;
    int partyIndex = ScriptGetVar(ctx);
    u16 *typePtr = ScriptGetVarPointer(ctx);
    Pokemon *mon = Party_GetMonByIndex(SaveArray_Party_Get(fieldSystem->saveData), partyIndex);

    u16 species = GetMonData(mon, MON_DATA_SPECIES, 0);

    if (GetMonData(mon, MON_DATA_IS_EGG, 0) == FALSE) {
        switch (species) {
        case SPECIES_CATERPIE:
        case SPECIES_METAPOD:
        case SPECIES_WEEDLE:
        case SPECIES_KAKUNA:
        case SPECIES_MAGIKARP:
        case SPECIES_DITTO:
        case SPECIES_WOBBUFFET:
        case SPECIES_SMEARGLE:
        case SPECIES_WURMPLE:
        case SPECIES_SILCOON:
        case SPECIES_CASCOON:
        case SPECIES_WYNAUT:
        case SPECIES_BELDUM:
        case SPECIES_KRICKETOT:
        case SPECIES_BURMY: // Despite Burmy learning hidden power by level up, you cannot check what type it is until it evolves
        case SPECIES_COMBEE:
            *typePtr = -1;
            return FALSE;
        }
    }

    GetHiddenPowerPowerType(mon, &power, &type);
    *typePtr = type;

    return FALSE;
}

static void GetHiddenPowerPowerType(Pokemon *mon, s32 *power, s32 *type) {
    int hpIv = GetMonData(mon, MON_DATA_HP_IV, 0);
    int atkIv = GetMonData(mon, MON_DATA_ATK_IV, 0);
    int defIv = GetMonData(mon, MON_DATA_DEF_IV, 0);
    int spdIv = GetMonData(mon, MON_DATA_SPEED_IV, 0);
    int spatkIv = GetMonData(mon, MON_DATA_SPATK_IV, 0);
    int spdefIv = GetMonData(mon, MON_DATA_SPDEF_IV, 0);
    if (power) {
        *power = ((hpIv & 2) >> 1) | (atkIv & 2) | ((defIv & 2) << 1) | ((spdIv & 2) << 2) | ((spatkIv & 2) << 3) | ((spdefIv & 2) << 4);
        *power = ((*power) * 40 / 63) + 30;
    }
    if (type) {
        *type = (hpIv & 1) | ((atkIv & 1) << 1) | ((defIv & 1) << 2) | ((spdIv & 1) << 3) | ((spatkIv & 1) << 4) | ((spdefIv & 1) << 5);
        *type = ((*type) * 15 / 63) + 1;

        if (*type >= 9) {
            (*type)++;
        }
    }
}

BOOL ScrCmd_SetFavoriteMon(ScriptContext *ctx) {
    FieldSystem *fieldSystem = ctx->fieldSystem;
    Pokemon *mon = Party_GetMonByIndex(SaveArray_Party_Get(ctx->fieldSystem->saveData), 0);
    SAVE_MISC_DATA *data = Save_Misc_Get(fieldSystem->saveData);
    u32 species = GetMonData(mon, MON_DATA_SPECIES, 0);
    u32 form = GetMonData(mon, MON_DATA_FORM, 0);
    u32 isEgg = GetMonData(mon, MON_DATA_IS_EGG, 0);
    SaveMisc_SetFavoriteMon(data, species, form, isEgg);
    return FALSE;
}

BOOL ScrCmd_GetFavoriteMon(ScriptContext *ctx) {
    FieldSystem *fieldSystem = ctx->fieldSystem;
    int sp, form, egg;
    u16 *species = ScriptGetVarPointer(ctx);
    u16 *formPtr = ScriptGetVarPointer(ctx);
    u16 *isEgg = ScriptGetVarPointer(ctx);
    SAVE_MISC_DATA *data = Save_Misc_Get(fieldSystem->saveData);
    SaveMisc_GetFavoriteMon(data, &sp, &form, &egg);
    *species = sp;
    *formPtr = form;
    *isEgg = egg;
    return FALSE;
}

BOOL ScrCmd_GetPartyMonForm(ScriptContext *ctx) {
    FieldSystem *fieldSystem = ctx->fieldSystem;
    u32 index = ScriptGetVar(ctx);
    u16 *form = ScriptGetVarPointer(ctx);
    Pokemon *mon = Party_GetMonByIndex(SaveArray_Party_Get(fieldSystem->saveData), index);
    *form = GetMonData(mon, MON_DATA_FORM, 0);
    return FALSE;
}

BOOL ScrCmd_SetOtherObjectsFlag13(ScriptContext *ctx) {
    int index;
    FieldSystem *fieldSystem;
    MapObjectManager *mapObjectManager;
    LocalMapObject *playerObj;
    LocalMapObject *curObj;
    Sprite *sprite;
    s32 y;
    VecFx32 vec;

    index = 0;

    fieldSystem = ctx->fieldSystem;
    mapObjectManager = fieldSystem->mapObjectManager;

    playerObj = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
    MapObject_CopyPositionVector(playerObj, &vec);

    y = vec.y;

    while (MapObjectManager_GetNextObjectWithFlagFromIndex(mapObjectManager, &curObj, &index, MAPOBJECTFLAG_ACTIVE) == TRUE) {
        if (curObj == playerObj) {
            continue;
        }
        MapObject_SetFlagsBits(curObj, MAPOBJECTFLAG_UNK13);
        if (MapObject_TestFlagsBits(curObj, MAPOBJECTFLAG_UNK12) == TRUE) {
            MapObject_CopyPositionVector(curObj, &vec);
            vec.y = y;
            MapObject_SetPositionVector(curObj, &vec);
            MapObject_SetCurrentY(curObj, (y >> 3) / FX32_ONE);
        }
        sprite = MapObject_GetBillboard(curObj);
        if (sprite) {
            MapObject_UpdateBillboardPosition(curObj, sprite);
            BillboardObject_SetVisible(sprite, 1);
        }
    }
    return FALSE;
}

BOOL ScrCmd_ClearOtherObjectsFlag13(ScriptContext *ctx) {
    int index = 0;
    FieldSystem *fieldSystem;
    MapObjectManager *mapObjectManager;
    LocalMapObject *playerObj;
    LocalMapObject *curObj;

    fieldSystem = ctx->fieldSystem;
    mapObjectManager = fieldSystem->mapObjectManager;

    playerObj = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);

    while (MapObjectManager_GetNextObjectWithFlagFromIndex(mapObjectManager, &curObj, &index, MAPOBJECTFLAG_ACTIVE) == TRUE) {
        if (curObj == playerObj) {
            continue;
        }
        MapObject_ClearFlagsBits(curObj, MAPOBJECTFLAG_UNK13);
    }

    return FALSE;
}

BOOL ScrCmd_BattleTowerSetUpMultiBattle(ScriptContext *ctx) {
    void *data = Save_WiFiHistory_Get(ctx->fieldSystem->saveData);
    Comm_RecordPlayerLocations(data);
    return TRUE;
}

BOOL ScrCmd_SetPlayerVolume(ScriptContext *ctx) {
    GF_SndHandleSetPlayerVolume(1, ScriptGetVar(ctx));
    return FALSE;
}

BOOL ScrCmd_CheckMonSeen(ScriptContext *ctx) {
    u16 monNumber = ScriptGetVar(ctx);
    u16 *seenFlag = ScriptGetVarPointer(ctx);

    *seenFlag = Pokedex_CheckMonSeenFlag(Save_Pokedex_Get(ctx->fieldSystem->saveData), monNumber);

    return FALSE;
}

// Related to the trapped floor in the rocket hideout
BOOL ScrCmd_PlayTrapEffect(ScriptContext *ctx) {
    u32 kind = ScriptGetVar(ctx);
    ShowTrapEffect(ctx->fieldSystem->taskman, kind);
    return TRUE;
}

BOOL ScrCmd_ShowLegendaryWing(ScriptContext *ctx) {
    u32 kind = ScriptGetVar(ctx);
    ShowLegendaryWing(ctx->fieldSystem->taskman, kind);
    return TRUE;
}

// Related to Persian statues in the rocket hideout
BOOL ScrCmd_PlayHideoutAlarm(ScriptContext *ctx) {
    ShowHideoutAlarm(ctx->fieldSystem->taskman);
    return TRUE;
}

BOOL ScrCmd_ScreenShake(ScriptContext *ctx) {
    u32 amplitudeX = ScriptGetVar(ctx);
    u32 amplitudeY = ScriptGetVar(ctx);
    u32 repeats = ScriptGetVar(ctx);
    u16 period = ScriptGetVar(ctx);
    ShakeScreen(ctx->fieldSystem->taskman, amplitudeX, amplitudeY, repeats, period);
    return TRUE;
}

// Related to Lance flying off on his dragonite in the Lake of Rage
BOOL ScrCmd_LanceFlyAway(ScriptContext *ctx) {
    u32 objIdA = ScriptGetVar(ctx);
    u32 objIdB = ScriptGetVar(ctx);

    LocalMapObject *objA = MapObjectManager_GetFirstActiveObjectByID(ctx->fieldSystem->mapObjectManager, objIdA);
    LocalMapObject *objB = MapObjectManager_GetFirstActiveObjectByID(ctx->fieldSystem->mapObjectManager, objIdB);

    LanceFliesAway(ctx->fieldSystem->taskman, objA, objB);

    return TRUE;
}

BOOL ScrCmd_OpenAlphHiddenRoom(ScriptContext *ctx) {
    OpenAlphHiddenRoom(ctx->fieldSystem->taskman, ScriptReadByte(ctx));
    return TRUE;
}

static u32 ClampToU16Max(u32 value) {
    u16 ret = 0xFFFF;
    if (value <= ret) {
        ret = value;
    }
    return ret;
}

// Related to Pokeathlon "Ball Sign"- whatever that is
BOOL ScrCmd_GetPokeathlonStat(ScriptContext *ctx) {
    u8 statId = ScriptGetVar(ctx);
    u16 *result = ScriptGetVarPointer(ctx);

    PokeathlonStatistics *stats = PokeathlonSave_GetStatistics(Save_Pokeathlon_Get(ctx->fieldSystem->saveData));

    if (statId <= 9) {
        *result = ClampToU16Max(stats->eventFirstPlaces[statId]);
        return FALSE;
    }

    switch (statId - 10) {
    case 0:
        *result = ClampToU16Max(stats->courseFirstPlaces);
        break;
    case 1:
        *result = ClampToU16Max(stats->fanCount);
        break;
    case 2:
        *result = ClampToU16Max(stats->coursesJoined);
        break;
    case 3:
        *result = ClampToU16Max(stats->monInstructions);
        break;
    case 4:
        *result = ClampToU16Max(stats->monJumps);
        break;
    case 5:
        *result = ClampToU16Max(stats->monTackles);
        break;
    case 6:
        *result = ClampToU16Max(stats->monDashes);
        break;
    case 7:
        *result = ClampToU16Max(stats->monSwitches);
        break;
    default:
        *result = 0;
        break;
    }

    return FALSE;
}

// This does something with pokeathlon records like "number times jumped"
BOOL ScrCmd_AdjustPokeathlonStat11(ScriptContext *ctx) {
    s32 val;

    u8 subtract = ScriptReadByte(ctx);
    u32 amount = ScriptGetVar(ctx);

    PokeathlonStatistics *stats = PokeathlonSave_GetStatistics2(Save_Pokeathlon_Get(ctx->fieldSystem->saveData));

    if (subtract == 0) {
        val = stats->fanCount + amount;
        if ((u32)val > 0xFFFF) {
            stats->fanCount = 0xFFFF;
        } else {
            stats->fanCount = val;
        }
    } else {
        val = stats->fanCount - amount;
        if (val < 0) {
            stats->fanCount = 0;
        } else {
            stats->fanCount = val;
        }
    }

    return FALSE;
}

BOOL ScrCmd_ProcessSoundplate(ScriptContext *ctx) {
    FieldSystem_ProcessSoundplate(ctx->fieldSystem, TRUE);
    return FALSE;
}

// Related to Kurt
BOOL ScrCmd_GetKurtApricornQuantity(ScriptContext *ctx) {
    u16 *apricornQuantity = ScriptGetVarPointer(ctx);
    *apricornQuantity = ApricornBox_GetKurtQuantity(Save_ApricornBox_Get(ctx->fieldSystem->saveData));
    return FALSE;
}

BOOL ScrCmd_ClearKurtApricorn(ScriptContext *ctx) {
    ApricornBox_SetKurtApricorn(Save_ApricornBox_Get(ctx->fieldSystem->saveData), 0, 0);
    return FALSE;
}

// Related to Kurt
BOOL ScrCmd_GetKurtBall(ScriptContext *ctx) {
    u16 *ball = ScriptGetVarPointer(ctx);
    *ball = ApricornBox_GetKurtBall(Save_ApricornBox_Get(ctx->fieldSystem->saveData));
    return FALSE;
}

// Related to Kurt
BOOL ScrCmd_GetTotalApricornCount(ScriptContext *ctx) {
    s32 i;
    u32 cnt;
    u16 *count = ScriptGetVarPointer(ctx);
    cnt = 0;
    SaveApricornBox *apricornBox = Save_ApricornBox_Get(ctx->fieldSystem->saveData);
    for (i = 0; i < 7; i = i + 1) {
        cnt += ApricornBox_CountApricorn(apricornBox, i);
    }
    *count = cnt;
    return FALSE;
}

// Related to Kurt- canceling
BOOL ScrCmd_LaunchApricornBoxForKurt(ScriptContext *ctx) { // todo: rename structs and find out stuff
    ApricornBoxArgs **args = FieldSysGetAttrAddr(ctx->fieldSystem, SCRIPTENV_RUNNING_APP_DATA);
    *args = ApricornBox_LaunchApp(ctx->fieldSystem, 2);
    SetupNativeScript(ctx, ScrNative_WaitApplication_DestroyTaskData);
    return TRUE;
}

// Related to aprijuice stand- canceling
BOOL ScrCmd_LaunchApricornBoxForJuice(ScriptContext *ctx) {
    ApricornBoxArgs **args = FieldSysGetAttrAddr(ctx->fieldSystem, SCRIPTENV_RUNNING_APP_DATA);
    u32 unkVar = ScriptGetVar(ctx);
    u16 *juiceUsedVar = ScriptGetVarPointer(ctx);
    *args = ApricornBox_LaunchAppForJuice(ctx->fieldSystem, unkVar, juiceUsedVar);
    SetupNativeScript(ctx, ScrNative_WaitApplication_DestroyTaskData);
    return TRUE;
}

// Related to aprijuice stand- viewing label
BOOL ScrCmd_GetApricornJuiceOffer(ScriptContext *ctx) {
    ApricornJuice juice;
    RTCDate date;
    s32 unkVar;
    MessageFormat **msgfmt;
    SaveApricornBox *apricornBox;
    u16 *price;
    u16 *juiceIndex;
    u16 *unkPtrC;
    String *str;

    apricornBox = Save_ApricornBox_Get(ctx->fieldSystem->saveData);
    msgfmt = FieldSysGetAttrAddr(ctx->fieldSystem, SCRIPTENV_MESSAGE_FORMAT);
    unkVar = ScriptGetVar(ctx);
    juiceIndex = ScriptGetVarPointer(ctx);
    price = ScriptGetVarPointer(ctx);
    unkPtrC = ScriptGetVarPointer(ctx);
    GF_RTC_CopyDate(&date);
    *juiceIndex = unkVar + 5;
    if (!ApricornBox_GetJuice(apricornBox, *juiceIndex, &juice)) {
        *juiceIndex = (date.week + 2 * unkVar) % 5;
        ApricornBox_GetJuice(apricornBox, *juiceIndex, &juice);
    }
    str = ApricornBox_GetJuiceName(apricornBox, *juiceIndex, 0x20);
    BufferString(*msgfmt, 0, str, 2, 1, gGameVersion); // buffer owner..?
    Heap_Free(str);
    str = ApricornBox_GetJuiceFlavorName(apricornBox, *juiceIndex, 0x20);
    BufferString(*msgfmt, 1, str, 2, 1, gGameVersion); // buffer juice type..?
    Heap_Free(str);

    unkVar = ApricornJuice_GetFlavorTotal(&juice);
    *price = (juice.unk0 / 10) + 2 * unkVar;

    if (*price < 100) {
        *price = 100;
    } else if (*price > 5000) {
        *price = 5000;
    }

    BufferIntegerAsString(*msgfmt, 2, *price, 4, PRINTING_MODE_LEFT_ALIGN, 1);
    BufferIntegerAsString(*msgfmt, 3, unkVar, 3, PRINTING_MODE_LEFT_ALIGN, 1);          // richness..?
    BufferIntegerAsString(*msgfmt, 4, juice.mildness, 3, PRINTING_MODE_LEFT_ALIGN, 1); // smoothness..?

    *unkPtrC = juice.unk0;

    return FALSE;
}

// Gets called after selecting which type of pokeathlon data to load- might just outright start the loading process
BOOL ScrCmd_ShowPokeathlonRecords(ScriptContext *ctx) {
    PokeathlonRecords_Show(ctx->taskman, (enum PokeathlonData)ScriptGetVar(ctx));
    return TRUE;
}

static const u8 sFriendshipRoomStatuesPositions[3][2] = {
    { 4,  10 },
    { 7,  5  },
    { 10, 10 },
};

BOOL ScrCmd_CreatePokeathlonFriendshipRoomStatues(ScriptContext *ctx) {
    s32 i;
    u16 species;
    FieldSystem *fieldSystem = ctx->fieldSystem;

    PokeathlonSave_FriendshipRecords *records = Save_Pokeathlon_FriendshipRecords_Get(fieldSystem->saveData);

    for (i = 0; i < 3; i++) {
        LocalMapObject *mapObj = MapObjectManager_GetFirstActiveObjectByID(fieldSystem->mapObjectManager, 0xf6 + i);

        if (mapObj) {
            MapObject_Delete(mapObj);
        }

        species = records->friendshipRoomStatues[i].species;

        if (species != SPECIES_NONE && species <= SPECIES_ARCEUS) {
            FriendshipRoom_CreateStatueObject(fieldSystem->mapObjectManager, (u8)i, species, records->friendshipRoomStatues[i].form, records->friendshipRoomStatues[i].gender, sFriendshipRoomStatuesPositions[i][0], sFriendshipRoomStatuesPositions[i][1], fieldSystem->location->mapId);
        }
    }
    return TRUE;
}

static LocalMapObject *FriendshipRoom_CreateStatueObject(MapObjectManager *mapObjectManager, u8 slot, u16 species, u16 form, u32 gender, u32 x, u32 y, u32 mapId) {
    LocalMapObject *mapObj;
    u32 spriteId;
    u32 size;

    spriteId = FollowMon_GetSpriteID(species, form, gender) << 1;
    size = FollowMon_GetSizeParamBySpecies(species) * 3 + slot;

    mapObj = MapObject_CreateWithParams(mapObjectManager, x, y, DIR_SOUTH, size + 0x19f, 0, mapId, 0, 0, spriteId);

    if (!mapObj) {
        GF_AssertFail();
    }

    MapObject_SetID(mapObj, slot + 0xf6);
    MapObject_SetType(mapObj, 0);
    MapObject_SetEventFlag(mapObj, 0);
    MapObject_SetScriptID(mapObj, 0);
    MapObject_SetParam(mapObj, 0, 2);
    FollowMon_SetObjectMon(mapObj, species, (u8)form, FALSE, spriteId);
    MapObject_SetXRange(mapObj, -1);
    MapObject_SetYRange(mapObj, -1);
    MapObject_SetFlagsBits(mapObj, MAPOBJECTFLAG_UNK30);
    MapObject_ClearFlagsBits(mapObj, MAPOBJECTFLAG_NONE);
    MapObject_SetFlag29(mapObj, FALSE);

    return mapObj;
}

BOOL ScrCmd_CheckSeenAllLetterUnown(ScriptContext *ctx) {
    u32 form;
    s32 i;
    u32 counter;
    s32 unownForms;
    Pokedex *pokedex;
    u16 *allUnownSeen;

    allUnownSeen = ScriptGetVarPointer(ctx);
    pokedex = Save_Pokedex_Get(ctx->fieldSystem->saveData);

    unownForms = Pokedex_GetSeenFormNum_Unown(pokedex, 1);
    if (unownForms < 26) {
        *allUnownSeen = FALSE;
        return TRUE;
    }

    counter = 0;
    for (i = 0; i < unownForms; i++) {
        form = Pokedex_GetSeenFormByIdx_Unown(pokedex, i, TRUE);
        if (form != 26 && form != 27) {
            counter++;
        }
    }

    if (counter == 26) {
        *allUnownSeen = TRUE;
    } else {
        *allUnownSeen = FALSE;
    }

    return TRUE;
}

BOOL ScrCmd_GiveTogepiEgg(ScriptContext *ctx) {
    s32 i;
    u8 pp;
    u16 moveData;
    Pokemon *mon;
    PlayerProfile *profile;
    Party *party;
    FieldSystem *fieldSystem = ctx->fieldSystem;

    profile = Save_PlayerData_GetProfile(fieldSystem->saveData);
    party = SaveArray_Party_Get(fieldSystem->saveData);

    if (Party_GetCount(party) >= 6) {
        return FALSE;
    }

    mon = AllocMonZeroed(HEAP_ID_FIELD2);
    ZeroMonData(mon);

    SetEggStats(mon, SPECIES_TOGEPI, 1, profile, 3, Mapsec_FromTypeAndIndex(MAPSECTYPE_GIFT, MAPLOC(METLOC_MR_POKEMON)));

    for (i = 0; i < MAX_MON_MOVES; i++) {
        if (GetMonData(mon, MON_DATA_MOVE1 + i, 0) == MOVE_NONE) {
            break;
        }
    }

    if (i == MAX_MON_MOVES) {
        i = MAX_MON_MOVES - 1;
    }

    moveData = MOVE_EXTRASENSORY;
    SetMonData(mon, MON_DATA_MOVE1 + i, &moveData);

    pp = GetMonData(mon, MON_DATA_MOVE1_MAX_PP + i, 0);
    SetMonData(mon, MON_DATA_MOVE1_PP + i, &pp);

    Party_AddMon(party, mon);

    Heap_Free(mon);

    SaveMisc_SetTogepiPersonalityGender(Save_Misc_Get(fieldSystem->saveData), GetMonData(mon, MON_DATA_PERSONALITY, 0), GetMonData(mon, MON_DATA_GENDER, 0));

    return FALSE;
}

// unused
BOOL ScrCmd_MonIsFromTogepiEgg(ScriptContext *ctx) {
    u32 partyIndex = ScriptGetVar(ctx);
    u16 *result = ScriptGetVarPointer(ctx);

    Pokemon *mon = Party_GetMonByIndex(SaveArray_Party_Get(ctx->fieldSystem->saveData), partyIndex);

    *result = MonIsFromTogepiEgg(mon, ctx->fieldSystem->saveData);

    return FALSE;
}

static const u16 sSpikyEarPichuMoveset[4] = {
    MOVE_HELPING_HAND,
    MOVE_VOLT_TACKLE,
    MOVE_SWAGGER,
    MOVE_PAIN_SPLIT,
};

BOOL ScrCmd_GiveSpikyEarPichu(ScriptContext *ctx) {
    s32 i;
    u8 form;
    u8 maxPP;
    u16 heldItem;
    Pokemon *mon;
    Party *party;
    FieldSystem *fieldSystem;
    PlayerProfile *profile;

    fieldSystem = ctx->fieldSystem;

    profile = Save_PlayerData_GetProfile(fieldSystem->saveData);
    party = SaveArray_Party_Get(fieldSystem->saveData);
    if (Party_GetCount(party) >= 6) {
        return FALSE;
    }
    mon = AllocMonZeroed(HEAP_ID_FIELD2);
    ZeroMonData(mon);

    u32 trId = PlayerProfile_GetTrainerID(profile);
    u32 personality = ChangePersonalityToNatureGenderAndAbility(trId, 0xac, NATURE_NAUGHTY, MON_FEMALE, 0, 0);
    CreateMon(mon, SPECIES_PICHU, 30, 0x20, 1, personality, 1, trId);

    form = 1;
    SetMonData(mon, MON_DATA_FORM, &form);

    for (i = 0; i < MAX_MON_MOVES; i++) {
        SetMonData(mon, MON_DATA_MOVE1 + i, &sSpikyEarPichuMoveset[i]);
        maxPP = GetMonData(mon, MON_DATA_MOVE1_MAX_PP + i, 0);
        SetMonData(mon, MON_DATA_MOVE1_PP + i, &maxPP);
    }

    heldItem = ITEM_ZAP_PLATE;
    SetMonData(mon, MON_DATA_HELD_ITEM, &heldItem);

    u32 mapsec = Mapsec_FromTypeAndIndex(MAPSECTYPE_NORMAL, MapHeader_GetMapSec(ctx->fieldSystem->location->mapId));

    MonSetCatchData(mon, profile, 4, mapsec, 0x18, HEAP_ID_FIELD2);

    Party_AddMon(party, mon);

    Heap_Free(mon);

    UpdatePokedexWithReceivedSpecies(fieldSystem->saveData, mon);

    return FALSE;
}

BOOL ScrCmd_PhotoAlbumIsFull(ScriptContext *ctx) {
    FieldSystem *fieldSystem = ctx->fieldSystem;
    u16 *albumIsFull = ScriptGetVarPointer(ctx);

    *albumIsFull = (PhotoAlbum_GetNumSaved(Save_PhotoAlbum_Get(fieldSystem->saveData)) >= 36);

    return FALSE;
}

BOOL ScrCmd_RadioMusicIsPlaying(ScriptContext *ctx) {
    u32 musicSeq = ScriptGetVar(ctx);
    u16 *isPlaying = ScriptGetVarPointer(ctx);

    *isPlaying = (GetRadioMusicPlayingSeq() == musicSeq);

    return FALSE;
}

static const int sSlotLuckDistribution[2][2] = {
    { 7, 3 },
    { 5, 2 },
};

// Return value is unused outside of Japanese copies.
static u32 SlotLuckiness(SaveData *saveData, u8 machineId, u8 city) {
    SAV_FRIEND_GRP *friendGroup;
    u8 *luckValues;
    u32 numMachines;
    s32 rngSeed;
    u8 luckiness;
    u8 randIdx;
    u8 i;
    u8 j;

    friendGroup = Save_FriendGroup_Get(saveData);

    if (city != 0) { // 1 = celadon; 0 = goldenrod
        numMachines = 14;
    } else {
        numMachines = 21;
    }

    if (machineId >= numMachines) {
        return 3;
    }

    rngSeed = GetLCRNGSeed();
    SetLCRNGSeed(Save_FriendGroup_GetCurrentSeed(friendGroup));

    luckValues = Heap_AllocAtEnd(HEAP_ID_FIELD3, numMachines);
    MI_CpuFill8(luckValues, 0, numMachines);

    for (i = 0; i < NELEMS(sSlotLuckDistribution[city]); ++i) {
        for (j = 0; j < sSlotLuckDistribution[city][i]; ++j) {
            do {
                randIdx = LCRandom() % (s32)numMachines; // this cast appears necessary
            } while (luckValues[randIdx] != 0);
            luckValues[randIdx] = i + 1;
        }
    }

    luckiness = luckValues[machineId];

    Heap_Free(luckValues);
    SetLCRNGSeed(rngSeed);

    return luckiness;
}

BOOL ScrCmd_CasinoGame(ScriptContext *ctx) {
    u8 machineId = ScriptReadByte(ctx);
    u8 city = ScriptReadByte(ctx); // 1 = celadon; 0 = goldenrod
    VoltorbFlipArgs **args = FieldSysGetAttrAddr(ctx->fieldSystem, SCRIPTENV_RUNNING_APP_DATA);

    *args = VoltorbFlip_LaunchApp(ctx->fieldSystem, SlotLuckiness(ctx->fieldSystem->saveData, machineId, city)); // this is messy, very very messy

    SetupNativeScript(ctx, ScrNative_WaitApplication_DestroyTaskData);
    return TRUE;
}

BOOL ScrCmd_BufferPokeathlonCourseName(ScriptContext *ctx) {
    u8 fieldNo = ScriptReadByte(ctx);
    u32 courseId = ScriptGetVar(ctx);
    BufferPokeathlonCourseName(*(MessageFormat **)FieldSysGetAttrAddr(ctx->fieldSystem, SCRIPTENV_MESSAGE_FORMAT), fieldNo, (u8)courseId);
    return FALSE;
}

// Gets called when getting a package from your mom
BOOL ScrCmd_PeekMomGift(ScriptContext *ctx) {
    u16 *unkPtr1 = ScriptGetVarPointer(ctx);
    u16 *unkPtr2 = ScriptGetVarPointer(ctx);

    *unkPtr2 = PhoneCallPersistentState_MomGiftQueue_Peek(SaveData_GetPhoneCallPersistentState(ctx->fieldSystem->saveData), 0, unkPtr1);

    return FALSE;
}

// Gets called when getting a package from your mom
BOOL ScrCmd_PopMomGift(ScriptContext *ctx) {
    PhoneCallPersistentState_MomGiftQueue_Get(SaveData_GetPhoneCallPersistentState(ctx->fieldSystem->saveData));
    return FALSE;
}

BOOL ScrCmd_GetBuenasPassword(ScriptContext *ctx) {
    u16 *msgPtr = ScriptGetVarPointer(ctx);
    u16 *unkPtr2 = ScriptGetVarPointer(ctx);

    u8 unkVar = Save_VarsFlags_GetBuenasPasswordSet(Save_VarsFlags_Get(ctx->fieldSystem->saveData)) % 30;

    *msgPtr = 2 * (unkVar / 3) + unkVar / 3 + msg_0066_D23R0102_00040;

    *unkPtr2 = unkVar % 3;
    return FALSE;
}

static u32 MonGetShinyLeafCount(Pokemon *mon) {
    int c;
    u32 shinyLeafCount = 0;

    for (c = 0; c < 5; c++) {
        shinyLeafCount += GetMonData(mon, MON_DATA_SHINY_LEAF_A + c, NULL);
    }
    if (shinyLeafCount == 5 && GetMonData(mon, MON_DATA_SHINY_LEAF_CROWN, NULL) != 0) {
        shinyLeafCount = 6;
    }

    return shinyLeafCount;
}

BOOL ScrCmd_GetShinyLeafCount(ScriptContext *ctx) {
    u32 monIndex = ScriptGetVar(ctx);
    u16 *shinyLeafCount = ScriptGetVarPointer(ctx);
    *shinyLeafCount = MonGetShinyLeafCount(Party_GetMonByIndex(SaveArray_Party_Get(ctx->fieldSystem->saveData), monIndex));
    return FALSE;
}

BOOL ScrCmd_TryGiveShinyLeafCrown(ScriptContext *ctx) {
    u32 monIndex = ScriptGetVar(ctx);

    Pokemon *mon = Party_GetMonByIndex(SaveArray_Party_Get(ctx->fieldSystem->saveData), monIndex);
    if (MonGetShinyLeafCount(Party_GetMonByIndex(SaveArray_Party_Get(ctx->fieldSystem->saveData), monIndex)) == 5) {
        u8 data = TRUE;
        SetMonData(mon, MON_DATA_SHINY_LEAF_CROWN, &data);
    }

    return FALSE;
}

BOOL ScrCmd_GetUniqueSealsQuantity(ScriptContext *ctx) {
    u16 *uniqueSeals = ScriptGetVarPointer(ctx);

    *uniqueSeals = SealCase_CountUniqueSeals(Save_SealCase_Get(ctx->fieldSystem->saveData));

    return FALSE;
}

BOOL ScrCmd_GetSealQuantity(ScriptContext *ctx) {
    u32 sealId = ScriptGetVar(ctx);
    u16 *sealQuantity = ScriptGetVarPointer(ctx);

    *sealQuantity = SealCase_CountSealOccurrenceAnywhere(Save_SealCase_Get(ctx->fieldSystem->saveData), sealId);

    return FALSE;
}

BOOL ScrCmd_GiveOrTakeSeal(ScriptContext *ctx) {
    u32 sealId = ScriptGetVar(ctx);
    u32 quantity = ScriptGetVar(ctx);

    GiveOrTakeSeal(Save_SealCase_Get(ctx->fieldSystem->saveData), sealId, quantity);

    return FALSE;
}

static BOOL IsSealNonUnique(u16 sealId, u16 *uniqueSealIds, s32 size) {
    s32 c;

    for (c = 0; c < size; c++) {
        if (sealId == uniqueSealIds[c]) {
            return TRUE;
        }
    }

    return FALSE;
}

BOOL ScrCmd_GiveRandomSeal(ScriptContext *ctx) {
    int i, j;
    u16 randVal;
    s32 avaliableSeals;
    u8 uniqueSeals[3];
    u16 givenSealIds[3];
    u16 *sealThresholds;
    SealCase *sealcase;
    u16 *seal1;
    u16 *seal2;
    u16 *seal3;
    s32 givenSeals;

    avaliableSeals = 0;

    seal1 = ScriptGetVarPointer(ctx);
    seal2 = ScriptGetVarPointer(ctx);
    seal3 = ScriptGetVarPointer(ctx);

    sealcase = Save_SealCase_Get(ctx->fieldSystem->saveData);

    sealThresholds = Heap_AllocAtEnd(HEAP_ID_FIELD3, sizeof(u16) * SEAL_MYSTERY);

    MI_CpuFill8(sealThresholds, 0, sizeof(u16) * SEAL_MYSTERY);
    MI_CpuFill8(uniqueSeals, 0, sizeof(u8) * 3);

    // Seals indexed 'mystery' and onward weren't officially released
    for (i = 0; i < SEAL_MYSTERY; i++) {
        avaliableSeals += SEAL_INVENTORY_QMAX - SealCase_CountSealOccurrenceAnywhere(sealcase, i + 1);
        sealThresholds[i] = avaliableSeals;
    }

    if (avaliableSeals > 3) {
        givenSeals = 3;
    } else {
        givenSeals = avaliableSeals;
    }

    i = 0;

    while (i < givenSeals) {
        do {
            randVal = LCRandom() % avaliableSeals;
        } while (IsSealNonUnique(randVal, givenSealIds, i));

        givenSealIds[i] = randVal;

        for (j = 0; j < SEAL_MYSTERY; j++) {
            if (randVal < sealThresholds[j]) {
                uniqueSeals[i] = j + 1;
                GiveOrTakeSeal(sealcase, uniqueSeals[i], 1);
                break;
            }
        }
        i++;
    }

    Heap_Free(sealThresholds);

    *seal1 = uniqueSeals[0];
    *seal2 = uniqueSeals[1];
    *seal3 = uniqueSeals[2];

    return FALSE;
}

BOOL ScrCmd_CheckKyogreGroudonInParty(ScriptContext *ctx) {
    int c;
    u32 unkVar;
    int partyCount;
    Party *playerParty;
    u16 *unkPtr;

    u8 kyogreGroudonFlags = 0;

    unkPtr = ScriptGetVarPointer(ctx);
    playerParty = SaveArray_Party_Get(ctx->fieldSystem->saveData);
    partyCount = Party_GetCount(playerParty);
    unkVar = *(ctx->script_ptr);

    for (c = 0; c < partyCount; c++) {
        Pokemon *mon = Party_GetMonByIndex(playerParty, c);
        u16 species = GetMonData(mon, MON_DATA_SPECIES, NULL);
        u8 gameVer = GetMonData(mon, MON_DATA_MET_GAME, NULL);
        if (!GetMonData(mon, MON_DATA_IS_EGG, NULL)
            && !GetMonData(mon, MON_DATA_CHECKSUM_FAILED, NULL)
            && !GetMonData(mon, MON_DATA_EGG_LOCATION, NULL)) {

            switch (species) {
            case SPECIES_KYOGRE:
                if (gameVer == 7) {
                    kyogreGroudonFlags |= 1;
                }
                break;
            case SPECIES_GROUDON:
                if (gameVer == 8) {
                    kyogreGroudonFlags |= 1 << 1;
                }
                break;
            }
        }
    }

    if (kyogreGroudonFlags == 3) {
        *unkPtr = TRUE;
    } else {
        *unkPtr = FALSE;
    }

    return FALSE;
}

BOOL ScrCmd_SysSetSleepFlag(ScriptContext *ctx) {
    if (ScriptGetVar(ctx) != 0) {
        Sys_SetSleepDisableFlag(1 << 3);
    } else {
        Sys_ClearSleepDisableFlag(1 << 3);
    }
    return FALSE;
}

BOOL ScrCmd_BugContestAction(ScriptContext *ctx) {
    u8 unkVar1 = ScriptReadByte(ctx);
    u32 weekday = ScriptGetVar(ctx);

    FieldSystem *fieldSystem = ctx->fieldSystem;

    if (unkVar1 == 0) {
        fieldSystem->bugContest = BugContest_New(fieldSystem, weekday);
    } else {
        BugContest_Delete(fieldSystem->bugContest);
        fieldSystem->bugContest = NULL;
        FieldSystem_TryTriggerBillPCFullCall(fieldSystem);
    }
    return FALSE;
}

BOOL ScrCmd_BufferBugContestWinner(ScriptContext *ctx) {
    MessageFormat **msgfmt;
    BugContest *bugContest;

    msgfmt = FieldSysGetAttrAddr(ctx->fieldSystem, SCRIPTENV_MESSAGE_FORMAT);
    bugContest = FieldSystem_BugContest_Get(ctx->fieldSystem);

    BugContest_BufferContestWinnerNames(bugContest, ctx->msgdata, *msgfmt, *ctx->script_ptr++);

    return FALSE;
}

BOOL ScrCmd_JudgeBugContest(ScriptContext *ctx) {
    BugContest *bugContest;
    u16 *prize;
    u16 *placement;
    u16 *species;

    bugContest = FieldSystem_BugContest_Get(ctx->fieldSystem);
    placement = ScriptGetVarPointer(ctx);
    prize = ScriptGetVarPointer(ctx);
    species = ScriptGetVarPointer(ctx);

    BugContest_Judge(bugContest);

    *placement = bugContest->placement;
    *prize = bugContest->prize;

    if (bugContest->caught_poke == 0) {
        *species = 0;
    } else {
        *species = GetMonData(bugContest->mon, MON_DATA_SPECIES, NULL);
    }

    return FALSE;
}

BOOL ScrCmd_BufferBugContestMonNick(ScriptContext *ctx) {
    MessageFormat **msgfmt;
    BugContest *bugContest;
    u32 script_index;

    msgfmt = FieldSysGetAttrAddr(ctx->fieldSystem, SCRIPTENV_MESSAGE_FORMAT);
    bugContest = FieldSystem_BugContest_Get(ctx->fieldSystem);

    script_index = ScriptReadByte(ctx);

    u16 *monNick = ScriptGetVarPointer(ctx);
    *monNick = BugContest_BufferCaughtMonNick(bugContest, *msgfmt, script_index);
    return FALSE;
}

BOOL ScrCmd_BugContestGetTimeLeft(ScriptContext *ctx) {
    MessageFormat **msgfmt;
    BugContest *bugContest;
    u32 script_index;
    u32 timeLeft;

    msgfmt = FieldSysGetAttrAddr(ctx->fieldSystem, SCRIPTENV_MESSAGE_FORMAT);

    script_index = ScriptReadByte(ctx);
    timeLeft = 1;

    bugContest = FieldSystem_BugContest_Get(ctx->fieldSystem);

    if (bugContest != 0 && bugContest->elapsed_time < 20) {
        timeLeft = 20 - bugContest->elapsed_time;
    }

    BufferIntegerAsString(*msgfmt, script_index, timeLeft, 2, PRINTING_MODE_LEFT_ALIGN, 1);

    return FALSE;
}

BOOL ScrCmd_IsBugContestantRegistered(ScriptContext *ctx) {
    BugContest *bugContest = FieldSystem_BugContest_Get(ctx->fieldSystem);
    u32 id = ScriptGetVar(ctx);
    u16 *ptr = ScriptGetVarPointer(ctx);
    *ptr = BugContest_ContestantIsRegistered(bugContest, id);
    return FALSE;
}
