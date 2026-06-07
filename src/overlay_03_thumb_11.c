#include "overlay_01.h"
#include "overlay_03.h"

#include "field_system.h"
#include "font.h"
#include "global.h"
#include "link_ruleset_data.h"
#include "mart.h"
#include "render_text.h"
#include "render_window.h"
#include "save_link_ruleset.h"
#include "scrcmd.h"
#include "sys_flags.h"
#include "task.h"
#include "text.h"
#include "text_0205B4EC.h"
#include "unk_02005D10.h"
#include "unk_02034354.h"
#include "unk_02035900.h"
#include "unk_020379A0.h"
#include "unk_02037C94.h"

#include "field/overlay_01_021E8744.h"
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

