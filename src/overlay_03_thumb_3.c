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

void ov03_02254C9C(UnkStruct_ov03 *unk_ov03_unused) {
    s32 num;
    s32 strno;

    if (sub_020347A0() >= ov03_02255B70()) {
        num = sub_020347A0();
        strno = 81;
    } else {
        num = ov03_02255B70() - sub_020347A0();
        strno = 80;
    }
    BufferIntegerAsString(ov03_022598A0->messageFormat[1], 0, num, 2, PRINTING_MODE_5_UNUSED, TRUE);
    FillWindowPixelBuffer(&ov03_022598A0->windows[1], 0xF);
    ReadMsgDataIntoString(ov03_022598A0->msgData, strno, ov03_022598A0->string[7]);
    StringExpandPlaceholders(ov03_022598A0->messageFormat[1], ov03_022598A0->string[6], ov03_022598A0->string[7]);
    AddTextPrinterParameterized(&ov03_022598A0->windows[1], 0, ov03_022598A0->string[6], 2, 2, 0, NULL);
}

/*void ov03_02254D24(u32 arg0, UnkStruct_ov03 *unk_ov03) {
    u16 ov03_02259370[41] = 
    {
        24,
        25,
        26,
        27,
        31,
        31,
        37,
        31,
        31,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        32,
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
        33,
        33,
        0,
        34,
        35,
        36,
        0,
        0,
        0,
        0,
        28,
        28,
        29,
    };
    
    PlayerProfile *profile = sub_02034818(arg0);
    if (profile) {
        BufferPlayersName(unk_ov03->messageFormat[2], 1, profile);
    }
    ov03_02253E20(ov03_02259370[unk_ov03->unk91], TRUE);
}*/