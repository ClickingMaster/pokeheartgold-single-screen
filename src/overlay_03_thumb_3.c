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

void ov03_02254C9C();

void ov03_02254C9C() {
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