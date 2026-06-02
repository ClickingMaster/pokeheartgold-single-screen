#include "overlay_01.h"
#include "overlay_03.h"

#include "field_system.h"
#include "global.h"
#include "mart.h"
#include "render_text.h"
#include "task.h"
#include "text.h"
#include "text_0205B4EC.h"
#include "unk_02005D10.h"
#include "unk_02034354.h"
#include "unk_02035900.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0182.h"

extern u32 ov03_02254D78;
extern u32 ov03_0225912C;
extern u32 ov03_02259134;
extern ListMenuTemplate ov03_02259164;
extern UnkStruct_ov03* ov03_022598A0;

// TODO: Make all of these static.
static void ov03_02254500(void *arg0, UnkStruct_ov03 *unk_ov03);
static void ov03_02254600(UnkStruct_ov03* unk_ov03_unused);
static void ov03_02254660(void *arg0, UnkStruct_ov03 *unk_ov03);
static void ov03_022546B0(void *arg0, UnkStruct_ov03 *unk_ov03);
static BOOL ov03_022547D8(void *arg0, UnkStruct_ov03 *unk_ov03);
static void ov03_022548E0(void *arg0, UnkStruct_ov03 *unk_ov03);
static void ov03_0225499C(void *arg0, UnkStruct_ov03 *unk_ov03);
static void ov03_022549D8(void *arg0, UnkStruct_ov03 *unk_ov03);
void ov03_02254A54(ListMenu *listMenu_unused, u16 arg1_unused, u8 arg2);
void ov03_02254B2C(UnkStruct_ov03 *unk_ov03);
void ov03_02254B44();
void ov03_02254B58(UnkStruct_ov03 *unk_ov03);
void ov03_02254BEC();
void ov03_02254D64(SysTask *, void *);
void ov03_02255280(s32 arg0, s32 arg1);
void ov03_0225574C(void *arg0, UnkStruct_ov03 *unk_ov03);
void ov03_02255804(UnkStruct_ov03 *unk_ov03); 
void ov03_02255A70(UnkStruct_ov03 *unk_ov03);
void ov03_02255ADC(UnkStruct_ov03 *unk_ov03);
BOOL ov03_02255C80();
void ov03_02255C84(UnkStruct_ov03 *unk_ov03);
int ov03_02255CA0(UnkStruct_ov03 *unk_ov03);
BOOL ov03_02255CD0(UnkStruct_ov03 *unk_ov03);
void ov03_02255CE4(void*);

int sub_0203511C();
u32 sub_02035150(u32 index);
u8 sub_02035184();
void sub_02035198();
void sub_020351DC(u16, PlayerProfile *profile);
BOOL sub_02037700();
s32 sub_02037BA0(s32, s32);
void sub_02037BC8();
void sub_020580E4(FieldSystem *fieldSystem, s32 arg1, s32 arg2);
void sub_02058164(u16 arg0);
void sub_02058284();

void ov03_022543AC(UnkStruct_ov03* unk_ov03_unused) {
    BufferPlayersName(ov03_022598A0->messageFormat[1], 0, ov03_022598A0->playerProfile[0]);
    BufferIntegerAsString(ov03_022598A0->messageFormat[1], 1, PlayerProfile_GetTrainerID(ov03_022598A0->playerProfile[0]) % 0x10000, 5, PRINTING_MODE_LEADING_ZEROS, TRUE);
    ReadMsgDataIntoString(ov03_022598A0->msgData, 75, ov03_022598A0->string[6]);
    StringExpandPlaceholders(ov03_022598A0->messageFormat[1], ov03_022598A0->string[7], ov03_022598A0->string[6]);
    AddTextPrinterParameterized(&ov03_022598A0->windows[1], 0, ov03_022598A0->string[7], 2, 2, 0, NULL);
}

void ov03_02254420() {
    if (IsPrintFinished(ov03_022598A0->printerID)) {
        ov03_022598A0->items = ListMenuItems_New(16, HEAP_ID_FIELD1);
        for (int i = 0; i < 16; i++) {
            BufferIntegerAsString(ov03_022598A0->messageFormat[0], 0, i + 1, 2, PRINTING_MODE_LEADING_ZEROS, TRUE);
            ReadMsgDataIntoString(ov03_022598A0->msgData, 0x4D, ov03_022598A0->string[1]);
            StringExpandPlaceholders(ov03_022598A0->messageFormat[0], ov03_022598A0->string[0], ov03_022598A0->string[1]);
            ListMenuItems_AddItem(ov03_022598A0->items, ov03_022598A0->string[0], 0);
        };
        
        ov03_02253ED0(ov03_02259164, 1, 2, 20, 10, 1);
        
        ov03_022598A0->unk9C = &ov03_02259134;
        ov03_022598A0->unkA0 = 2;
        ov03_022598A0->unkA2 = 0xFFFF;
        ov03_022598A0->unkA1 = 0;
        ov01_021F6A9C(ov03_022598A0->fieldSystem, 7, &ov03_022598A0->unk9C);
        ov03_02254B4C(&ov03_02254500);
    }
}

static void ov03_02254500(void *arg0, UnkStruct_ov03 *unk_ov03) {
    ov03_022543AC(unk_ov03);
    if (ov01_021F6B10(unk_ov03->fieldSystem) == TRUE) {
        if (sub_02037700()) {
            ov03_022598A0->unk88 = 3;
            ov03_02254B44();
            ov03_0225574C(arg0, unk_ov03);
            return;
        }
        int listInput;
        if (sub_02035184() == 0) {
            listInput = ListMenu_ProcessInput(ov03_022598A0->listMenu);
            if (listInput == -1) {
                if (unk_ov03->unkA2 == 0) {
                    u16 currentItem;
                    ListMenuGetCurrentItemArrayId(ov03_022598A0->listMenu, &currentItem);
                    listInput = currentItem;
                } else if (unk_ov03->unkA2 == 1) {
                    listInput = -2;
                }
            }
        } else {
            listInput = -1;
        }
        switch(listInput) {
        case -1:
            ov03_02254600(unk_ov03);
            break;
            
        case -2:
            PlaySE(SEQ_SE_DP_SELECT);
            ov03_022598A0->unk88 = 1;
            ov03_02254B44();
            ov03_0225574C(arg0, unk_ov03);
            break;
        default:
            PlaySE(SEQ_SE_DP_SELECT);
            ListMenuGetCurrentItemArrayId(unk_ov03->listMenu, &unk_ov03->unk8E);
            if (sub_0203511C() > unk_ov03->unk8E) {
                ov03_02254600(unk_ov03);
                unk_ov03->unk8E = sub_02035150(unk_ov03->unk8E);
                sub_02058164(unk_ov03->unk8E);
                sub_02037BC8();
                ov03_02254B4C(&ov03_02254660);
            }
        }
    }
}

static void ov03_02254600(UnkStruct_ov03 *unk_ov03_unused) {
    if (sub_02035184() || ov03_022598A0->unk98) {
        ov03_022598A0->unk98 = 0;
        sub_02035198();
        FillWindowPixelRect(&ov03_022598A0->windows[0], 0xF, 8, 0, 0xC, 0x50);
        ov03_02254120(ov03_022598A0->listMenu, LISTMENUATTR_MOVECURSORFUNC, 0);
        CopyWindowToVram(&ov03_022598A0->windows[0]);
    }
}

static void ov03_02254660(void *arg0, UnkStruct_ov03 *unk_ov03) {
    if (sub_02037700()) {
        ov03_0225574C(arg0, unk_ov03);
        ov03_02253E20(0, FALSE);
        ov03_02255ADC(unk_ov03);
        ov03_022598A0->unkA1 = 1;
    } else {
        if (sub_02034818(sub_0203769C())) {
            sub_02037BC8();
            ov03_022546B0(arg0, unk_ov03);
        }
    }
}

static void ov03_022546B0(void *arg0, UnkStruct_ov03 *unk_ov03) {
    if (sub_02037700()) {
        ov03_0225574C(arg0, unk_ov03);
        ov03_02253E20(0, FALSE);
        ov03_02255ADC(unk_ov03);
        ov03_022598A0->unkA1 = 1;
        return;
    }
    sub_020351DC(unk_ov03->unk8E, ov03_022598A0->playerProfile[1]);
    BufferPlayersName(unk_ov03->messageFormat[2], 1, ov03_022598A0->playerProfile[1]);
    if (ov03_02255B70() <= 2) {
        ov03_02253E20(1, TRUE);
    } else {
        if (IsPrintFinished(ov03_022598A0->printerID) == FALSE) {
            RemoveTextPrinter(ov03_022598A0->printerID);
        }
        ReadMsgDataIntoString(ov03_022598A0->msgData, 2, ov03_022598A0->string[4]);
        StringExpandPlaceholders(ov03_022598A0->messageFormat[2], ov03_022598A0->string[5], ov03_022598A0->string[4]);
        if (WindowIsInUse(&ov03_022598A0->windows[2]) == FALSE) {
            sub_0205B514(ov03_022598A0->fieldSystem->bgConfig, &ov03_022598A0->windows[2], 3);
        }
        sub_0205B564(&ov03_022598A0->windows[2], Save_PlayerData_GetOptionsAddr(ov03_022598A0->fieldSystem->saveData));
        TextFlags_SetCanABSpeedUpPrint(TRUE);
        TextFlags_SetAutoScrollParam(0);
        TextFlags_SetCanTouchSpeedUpPrint(FALSE);
        ov03_022598A0->printerID = AddTextPrinterParameterized(&ov03_022598A0->windows[2], 1, ov03_022598A0->string[5], 0, 0, 1, 0);
    }
    ov03_022598A0->unkA8 = 0;
    ov03_022598A0->unkA1 = 1;
    ov03_02254B4C(&ov03_022548E0);
}

static BOOL ov03_022547D8(void *arg0, UnkStruct_ov03 *unk_ov03) {
    if (sub_02037700()) {
        ov03_0225574C(arg0, unk_ov03);
        ov03_02253E20(0, FALSE);
        ov03_02255ADC(unk_ov03);
    } else if (sub_02037BA0(0, 3) == sub_0203769C()) {
        sub_02037BC8();
        ov03_0225574C(arg0, unk_ov03);
        ov03_02255A70(unk_ov03);
    } else if (sub_02037BA0(0, 5) != -1) {
        sub_02037BC8();
        ov03_0225574C(arg0, unk_ov03);
        ov03_02255A70(unk_ov03);
    } else if (sub_02037BA0(0, 4) == sub_0203769C()) {
        ov03_02253E20(0x74, FALSE);
        ov03_02254B4C(&ov03_02255280);
    } else if (sub_02037BA0(0, 2) == sub_0203769C()) {
        sub_02037BC8();
        unk_ov03->unk90 = sub_020347A0();
        if (IsPrintFinished(ov03_022598A0->printerID) == FALSE) {
            RemoveTextPrinter(ov03_022598A0->printerID);
        }
        ov03_0225574C(arg0, unk_ov03);
        ov03_02255804(unk_ov03);
    } else if (ov03_02255C80()) {
        ov03_0225574C(arg0, unk_ov03);
        ov03_02255A70(unk_ov03);
    } else {
        return FALSE;
    }
    return TRUE;
}

static void ov03_022548E0(void *arg0, UnkStruct_ov03* unk_ovo3) {
    if (IsPrintFinished(ov03_022598A0->printerID)) {
        if (unk_ovo3->unkA8 == 0) {
            ov03_022598A0->unk9C = &ov03_0225912C;
            ov03_022598A0->unkA0 = 1;
            ov03_022598A0->unkA2 = 0xFFFF;
            ov03_022598A0->unkA1 = 0;
            ov01_021F6A9C(ov03_022598A0->fieldSystem, 7, &ov03_022598A0->unk9C);
            unk_ovo3->unkA8 = 1;
        }
        else if (ov01_021F6B10(unk_ovo3->fieldSystem) == TRUE) {
            if (ov03_022547D8(arg0, unk_ovo3)) {
                unk_ovo3->unkA1 = 2;
            }
            else if (PAD_BUTTON_B & gSystem.newKeys || unk_ovo3->unkA2 == 0) {
                PlaySE(SEQ_SE_DP_SELECT);
                unk_ovo3->unkA8 = 0;
                ov03_02253E20(3, FALSE);
                ov03_02254B4C(&ov03_0225499C);
            }
        }
    }
}

static void ov03_0225499C(void *arg0, UnkStruct_ov03 *unk_ov03) {
    if (ov03_022547D8(arg0, unk_ov03)) {
        unk_ov03->unkA1 = 2;
        return;
    }
    if (IsPrintFinished(ov03_022598A0->printerID)) {
        ov03_02255C84(unk_ov03);
        ov03_02254B4C(&ov03_022549D8);
    }
}

static void ov03_022549D8(void *arg0, UnkStruct_ov03 *unk_ov03) {
    if (unk_ov03->unkA8 == 0) {
        if (ov03_02255CA0(unk_ov03) == 1) {
            unk_ov03->unkA8 = 1;
        } else {
            return;
        }
    }
    if (ov03_02255CD0(unk_ov03)) {
        if (ov03_022547D8(arg0, unk_ov03)) {
            ov03_02255CE4(unk_ov03);
        }
        else if (ov01_021F6AEC(unk_ov03->fieldSystem) == 6) {
            if (unk_ov03->unkA4 == 0) {
                ov03_0225574C(arg0, unk_ov03);
                ov03_02253E20(6, 0);
                ov03_02255ADC(unk_ov03);
            } else if (unk_ov03->unkA4 == 1) {
                ov03_02254B4C(&ov03_022546B0);
            }
        }
    }
}

void ov03_02254A54(ListMenu *listMenu_unused, u16 arg1_unused, u8 arg2) {
    u16 spC = 0;
    if (ov03_022598A0->listMenu) {
        ListMenuGetScrollAndRow(ov03_022598A0->listMenu, &spC, NULL);
    }
    spC += arg2;
    if (sub_02034818(spC)) {
        BufferPlayersName(ov03_022598A0->messageFormat[0], 0, sub_02034818(spC));
        ReadMsgDataIntoString(ov03_022598A0->msgData, 79, ov03_022598A0->string[1]);
        StringExpandPlaceholders(ov03_022598A0->messageFormat[0], ov03_022598A0->string[0], ov03_022598A0->string[1]);
        s32 temp_r4 = arg2 * 16;
        AddTextPrinterParameterized(&ov03_022598A0->windows[0], 0, ov03_022598A0->string[0], 8, temp_r4, 0, 0);
        BufferIntegerAsString(ov03_022598A0->messageFormat[0], 2, PlayerProfile_GetTrainerID_VisibleHalf(sub_02034818(spC)), 5, PRINTING_MODE_LEADING_ZEROS, TRUE);
        ReadMsgDataIntoString(ov03_022598A0->msgData, 78, ov03_022598A0->string[3]);
        StringExpandPlaceholders(ov03_022598A0->messageFormat[0], ov03_022598A0->string[2], ov03_022598A0->string[3]);
        AddTextPrinterParameterized(&ov03_022598A0->windows[0], 0, ov03_022598A0->string[2], 72, temp_r4, 0, 0);
    }
}

void ov03_02254B2C(UnkStruct_ov03 *unk_ov03) {
    sub_020580E4(unk_ov03->fieldSystem, unk_ov03->unk91, ov03_02255B98());
}

void ov03_02254B44() {
    sub_02058284();
}

void ov03_02254B4C(void* arg0) {
    ov03_022598A0->unk6C = arg0;
}

void ov03_02254B58(UnkStruct_ov03 *unk_ov03) {
    SysTask_CreateOnMainQueue(&ov03_02254D64, unk_ov03, 0);
    ov03_02254BEC();
    if (WindowIsInUse(&ov03_022598A0->windows[1]) == FALSE) {
        AddWindowParameterized(ov03_022598A0->fieldSystem->bgConfig, &ov03_022598A0->windows[1], 3, 0x16, 2, 9, 4, 0xD, 0xCD);
    }
    DrawFrameAndWindow1(&ov03_022598A0->windows[1], 1, 0x3D9, 0xB);
    FillWindowPixelBuffer(&ov03_022598A0->windows[1], 0xF);
    CopyWindowToVram(&ov03_022598A0->windows[1]);
    unk_ov03->unk97 = 1;
    sub_0203476C(0);
    ov03_02254B4C(&ov03_02254D78);
}

/*void ov03_02254BEC() {
    u16 ov03_0225931E[41] = {
        7,
        8,
        9,
        11,
        10,
        10,
        15,
        16,
        17,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        18,
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
        19,
        20,
        0,
        21,
        22,
        23,
        0,
        0,
        0,
        0,
        12,
        12,
        13,
        0,
    };

    if (ov03_022598A0->unk91 >= 41) {
        GF_AssertFail();
    }
    if (ov03_022598A0->unk91 == 8) {
        if (sub_020347A0() > 1) {
            ov03_02253E20(ov03_022598A0->unk92 + 127, 0);
        } else {
            ov03_02253E20(ov03_022598A0->unk92 + 117, 0);
        }
    } else if (ov03_022598A0->unk91 == 40) {
        if (sub_020347A0() > 1) {
            ov03_02253E20(ov03_022598A0->unk92 + 154, 0);
        } else {
            ov03_02253E20(ov03_022598A0->unk92 + 142, 0);
        }
    } else {
        ov03_02253E20(ov03_0225931E[ov03_022598A0->unk91], 0);
    }
}*/