#include "overlay_03.h"

#include "field_system.h"
#include "global.h"
#include "mart.h"
#include "task.h"
#include "text.h"
#include "unk_02005D10.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0182.h"

extern u16 ov03_0225927A[41];
extern UnkStruct_ov03* ov03_022598A0;

// TODO: Make all of these static.
void ov03_02253ED0(ListMenuTemplate listMenuTemplate, u8 x, u8 y, u8 width, u8 height, u16 baseTile);
void ov03_02253F74(FieldSystem *fieldSystem); 
void Unk03_Delete();
void ov03_022540E8(UnkStruct_ov03 *unk_ov03);
void ov03_02254100(u32 unused_0, u32 unused_1, BOOL arg2);
static void ov03_02254150(ListMenu *list, u16 cursorPos_unused, u8 itemsAbove);
void ov03_022542C8(UnkStruct_ov03* unk_ov03);

void ov03_022543AC(UnkStruct_ov03 *unk_ov03);
void ov03_02254D64(SysTask *, void *);

int sub_0203511C();
void sub_020351DC(u16 cursorPos, PlayerProfile *profile);
void sub_02058124(FieldSystem *fieldSystem, u8, u32);

void ov03_02253E20(s32 strno, BOOL arg1) {
    if (IsPrintFinished(ov03_022598A0->printerID) == FALSE) {
        RemoveTextPrinter(ov03_022598A0->printerID);
    }
    if (arg1) {
        ReadMsgDataIntoString(ov03_022598A0->msgData, strno, ov03_022598A0->string[4]);
        StringExpandPlaceholders(ov03_022598A0->messageFormat[2], ov03_022598A0->string[5], ov03_022598A0->string[4]);
    } else {
        ReadMsgDataIntoString(ov03_022598A0->msgData, strno, ov03_022598A0->string[5]);
    }
    if (WindowIsInUse(&ov03_022598A0->windows[2]) == FALSE) {
        sub_0205B514(ov03_022598A0->fieldSystem->bgConfig, &ov03_022598A0->windows[2], 3);
    }
    sub_0205B564(&ov03_022598A0->windows[2], Save_PlayerData_GetOptionsAddr(ov03_022598A0->fieldSystem->saveData));
    ov03_022598A0->printerID = sub_0205B5B4(&ov03_022598A0->windows[2], ov03_022598A0->string[5], Save_PlayerData_GetOptionsAddr(ov03_022598A0->fieldSystem->saveData), TRUE);
}

void ov03_02253ED0(ListMenuTemplate listMenuTemplate, u8 x, u8 y, u8 width, u8 height, u16 baseTile) {
    if (WindowIsInUse(&ov03_022598A0->windows[0]) == FALSE) {
        AddWindowParameterized(ov03_022598A0->fieldSystem->bgConfig, &ov03_022598A0->windows[0], 3, x, y, width, height, 13, baseTile);
    }
    DrawFrameAndWindow1(&ov03_022598A0->windows[0], TRUE, 0x3D9, HEAP_ID_FIELD2);
    ListMenuTemplate temp = listMenuTemplate;
    temp.items = ov03_022598A0->items;
    temp.window = &ov03_022598A0->windows[0];
    ov03_022598A0->listMenu = ListMenuInit(&temp, 0, 0, HEAP_ID_FIELD1);
    CopyWindowToVram(&ov03_022598A0->windows[0]);
}

void ov03_02253F74(FieldSystem *fieldSystem) {
    if (ov03_022598A0) {
        GF_AssertFail();
    }
    UnkStruct_ov03 *unk_ov03 = Heap_Alloc(HEAP_ID_FIELD1, sizeof(UnkStruct_ov03));
    ov03_022598A0 = unk_ov03;
    MI_CpuFill8(unk_ov03, 0, sizeof(UnkStruct_ov03));
    ov03_022598A0->unk88 = 0;
    ov03_022598A0->fieldSystem = fieldSystem;
    ov03_022598A0->unk97 = 0;
    ov03_022598A0->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0182_bin, HEAP_ID_FIELD1);
    ov03_022598A0->playerProfile[0] = Save_PlayerData_GetProfile(FieldSystem_GetSaveData(ov03_022598A0->fieldSystem));
    ov03_022598A0->playerProfile[1] = PlayerProfile_New(HEAP_ID_FIELD1);
    InitWindow(&ov03_022598A0->windows[0]);
    InitWindow(&ov03_022598A0->windows[1]);
    InitWindow(&ov03_022598A0->windows[2]);
    ov03_022598A0->messageFormat[0] = MessageFormat_New(HEAP_ID_FIELD1);
    ov03_022598A0->messageFormat[1] = MessageFormat_New(HEAP_ID_FIELD1);
    ov03_022598A0->messageFormat[2] = MessageFormat_New(HEAP_ID_FIELD1);
    ov03_022598A0->unk98 = 0;
    int i;
    for (i = 0; i < 8; i++) { // Fascinating choice not to just do these together as one loop.
        ov03_022598A0->unk80[i] = 0;
    }
    for (i = 0; i < 8; i++) {
        ov03_022598A0->string[i] = String_New(0x8C, HEAP_ID_FIELD1);
    }
}

void Unk03_Delete() {
    for (int i = 0; i < 8; i++) {
        String_Delete(ov03_022598A0->string[i]);
    };

    Heap_Free(ov03_022598A0->playerProfile[1]);

    if (ov03_022598A0->messageFormat[0]) {
        MessageFormat_Delete(ov03_022598A0->messageFormat[0]);
        ov03_022598A0->messageFormat[0] = NULL;
    }
    if (ov03_022598A0->messageFormat[1]) {
        MessageFormat_Delete(ov03_022598A0->messageFormat[1]);
        ov03_022598A0->messageFormat[1] = NULL;
    }
    if (ov03_022598A0->messageFormat[2]) {
        MessageFormat_Delete(ov03_022598A0->messageFormat[2]);
        ov03_022598A0->messageFormat[2] = NULL;
    }
    
    DestroyMsgData(ov03_022598A0->msgData);

    if (ov03_022598A0->windows[2].pixelBuffer) {
        RemoveWindow(&ov03_022598A0->windows[2]);
    }
    
    Heap_Free(ov03_022598A0);
    ov03_022598A0 = NULL;
}

void ov03_022540E8(UnkStruct_ov03 *unk_ov03) {
    sub_02058124(unk_ov03->fieldSystem, unk_ov03->unk91, ov03_02255B98());
}

void ov03_02254100(u32 unused_0, u32 unused_1, BOOL arg2) {
    ov03_022598A0->unk98 = 1;
    if (arg2 == FALSE) {
        PlaySE(SEQ_SE_DP_SELECT);
    }
}

void ov03_02254120(ListMenu *list, enum ListMenuAttr attr_unused, s32 unk2_unused) {
    for (u32 i = 0; i < ListMenuGetTemplateField(list, LISTMENUATTR_MAXSHOWED); i++) {
        ov03_02254150(list, 0, i);
    };
}

static void ov03_02254150(ListMenu *list, u16 cursorPos_unused, u8 itemsAbove) {
    s32 res = sub_0203511C();
    u16 cursorPos = 0;
    if (ov03_022598A0->listMenu) {
        ListMenuGetScrollAndRow(ov03_022598A0->listMenu, &cursorPos, NULL);
    }
    cursorPos += itemsAbove;
    FillWindowPixelRect(&ov03_022598A0->windows[0], 15, 8, itemsAbove * 16, (GetWindowWidth(&ov03_022598A0->windows[0]) * 8) - 8, 16);
    if (cursorPos < res) {
        sub_020351DC(cursorPos, ov03_022598A0->playerProfile[1]);
        BufferIntegerAsString(ov03_022598A0->messageFormat[0], 0, cursorPos + 1, 2, PRINTING_MODE_LEADING_ZEROS, TRUE);
        BufferPlayersName(ov03_022598A0->messageFormat[0], 1, ov03_022598A0->playerProfile[1]);
        ReadMsgDataIntoString(ov03_022598A0->msgData, msg_0182_00076, ov03_022598A0->string[1]); // {STRVAR_1 51, 0, 0}:{STRVAR_1 3, 1, 0}
        StringExpandPlaceholders(ov03_022598A0->messageFormat[0], ov03_022598A0->string[0], ov03_022598A0->string[1]);
        AddTextPrinterParameterized(&ov03_022598A0->windows[0], 0, ov03_022598A0->string[0], 8, itemsAbove * 16, TEXT_SPEED_NOTRANSFER, 0);
        BufferIntegerAsString(ov03_022598A0->messageFormat[0], 2, PlayerProfile_GetTrainerID(ov03_022598A0->playerProfile[1]) % 0x10000, 5, PRINTING_MODE_LEADING_ZEROS, TRUE);
        ReadMsgDataIntoString(ov03_022598A0->msgData, msg_0182_00078, ov03_022598A0->string[3]); // ID{STRVAR_1 54, 2, 0}
        StringExpandPlaceholders(ov03_022598A0->messageFormat[0], ov03_022598A0->string[2], ov03_022598A0->string[3]);
        AddTextPrinterParameterized(&ov03_022598A0->windows[0], 0, ov03_022598A0->string[2], 88, itemsAbove * 16, TEXT_SPEED_NOTRANSFER, 0);
    } else {
        BufferIntegerAsString(ov03_022598A0->messageFormat[0], 0, cursorPos + 1, 2, PRINTING_MODE_LEADING_ZEROS, TRUE);
        ReadMsgDataIntoString(ov03_022598A0->msgData, msg_0182_00077, ov03_022598A0->string[1]); // {STRVAR_1 51, 0, 0}:
        StringExpandPlaceholders(ov03_022598A0->messageFormat[0], ov03_022598A0->string[0], ov03_022598A0->string[1]);
        AddTextPrinterParameterized(&ov03_022598A0->windows[0], 0, ov03_022598A0->string[0], 8, itemsAbove * 16, TEXT_SPEED_NOTRANSFER, 0);
    }
}

/*void ov03_022542C8(UnkStruct_ov03* unk_ov03) {
    if (WindowIsInUse(&ov03_022598A0->windows[1]) == FALSE) {
        AddWindowParameterized(ov03_022598A0->fieldSystem->bgConfig, &ov03_022598A0->windows[1], 3, 23, 2, 8, 4, 13, 201);
    }
    DrawFrameAndWindow1(&ov03_022598A0->windows[1], TRUE, 985, HEAP_ID_FIELD2);
    FillWindowPixelBuffer(&ov03_022598A0->windows[1], 15);
    CopyWindowToVram(&ov03_022598A0->windows[1]);
    unk_ov03->unk97 = 1;
    SysTask_CreateOnMainQueue(ov03_02254D64, unk_ov03, 0);
    ov03_022543AC(ov03_022598A0);
    
    u16 ov03_0225927A[41] = {
        48,
        49,
        50,
        52,
        51,
        51,
        56,
        57,
        58,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        59,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        60,
        61,
        0,
        62,
        63,
        64,
        0,
        0,
        0,
        0,
        53,
        53,
        54,
        55,
    };

    if (ov03_022598A0->unk91 == 8) {
        ov03_02253E20(ov03_022598A0->unk92 + 0x7A, FALSE);
    } else if (ov03_022598A0->unk91 == 0x28) {
        ov03_02253E20(ov03_022598A0->unk92 + 0x94, FALSE);
    } else {
        ov03_02253E20(ov03_0225927A[ov03_022598A0->unk91], 0);
    }
    ov03_02254B4C(&ov03_02254420);
}*/
