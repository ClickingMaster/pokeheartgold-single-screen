#include "msgdata/msg.naix"

#include "bg_window.h"
#include "dialog_box.h"
#include "field_system.h"
#include "font.h"
#include "mail_message.h"
#include "map_object.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_01.h"
#include "pm_string.h"
#include "scrcmd.h"
#include "script.h"
#include "script_manager.h"
#include "text.h"

typedef struct MessageBox {
    String *message;
    String *buffer;
    MessageFormat *messageFormat;
    Window *window;
    u8 *windowOpenPtr;
    u8 *textPrinterNumPtr;
} MessageBox;

static void ovFieldMain_ShowMessageInField(ScriptContext *ctx, MsgData *messageData, u32 messageNum);
static void ovFieldMain_ShowMessageWithFormat(ScriptContext *ctx, MessageFormat *messageFormat, u8 messageNum, u32 canABSpeedUp);
static u32 ovFieldMain_GetTextFrameDelay(ScriptContext *ctx);
static void ovFieldMain_GetMsgBoxParameters(FieldSystem *fieldSystem, MessageBox *messageBox);
static void ovFieldMain_GetMsgBoxParametersEx(FieldSystem *fieldSystem, MessageFormat *messageFormat, MessageBox *messageBox);
static void ovFieldMain_CreateMessageBox(FieldSystem *fieldSystem, MessageBox *messageBox);
static void ovFieldMain_ReadAndExpandMsgDataViaBuffer(MessageBox *messageBox, MsgData *messageData, u32 messageNum);
static void ovFieldMain_GetFormattedECMessage(MessageBox *messageBox, u16 messageBank, u16 messageNum, u16 word1, u16 word2);
static void ovFieldMain_PrintMessageEx(MessageBox *messageBox, FontID fontId, u32 textFrameDelay, BOOL canABSpeedUp, u32 autoScrollParam);
static void ovFieldMain_AddTextPrinterParameterized(MessageBox *messageBox, FontID fontId);

const u16 gScriptStdMsgBanks[] = {
    NARC_msg_msg_0752_bin, // day of the week siblings
    NARC_msg_msg_0211_bin, // field moves
    NARC_msg_msg_0030_bin, // cameron the photographer
    NARC_msg_msg_0435_bin, // pokemart and various shops
};

BOOL ScrCmd_NonNPCMsg(ScriptContext *ctx) {
    u8 messageNum = ScriptReadByte(ctx);
    ovFieldMain_ShowMessageInField(ctx, ctx->msgdata, messageNum);
    return FALSE;
}

BOOL ScrCmd_GetStdMsgNaix(ScriptContext *ctx) {
    u16 r5 = ScriptGetVar(ctx);
    u16 *fileIdPtr = ScriptGetVarPointer(ctx);
    if (r5 >= NELEMS(gScriptStdMsgBanks)) {
        *fileIdPtr = 0;
    } else {
        *fileIdPtr = gScriptStdMsgBanks[r5];
    }
    return FALSE;
}

BOOL ScrCmd_NonNPCMsgExtern(ScriptContext *ctx) {
    u16 fileId = ScriptGetVar(ctx);
    u16 messageNum = ScriptGetVar(ctx);
    MsgData *messageData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, fileId, HEAP_ID_FIELD3);
    ovFieldMain_ShowMessageInField(ctx, messageData, messageNum);
    DestroyMsgData(messageData);
    return FALSE;
}

BOOL ScrCmd_MsgBoxExtern(ScriptContext *ctx) {
    u16 fileId = ScriptGetVar(ctx);
    u16 messageNum = ScriptGetVar(ctx);
    MsgData *messageData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, fileId, HEAP_ID_FIELD3);
    ovFieldMain_ShowMessage(ctx, messageData, messageNum, TRUE, NULL);
    DestroyMsgData(messageData);
    SetupNativeScript(ctx, ScrNative_WaitTextPrinter);
    return TRUE;
}

BOOL ScrCmd_441(ScriptContext *ctx) {
    u16 messageBank = ScriptReadHalfword(ctx);
    u16 messageNum = ScriptReadHalfword(ctx);
    u16 word1 = ScriptReadHalfword(ctx);
    u16 word2 = ScriptReadHalfword(ctx);
    ovFieldMain_ShowEasyChatMessage(ctx, messageBank, messageNum, word1, (s16)word2, 0xFF);
    return FALSE;
}

BOOL ScrCmd_442(ScriptContext *ctx) {
    u16 messageBank = ScriptReadHalfword(ctx);
    u16 messageNum = ScriptReadHalfword(ctx);
    u16 word1 = ScriptReadHalfword(ctx);
    u16 word2 = ScriptReadHalfword(ctx);
    ovFieldMain_ShowEasyChatMessage(ctx, messageBank, messageNum, word1, (s16)word2, TRUE);
    SetupNativeScript(ctx, ScrNative_WaitTextPrinter);
    return TRUE;
}

BOOL ScrCmd_443(ScriptContext *ctx) {
    u8 index = ScriptReadByte(ctx);
    FrontierFieldSystem *frontierFsys = ctx->fieldSystem->frontierFsys;
    if (frontierFsys == NULL) {
        return FALSE;
    }
    MailMessage *intro = &frontierFsys->opponents[index].introMessage;
    if (intro->msg_bank == 0xFFFF) {
        MsgData *messageData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 723, HEAP_ID_FIELD3);
        ovFieldMain_ShowMessage(ctx, messageData, intro->msg_no, TRUE, NULL);
        DestroyMsgData(messageData);
    } else {
        ovFieldMain_ShowEasyChatMessage(ctx, intro->msg_bank, intro->msg_no, intro->fields[0], intro->fields[1], TRUE);
    }
    SetupNativeScript(ctx, ScrNative_WaitTextPrinter);
    return TRUE;
}

BOOL ScrCmd_444(ScriptContext *ctx) {
    FieldSystem *fieldSystem = ctx->fieldSystem;
    u8 baseMessageNum = ScriptReadByte(ctx);
    u16 numEligiblePokemon = ScriptGetVar(ctx);
    u16 r2 = ScriptReadHalfword(ctx);
    u8 r3 = ScriptReadByte(ctx);
    u8 numLegendaryPokemonSeen = 0;
    MessageFormat *messageFormat = Frontier_NewBannedSpeciesMessageFormat(fieldSystem->saveData, numEligiblePokemon, r2, r3, &numLegendaryPokemonSeen);
    ovFieldMain_ShowMessageWithFormat(ctx, messageFormat, baseMessageNum + numLegendaryPokemonSeen, TRUE);
    MessageFormat_Delete(messageFormat);
    SetupNativeScript(ctx, ScrNative_WaitTextPrinter);
    return TRUE;
}

BOOL ScrCmd_527(ScriptContext *ctx) {
    struct MessagePrintOptions unkStruct;
    u16 messageNum = ScriptReadHalfword(ctx);
    ovFieldMain_InitMessagePrintOptions(&unkStruct, ctx);
    unkStruct.fontId = 3;
    ovFieldMain_ShowMessage(ctx, ctx->msgdata, messageNum, FALSE, &unkStruct);
    SetupNativeScript(ctx, ScrNative_WaitTextPrinter);
    return TRUE;
}

BOOL ScrCmd_NPCMsg(ScriptContext *ctx) {
    u8 messageNum = ScriptReadByte(ctx);
    ovFieldMain_ShowMessage(ctx, ctx->msgdata, messageNum, TRUE, NULL);
    SetupNativeScript(ctx, ScrNative_WaitTextPrinter);
    return TRUE;
}

BOOL ScrNative_WaitTextPrinter(ScriptContext *ctx) {
    u8 *textPrinterNumPtr = FieldSysGetAttrAddr(ctx->fieldSystem, SCRIPTENV_TEXT_PRINTER_NUMBER);
    return DialogBox_IsPrintFinished(*textPrinterNumPtr);
}

BOOL ScrCmd_NonNPCMsgVar(ScriptContext *ctx) {
    u16 messageNum = ScriptGetVar(ctx);
    ovFieldMain_ShowMessage(ctx, ctx->msgdata, (u8)messageNum, TRUE, NULL);
    SetupNativeScript(ctx, ScrNative_WaitTextPrinter);
    return TRUE;
}

BOOL ScrCmd_592(ScriptContext *ctx) {
    struct MessagePrintOptions unkStruct;
    u16 messageNum = ScriptGetVar(ctx);
    ovFieldMain_InitMessagePrintOptions(&unkStruct, ctx);
    unkStruct.autoScroll = 1;
    ovFieldMain_ShowMessage(ctx, ctx->msgdata, (u8)messageNum, TRUE, &unkStruct);
    SetupNativeScript(ctx, ScrNative_WaitTextPrinter);
    return TRUE;
}

BOOL ScrCmd_NPCMsgVar(ScriptContext *ctx) {
    u16 messageNum = ScriptGetVar(ctx);
    ovFieldMain_ShowMessage(ctx, ctx->msgdata, (u8)messageNum, FALSE, NULL);
    SetupNativeScript(ctx, ScrNative_WaitTextPrinter);
    return TRUE;
}

BOOL ScrCmd_GenderMsgBox(ScriptContext *ctx) {
    void *unused = Save_PlayerData_GetProfile(FieldSystem_GetSaveData(ctx->fieldSystem));
    u8 messageNumMale = ScriptReadByte(ctx);
    u8 messageNumFemale = ScriptReadByte(ctx);
    u32 gender = PlayerProfile_GetTrainerGender(Save_PlayerData_GetProfile(FieldSystem_GetSaveData(ctx->fieldSystem)));
    if (gender != PLAYER_GENDER_MALE) {
        ovFieldMain_ShowMessage(ctx, ctx->msgdata, messageNumFemale, TRUE, NULL);
    } else {
        ovFieldMain_ShowMessage(ctx, ctx->msgdata, messageNumMale, TRUE, NULL);
    }
    SetupNativeScript(ctx, ScrNative_WaitTextPrinter);
    return TRUE;
}

BOOL ScrCmd_455(ScriptContext *ctx) {
    LocalMapObject **objPtr = FieldSysGetAttrAddr(ctx->fieldSystem, SCRIPTENV_LAST_INTERACTED);
    u8 mapObjectType = MapObject_GetType(*objPtr);
    ovFieldMain_ShowMessage(ctx, ctx->msgdata, mapObjectType, TRUE, NULL);
    SetupNativeScript(ctx, ScrNative_WaitTextPrinter);
    return TRUE;
}

void ovFieldMain_InitMessagePrintOptions(struct MessagePrintOptions *unkStruct, ScriptContext *ctx) {
    unkStruct->textFrameDelay = ovFieldMain_GetTextFrameDelay(ctx);
    unkStruct->autoScroll = 0;
    unkStruct->fontId = 1;
}

void ovFieldMain_ShowMessage(ScriptContext *ctx, MsgData *messageData, u16 messageNum, u8 canABSpeedUp, struct MessagePrintOptions *unkStruct) {
    MessageBox messageBox;
    ovFieldMain_GetMsgBoxParameters(ctx->fieldSystem, &messageBox);
    ovFieldMain_CreateMessageBox(ctx->fieldSystem, &messageBox);
    ovFieldMain_ReadAndExpandMsgDataViaBuffer(&messageBox, messageData, messageNum);
    u32 textFrameDelay;
    u32 unk1;
    u8 fontId;
    if (unkStruct == NULL) {
        textFrameDelay = ovFieldMain_GetTextFrameDelay(ctx);
        unk1 = 0;
        fontId = 1;
    } else {
        textFrameDelay = unkStruct->textFrameDelay;
        unk1 = unkStruct->autoScroll;
        fontId = unkStruct->fontId;
    }
    ovFieldMain_PrintMessageEx(&messageBox, fontId, textFrameDelay, canABSpeedUp, unk1);
}

static void ovFieldMain_ShowMessageInField(ScriptContext *ctx, MsgData *messageData, u32 messageNum) {
    MessageBox messageBox;
    ovFieldMain_GetMsgBoxParameters(ctx->fieldSystem, &messageBox);
    ovFieldMain_CreateMessageBox(ctx->fieldSystem, &messageBox);
    ovFieldMain_ReadAndExpandMsgDataViaBuffer(&messageBox, messageData, messageNum);
    ovFieldMain_AddTextPrinterParameterized(&messageBox, 1);
}

// word2 needs to be signed to match
void ovFieldMain_ShowEasyChatMessage(ScriptContext *ctx, u16 messageBank, u16 messageNum, u16 word1, s16 word2, u8 canABSpeedUp) {
    MessageBox messageBox;
    ovFieldMain_GetMsgBoxParameters(ctx->fieldSystem, &messageBox);
    ovFieldMain_CreateMessageBox(ctx->fieldSystem, &messageBox);
    ovFieldMain_GetFormattedECMessage(&messageBox, messageBank, messageNum, word1, (u16)word2);
    if (canABSpeedUp != 0xFF) {
        ovFieldMain_PrintMessageEx(&messageBox, 1, ovFieldMain_GetTextFrameDelay(ctx), canABSpeedUp, 0);
    } else {
        ovFieldMain_AddTextPrinterParameterized(&messageBox, 1);
    }
}

static void ovFieldMain_ShowMessageWithFormat(ScriptContext *ctx, MessageFormat *messageFormat, u8 messageNum, u32 canABSpeedUp) {
    MessageBox messageBox;
    ovFieldMain_GetMsgBoxParametersEx(ctx->fieldSystem, messageFormat, &messageBox);
    ovFieldMain_CreateMessageBox(ctx->fieldSystem, &messageBox);
    ovFieldMain_ReadAndExpandMsgDataViaBuffer(&messageBox, ctx->msgdata, messageNum);
    ovFieldMain_PrintMessageEx(&messageBox, 1, ovFieldMain_GetTextFrameDelay(ctx), canABSpeedUp, 0);
}

static u32 ovFieldMain_GetTextFrameDelay(ScriptContext *ctx) {
    return Options_GetTextFrameDelay(Save_PlayerData_GetOptionsAddr(ctx->fieldSystem->saveData));
}

static void ovFieldMain_GetMsgBoxParameters(FieldSystem *fieldSystem, MessageBox *messageBox) {
    messageBox->message = *(String **)FieldSysGetAttrAddr(fieldSystem, SCRIPTENV_STRING_BUFFER_0);
    messageBox->buffer = *(String **)FieldSysGetAttrAddr(fieldSystem, SCRIPTENV_STRING_BUFFER_1);
    messageBox->messageFormat = *(MessageFormat **)FieldSysGetAttrAddr(fieldSystem, SCRIPTENV_MESSAGE_FORMAT);
    messageBox->window = (Window *)FieldSysGetAttrAddr(fieldSystem, SCRIPTENV_WINDOW);
    messageBox->windowOpenPtr = (u8 *)FieldSysGetAttrAddr(fieldSystem, SCRIPTENV_FIELD_08);
    messageBox->textPrinterNumPtr = (u8 *)FieldSysGetAttrAddr(fieldSystem, SCRIPTENV_TEXT_PRINTER_NUMBER);
}

static void ovFieldMain_GetMsgBoxParametersEx(FieldSystem *fieldSystem, MessageFormat *messageFormat, MessageBox *messageBox) {
    messageBox->message = *(String **)FieldSysGetAttrAddr(fieldSystem, SCRIPTENV_STRING_BUFFER_0);
    messageBox->buffer = *(String **)FieldSysGetAttrAddr(fieldSystem, SCRIPTENV_STRING_BUFFER_1);
    messageBox->messageFormat = messageFormat;
    messageBox->window = (Window *)FieldSysGetAttrAddr(fieldSystem, SCRIPTENV_WINDOW);
    messageBox->windowOpenPtr = (u8 *)FieldSysGetAttrAddr(fieldSystem, SCRIPTENV_FIELD_08);
    messageBox->textPrinterNumPtr = (u8 *)FieldSysGetAttrAddr(fieldSystem, SCRIPTENV_TEXT_PRINTER_NUMBER);
}

static void ovFieldMain_CreateMessageBox(FieldSystem *fieldSystem, MessageBox *messageBox) {
    if (*(messageBox->windowOpenPtr) == 0) {
        DialogBox_AddWindowToLayer3(fieldSystem->bgConfig, messageBox->window, GF_BG_LYR_MAIN_3);
        DialogBox_LoadFrame(messageBox->window, Save_PlayerData_GetOptionsAddr(fieldSystem->saveData));
        *(messageBox->windowOpenPtr) = 1;
        fieldSystem->textbox_open = TRUE;
    }
    FillWindowPixelBuffer(messageBox->window, 15);
}

static void ovFieldMain_ReadAndExpandMsgDataViaBuffer(MessageBox *messageBox, MsgData *messageData, u32 messageNum) {
    ReadMsgDataIntoString(messageData, messageNum, messageBox->buffer);
    StringExpandPlaceholders(messageBox->messageFormat, messageBox->message, messageBox->buffer);
}

static void ovFieldMain_GetFormattedECMessage(MessageBox *messageBox, u16 messageBank, u16 messageNum, u16 word1, u16 word2) {
    struct MailMessage mailMessage;
    MailMsg_Init(&mailMessage);
    MailMsg_SetMsgBankAndNum(&mailMessage, messageBank, messageNum);
    MailMsg_SetFieldI(&mailMessage, 0, word1);
    MailMsg_SetFieldI(&mailMessage, 1, word2);
    String *string = MailMsg_GetExpandedString(&mailMessage, HEAP_ID_FIELD3);
    String_Copy(messageBox->message, string);
    String_Delete(string);
}

static void ovFieldMain_PrintMessageEx(MessageBox *messageBox, FontID fontId, u32 textFrameDelay, BOOL canABSpeedUp, u32 autoScrollParam) {
    *(messageBox->textPrinterNumPtr) = DialogBox_PrintMessageEx(messageBox->window, messageBox->message, fontId, textFrameDelay, canABSpeedUp, autoScrollParam);
}

static void ovFieldMain_AddTextPrinterParameterized(MessageBox *messageBox, FontID fontId) {
    *(messageBox->textPrinterNumPtr) = AddTextPrinterParameterized(messageBox->window, fontId, messageBox->message, 0, 0, 0, NULL);
}
