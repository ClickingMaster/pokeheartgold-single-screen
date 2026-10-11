#include "overlay14_input_dispatch_r22_private.h"

u32 PCBox_State_StartSubApp(void *work)
{
    sPCBoxSubApps[U8_AT(work, 0x1e)].init(work);
    return 10;
}

u32 PCBox_State_RunSubApp(void *work)
{
    if (!OverlayManager_Run(PTR_AT(work, 0x14))) {
        return 10;
    }
    OverlayManager_Delete(PTR_AT(work, 0x14));
    sPCBoxSubApps[U8_AT(work, 0x1e)].exit(work);
    U32_AT(work, 0x30) = sPCBoxSubApps[U8_AT(work, 0x1e)].next;
    return 0;
}

void PCBox_State_Start(void *work)
{
    u32 msg;

    PlaySE(0x60c);
    switch (U32_AT(PTR_AT(work, 0), 8)) {
    case 0:
        ov14_021F0BF4(work);
        PCBox_PrintMonActionMessage(work, 0, 0, 0x27);
        PCBox_SetBoxArrowsVisible(PTR_AT(work, 0x34), 0);
        PCBox_SetIconsTranslucent(work, 1, 1);
        PCBox_ShowSlotMonInfo(work, 0x1e);
        msg = 0x5b;
        break;
    case 1:
        PCBox_PrintMonActionMessage(work, 0, 0, 0x27);
        PCBox_ShowSlotMonInfo(work, 0);
        msg = 0x51;
        break;
    case 2:
        PCBox_ShowSlotMonInfo(work, 0);
        msg = 0xc;
        break;
    case 3:
        PCBox_SetIconsTranslucent(work, 0x81, 1);
        PCBox_ShowSlotMonInfo(work, 0);
        msg = 0x75;
        break;
    }
    PCBox_StartFadeIn(work, msg);
}
