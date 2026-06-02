#include "overlay_01.h"
#include "overlay_03.h"

#include "field_system.h"
#include "global.h"
#include "mart.h"
#include "task.h"
#include "text.h"
#include "unk_02005D10.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0182.h"

extern ListMenuTemplate ov03_02259164;
extern u32 ov03_02259134;
extern UnkStruct_ov03* ov03_022598A0;

// TODO: Make all of these static.
static void ov03_02254500(void *arg0, UnkStruct_ov03 *unk_ov03);
static void ov03_02254600(UnkStruct_ov03* unk_ov03_unused);
void ov03_02254660(s32 arg0, s32 arg1);
void ov03_0225574C(void *arg0, UnkStruct_ov03 *unk_ov03);
void ov03_02254B44();

int sub_0203511C();
u32 sub_02035150(u32 index);
u8 sub_02035184();
void sub_02035198();
BOOL sub_02037700();
void sub_02037BC8();
void sub_02058164(u16 arg0);

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