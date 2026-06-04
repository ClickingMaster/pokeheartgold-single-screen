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
#include "unk_020379A0.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0182.h"

extern UnkStruct_ov03* ov03_022598A0;
extern u32 ov03_022556BC;

void ov03_022553C8(void *arg0_unused, UnkStruct_ov03 *unk_ov03);
void ov03_02255404(void *arg0, UnkStruct_ov03 *unk_ov03);

int sub_02037454();
void sub_02037EC0(u32 arg0);

void ov03_022553C8(void *arg0_unused, UnkStruct_ov03 *unk_ov03) {
    ov03_02254C9C(unk_ov03);
    sub_02037EC0(0);
    ListMenu_ProcessInput(unk_ov03->listMenu);
    if (IsPrintFinished(ov03_022598A0->printerID)) {
        ov03_02255C84(unk_ov03);
        ov03_02254B4C(&ov03_02255404);
    }
}

void ov03_02255404(void *arg0, UnkStruct_ov03 *unk_ov03) {
    ov03_02254C9C(unk_ov03);
    if (unk_ov03->unkA8 == 0) {
        if (ov03_02255CA0(unk_ov03) == 1) {
            unk_ov03->unkA8 = 1;
        } else {
            return;
        }
    }
    
    if (ov03_02255CD0(unk_ov03)) {
        if (sub_02037700() || unk_ov03->unk90 != sub_02037454()) {
            ov03_02255CE4(unk_ov03);
            ov03_02254B4C(&ov03_022554E0);
        } else {
            if (ov01_021F6AEC(unk_ov03->fieldSystem) == 6) {
                if (unk_ov03->unkA4 == 0) {
                    if (unk_ov03->unk91 == 8 || unk_ov03->unk91 == 40) {
                        for (int i = 1; i < sub_02037454(); i++)
                        {
                            if (sub_020373B4(i) == 0) {
                                ov03_02254B4C(&ov03_022554E0);
                                return;
                            }
                        }
                    }
                    sub_02037AC0(10);
                    ov03_02254B4C(&ov03_022556BC);
                } else if (unk_ov03->unkA4 == 1) {
                    sub_02037EC0(1);
                    ov03_02254B44();
                    ov03_0225574C(arg0, unk_ov03);
                    ov03_022598A0->unk88 = 1;
                }
            }
        }    
    }
}

/*void ov03_022554E0(void *arg0, UnkStruct_ov03 *unk_ov03) {
    u16 ov03_022592CC[] = {
        0,
        0,
        0,
        0,
        0,
        0,
        0x41,
        0x42,
        0x43,
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
        0,
        0x44,
    };

    ov03_0225574C(arg0, unk_ov03);
    if (ov03_022598A0->unk91 == 8) {
        ov03_02253E20(ov03_022598A0->unk92 + 0x89, 0);
    } else {
        ov03_02253E20(ov03_022592CC[unk_ov03->unk91], 0);
    }
    sub_02058284();
    SysTask_CreateOnMainQueue(ov03_0225554C, unk_ov03, 0);
}*/