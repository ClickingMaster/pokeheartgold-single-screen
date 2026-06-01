#include "overlay_03.h"

#include "field_system.h"
#include "global.h"
#include "mart.h"
#include "task.h"
#include "text.h"

extern UnkStruct_ov03* ov03_022598A0;

static void ov03_02253E20(s32 strno, BOOL arg1) { // ov03 func #1
    if (IsPrintFinished(ov03_022598A0->printerID) == FALSE) {
        RemoveTextPrinter(ov03_022598A0->printerID);
    }
    if (arg1) {
        ReadMsgDataIntoString(ov03_022598A0->msgData, strno, ov03_022598A0->string[4]);
        StringExpandPlaceholders(ov03_022598A0->messageFormat[2], ov03_022598A0->string[5], ov03_022598A0->string[4]);
    } else {
        ReadMsgDataIntoString(ov03_022598A0->msgData, strno, ov03_022598A0->string[5]);
    }
    if (WindowIsInUse(&ov03_022598A0->windows[2]) == FALSE) {
        sub_0205B514(ov03_022598A0->fieldSystem->bgConfig, &ov03_022598A0->windows[2], 3);
    }
    sub_0205B564(&ov03_022598A0->windows[2], Save_PlayerData_GetOptionsAddr(ov03_022598A0->fieldSystem->saveData));
    ov03_022598A0->printerID = sub_0205B5B4(&ov03_022598A0->windows[2], ov03_022598A0->string[5], Save_PlayerData_GetOptionsAddr(ov03_022598A0->fieldSystem->saveData), TRUE);
}