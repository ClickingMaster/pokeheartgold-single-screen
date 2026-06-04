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

extern UnkStruct_ov03* ov03_022598A0;
extern ListMenuTemplate ov03_02259144;

void ov03_02254D64(SysTask *task, void *arg1) {
    if (ov03_022598A0->unk6C) {
        ov03_022598A0->unk6C(task, arg1);
    }
}

void ov03_02254D78(s32 arg0_unused, UnkStruct_ov03 *unk_ov03) {
    ov03_02254C9C(unk_ov03);
    if (IsPrintFinished(ov03_022598A0->printerID)) {
        ov03_022598A0->items = ListMenuItems_New(5, HEAP_ID_FIELD1);
        int i;
        for (i = 0; i < 5; i++) {
            ListMenuItems_AppendFromMsgData(ov03_022598A0->items, ov03_022598A0->msgData, 82, 0);
        }
        ov03_022598A0->unk98 = 1;
        for (i = 0; i < 8; i++) {
            ov03_022598A0->unk80[i] = 0;
        }

        ov03_02253ED0(ov03_02259144, 1, 2, 17, 12, 1);
        ov03_02254B4C(&ov03_02254E70);
        if (ov03_02255B84() > 2) {
            ov03_022598A0->unk9C = &ov03_0225913C;
            ov03_022598A0->unkA0 = 2;
        } else {
            ov03_022598A0->unk9C = &ov03_02259130;
            ov03_022598A0->unkA0 = 1;
        }
        ov03_022598A0->unkA2 = 0xFFFF;
        ov03_022598A0->unkA1 = 0;
        ov01_021F6A9C(ov03_022598A0->fieldSystem, 7, &ov03_022598A0->unk9C);
    }
}

/*void ov03_02254E70(void *arg0, UnkStruct_ov03 *unk_ov03) {
    if (ov01_021F6B10(unk_ov03->fieldSystem) != 1) {
        return;
    }
    
    ov03_02254C9C(unk_ov03);
    
    u32 i;
    for (i = 0; i < 8; i++) {
        if (sub_02034714(i) && i != 0) {
            ov03_022598A0->unk80[i] = 1;
            ov03_022598A0->unk98 = 1;
        } else if (sub_02034818(i) && ov03_022598A0->unk80[i] == 0) {
            ov03_022598A0->unk80[i] = 1;
            ov03_022598A0->unk98 = 1;
        } else if (sub_02034818(i) == 0 && ov03_022598A0->unk80[i] != 0) {
            ov03_022598A0->unk80[i] = 0;
            ov03_022598A0->unk98 = 1;
        }
    };
    
    if (ov03_022598A0->unk98) {
        ov03_022598A0->unk98 = 0;
        RedrawListMenu(ov03_022598A0->listMenu);
        for (i = 0; i < ListMenuGetTemplateField(ov03_022598A0->listMenu, 3); i++) {
            ov03_02254A54(ov03_022598A0->listMenu, 0, i);
        };
    }
    
    s32 listState = ListMenu_ProcessInput(unk_ov03->listMenu);
    if (sub_02034780() != 0xFF) {
        unk_ov03->unk95 = sub_02034780();
        PlaySE(SEQ_SE_DP_PC_LOGIN);
        unk_ov03->unkA1 = 1;
        ov03_02254D24(unk_ov03->unk95, unk_ov03);
        ov03_02254B4C(&ov03_02255714);
        return;
    }
    if (sub_02037700()) {
        ov03_02254B44();
        ov03_0225574C(arg0, unk_ov03);
        ov03_022598A0->unk88 = 3;
        return;
    }
    
    if (listState == LIST_NOTHING_CHOSEN) {
        if (ov03_02255B84() > 2) {
            if (ov03_022598A0->unkA2 == 0) {
                listState = LIST_NO_MULTIPLE_SCROLL;
            } else if (ov03_022598A0->unkA2 == 1) {
                listState = LIST_CANCEL;
            }
        } else if (ov03_022598A0->unkA2 == 0) {
            listState = LIST_CANCEL;
        }
    
    }
    
    switch (listState) {
    case LIST_CANCEL:
        PlaySE(SEQ_SE_DP_SELECT);
        ov03_02254B4C(&ov03_0225558C);
        break;
    default:
        if (ov03_02255B70() <= sub_020347A0()) {
            u16 ov03_02259184[41] = {
                0,
                0,
                0,
                38,
                38,
                38,
                39,
                40,
                41,
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
                42,
            };
            
            PlaySE(SEQ_SE_DP_SELECT);
            unk_ov03->unk90 = sub_020347A0();
            ov03_02253E20(ov03_02259184[unk_ov03->unk91], 0);
            ov03_02254B4C(&ov03_022553C8);
        }
        break;
    case LIST_NOTHING_CHOSEN:
        break;
    }
}*/