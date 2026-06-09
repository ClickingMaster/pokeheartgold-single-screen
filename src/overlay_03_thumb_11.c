#include "overlay_01.h"
#include "overlay_03.h"

#include "bag.h"
#include "field_system.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "global.h"
#include "link_ruleset_data.h"
#include "mart.h"
#include "render_text.h"
#include "render_window.h"
#include "save_link_ruleset.h"
#include "scrcmd.h"
#include "sys_flags.h"
#include "sys_vars.h"
#include "task.h"
#include "text.h"
#include "text_0205B4EC.h"
#include "unk_02005D10.h"
#include "unk_02034354.h"
#include "unk_02035900.h"
#include "unk_020379A0.h"
#include "unk_02037C94.h"
#include "unk_02091054.h"

#include "field/overlay_01_021E8744.h"
#include "field/ov01_021E7FDC.h"
#include "field/overlay_01_02204004.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0182.h"

extern u8 ov03_02259820[9];
extern u8 ov03_0225982C[9];
extern u16 ov03_0225945C[3];

int BattleRegulationMenu_ProcessListMenuInputConfirm(BattleRegulationMenu *menu) {
    int input = ListMenu_ProcessInput(menu->listMenu[REGULATION_MENU_CONFIRM]);
    ListMenuGetScrollAndRow(menu->listMenu[REGULATION_MENU_CONFIRM], 0, &menu->itemsAbove[REGULATION_MENU_CONFIRM]);
    
    u16 prev = menu->unk80[REGULATION_MENU_CONFIRM];
    ListMenuGetCurrentItemArrayId(menu->listMenu[REGULATION_MENU_CONFIRM], &menu->unk80[REGULATION_MENU_CONFIRM]);
    
    if (prev != menu->unk80[REGULATION_MENU_CONFIRM]) {
        PlaySE(SEQ_SE_DP_SELECT);
    }

    switch(input) {
        case -1:
            return 0;
        case -2:
            PlaySE(SEQ_SE_DP_SELECT);
            input = -1;
            break;
        default:
            PlaySE(SEQ_SE_DP_SELECT);
            BattleRegulationMenu_RemoveListMenuRegulations(menu);
            break;
    }

    if (menu->listMenu[REGULATION_MENU_CONFIRM]) {
        DestroyListMenu(menu->listMenu[REGULATION_MENU_CONFIRM], NULL, NULL);
        sub_0200E5D4(&menu->windows[REGULATION_MENU_WINDOW_CONFIRM], TRUE);
        ScheduleBgTilemapBufferTransfer(menu->windows[REGULATION_MENU_WINDOW_CONFIRM].bgConfig, menu->windows[REGULATION_MENU_WINDOW_CONFIRM].bgId);
        RemoveWindow(&menu->windows[REGULATION_MENU_WINDOW_CONFIRM]);
        ListMenuItems_Delete(menu->items[REGULATION_MENU_CONFIRM]);
        menu->listMenu[REGULATION_MENU_CONFIRM] = NULL;
    }
    return input;
}

void BattleRegulationMenu_ShowRules(BattleRegulationMenu *menu) {
    MessageFormat *messageFormat = menu->messageFormat;
    int i, ruleValue, valueMessage;
    const int yOffset = 16;
    const int xOffset = 2;
    const int lineHeight = 15;
    const int xOffsetCupName = 55;
    const int xRightSide = (24 * 8) - 1;
    
    MsgData *msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0xB6, HEAP_ID_FIELD1);
    String *fmtString = String_New(180, HEAP_ID_FIELD1);
    String *destString = String_New(180, HEAP_ID_FIELD1);
    Window *window = &menu->windows[REGULATION_MENU_WINDOW_RULES];
    
    AddWindowParameterized(menu->fieldSystem->bgConfig, window, 3, 4, 2, 24, 19, 13, 1);
    DrawFrameAndWindow1(window, 1, 0x3D9, 11);
    FillWindowPixelBuffer(window, 15);
    
    BattleRegulationMenu_GetRegulationName(menu, menu->itemsAbove[REGULATION_MENU_REGULATIONS] - 1);
    
    ReadMsgDataIntoString(msgData, 0x71, fmtString); // Text_Cup?
    StringExpandPlaceholders(messageFormat, destString, fmtString);
    AddTextPrinterParameterized(window, 0, destString, xOffset + xOffsetCupName, 0, TEXT_SPEED_NOTRANSFER, NULL);

    for (i = 0; i < 9; i++) {
        ReadMsgDataIntoString(msgData, i + 0x5D, fmtString);
        AddTextPrinterParameterized(window, 0, fmtString, xOffset, yOffset + lineHeight * i, TEXT_SPEED_NOTRANSFER, NULL);
    };

    for (i = 0; i < 9; i++) {
        ruleValue = LinkBattleRuleset_GetRuleValue(&menu->fieldSystem->linkBattleRuleset->rules[0], (LinkBattleRule)ov03_02259820[i]);
        valueMessage = ov03_0225982C[i];
        
        switch (ov03_02259820[i]) {
        case LINKBATTLERULE_PARTY_COUNT:
            BufferIntegerAsString(messageFormat, 0, ruleValue, 1, PRINTING_MODE_RIGHT_ALIGN, TRUE);
            break;
        case LINKBATTLERULE_MAX_LEVEL:
            BufferIntegerAsString(messageFormat, 0, ruleValue, 3, PRINTING_MODE_LEFT_ALIGN, TRUE);
            break;
        case LINKBATTLERULE_MAX_TOTAL_LEVEL:
            if (ruleValue == 0) {
                valueMessage = 0x72; // NoRestrictions
            } else {
                BufferIntegerAsString(messageFormat, 0, ruleValue, 3, PRINTING_MODE_LEFT_ALIGN, TRUE);
            }
            break;
        case LINKBATTLERULE_HEIGHT_LIMIT:
            ruleValue = (ruleValue >= 0) ? (((ruleValue * 10 * 1000) / 254 + 5) / 10) : -(((-ruleValue * 10 * 1000) / 254 + 5) / 10);

            BufferIntegerAsString(messageFormat, 0, abs(ruleValue / 12), 2, PRINTING_MODE_LEFT_ALIGN, TRUE);
            BufferIntegerAsString(messageFormat, 1, abs(ruleValue % 12), 2, PRINTING_MODE_LEADING_ZEROS, TRUE);
            
            if (ruleValue == 0) {
                valueMessage = 0x72; // NoRestrictions
            } else if (ruleValue > 0) {
                valueMessage++;
            }
            break;
        case LINKBATTLERULE_WEIGHT_LIMIT:
            ruleValue = (ruleValue >= 0) ? (((ruleValue * 220462) + 50000) / 100000) : -(((-ruleValue * 220462) + 50000) / 100000);
            BufferIntegerAsString(messageFormat, 0, abs(ruleValue), 3, PRINTING_MODE_LEFT_ALIGN, TRUE);
            if (ruleValue == 0) {
                valueMessage = 0x72; // NoRestrictions
            } else if (ruleValue > 0) {
                valueMessage++;
            }
            break;
        case LINKBATTLERULE_UBERS_CLAUSE:
            if (LinkBattleRuleset_GetRuleValue(&menu->fieldSystem->linkBattleRuleset->rules[0], LINKBATTLERULE_SOUL_DEW_CLAUSE) == FLAG_RULESET_BAN_SOUL_DEW) {
                valueMessage = 0x73;
            } else if (ruleValue == 0) {
                valueMessage++;
            }
            break;
        case LINKBATTLERULE_EVOLVED_POKEMON:
        case LINKBATTLERULE_ITEM_DUPE_CLAUSE:
        case LINKBATTLERULE_SPECIES_DUPE_CLAUSE:
            if (ruleValue == 0) {
                valueMessage++;
            }
            break;
        }
        
        ReadMsgDataIntoString(msgData, valueMessage, fmtString);
        StringExpandPlaceholders(messageFormat, destString, fmtString);

        int valueWidth = FontID_String_GetWidth(0, destString, 0);
        int valueXOffset = xRightSide - valueWidth;
        
        AddTextPrinterParameterized(window, 0, destString, valueXOffset, yOffset + lineHeight * i, 0xFF, NULL);
    }
    String_Delete(fmtString);
    String_Delete(destString);
    DestroyMsgData(msgData);
    CopyWindowToVram(window);
}

void BattleRegulationMenu_RemoveRulesWindow(BattleRegulationMenu *menu) {
    sub_0200E5D4(&menu->windows[REGULATION_MENU_WINDOW_RULES], TRUE);
    ScheduleBgTilemapBufferTransfer(menu->windows[REGULATION_MENU_WINDOW_RULES].bgConfig, menu->windows[REGULATION_MENU_WINDOW_RULES].bgId);
    RemoveWindow(&menu->windows[REGULATION_MENU_WINDOW_RULES]);
}

BOOL BattleRegulationMenu_HandleValidationResult(BattleRegulationMenu *menu) {
    int result = sub_02074CD0(&menu->fieldSystem->linkBattleRuleset->rules[0], SaveArray_Party_Get(menu->fieldSystem->saveData), menu->pokedexData);
    switch (result) {
    case 0: // BATTLE_REGULATION_VALIDATION_RESULT_SUCCESS
        return TRUE;
    case 4: // BATTLE_REGULATION_VALIDATION_RESULT_INVALID_TEAM_SIZE
        PlaySE(SEQ_SE_DP_BOX03);
        BattleRegulationMenu_GetRegulationName(menu, menu->itemsAbove[REGULATION_MENU_REGULATIONS] - 1);
        BufferIntegerAsString(menu->messageFormat, 1, LinkBattleRuleset_GetRuleValue(&menu->fieldSystem->linkBattleRuleset->rules[0], LINKBATTLERULE_PARTY_COUNT), 1, PRINTING_MODE_RIGHT_ALIGN, TRUE);
        BattleRegulationMenu_PrintMessage(menu, 0x7A); // PokemonCenter2FCommon_Text_NeedXPokemonForCup
        break;
    default:
    case 1: // BATTLE_REGULATION_VALIDATION_RESULT_TOTAL_LEVEL_EXCEEDED
        PlaySE(SEQ_SE_DP_BOX03);
        BattleRegulationMenu_GetRegulationName(menu, menu->itemsAbove[REGULATION_MENU_REGULATIONS] - 1);
        BufferIntegerAsString(menu->messageFormat, 1, LinkBattleRuleset_GetRuleValue(&menu->fieldSystem->linkBattleRuleset->rules[0], LINKBATTLERULE_MAX_TOTAL_LEVEL), 3, PRINTING_MODE_LEFT_ALIGN, TRUE);
        BattleRegulationMenu_PrintMessage(menu, 0x88); // PokemonCenter2FCommon_Text_CantFormTeamWithLevelLimit
        break;
    }
    return FALSE;
}

enum BattleRegulationMenuState {
    STATE_REGULATION_MENU_PRINT_WHICH_RULESET = 0,
    STATE_REGULATION_MENU_SHOW_LIST_MENU_REGULATIONS,
    STATE_REGULATION_MENU_HANDLE_INPUT_REGULATIONS,
    STATE_REGULATION_MENU_SHOW_LIST_MENU_CONFIRM,
    STATE_REGULATION_MENU_HANDLE_INPUT_CONFIRM,
    STATE_REGULATION_MENU_CHECK_VALID_TEAM, // 5
    STATE_REGULATION_MENU_WAIT_MESSAGE_INVALID_TEAM,
    STATE_REGULATION_MENU_PRINT_RULES,
    STATE_REGULATION_MENU_RESHOW_MENU_AFTER_RULES,
    STATE_REGULATION_MENU_WAIT_RESHOW_CONFIRM_MENU,
    STATE_REGULATION_MENU_UNUSED, // 10
    STATE_REGULATION_MENU_EXIT,
};

BOOL Task_BattleRegulationMenu(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    BattleRegulationMenu *menu = TaskManager_GetEnvironment(taskManager);
    int input;
    switch (menu->state) {
    case STATE_REGULATION_MENU_PRINT_WHICH_RULESET:
        BattleRegulationMenu_PrintMessage(menu, 0x78); // PokemonCenter2FCommon_Text_WhichSetOfRules
        menu->state++;
        break;
    case STATE_REGULATION_MENU_SHOW_LIST_MENU_REGULATIONS:
        if (IsPrintFinished(menu->printerID)) {
            BattleRegulationMenu_ShowListMenuRegulations(menu);
            menu->state++;
        }
        break;
    case STATE_REGULATION_MENU_HANDLE_INPUT_REGULATIONS:
        input = BattleRegulationMenu_ProcessListMenuInputRegulations(menu);
        if (input == -1) {
            *menu->result = 2;
            menu->state = STATE_REGULATION_MENU_EXIT;
        } else if (input == 2) {
            *menu->result = 3;
            menu->state = STATE_REGULATION_MENU_EXIT;
        } else if (input == 1) {
            menu->state = STATE_REGULATION_MENU_SHOW_LIST_MENU_CONFIRM;
        }
        break;
    case STATE_REGULATION_MENU_SHOW_LIST_MENU_CONFIRM:
        BattleRegulationMenu_ShowListMenuConfirm(menu);
        menu->state++;
        break;
    case STATE_REGULATION_MENU_HANDLE_INPUT_CONFIRM:
        input = BattleRegulationMenu_ProcessListMenuInputConfirm(menu);
        if (input == -1) {
            menu->state = STATE_REGULATION_MENU_HANDLE_INPUT_REGULATIONS;
        } else if (input == 1) {
            menu->state = STATE_REGULATION_MENU_CHECK_VALID_TEAM;
        } else if (input == 2) {
            menu->state = STATE_REGULATION_MENU_PRINT_RULES;
        }
        break;
    case STATE_REGULATION_MENU_CHECK_VALID_TEAM:
        if (BattleRegulationMenu_HandleValidationResult(menu)) {
            *menu->result = 1;
            menu->state = STATE_REGULATION_MENU_EXIT;
        } else {
            menu->state = STATE_REGULATION_MENU_WAIT_MESSAGE_INVALID_TEAM;
        }
        break;
    case STATE_REGULATION_MENU_WAIT_MESSAGE_INVALID_TEAM:
        if (IsPrintFinished(menu->printerID) && (PAD_BUTTON_A | PAD_BUTTON_B) & gSystem.newKeys) {
            menu->state = STATE_REGULATION_MENU_PRINT_WHICH_RULESET;
        }
        break;
    case STATE_REGULATION_MENU_PRINT_RULES:
        BattleRegulationMenu_RemoveMsgBox(menu, TRUE);
        BattleRegulationMenu_ShowRules(menu);
        menu->state++;
        break;
    case STATE_REGULATION_MENU_RESHOW_MENU_AFTER_RULES:
        if ((PAD_BUTTON_A | PAD_BUTTON_B) & gSystem.newKeys) {
            BattleRegulationMenu_RemoveRulesWindow(menu);
            BattleRegulationMenu_PrintMessage(menu, 0x78); // PokemonCenter2FCommon_Text_WhichSetOfRules
            BattleRegulationMenu_ShowListMenuRegulations(menu);
            menu->state = STATE_REGULATION_MENU_WAIT_RESHOW_CONFIRM_MENU;
        }
        break;
    case STATE_REGULATION_MENU_WAIT_RESHOW_CONFIRM_MENU:
        if (IsPrintFinished(menu->printerID)) {
            menu->state = STATE_REGULATION_MENU_SHOW_LIST_MENU_CONFIRM;
        }
        break;
    case STATE_REGULATION_MENU_UNUSED:
        break;
    case STATE_REGULATION_MENU_EXIT:
        BattleRegulationMenu_RemoveListMenuRegulations(menu);
        BattleRegulationMenu_RemoveMsgBox(menu, FALSE);
        MessageFormat_Delete(menu->messageFormat);
        DestroyMsgData(menu->msgData);
        String_Delete(menu->strings[REGULATION_MENU_STRING_FMT]);
        String_Delete(menu->strings[REGULATION_MENU_STRING_DESTINATION]);
        String_Delete(menu->strings[REGULATION_MENU_STRING_REGULATION_NAME]);
        String_Delete(menu->strings[REGULATION_MENU_STRING_CUP_NAME]);
        PokedexData_UnloadAndDelete(menu->pokedexData);
        Heap_Free(menu);
        return TRUE;
    default:
        return TRUE;
    }
    return FALSE;
}

BattleRegulationMenu *BattleRegulationMenu_New(FieldSystem *fieldSystem) {
    BattleRegulationMenu *menu = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(BattleRegulationMenu));
    MI_CpuFill8(menu, 0, sizeof(BattleRegulationMenu));
    menu->state = STATE_REGULATION_MENU_PRINT_WHICH_RULESET;
    menu->fieldSystem = fieldSystem;
    fieldSystem->linkBattleRuleset = NULL;
    menu->messageFormat = MessageFormat_New(HEAP_ID_FIELD1);
    menu->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0x2E, HEAP_ID_FIELD1); // TEXT_BANK_POKEMON_CENTER_2F_COMMON
    menu->strings[REGULATION_MENU_STRING_FMT] = String_New(180, HEAP_ID_FIELD1);
    menu->strings[REGULATION_MENU_STRING_DESTINATION] = String_New(180, HEAP_ID_FIELD1);
    menu->strings[REGULATION_MENU_STRING_REGULATION_NAME] = String_New(180, HEAP_ID_FIELD1);
    menu->strings[REGULATION_MENU_STRING_CUP_NAME] = String_New(180, HEAP_ID_FIELD1);
    menu->pokedexData = PokedexData_CreateAndLoad(HEAP_ID_FIELD2);
    return menu;
}

void StartTask_BattleRegulationMenu(TaskManager *taskManager, u16 *result) {
    BattleRegulationMenu *menu = BattleRegulationMenu_New(TaskManager_GetFieldSystem(taskManager));
    menu->result = result;
    TaskManager_Call(taskManager, Task_BattleRegulationMenu, menu);
}

void ov03_022566D0(FieldSystem *fieldSystem, MessageFormat *messageFormat, u32 ruleset) {
    String *string = String_New(180, HEAP_ID_FIELD1);
    sub_0202921C(fieldSystem->saveData, ruleset, string, HEAP_ID_FIELD1);
    BufferString(messageFormat, 0, string, 0, 1, 2);
    String_Delete(string);
}

void ov03_02256710(FieldSystem *fieldSystem, u16 arg1) {
    if (arg1 == 12 || arg1 == 6) {
        &fieldSystem->linkBattleRuleset->rules[0] = NULL;
    } else {
        &fieldSystem->linkBattleRuleset->rules[0] = sub_020291E8(fieldSystem->saveData, arg1);
    }
}

void ov03_02256730(FieldSystem *fieldSystem, Window *window, u32 ruleset) {
    int ruleValue, valueMessage;
    const int yOffset = 16;
    const int xOffset = 2;
    const int lineHeight = 15;
    const int xOffsetCupName = 55;
    const int xRightSide = (24 * 8) - 1;

    AddWindowParameterized(fieldSystem->bgConfig, window, 3, 4, 2, 24, 19, 13, 1);
    LoadUserFrameGfx1(fieldSystem->bgConfig, GF_BG_LYR_MAIN_3, 0x3D9, 11, 0, HEAP_ID_FIELD1);
    DrawFrameAndWindow1(window, 1, 0x3D9, 11);
    FillWindowPixelBuffer(window, 15);
    MsgData *msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0xB6, HEAP_ID_FIELD1);
    MessageFormat *messageFormat = MessageFormat_New(HEAP_ID_FIELD1);
    String *fmtString = String_New(180, HEAP_ID_FIELD1);
    String *destString = String_New(180, HEAP_ID_FIELD1);
    ov03_022566D0(fieldSystem, messageFormat, ruleset);
    ReadMsgDataIntoString(msgData, 0x71, fmtString);
    StringExpandPlaceholders(messageFormat, destString, fmtString);
    AddTextPrinterParameterized(window, 0, destString, xOffset + xOffsetCupName, 0, TEXT_SPEED_NOTRANSFER, 0);

    for (int i = 0; i < 9; i++) {
        ReadMsgDataIntoString(msgData, i + 0x5D, fmtString);
        AddTextPrinterParameterized(window, 0, fmtString, xOffset, yOffset + lineHeight * i, TEXT_SPEED_NOTRANSFER, NULL);
        ruleValue = LinkBattleRuleset_GetRuleValue(&fieldSystem->linkBattleRuleset->rules[0], (LinkBattleRule)ov03_02259820[i]);
        valueMessage = ov03_0225982C[i];
        switch (ov03_02259820[i]) {
        case LINKBATTLERULE_PARTY_COUNT:
            BufferIntegerAsString(messageFormat, 0, ruleValue, 1, PRINTING_MODE_RIGHT_ALIGN, TRUE);
            break;
        case LINKBATTLERULE_MAX_LEVEL:
            BufferIntegerAsString(messageFormat, 0, ruleValue, 3, PRINTING_MODE_LEFT_ALIGN, TRUE);
            break;
        case LINKBATTLERULE_MAX_TOTAL_LEVEL:
            if (ruleValue == 0) {
                valueMessage = 0x72;
            } else {
                BufferIntegerAsString(messageFormat, 0, ruleValue, 3, PRINTING_MODE_LEFT_ALIGN, TRUE);
            }
            break;
        case LINKBATTLERULE_HEIGHT_LIMIT:
            ruleValue = (ruleValue >= 0) ? (((ruleValue * 10 * 1000) / 254 + 5) / 10) : -(((-ruleValue * 10 * 1000) / 254 + 5) / 10);
            BufferIntegerAsString(messageFormat, 0, abs(ruleValue / 12), 2, PRINTING_MODE_LEFT_ALIGN, TRUE);
            BufferIntegerAsString(messageFormat, 1, abs(ruleValue % 12), 2, PRINTING_MODE_LEADING_ZEROS, TRUE);
            if (ruleValue == 0) {
                valueMessage = 0x72;
            } else if (ruleValue > 0) {
                valueMessage++;
            }
            break;
        case LINKBATTLERULE_WEIGHT_LIMIT:
            ruleValue = (ruleValue >= 0) ? (((ruleValue * 220462) + 50000) / 100000) : -(((-ruleValue * 220462) + 50000) / 100000);
            BufferIntegerAsString(messageFormat, 0, abs(ruleValue), 3, PRINTING_MODE_LEFT_ALIGN, TRUE);
            if (ruleValue == 0) {
                valueMessage = 0x72;
            } else if (ruleValue > 0) {
                valueMessage++;
            }
            break;
        case LINKBATTLERULE_UBERS_CLAUSE:
            if (LinkBattleRuleset_GetRuleValue(&fieldSystem->linkBattleRuleset->rules[0], LINKBATTLERULE_SOUL_DEW_CLAUSE) == FLAG_RULESET_BAN_SOUL_DEW) {
                valueMessage = 0x73;
            } else if (ruleValue == 0) {
                valueMessage++;
            }
            break;
        case LINKBATTLERULE_EVOLVED_POKEMON:
        case LINKBATTLERULE_ITEM_DUPE_CLAUSE:
        case LINKBATTLERULE_SPECIES_DUPE_CLAUSE:
            if (ruleValue == 0) {
                valueMessage++;
            }
            break;
        }
        ReadMsgDataIntoString(msgData, valueMessage, fmtString);
        StringExpandPlaceholders(messageFormat, destString, fmtString);
        
        int valueWidth = FontID_String_GetWidth(0, destString, 0);
        int valueXOffset = xRightSide - valueWidth;
        AddTextPrinterParameterized(window, 0, destString, valueXOffset, yOffset + lineHeight * i, TEXT_SPEED_NOTRANSFER, NULL);
    }
    String_Delete(destString);
    String_Delete(fmtString);
    MessageFormat_Delete(messageFormat);
    DestroyMsgData(msgData);
    CopyWindowToVram(window);
}

u16 ov03_02256A2C(FieldSystem *fieldSystem, MessageFormat *messageFormat, u32 ruleset) {
    Party *party = SaveArray_Party_Get(fieldSystem->saveData);
    PokedexData *pokedexData = PokedexData_CreateAndLoad(HEAP_ID_FIELD2);
    u32 unkValue;
    if (ruleset == 10) {
        int partySize = Party_GetCount(party);
        int validMons = partySize;
        for (int index = 0; index < partySize; index++) {
            if (GetMonData(Party_GetMonByIndex(party, index), MON_DATA_IS_EGG, NULL)) {
                validMons--;
            }
        }
        unkValue = validMons < 2 ? 7 : 0;
    } else {
        unkValue = sub_02074CD0(&fieldSystem->linkBattleRuleset->rules[0], party, pokedexData);
    }
    PokedexData_UnloadAndDelete(pokedexData);
    int ruleValue;
    switch (unkValue)
    {
        case 0:
            return 0;
        case 4:
            ov03_022566D0(fieldSystem, messageFormat, ruleset);
            ruleValue = LinkBattleRuleset_GetRuleValue(&fieldSystem->linkBattleRuleset->rules[0], LINKBATTLERULE_PARTY_COUNT);
            BufferIntegerAsString(messageFormat, 1, ruleValue, 1, PRINTING_MODE_RIGHT_ALIGN, TRUE);
            return 1;
        case 7:
            ov03_022566D0(fieldSystem, messageFormat, ruleset);
            BufferIntegerAsString(messageFormat, 1, 2, 1, PRINTING_MODE_RIGHT_ALIGN, TRUE);
            return 1;
        case 1:
        case 2:
        case 3:
        case 5:
        case 6:
        default:
            ov03_022566D0(fieldSystem, messageFormat, ruleset);
            ruleValue = LinkBattleRuleset_GetRuleValue(&fieldSystem->linkBattleRuleset->rules[0], LINKBATTLERULE_MAX_TOTAL_LEVEL);
            BufferIntegerAsString(messageFormat, 1, ruleValue, 3, PRINTING_MODE_LEFT_ALIGN, TRUE);
            return 2;
    }
}

u32 ov03_02256B40(int arg0) {
    u32 unkValue = arg0;
    switch (unkValue) {
        case 6:
            return 0xFF;
        case 7:
            return 0;
        case 8:
            return 3;
        case 9:
            return 4;
        case 10:
        case 11:
            return -1;
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        default:
            break;
    }
    if (unkValue >= 9) {
        unkValue = 0xFF;
    }
    return unkValue;
}

BOOL ScrCmd_710(ScriptContext *ctx) {
    for (int i = 0; i < 3; i++) {
        ov03_02256BA8(ctx->fieldSystem, i);
    }
    return FALSE;
}

void ov03_02256BA8(FieldSystem *fieldSystem, u8 index) {
    u16 modelID = ov03_0225945C[index];
    UnkStruct_FieldSysC0_SubC *renderObject = Field3dObjectList_GetRenderObjectByID(fieldSystem->unkC0, modelID);
    if (renderObject == NULL) {
        GF_AssertFail();
    } else {
        ov01_021E8970(modelID, sub_020669B4(Save_VarsFlags_Get(fieldSystem->saveData), index), 1, renderObject, fieldSystem->unk54); 
    }
}

BOOL Task_Mart(TaskManager *taskManager);
static void MartTask_InitMartMessageData(MartData *data);
static void MartData_InitMessageData(MartData *data);
static void MartData_InitCamera(FieldSystem *fieldSystem, MartData *data);
static void MartData_AddWindows(MartData *data);
static u8 ov03_02257334(FieldSystem *fieldSystem, MartData *data);
static void ov03_022573D4(MartData *data, u32 arg1);
static int ov03_02257510(MartData *data);
u8 ov03_0225761C(MartData *data, u32);
static u8 ov03_022576F8(MartData *data);
static u8 ov03_02257728(MartData *data);
static void ov03_02257758(MartData *data, int, u8);
u8 ov03_022577D0(MartData *data);
static u8 ov03_02257874(MartData *data, u16 itemID);
static u8 ov03_02257944(MartData *data);
static u8 ov03_022579E0(MartData *data);
static u8 ov03_02257A70(MartData *data);
static u8 ov03_02257ADC(MartData *data);
u8 ov03_02257B4C(MartData *data, u32);
static u8 ov03_02257CA0(MartData *data);
static u8 ov03_02257D6C(MartData *data);
static u8 ov03_02257D90(MartData *data, u32);
static u8 MartData_PerformTransaction(MartData *data);
static u8 ov03_02257F24(MartData *data);
static u8 ov03_02257FF8(MartData *data);
static u8 ov03_02258078(MartData *data);
static void ov03_02258164(FieldSystem *fieldSystem, MartData *data_unused);
static u8 ov03_02258170(FieldSystem *fieldSystem, MartData *data);
void ov03_02258288(MartData *data);
void ov03_02258560(MartData *data, int);
void ov03_022586BC(MartData *data, int);
u8 ov03_022586CC(MartData *data, int, int);
u8 ov03_022586E0();
void ov03_0225874C(FieldSystem *fieldSystem, MartData *data);
void ov03_02258764(TaskManager *taskManager);
u8 ov03_022587D4(FieldSystem *fieldSystem, MartData *data);
static void MartData_RestoreBgPriorities(MartData *data);
static BOOL ov03_0225709C(FieldSystem *fieldSystem_unused, MartData *data);
u32 ov03_02258120(MartData *data, u16 itemID);
void ov03_022581BC(MartData *data);
void ov03_022582C0(MartData *data, int);
void ov03_022585A4(MartData *data, u16 itemID);
void ov03_02258648(MartData *data, int charID, int paletteID, u16 item);
s16 ov03_022587E8(s16 currentQuantity, u16, s16 modifier);

extern WindowTemplate ov03_02259464;
extern WindowTemplate ov03_022594C6[6];

extern u8 ov03_0225949E;
extern u8 ov03_0225949F;
extern u8 ov03_022594A0;
extern u8 ov03_022594A1;

extern u8 ov03_0225947A[][4];

extern u16 ov03_0225946C;
extern SpriteTemplate_ov01_021E81F0 ov03_022594F8[];

static u32 ov03_02256BEC(const u16 *items, u16 *priceOverrides, u32 martType) {
    int i;
    if (martType == MART_TYPE_3 || martType == MART_TYPE_4) {
        for (i = 0; i < 0x100; i++) {
            if (*priceOverrides != 0xFFFF) {
                priceOverrides += 2;
            } else {
                break;
            }
        }
    } else { // MART_TYPE_NORMAL, MART_TYPE_1
        for (i = 0; i < 0x100; i++) {
            if (*items != 0xFFFF) {
                items++;
            } else {
                break;
            }
        }
    }
    return i;
}

static void ov03_02256C2C(MartData *data, const u16 *items, BOOL flag09A) {
    int max = data->unk270;
    int i;
    int k = 0;
    for (i = 0; i < max; i++) {
        if (flag09A == FALSE) {
            if(items[i] == 4) {
                data->unk270--;
            } else {
                data->unk268[k] = items[i];
                k++;
            }
        } else {
            data->unk268[k] = items[i];
            k++;
        }
    }
}

static void ov03_02256C84(MartData *data, u16 *priceOverrides) {
    for (int i = 0, j = 0; i < data->unk270; i++, j += 2) {
        data->unk268[i] = priceOverrides[j]; // martItem->cost?
    }
}

static void ov03_02256CB4(MartData *data, const u16 *items, BOOL flag09A, const struct MartItem *priceOverrides) {
    data->unk270 = ov03_02256BEC(items, (u16 *)priceOverrides, data->martType);
    if (data->unk270 >= 0xFF) {
        GF_AssertFail();
    }
    data->unk268 = Heap_Alloc(HEAP_ID_FIELD2, data->unk270 * 2);
    if (data->martType == MART_TYPE_3 || data->martType == MART_TYPE_4) {
        ov03_02256C84(data, (u16 *)priceOverrides);
    } else {
        ov03_02256C2C(data, items, flag09A);
    }
}

static MartData *Mart_Alloc() {
    MartData *data = Heap_Alloc(HEAP_ID_FIELD2, sizeof(MartData));
    memset(data, 0, sizeof(MartData));
    return data;
}

void Mart_Init(TaskManager *taskManager, FieldSystem *fieldSystem, const u16 *items, int kind, u8 buy_sell, int deco_which, const struct MartItem *priceOverrides) {
    MartData *martData = Mart_Alloc();
    martData->bgConfig = fieldSystem->bgConfig;
    martData->string = String_New(96, HEAP_ID_FIELD2);
    martData->playerProfile = Save_PlayerData_GetProfile(fieldSystem->saveData);
    martData->options = Save_PlayerData_GetOptionsAddr(fieldSystem->saveData);
    martData->gameStats = Save_GameStats_Get(fieldSystem->saveData);
    martData->varsFlags = Save_VarsFlags_Get(fieldSystem->saveData);
    martData->apricornBox = Save_ApricornBox_Get(fieldSystem->saveData);
    martData->unk264 = deco_which;
    martData->martType = kind;
    martData->unk271 = 0;
    martData->unk290 = 0;
    martData->unk294 = -1;
    martData->unk298 = 0;
    martData->pokeathlonSave = Save_Pokeathlon_Get(fieldSystem->saveData);
    martData->priceOverrides = priceOverrides;
    martData->unk273 = buy_sell;
    if (martData->martType == MART_TYPE_NORMAL || martData->martType == MART_TYPE_3 || martData->martType == MART_TYPE_4) {
        martData->inventory = Save_Bag_Get(fieldSystem->saveData);
    } else {
        martData->inventory = Save_SealCase_Get(fieldSystem->saveData);
    }
    // Mart_SetItemsForSale?
    ov03_02256CB4(martData, items, CheckFlag09A(martData->varsFlags), priceOverrides);
    TaskManager_Call(taskManager, Task_Mart, martData);
}

enum {
    TASK_MART_START,
    TASK_MART_1,
    TASK_MART_2,
    TASK_MART_3,
    TASK_MART_4,
    TASK_MART_5,
    TASK_MART_6,
    TASK_MART_7,
    TASK_MART_8,
    TASK_MART_9,
    TASK_MART_10,
    TASK_MART_11,
    TASK_MART_12,
    TASK_MART_13,
    TASK_MART_14,
    TASK_MART_15,
    TASK_MART_16,
    TASK_MART_17,
    TASK_MART_18,
    TASK_MART_19,
    TASK_MART_20,
    TASK_MART_21,
    TASK_MART_22,
    TASK_MART_23,
    TASK_MART_24,
    TASK_MART_25,
    TASK_MART_26,
    TASK_MART_27
};

BOOL Task_Mart(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    MartData *data = TaskManager_GetEnvironment(taskManager);
    
    switch (data->state) {
    case TASK_MART_START:
        MartTask_InitMartMessageData(data); // MartData_InitMessageData
        if (data->unk273 == 0) {
            data->state = TASK_MART_1;
        } else {
            data->state = 22;
        }
        break;
    case TASK_MART_1:
        MartData_InitCamera(fieldSystem, data); // Initialize Camera & Windows.
        ov01_021F6A9C(fieldSystem, 2, data); // Some sort of generic SysTask function.
        data->state = TASK_MART_2;
        break;
    case TASK_MART_2:
        data->state = ov03_02257334(fieldSystem, data); // Returns 2 or 3.
        break;
    case TASK_MART_3: 
        if (data->unk294 == -1) {
            data->state = ov03_02257510(data);
        } else {
            data->state = ov03_0225761C(data, data->unk294);
            data->unk294 = -1;
        }
        break;
    case TASK_MART_4:
        data->state = TASK_MART_3;
        break;
    case TASK_MART_6:
        if (!Sprite_IsAnimated(data->sprites[10])) {
            data->state = ov03_02257874(data, data->unk268[data->unk290 + data->unk271]);
        }
        break;
    case TASK_MART_5:
        data->state = ov03_02257944(data); // Returns 5 or 7. Why these are out of order, I have no idea.
        break;
    case TASK_MART_7:
        if (data->unk294 == -1) {
            data->state = ov03_02257ADC(data);
        } else {
            data->state = ov03_02257B4C(data, data->unk294);
            data->unk294 = -1;
        }
        break;
    case TASK_MART_8:
        data->state = ov03_022579E0(data);
        break;
    case TASK_MART_9:
        data->state = ov03_02257A70(data); // Always returns 3.
        break;
    case TASK_MART_10:
        data->state = ov03_02257D6C(data);
        break;
    case TASK_MART_11:
        if (data->unk294 != -1) {
            data->state = ov03_02257D90(data, data->unk294);
            data->unk294 = -1;
        }
        break;
    case TASK_MART_12:
        data->state = MartData_PerformTransaction(data);
        break;
    case TASK_MART_13:
        data->state = ov03_02257F24(data);
        break;
    case TASK_MART_14:
        data->state = ov03_02257FF8(data);
        break;
    case TASK_MART_15:
        data->state = ov03_02258078(data);
        break;
    case TASK_MART_16:
        data->state = ov03_022577D0(data); // Always returns 17.
        break;
    case TASK_MART_17:
        MartData_RestoreBgPriorities(data);
        ov03_02258164(fieldSystem, data);
        data->state = TASK_MART_18;
        break;
    case TASK_MART_18:
        data->state = ov03_02258170(fieldSystem, data);
        break;
    case TASK_MART_19:
        data->state = ov03_022576F8(data); // Always returns 3.
        break;
    case TASK_MART_20:
        data->state = ov03_02257728(data); // Always returns 3.
        break;
    case TASK_MART_21:
        data->state = ov03_022586E0();
        break;
    case TASK_MART_22:
        ov03_0225874C(fieldSystem, data);
        break;
    case TASK_MART_23:
        ov03_02258764(taskManager);
        break;
    case TASK_MART_26:
        data->state = ov03_022587D4(fieldSystem, data);
        break;
    case TASK_MART_27:
        return ov03_0225709C(fieldSystem, data);
    }

    for (int i = 0; i < 19; i++) {
        if (data->sprites[i]) {
            Sprite_UpdateAnim(data->sprites[i], 0x1000);
        }
    }
    if (data->unk_ov01_021E7FDC.spriteList) {
        SpriteList_RenderAndAnimateSprites(data->unk_ov01_021E7FDC.spriteList);
    }
    return FALSE;
}

static void MartTask_InitMartMessageData(MartData *data) {
    MartData_InitMessageData(data);
}

static void MartData_InitMessageData(MartData *data) {
    data->messageData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0x1B3, HEAP_ID_FIELD2);
    data->messageFormat = MessageFormat_New(HEAP_ID_FIELD2);
}

static BOOL ov03_0225709C(FieldSystem *fieldSystem_unused, MartData *data) {
    DestroyMsgData(data->messageData);
    MessageFormat_Delete(data->messageFormat);
    String_Delete(data->string);
    Heap_Free(data->unk268);
    Heap_Free(data);
    return TRUE;
}

static void MartData_InitCamera(FieldSystem *fieldSystem, MartData *data) {
    FillBgTilemapRect(data->bgConfig, 3, 0, 0, 0x12, 0x20, 6, 0);
    ScheduleBgTilemapBufferTransfer(data->bgConfig, 3);
    MartData_AddWindows(data);
    data->camera = Camera_New(HEAP_ID_FIELD2);
    Camera_Copy(fieldSystem->camera, data->camera);
    Camera_SetStaticPtr(data->camera);
    data->unk281 = 0;
}

static void MartData_AddWindows(MartData *data) {
    for (u32 i = 0; i < 6; i++) {
        if (data->martType != MART_TYPE_NORMAL && data->martType != MART_TYPE_3 && data->martType != MART_TYPE_4 && i == 1) {
            AddWindow(data->bgConfig, &data->windows[i], &ov03_02259464);
        } else {
            AddWindow(data->bgConfig, &data->windows[i], &ov03_022594C6[i]);
        }
    }
}

static void ov03_02257184(MartData *data) {
    sub_0200E5D4(&data->windows[2], TRUE);
    for (u32 i = 0; i < 6; i++) {
        ClearWindowTilemapAndScheduleTransfer(&data->windows[i]);
        RemoveWindow(&data->windows[i]);
    };
}

void ov03_022571AC(MartData *data);

/*static*/ void ov03_022571AC(MartData *data) { // MartData_LoadGraphics?
    GfGfxLoader_LoadCharData(NARC_a_0_6_0, 0, data->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, FALSE, HEAP_ID_FIELD2);
    if (data->martType == MART_TYPE_NORMAL || data->martType == MART_TYPE_3 || data->martType == MART_TYPE_4) {
        GfGfxLoader_LoadScrnData(NARC_a_0_6_0, 2, data->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, FALSE, HEAP_ID_FIELD2);
    } else {
        GfGfxLoader_LoadScrnData(NARC_a_0_6_0, 3, data->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, FALSE, HEAP_ID_FIELD2);
    }
    GfGfxLoader_GXLoadPal(NARC_a_0_6_0, 1, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 32, HEAP_ID_FIELD2);
    LoadUserFrameGfx1(data->bgConfig, GF_BG_LYR_MAIN_3, 31, 11, 0, HEAP_ID_FIELD2);
    LoadUserFrameGfx2(data->bgConfig, GF_BG_LYR_MAIN_3, 1, 10, Options_GetFrame(data->options), HEAP_ID_FIELD2);
}

static void ov03_0225725C(MartData *data) { // MartData_ToggleBgOn?
    data->engineAPlanes = GfGfx_EngineAGetPlanes();
    data->bgPriority[0] = GetBgPriority(data->bgConfig, 0);
    data->bgPriority[1] = GetBgPriority(data->bgConfig, 1);
    data->bgPriority[2] = GetBgPriority(data->bgConfig, 2);
    data->bgPriority[3] = GetBgPriority(data->bgConfig, 3);
    SetBgPriority(0, 3);
    SetBgPriority(1, 2);
    SetBgPriority(2, 1);
    SetBgPriority(3, 0);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, TRUE);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, TRUE);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, TRUE);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, TRUE);
}

static void MartData_RestoreBgPriorities(MartData *data) {
    SetBgPriority(0, data->bgPriority[0]);
    SetBgPriority(1, data->bgPriority[1]);
    SetBgPriority(2, data->bgPriority[2]);
    SetBgPriority(3, data->bgPriority[3]);
    GfGfx_EngineASetPlanes(data->engineAPlanes);
}

static u8 ov03_02257334(FieldSystem *fieldSystem, MartData *data) {
    if (ov01_021F6B10(fieldSystem) != TRUE) {
        return TASK_MART_2;
    }
    data->unk281 = 0;
    ov03_022571AC(data); // MartData_InitFrames?
    ov03_0225725C(data); // MartData_ToggleBgOn?
    ov03_022573D4(data, data->unk290);
    Sprite_SetDrawFlag(data->sprites[3], TRUE);
    return TASK_MART_3;
}

static void ov03_02257378(MartData *data, int arg1, int arg2) {
    switch (arg2) {
    case 0:
        Sprite_SetAnimCtrlSeq(data->sprites[10], *(&ov03_022594A0 + (data->unk290 * 4)));
        thunk_Sprite_SetPaletteOverride(data->sprites[10], *(&ov03_022594A1 + arg1 * 4));
        Sprite_SetPositionXY(data->sprites[10], *(&ov03_0225949E + arg1 * 4), *(&ov03_0225949F + arg1 * 4));
    case 1:
    case 2:
        break;
    }
}

static void ov03_022573D4(MartData *data, u32 arg1) {
    MsgData *msgData;
    String *string;
    u16 itemID = data->unk268[data->unk290 + data->unk271];
    switch (arg1) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            FillWindowPixelBuffer(&data->windows[1], 0);
            if (data->unk290 + data->unk271 < data->unk270) {
                if (data->martType == MART_TYPE_NORMAL || data->martType == MART_TYPE_3 || data->martType == MART_TYPE_4) {
                    string = String_New(130, HEAP_ID_FIELD2);
                    GetItemDescIntoString(string, itemID, HEAP_ID_FIELD2);
                } else if (data->martType == MART_TYPE_1) {
                    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0x2E1, HEAP_ID_FIELD2);
                    string = NewString_ReadMsgData(msgData, itemID + 0x8A);
                    DestroyMsgData(msgData);
                } else {
                    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0x1B2, HEAP_ID_FIELD2);
                    string = NewString_ReadMsgData(msgData, sub_020910B8((u8)itemID));
                    DestroyMsgData(msgData);
                }
                AddTextPrinterParameterizedWithColor(&data->windows[1], 0, string, 0, 0, 0xFF, 0xF0E00, 0);
                String_Delete(string);
                ov03_022585A4(data, itemID);
            } else {
                Sprite_SetDrawFlag(data->sprites[3], 0);
            }
            ScheduleWindowCopyToVram(&data->windows[1]);
            break;
        case 8:
            FillWindowPixelBuffer(&data->windows[1], 0);
            ov03_022585A4(data, 0xFFFF);
            ScheduleWindowCopyToVram(&data->windows[1]);
        case 6:
        case 7:
            break;
    }
}

static int ov03_02257510(MartData *data) {
    u8 unkExternVal;
    BOOL unkBool = FALSE;
    if (gSystem.newKeys & PAD_KEY_UP) {
        data->unk290 = ov03_0225947A[data->unk290][0];
        PlaySE(SEQ_SE_DP_SELECT);
        unkBool++;
    }
    else if (gSystem.newKeys & PAD_KEY_DOWN) {
        data->unk290 = ov03_0225947A[data->unk290][1];
        PlaySE(SEQ_SE_DP_SELECT);
        unkBool++;
    }
    else if (gSystem.newKeys & PAD_KEY_LEFT) {
        unkExternVal = ov03_0225947A[data->unk290][2];
        if (unkExternVal == 6) {
            return ov03_0225761C(data, 6);
        }
        if (data->unk290 != 8) {
            data->unk290 = unkExternVal;
            PlaySE(SEQ_SE_DP_SELECT);
            unkBool++;
        }
    }
    else if (gSystem.newKeys & PAD_KEY_RIGHT) {
        unkExternVal = ov03_0225947A[data->unk290][3];
        if (unkExternVal == 7) {
            return ov03_0225761C(data, 7);
        }
        if (data->unk290 != 8) {
            data->unk290 = unkExternVal;
            PlaySE(SEQ_SE_DP_SELECT);
            unkBool++;
        }
    }

    if (unkBool) {
        ov03_02257378(data, data->unk290, 0);
        ov03_022573D4(data, data->unk290);
    }
    if (gSystem.newKeys & PAD_BUTTON_B) {
        PlaySE(SEQ_SE_GS_GEARCANCEL);
        return ov03_022586CC(data, 13, 16);
    }
    if (gSystem.newKeys & PAD_BUTTON_A) {
        return ov03_0225761C(data, data->unk290);
    }
    return TASK_MART_3;
}

u8 ov03_0225761C(MartData *data, u32 arg1) {
    switch (arg1) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            if (arg1 + data->unk271 < data->unk270) {
                data->unk290 = arg1;
                ov03_02257378(data, data->unk290, 0);
                Sprite_SetAnimCtrlSeq(data->sprites[10], 19);
                PlaySE(SEQ_SE_DP_SELECT);
                ov03_022573D4(data, data->unk290);
                return TASK_MART_6;
            }
            break;
        case 6:
            if (data->unk271) {
                PlaySE(SEQ_SE_DP_SELECT);
                return ov03_022586CC(data, 11, 19);
            }
            break;
        case 7:
            if (data->unk271 + 6 < data->unk270) {
                PlaySE(SEQ_SE_DP_SELECT);
                return ov03_022586CC(data, 12, 20);
            }
            break;
        case 8:
            ov03_02257378(data, arg1, 0);
            Sprite_SetAnimCtrlSeq(data->sprites[10], 7);
            PlaySE(SEQ_SE_GS_GEARCANCEL);
            ov03_022573D4(data, 8);
            return ov03_022586CC(data, 13, 16);
    }
    return TASK_MART_3;
}

static u8 ov03_022576F8(MartData *data) {
    data->unk271 -= 6;
    ov03_02257758(data, data->unk271, data->unk270);
    data->unk298 = 1;
    ov03_022573D4(data, data->unk290);
    return TASK_MART_3;
}

static u8 ov03_02257728(MartData *data) {
    data->unk271 += 6;
    ov03_02257758(data, data->unk271, data->unk270);
    data->unk298 = 1;
    ov03_022573D4(data, data->unk290);
    return TASK_MART_3;
}

static void ov03_02257758(MartData *data, int arg1, u8 arg2) {
    int temp;
    for (int i = 0; i < 6; i++) {
        if (arg2 - arg1 > 6) {
            temp = 6;
        } else if (arg2 - arg1 < 0) {
            temp = 0;
        } else {
            temp = arg2 - arg1;
        }
        
        if (i < temp) {
            ov03_02258648(data, i + 3, i + 2, data->unk268[i + arg1]);
            Sprite_SetDrawFlag(data->sprites[i + 4], TRUE);
        } else {
            Sprite_SetDrawFlag(data->sprites[i + 4], FALSE);
        }
    };
    ov03_022582C0(data, 0);
}

u8 ov03_022577D0(MartData *data) {
    ov03_02257184(data);
    BgFillTilemapBufferAndSchedule(data->bgConfig, 1, 0);
    Sprite_SetDrawFlag(data->sprites[3], FALSE);
    return TASK_MART_17;
}

u32 ov03_022577F4(MartData *data, u32 martType);

u32 ov03_022577F4(MartData *data, u32 martType) {
    if (martType == MART_TYPE_3 || martType == MART_TYPE_4) {
        return PokeathlonSave_GetAthletePoints(data->pokeathlonSave);
    } else {
        return PlayerProfile_GetMoney(data->playerProfile);
    }
}

int ov03_02257814(MartData *data, u32 unkAmount) {
    if (data->martType == MART_TYPE_3) {
        if (PokeathlonSave_GetUnkB7C_AtIndex(data->pokeathlonSave, data->unk290 + data->unk271)) {
            return 2;
        }
    }
    else if (data->martType == MART_TYPE_4 && PokeathlonSave_GetUnkB78_AtIndex(data->pokeathlonSave, data->item - 505)) {
        return 3;
    }
    
    return unkAmount < data->cost;
}

static u8 ov03_02257874(MartData *data, u16 itemID) {
    data->spriteDrawn[0] = Sprite_GetDrawFlag(data->sprites[0]);
    data->spriteDrawn[1] = Sprite_GetDrawFlag(data->sprites[1]);
    Sprite_SetDrawFlag(data->sprites[0], FALSE);
    Sprite_SetDrawFlag(data->sprites[1], FALSE);
    ov03_022586BC(data, 1);
    data->item = itemID;
    data->quantity = 1;
    data->cost = ov03_02258120(data, data->item);
    u32 unkAmount = ov03_022577F4(data, data->martType);
    if (ov03_02257814(data, unkAmount)) {
        data->unk298 = 10;
        return TASK_MART_14;
    }
    data->unk288 = unkAmount / data->cost;
    if (data->unk288 > 99) {
        data->unk288 = 99;
    }
    if (data->martType == 1 || data->martType == MART_TYPE_3 || data->martType == MART_TYPE_4) {
        return ov03_02257CA0(data);
    }
    ov03_022582C0(data, 1);
    data->unk298 = 2;
    return TASK_MART_5;
}

static u8 ov03_02257944(MartData *data) {
    if (IsPrintFinished(data->printerID) == FALSE) {
        return TASK_MART_5;
    }
    Sprite_SetDrawFlag(data->sprites[0], FALSE);
    Sprite_SetDrawFlag(data->sprites[1], FALSE);
    return TASK_MART_7;
}

int ov03_02257978(MartData *data, int itemID) {
    if (data->martType == MART_TYPE_NORMAL || data->martType == MART_TYPE_1 || data->martType == MART_TYPE_3 || data->martType == MART_TYPE_4) {
        if (itemID >= ITEM_RED_APRICORN && itemID <= ITEM_BLK_APRICORN) {
            return ApricornBox_CountApricorn(data->apricornBox, itemID - ITEM_RED_APRICORN);
        }
        return Bag_GetQuantity(data->inventory, data->item, HEAP_ID_FIELD2);
    }
    if (data->martType == MART_TYPE_SEAL) {
        return SealCase_CountSealOccurrenceAnywhere(data->inventory, data->item);
    }
    return 0;
}

static u8 ov03_022579E0(MartData *data) {
    Sprite_SetDrawFlag(data->sprites[0], FALSE);
    Sprite_SetDrawFlag(data->sprites[1], FALSE);
    sub_0200E5D4(&data->windows[4], TRUE);
    sub_0200E5D4(&data->windows[3], TRUE);
    FillWindowPixelBuffer(&data->windows[5], 15);
    data->unk298 = 7;
    Sprite_SetDrawFlag(data->sprites[18], FALSE);
    Sprite_SetDrawFlag(data->sprites[13], FALSE);
    Sprite_SetDrawFlag(data->sprites[14], FALSE);
    Sprite_SetDrawFlag(data->sprites[15], FALSE);
    Sprite_SetDrawFlag(data->sprites[16], FALSE);
    Sprite_SetDrawFlag(data->sprites[17], FALSE);
    return ov03_02257CA0(data);
}

static u8 ov03_02257A70(MartData *data) {
    sub_0200E5D4(&data->windows[4], TRUE);
    sub_0200E5D4(&data->windows[3], TRUE);
    ClearFrameAndWindow2(&data->windows[5], FALSE);
    ov03_02258560(data, 0);
    Sprite_SetDrawFlag(data->sprites[0], data->spriteDrawn[0]);
    Sprite_SetDrawFlag(data->sprites[1], data->spriteDrawn[1]);
    ov03_022586BC(data, 0);
    ov03_022582C0(data, 0);
    data->unk298 = 4;
    Sprite_SetDrawFlag(data->sprites[13], TRUE);
    return TASK_MART_3;
}

static u8 ov03_02257ADC(MartData *data) {
    if (sub_020881C0(&data->quantity, data->unk288)) {
        PlaySE(SEQ_SE_DP_BAG_004);
        data->unk298 = 6;
        return TASK_MART_7;
    }
    if (PAD_BUTTON_A & gSystem.newKeys) {
        PlaySE(SEQ_SE_DP_SELECT);
        return ov03_022586CC(data, 18, 8);
    }
    if (PAD_BUTTON_B & gSystem.newKeys) {
        PlaySE(SEQ_SE_GS_GEARCANCEL);
        return ov03_022586CC(data, 13, 9);
    }
    return TASK_MART_7;
}

u8 ov03_02257B4C(MartData *data, u32 arg1) {
    switch (arg1)
    {
        case 0:
            if (data->unk288 >= 10) {
                data->quantity = ov03_022587E8(data->quantity, data->unk288, 10);
                Sprite_SetAnimationFrame(data->sprites[14], 0);
                Sprite_SetAnimCtrlSeq(data->sprites[14], 13);
                PlaySE(SEQ_SE_DP_BAG_004);
                data->unk298 = 6;
            }
            break;
        case 1:
            if (data->unk288 != 1) {
                data->quantity = ov03_022587E8(data->quantity, data->unk288, 1);
                Sprite_SetAnimationFrame(data->sprites[15], 0);
                Sprite_SetAnimCtrlSeq(data->sprites[15], 13);
                PlaySE(SEQ_SE_DP_BAG_004);
                data->unk298 = 6;
            }
            break;
        case 2:
            if (data->unk288 >= 10) {
                data->quantity = ov03_022587E8(data->quantity, data->unk288, -10);
                Sprite_SetAnimationFrame(data->sprites[16], 0);
                Sprite_SetAnimCtrlSeq(data->sprites[16], 15);
                PlaySE(SEQ_SE_DP_BAG_004);
                data->unk298 = 6;
            }
            break;
        case 3:
            if (data->unk288 != 1) {
                data->quantity = ov03_022587E8(data->quantity, data->unk288, -1);
                Sprite_SetAnimationFrame(data->sprites[17], 0);
                Sprite_SetAnimCtrlSeq(data->sprites[17], 15);
                PlaySE(SEQ_SE_DP_BAG_004);
                data->unk298 = 6;
            }
            break;
        case 4:
            PlaySE(SEQ_SE_DP_SELECT);
            return ov03_022586CC(data, 18, 8);
        case 5:
            PlaySE(SEQ_SE_GS_GEARCANCEL);
            return ov03_022586CC(data, 13, 9);
    }
    return TASK_MART_7;
}

static u8 ov03_02257CA0(MartData *data) {
    int quantity;
    if (data->martType == MART_TYPE_NORMAL) {
        quantity = Bag_HasSpaceForItem(data->inventory, data->item, data->quantity, HEAP_ID_FIELD2);
    } else if (data->martType == MART_TYPE_1) {
        quantity = 0;
    } else if (data->martType == MART_TYPE_3 || data->martType == MART_TYPE_4) {
        if (data->item >= ITEM_RED_APRICORN && data->item <= ITEM_BLK_APRICORN) {
            if (ApricornBox_CountApricorn(data->apricornBox, data->item - ITEM_RED_APRICORN) == 99) {
                data->unk298 = 12;
                return TASK_MART_14;
            }
        } else if (Bag_HasSpaceForItem(data->inventory, data->item, data->quantity, HEAP_ID_FIELD2) == FALSE) {
            data->unk298 = 12;
            return TASK_MART_14;
        }
        ov03_022582C0(data, 2);
        data->unk298 = 3;
        return TASK_MART_10;
    } else {
        quantity = SealCase_CheckSealQuantity(data->inventory, data->item, data->quantity);
    }
    
    if (quantity == 0) {
        data->quantity = 0;
        data->unk298 = 11;
        return TASK_MART_13;
    }
    return TASK_MART_10;
}

static u8 ov03_02257D6C(MartData *data) {
    if (IsPrintFinished(data->printerID) == FALSE) {
        return TASK_MART_10;
    }
    data->unk298 = 8;
    return TASK_MART_11;
}

static u8 ov03_02257D90(MartData *data, u32 arg1) {
    switch (arg1) {
    case 0:
        data->unk298 = 9;
        return TASK_MART_12;
    case 1:
        ClearFrameAndWindow2(&data->windows[5], FALSE);
        ov03_02258560(data, 0);
        Sprite_SetDrawFlag(data->sprites[0], data->spriteDrawn[0]);
        Sprite_SetDrawFlag(data->sprites[1], data->spriteDrawn[1]);
        ov03_022586BC(data, 0);
        ov03_022582C0(data, 0);
        data->unk298 = 4;
        return TASK_MART_3;
    default:
        return TASK_MART_11;
    }
}

static void MartData_SubCurrency(MartData *data, int currency) {
    if (data->martType == MART_TYPE_3 || data->martType == MART_TYPE_4) {
        PokeathlonSave_SubAthletePoints(data->pokeathlonSave, currency);
    } else {
        PlayerProfile_SubMoney(data->playerProfile, data->cost * data->quantity);
    }
}

static void MartData_Inventory_AddApricornOrItem(MartData *data, int item) {
    if (item >= ITEM_RED_APRICORN && item <= ITEM_BLK_APRICORN) {
        ApricornBox_GiveApricorn(data->apricornBox, item - ITEM_RED_APRICORN, 1);
    } else {
        Bag_AddItem(data->inventory, data->item, 1, HEAP_ID_FIELD2);
    }
}

static u8 MartData_PerformTransaction(MartData *data) {
    if (IsPrintFinished(data->printerID) == FALSE) {
        return TASK_MART_12;
    }
    if (data->martType == MART_TYPE_NORMAL) {
        Bag_AddItem(data->inventory, data->item, data->quantity, HEAP_ID_FIELD2);
    } else if (data->martType == MART_TYPE_3) {
        MartData_Inventory_AddApricornOrItem(data, data->item);
        PokeathlonSave_SetUnkB7C_AtIndex(data->pokeathlonSave, data->unk290 + data->unk271);
    } else if (data->martType == MART_TYPE_4) {
        PokeathlonSave_SetUnkB78_AtIndex(data->pokeathlonSave, data->item - 505);
    } else {
        GiveOrTakeSeal(data->inventory, data->item, data->quantity);
    }
    MartData_SubCurrency(data, data->cost * data->quantity);
    GameStats_Add(data->gameStats, GAME_STAT_CURRENCY_SPENT, data->cost * data->quantity);
    return TASK_MART_13;
}

static u8 ov03_02257F24(MartData *data) {
    if (IsPrintFinished(data->printerID) == FALSE) {
        return TASK_MART_13;
    }
    if ((PAD_BUTTON_A | PAD_BUTTON_B) & gSystem.newKeys || gSystem.touchNew) {
        if (data->unk264 == 1) {
            sub_02066D80(data->varsFlags);
        }
        if (data->martType == MART_TYPE_NORMAL && data->item == ITEM_POKE_BALL && data->quantity >= 10 && Bag_AddItem(data->inventory, ITEM_PREMIER_BALL, 1, HEAP_ID_FIELD2) == TRUE) {
            data->unk298 = 13;
            GameStats_Inc(data->gameStats, GAME_STAT_PREMIER_BALLS_EARNED);
            return TASK_MART_15;
        }
        ClearFrameAndWindow2(&data->windows[5], 0);
        ov03_02258560(data, 0);
        Sprite_SetDrawFlag(data->sprites[0], data->spriteDrawn[0]);
        Sprite_SetDrawFlag(data->sprites[1], data->spriteDrawn[1]);
        ov03_022586BC(data, 0);
        ov03_022582C0(data, 0);
        data->unk298 = 4;
        return TASK_MART_4;
    }
    return TASK_MART_13;
}

static u8 ov03_02257FF8(MartData *data) {
    if (IsPrintFinished(data->printerID) == FALSE) {
        return TASK_MART_14;
    }
    if ((PAD_BUTTON_A | PAD_BUTTON_B) & gSystem.newKeys || gSystem.touchNew) {
        ClearFrameAndWindow2(&data->windows[5], FALSE);
        ov03_02258560(data, 0);
        Sprite_SetDrawFlag(data->sprites[0], data->spriteDrawn[0]);
        Sprite_SetDrawFlag(data->sprites[1], data->spriteDrawn[1]);
        ov03_022586BC(data, 0);
        ov03_022582C0(data, 0);
        data->unk298 = 5;
        return TASK_MART_4;
    }
    return TASK_MART_14;
}

static u8 ov03_02258078(MartData *data) {
    if (IsPrintFinished(data->printerID) == FALSE) {
        return TASK_MART_15;
    }
    if ((PAD_BUTTON_A | PAD_BUTTON_B) & gSystem.newKeys || gSystem.touchNew) {
        ClearFrameAndWindow2(&data->windows[5], FALSE);
        ov03_02258560(data, 0);
        Sprite_SetDrawFlag(data->sprites[0], data->spriteDrawn[0]);
        Sprite_SetDrawFlag(data->sprites[1], data->spriteDrawn[1]);
        ov03_022586BC(data, 0);
        ov03_022582C0(data, 0);
        data->unk298 = 4;
        return TASK_MART_3;
    }
    return TASK_MART_15;
}

static u16 ov03_022580F8(u16 itemID, const struct MartItem *priceOverrides, u8 unk270) {
    for (int i = 0; i < unk270; i++) {
        if (itemID == priceOverrides[i].item_id) {
            return priceOverrides[i].cost;
        }
    }
    GF_AssertFail();
    return 0;
}

u32 ov03_02258120(MartData *data, u16 itemID) {
    if (data->martType == MART_TYPE_NORMAL) {
        return GetItemAttr(itemID, 0, HEAP_ID_FIELD2);
    } else if (data->martType == MART_TYPE_1) {
        return 100;
    } else if (data->martType == MART_TYPE_3 || data->martType == MART_TYPE_4) {
        return ov03_022580F8(itemID, data->priceOverrides, data->unk270);
    }
    return 100;
}

static void ov03_02258164(FieldSystem *fieldSystem, MartData *data_unused) {
    ov01_021F6A9C(fieldSystem, fieldSystem->unk1C, NULL);
}

static u8 ov03_02258170(FieldSystem *fieldSystem, MartData *data) {
    if (ov01_021F6B10(fieldSystem) == FALSE && ov01_021F6B00(fieldSystem) != 1) {
        return TASK_MART_18;
    }
    data->unk281 = 0;
    Camera_Copy(data->camera, fieldSystem->camera);
    Camera_Delete(data->camera);
    Camera_SetStaticPtr(fieldSystem->camera);
    ov03_02258288(data);
    return TASK_MART_27;
}

void ov03_022581BC(MartData *data) {
    UnkFieldSpriteRenderer_ov01_021E7FDC_Init(&data->unk_ov01_021E7FDC, &ov03_0225946C, 19, HEAP_ID_FIELD2);
    for (u32 i = 0; i < 19; i++) {
        data->sprites[i] = ov01_021E81F0(&data->unk_ov01_021E7FDC, &ov03_022594F8[i]);
    }
    Sprite_SetPriority(data->sprites[13], 1);
    Sprite_SetPriority(data->sprites[18], 1);
    Sprite_SetAnimActiveFlag(data->sprites[10], 1);
    Sprite_SetDrawFlag(data->sprites[2], 0);
    Sprite_SetDrawFlag(data->sprites[0], 0);
    Sprite_SetDrawFlag(data->sprites[1], 0);
    Sprite_SetDrawFlag(data->sprites[3], 0);
    GfGfx_EngineATogglePlanes(0x10, 1);
    GfGfx_EngineBTogglePlanes(0x10, 1);
    ov03_022582C0(data, 0);
    ov03_02257378(data, data->unk290, 0);
    ov03_02257758(data, data->unk271, data->unk270);
}
