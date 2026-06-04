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
extern u32 ov03_02255388;

void ov03_022552C8(void *arg0, UnkStruct_ov03 *unk_ov03);

s32 sub_020348F0();
void sub_02037B8C(u32, u8);

void ov03_02255078(void *arg0, UnkStruct_ov03 *unk_ov03) {
    ov03_02254C9C(unk_ov03);
    if (unk_ov03->unkA8 == 0) {
        if (ov03_02255CA0(unk_ov03) == 1) {
            unk_ov03->unkA8 = 1;
        } else {
            return;
        }
    }
    
    if (ov03_02255CD0(unk_ov03) == FALSE) {
        return;
    }
        int i;
        if (sub_020373B4(unk_ov03->unk95) == FALSE) {
            ov03_02255CE4(unk_ov03);
            if (unk_ov03->unk91 == 8 || unk_ov03->unk91 == 40) {
                for (i = 1; i < ov03_02255B84(); i++) {
                    if (sub_020373B4(i)) {
                        sub_020346E8(i);
                        sub_02037B8C(5, i);
                    }
                }
                ov03_02253E20(0, 0);
                ov03_02254B4C(&ov03_022552C8);
            } else {
                sub_020346E8(unk_ov03->unk95);
                ov03_02253E20(0, 0);
                ov03_02254B4C(&ov03_022552C8);
            }
        } else if (sub_02037700() || ov03_02255C80()) {
            ov03_02255CE4(unk_ov03);
            sub_020346E8(unk_ov03->unk95);
            ov03_02253E20(0, 0);
            ov03_02254B4C(&ov03_022552C8);
        } else if (ov01_021F6AEC(unk_ov03->fieldSystem) == 6) {
            if (unk_ov03->unkA4 == 0) {
            if ((unk_ov03->unk91 == 1 || unk_ov03->unk91 == 2 || (u8) (unk_ov03->unk91 + 0xDB) <= 1) && sub_020348F0() == 0) {
                ov03_02253E20(0x74, 0);
                ov03_02254B4C(&ov03_02255280);
                sub_02037B8C(4, unk_ov03->unk95);
            } else {
                sub_0203476C(unk_ov03->unk95);
                sub_02037B8C(2, unk_ov03->unk95);
                if (ov03_02255B84() == sub_020347A0()) {
                    unk_ov03->unk90 = sub_020347A0();
                    if (ov03_02255B84() > 2) {
                        ov03_02254B4C(&ov03_02255388);
                    } else {
                        ov03_0225574C(arg0, unk_ov03);
                        ov03_022557CC(unk_ov03);
                    }
                } else {
                    ov03_02254BEC();
                    ov03_02254B4C(&ov03_0225530C);
                }
            }
        } else if (unk_ov03->unkA4 == 1) {
            if (unk_ov03->unk91 == 8 || unk_ov03->unk91 == 40) {
                for (i = 1; i < ov03_02255B84(); i++) {
                    if (sub_020373B4(i)) {
                        sub_020346E8(i);
                        sub_02037B8C(5, i);
                    }
                }
                ov03_02254BEC();
                ov03_02254B4C(&ov03_0225530C);
            } else {
                sub_020346E8(unk_ov03->unk95);
                sub_02037B8C(3, unk_ov03->unk95);
                ov03_02254BEC();
                ov03_02254B4C(&ov03_0225530C);
            }
        }
    }
}

void ov03_02255280(void *arg0, UnkStruct_ov03 *unk_ov03) {
    if (IsPrintFinished(ov03_022598A0->printerID) && ((PAD_BUTTON_A | PAD_BUTTON_B) & gSystem.newKeys || System_GetTouchNew() == TRUE)) {
        ov03_0225574C(arg0, unk_ov03);
        ov03_02254B44();
        ov03_022598A0->unk88 = 4;
    }
}

void ov03_022552C8(void *arg0, UnkStruct_ov03 *unk_ov03) {
    ov03_02254C9C(unk_ov03);
    if (IsPrintFinished(ov03_022598A0->printerID) && ((PAD_BUTTON_A | PAD_BUTTON_B) & gSystem.newKeys || System_GetTouchNew() == TRUE)) {
        ov03_02254BEC();
        ov03_02254B4C(&ov03_0225530C);
    }
}

void ov03_0225530C(void *arg0_unused, UnkStruct_ov03 *unk_ov03) {
    ov03_02254C9C(unk_ov03);
    if (IsPrintFinished(ov03_022598A0->printerID)) {
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

/*void ov03_02255388(void *arg0_unused, UnkStruct_ov03 *unk_ov03) {
    u16 ov03_022591D6[] = {
        0,
        0,
        0,
        0x26,
        0x26,
        0x26,
        0x27,
        0x28,
        0x29,
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
        0x2A,
    };
    
    ov03_02254C9C(unk_ov03);
    ov03_02253E20(ov03_022591D6[unk_ov03->unk91], 0);
    ov03_02254B4C(&ov03_022553C8);
}*/