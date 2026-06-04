#include "overlay_01.h"
#include "overlay_03.h"

#include "field_system.h"
#include "global.h"
#include "mart.h"
#include "render_text.h"
#include "render_window.h"
#include "task.h"
#include "text.h"
#include "text_0205B4EC.h"
#include "unk_02005D10.h"
#include "unk_02034354.h"
#include "unk_02035900.h"
#include "unk_020379A0.h"
#include "unk_02037C94.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0182.h"

extern UnkStruct_ov03* ov03_022598A0;

void ov03_02255A00();
void ov03_02255A2C(SysTask *task);
void ov03_02255A70(UnkStruct_ov03 *data);
void ov03_02255AB0(SysTask *task, void *data);
void ov03_02255ADC(void *data);
void ov03_02255B04(SysTask *task, void *data);
void ov03_02255B44();
void ov03_02255BEC();
void ov03_02255C54();

void sub_020356C0(u32 arg0);
int sub_0203775C(u32 arg0);
int sub_0203774C(u32 arg0);
void sub_02037EC0(u32 arg0);
void sub_02058180();

void ov03_02255A00() {
    if (IsPrintFinished(ov03_022598A0->printerID)) {
        ov03_02253E20(0, FALSE);
        ov03_02254B4C(&ov03_02255A2C);
    }
}

void ov03_02255A2C(SysTask *task) {
    if (IsPrintFinished(ov03_022598A0->printerID) && ((PAD_BUTTON_A | PAD_BUTTON_B) & gSystem.newKeys || System_GetTouchNew() == TRUE)) {
        SysTask_Destroy(task);
        ov03_02254B44();
        ov03_022598A0->unk88 = 1;
    }
}

void ov03_02255A70(UnkStruct_ov03 *data) {
    sub_020351DC(data->unk8E, ov03_022598A0->playerProfile[1]);
    BufferPlayersName(data->messageFormat[2], 1, ov03_022598A0->playerProfile[1]);
    ov03_02253E20(5, TRUE);
    sub_02058180();
    SysTask_CreateOnMainQueue(&ov03_02255AB0, data, 0);
}

void ov03_02255AB0(SysTask *task, void *data) {
    if (IsPrintFinished(ov03_022598A0->printerID)) {
        sub_02058180();
        ov03_022542C8(data);
        SysTask_Destroy(task);
    }
}

void ov03_02255ADC(void *data) {
    sub_02058180();
    for (int i = 0; i < 8; i++) {
        sub_020346E8(i);
    }
    SysTask_CreateOnMainQueue(&ov03_02255B04, data, 0);
}

void ov03_02255B04(SysTask *task, void *data) {
    if (IsPrintFinished(ov03_022598A0->printerID) && ((PAD_BUTTON_A | PAD_BUTTON_B) & gSystem.newKeys || System_GetTouchNew() == TRUE)) {
        ov03_022542C8(data);
        SysTask_Destroy(task);
    }
}

void ov03_02255B44() {
    sub_02037EC0(0);
    ov03_022598A0->unk88 = 2;
    sub_020398D4(1, 1);
    sub_02034434();
    sub_020356C0(1);
}

int ov03_02255B70() {
    return sub_0203775C(ov03_022598A0->unk91);
}

int ov03_02255B84() {
    return sub_0203774C(ov03_022598A0->unk91);
}

int ov03_02255B98() {
    return ov03_022598A0->unk92 + (ov03_022598A0->unk93 * 16);
}

void ov03_02255BB0(FieldSystem *fieldSystem, u32 arg1, u16 arg2, u16 arg3) {
    ov03_02253F74(fieldSystem);
    ov03_022598A0->unk91 = arg1;
    if (arg1 - 3 <= 1) {
        fieldSystem->linkBattleRuleset = NULL;
    }
    ov03_022598A0->unk92 = arg2;
    ov03_022598A0->unk93 = arg3;
    ov03_022540E8(ov03_022598A0);
}

void ov03_02255BEC() {
    ov03_022542C8(ov03_022598A0);
}

u32 ov03_02255BFC() {
    u32 unk88 = ov03_022598A0->unk88; // Needs to be reserved as the pointer will be null after the next line.
    if (unk88) {
        Unk03_Delete();
    }
    return unk88;
}

void ov03_02255C18(FieldSystem *fieldSystem, u32 arg1, u16 arg2, u16 arg3) {
    ov03_02253F74(fieldSystem);
    ov03_022598A0->unk91 = arg1;
    if (arg1 - 3 <= 1) {
        fieldSystem->linkBattleRuleset = NULL;
    }
    ov03_022598A0->unk92 = arg2;
    ov03_022598A0->unk93 = arg3;
    ov03_02254B2C(ov03_022598A0);
}

void ov03_02255C54() {
    ov03_02254B58(ov03_022598A0);
}

u32 ov03_02255C64() {
    u32 unk88 = ov03_022598A0->unk88;
    if (unk88) {
        Unk03_Delete();
    }
    return unk88;
}

BOOL ov03_02255C80() {
    return FALSE;
}

void ov03_02255C84(UnkStruct_ov03 *data) {
    ov01_021F6A9C(data->fieldSystem, 3, NULL);
    data->unkA8 = 0;
    data->unkA4 = 0;
}

BOOL ov03_02255CA0(UnkStruct_ov03 *data) {
    int unkInt = ov01_021F6B00(data->fieldSystem);
    BOOL unkBool = ov01_021F6B10(data->fieldSystem);
    if (unkInt == 3 && unkBool == TRUE) {
        ov01_021F6ABC(data->fieldSystem, 3, 3, &data->unkA4);
        return TRUE;
    }
    return FALSE;
}

BOOL ov03_02255CD0(UnkStruct_ov03 *data) {
    return ov01_021F6AEC(data->fieldSystem) >= 4;
}
