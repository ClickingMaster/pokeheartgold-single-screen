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

void ov03_0225554C(SysTask *task);

void ov03_0225554C(SysTask *task) {
    if (IsPrintFinished(ov03_022598A0->printerID) && ((PAD_BUTTON_A | PAD_BUTTON_B) & gSystem.newKeys || System_GetTouchNew() == TRUE)) {
        ov03_022598A0->unk88 = 1;
        SysTask_Destroy(task);
    }
}

/*void ov03_0225558C(void *arg0, UnkStruct_ov03 *unk_ov03) {
    u16 ov03_022593C2[] = { // This is probably not the actual contents.
        0,
        0,
        0,
        0x2B,
        0x2B,
        0x2B,
        0x2C,
        0x2D,
        0x2E,
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
        0x2F,
        0xBF,
        0,
        0xBF,
        0,
        2,
        0,
        0x37,
        0,
        15,
        0,
        15,
        0,
        16,
        0,
        0x37,
        0,
        2,
        0,
        16,
        0,
    };

    ov03_02254C9C(unk_ov03);
    if (sub_020347A0() > 1) {
        ov03_02253E20(ov03_022593C2[unk_ov03->unk91], 0);
        ov03_02254B4C(&ov03_022555F4);
    } else {
        ov03_02254B44();
        ov03_0225574C(arg0, unk_ov03);
        ov03_022598A0->unk88 = 1;
    }
}*/