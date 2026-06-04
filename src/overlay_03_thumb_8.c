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

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0182.h"

extern UnkStruct_ov03* ov03_022598A0;
extern u32 ov03_02255860;

void ov03_022555F4(void* arg0_unused, UnkStruct_ov03* unk_ov03);
void ov03_0225562C(void *arg0, UnkStruct_ov03 *unk_ov03);
void ov03_022556BC(void *arg0, UnkStruct_ov03 *unk_ov03);
void ov03_02255714(s32 arg0_unused, UnkStruct_ov03 *unk_ov03);
void ov03_02255B44(UnkStruct_ov03 *unk_ov03);

BOOL sub_02037A10();

void ov03_022555F4(void* arg0_unused, UnkStruct_ov03* unk_ov03) {
    ov03_02254C9C(unk_ov03);
    ListMenu_ProcessInput(unk_ov03->listMenu);
    if (IsPrintFinished(ov03_022598A0->printerID)) {
        ov03_02255C84(unk_ov03);
        ov03_02254B4C(&ov03_0225562C);
    }
}

void ov03_0225562C(void *arg0, UnkStruct_ov03 *unk_ov03) {
    ov03_02254C9C(unk_ov03);
    if (unk_ov03->unkA8 == 0) {
        if (ov03_02255CA0(unk_ov03) == 1) {
            unk_ov03->unkA8 = 1;
        } else {
            return;
        }
    }
    
    if (ov03_02255CD0(unk_ov03)) {
        if (sub_02037700()) {
            ov03_02255CE4(unk_ov03);
            ov03_02254BEC();
            ov03_02254B4C(&ov03_0225530C);
        }
        else if (ov01_021F6AEC(unk_ov03->fieldSystem) == 6) {
            if (unk_ov03->unkA4 == 0) {
                ov03_02254B44();
                ov03_0225574C(arg0, unk_ov03);
                ov03_022598A0->unk88 = 1;
            } else if (unk_ov03->unkA4 == 1) {
                ov03_02254BEC();
                ov03_02254B4C(&ov03_0225530C);
            }
        }
    }
}

void ov03_022556BC(void *arg0, UnkStruct_ov03 *unk_ov03) {
    if (sub_02037700() || unk_ov03->unk90 != sub_02037454()) {
        ov03_02254B4C(&ov03_022554E0);
    } else {
        ov03_02254C9C(unk_ov03);
        ListMenu_ProcessInput(unk_ov03->listMenu);
        if (sub_02037B38(10) && sub_02037A10()) {
            ov03_0225574C(arg0, unk_ov03);
            ov03_02255B44(unk_ov03);
        }
    } 
}

void ov03_02255714(s32 arg0_unused, UnkStruct_ov03 *unk_ov03) {
    ov03_02254C9C(unk_ov03);
    ListMenu_ProcessInput(unk_ov03->listMenu);
    if (IsPrintFinished(ov03_022598A0->printerID)) {
        ov03_02255C84(unk_ov03);
        ov03_02254B4C(&ov03_02255078);
    }
}

void ov03_0225574C(void *task, UnkStruct_ov03 *unk_ov03) {
    FillWindowPixelBuffer(&ov03_022598A0->windows[2], 15);
    sub_0200E5D4(&unk_ov03->windows[0], FALSE);
    ListMenuItems_Delete(unk_ov03->items);
    DestroyListMenu(unk_ov03->listMenu, NULL, NULL);
    unk_ov03->listMenu = NULL;
    ClearWindowTilemapAndCopyToVram(&unk_ov03->windows[0]);
    RemoveWindow(&unk_ov03->windows[0]);
    if (unk_ov03->unk97) {
        sub_0200E5D4(&ov03_022598A0->windows[1], 0);
        ClearWindowTilemapAndCopyToVram(&ov03_022598A0->windows[1]);
        RemoveWindow(&ov03_022598A0->windows[1]);
        unk_ov03->unk97 = 0;
    }
    if (task) {
        SysTask_Destroy(task);
    }
}

void ov03_022557CC(UnkStruct_ov03 *unk_ov03) {
    BufferPlayersName(unk_ov03->messageFormat[2], 1, sub_02034818(1));
    ov03_02253E20(69, TRUE);
    SysTask_CreateOnMainQueue(&ov03_02254D64, unk_ov03, 0);
    ov03_02254B4C(&ov03_02255860);
}