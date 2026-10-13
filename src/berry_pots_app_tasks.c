#include "global.h"

#include "constants/sndseq.h"

#include "berry_pots_app.h"
#include "berry_pots_app_internal.h"
#include "system.h"
#include "touchscreen.h"
#include "sound_playback.h"

typedef struct BerryPotsWateringCanMoveArgs {
    BerryPotsAppData *data;
    u8 unk4;
    u8 unk5;
    u8 startPot;
    u8 targetPot;
    u8 framesLeft;
    s8 sign;
    u8 lastWateredPot;
} BerryPotsWateringCanMoveArgs;

static const TouchscreenHitbox sBerryPotsCancelHitbox = {
    .rect = { 160, 192, 192, 0 }
};

static void Task_BerryPotsApp_MoveWateringCan(SysTask *task, BerryPotsWateringCanMoveArgs *args);

void BerryPotsApp_StartWateringCanMove(BerryPotsAppData *data) {
    BerryPotsWateringCanMoveArgs *args = Heap_Alloc(data->heapID, sizeof(BerryPotsWateringCanMoveArgs));
    MI_CpuFill8(args, 0, sizeof(BerryPotsWateringCanMoveArgs));
    args->data = data;
    args->startPot = data->wateringPot;
    args->targetPot = data->nextWateringPot;
    s8 diff = data->nextWateringPot - data->wateringPot;
    if (diff > 0) {
        args->sign = 1;
    } else {
        args->sign = -1;
        diff = -diff;
    }

    args->framesLeft = (diff *= 27) / 4;
    args->lastWateredPot = data->wateringPot;
    SysTask_CreateOnMainQueue((SysTaskFunc)Task_BerryPotsApp_MoveWateringCan, args, 0);
    data->runningTasks++;
}

static void Task_BerryPotsApp_MoveWateringCan(SysTask *task, BerryPotsWateringCanMoveArgs *args) {
    BerryPotsAppData *data = args->data;

    Sprite_OffsetPositionXY(data->sprites[2], args->sign * 4, 0);
    s16 x, y;
    Sprite_GetPositionXY(data->sprites[2], &x, &y);

    u8 potIndex = (x - 31) / 27;
    if (potIndex != args->lastWateredPot && potIndex != args->targetPot) {
        BerryPotsApp_WaterPot(data, potIndex);
        args->lastWateredPot = potIndex;
    }

    if (--args->framesLeft != 0) {
        return;
    }

    Sprite_SetPositionXY(data->sprites[2], args->targetPot * 27 + 31, 56);

    data->runningTasks--;
    Heap_Free(args);
    SysTask_Destroy(task);
}

typedef struct BerryPotsHarvestBerryArgs {
    BerryPotsAppData *data;
    u8 state;
    u8 framesOnScreen;
    u8 potIndex;
    ManagedSprite *berrySprite;
} BerryPotsHarvestBerryArgs;

static void Task_BerryPotsApp_HarvestBerry(SysTask *, BerryPotsHarvestBerryArgs *);

void BerryPotsApp_StartHarvestBerryAnim(BerryPotsAppData *data) {
    BerryPotsHarvestBerryArgs *args = Heap_Alloc(data->heapID, sizeof(BerryPotsHarvestBerryArgs));
    MI_CpuFill8(args, 0, sizeof(BerryPotsHarvestBerryArgs));

    args->data = data;
    args->potIndex = data->selectedPot;

    SysTask_CreateOnMainQueue((SysTaskFunc)Task_BerryPotsApp_HarvestBerry, args, 0);
    data->runningTasks++;
}

static void Task_BerryPotsApp_HarvestBerry(SysTask *task, BerryPotsHarvestBerryArgs *args) {
    BerryPotsAppData *data = args->data;
    switch (args->state) {
    case 0:
        args->berrySprite = BerryPotsApp_CreateBerryIconSprite(data, args->potIndex * 27 + 35, 80, data->harvestBerriesLeft + 2);
        PlaySE(SEQ_SE_DP_DECIDE);
        args->state++;
        break;
    case 1:
        ManagedSprite_OffsetPositionXY(args->berrySprite, 2, -2);
        if (args->framesOnScreen++ >= 8) {
            args->state++;
        }
        break;
    case 2:
        Sprite_DeleteAndFreeResources(args->berrySprite);

        data->runningTasks--;
        Heap_Free(args);
        SysTask_Destroy(task);
        break;
    }
}

void BerryPotsApp_FormatMessage(BerryPotsAppData *data, enum BerryPotsAppMessage action, u8 potIndex) {
    BerryPotsAppPot *unk = &data->pots[potIndex];

    switch (action) {
    case BERRY_POTS_APP_MSG_MULCH_LAID:
    case BERRY_POTS_APP_MSG_MULCH_SCATTERED:
        BufferItemName(data->msgFmt, 2, MulchIdToItemId(unk->mulch));
        break;
    case BERRY_POTS_APP_MSG_HAS_BERRIES:
    case BERRY_POTS_APP_MSG_PICKED:
        BufferIntegerAsString(data->msgFmt, 1, unk->quantityOrYieldMaybe, 2, PRINTING_MODE_LEFT_ALIGN, 0);
        // fallthrough
    case BERRY_POTS_APP_MSG_PLANTED:
    case BERRY_POTS_APP_MSG_SPROUTED:
    case BERRY_POTS_APP_MSG_GROWING:
    case BERRY_POTS_APP_MSG_BLOOM:
    case BERRY_POTS_APP_MSG_BERRY_PLANTED:
    case BERRY_POTS_APP_MSG_PUT_AWAY:
    case BERRY_POTS_APP_MSG_BAG_FULL: {
        String *berryName = GetNutName(unk->berryId, data->heapID);
        BufferString(data->msgFmt, 0, berryName, 0, 0, 2);
        String_Delete(berryName);
        break;
    }
    default:
        String_Copy(data->currentStatusString, data->statusStrings[action]);
        return;
    }

    StringExpandPlaceholders(data->msgFmt, data->currentStatusString, data->statusStrings[action]);
}

void BerryPotsApp_UpdateGrowthFromRTC(BerryPotsAppData *data, BOOL a1) {
    RTCDate date1;
    RTCDate date2;
    RTCTime time1;
    RTCTime time2;

    GF_RTC_CopyDateTime(&date1, &time1);
    s64 timestamp1 = RTC_ConvertDateTimeToSecond(&date1, &time1);
    BerryPots_CopyBerryDatetime(data->berryDatetime, &date2, &time2);
    s64 timestamp2 = RTC_ConvertDateTimeToSecond(&date2, &time2);
    if (timestamp1 < timestamp2) {
        BerryPots_SetBerryDatetime(data->berryDatetime, date1, time1);
        return;
    }

    u32 minutesDelta = (timestamp1 - timestamp2) / 60;
    if (minutesDelta > 0) {
        BerryPots_UpdateGrowth(data->berryPots, data->berryGrowthProperties, minutesDelta);
        BerryPotsApp_RefreshPotData(data);
        BerryPots_SetBerryDatetime(data->berryDatetime, date1, time1);
        if (a1) {
            BerryPotsApp_StartGrowthAnims(data);
        }
    }
}

void BerryPotsApp_RefreshPotData(BerryPotsAppData *data) {
    for (int i = 0; i < MAX_BERRY_POT; i++) {
        data->pots[i].prevBerryId = data->pots[i].berryId;
        data->pots[i].prevGrowthStage = data->pots[i].growthStage;
        data->pots[i].prevSoilState = data->pots[i].soilState;

        data->pots[i].berryId = BerryPots_GetPotBerryId(data->berryPots, i);
        data->pots[i].growthStage = BerryPots_GetPotGrowthStage(data->berryPots, i);
        data->pots[i].health = BerryPots_GetPotHealth(data->berryPots, i);
        data->pots[i].moisture = BerryPots_GetPotMoisture(data->berryPots, i);
        data->pots[i].mulch = BerryPots_GetPotMulch(data->berryPots, i);
        data->pots[i].quantityOrYieldMaybe = BerryPots_GetPotYield(data->berryPots, i);
        data->pots[i].minutesToNextStage = BerryPots_GetPotMinutesToNextStage(data->berryPots, i);
        data->pots[i].drainMinutes = BerryPots_GetPotDrainMinutes(data->berryPots, i);
        data->pots[i].soilState = BerryPots_GetSoilState(data->berryPots, i);
    }
}

typedef struct BerryPotsGrowthAnimArgs {
    BerryPotsAppData *data;
    u8 potIndex;
    u8 frame;
    u8 state;
} BerryPotsGrowthAnimArgs;

static void Task_BerryPotsApp_GrowthAnim(SysTask *task, BerryPotsGrowthAnimArgs *args);

static void BerryPotsApp_StartGrowthAnims(BerryPotsAppData *data) {
    for (int i = 0; i < MAX_BERRY_POT; i++) {
        BerryPotsAppPot *sub = &data->pots[i];

        if (sub->soilState != sub->prevSoilState) {
            Sprite_SetAnimationFrame(data->sprites[3 + i], sub->soilState);
            if (sub->growthStage == BERRY_POT_GROWTH_STAGE_PLANTED) {
                Sprite_SetAnimationFrame(sub->soilSpriteMaybe, sub->soilState);
            }
        }

        if (sub->growthStage == sub->prevGrowthStage) {
            continue;
        }

        BerryPotsGrowthAnimArgs *args = Heap_Alloc(data->heapID, sizeof(BerryPotsGrowthAnimArgs));
        MI_CpuFill8(args, 0, sizeof(BerryPotsGrowthAnimArgs));
        args->data = data;
        args->potIndex = i;
        SysTask_CreateOnMainQueue((SysTaskFunc)Task_BerryPotsApp_GrowthAnim, args, 0);
        data->runningTasks++;
    }
}

static void Task_BerryPotsApp_GrowthAnim(SysTask *task, BerryPotsGrowthAnimArgs *args) {
    BerryPotsAppData *data = args->data;

    switch (args->state) {
    case 0:
        Sprite_SetDrawFlag(data->sprites[7 + args->potIndex], TRUE);
        PlaySE(SEQ_SE_PL_EFF05);
        args->state++;
        break;
    case 1:
        if (args->frame++ == 15) {
            BerryPotsApp_UpdatePlantSprite(data, args->potIndex);
            if (data->selectedPot == args->potIndex) {
                BerryPotsApp_UpdateCursor(data);
            }
        }

        if (args->frame >= 30) {
            Sprite_SetDrawFlag(data->sprites[7 + args->potIndex], FALSE);

            data->runningTasks--;
            Heap_Free(args);
            SysTask_Destroy(task);
        }

        break;
    }
}

u32 BerryPotsApp_HandleSelectPotInput(BerryPotsAppData *data) {
    BOOL flag = FALSE;
    u32 r0 = BerryPotsApp_HandleSelectPotTouch(data, &flag);
    if (!flag) {
        r0 = BerryPotsApp_HandleSelectPotKeys(data);
    }
    return r0;
}

BOOL BerryPotsApp_HandleWateringInput(BerryPotsAppData *data) {
    BOOL flag = FALSE;
    BOOL ret = BerryPotsApp_HandleWateringTouch(data, &flag);
    if (!flag) {
        ret = BerryPotsApp_HandleWateringKeys(data);
    }
    return ret;
}

static u32 BerryPotsApp_SelectPotAction(BerryPotsAppData *data, u32 a1) {
    switch (a1) {
    case 0:
    case 1:
    case 2:
    case 3:
        PlaySE(SEQ_SE_DP_SELECT);
        BerryPotsApp_UpdateCursor(data);
        break;
    case 4:
        if (data->runningTasks == 0) {
            return BerryPotsApp_GetPotMenuState(data);
        }
        break;
    case 5:
    default:
        if (data->runningTasks == 0) {
            PlaySE(SEQ_SE_DP_SELECT);
            BerryPotsApp_DrawCancelButton(data, 1);
            return 1;
        }
        break;
    }

    return 2;
}

static u32 BerryPotsApp_HandleSelectPotKeys(BerryPotsAppData *data) {
    BOOL flag = FALSE;

    if (gSystem.newKeys & (PAD_BUTTON_Y | PAD_BUTTON_X | PAD_KEY_DOWN | PAD_KEY_UP | PAD_KEY_LEFT | PAD_KEY_RIGHT | PAD_BUTTON_B | PAD_BUTTON_A)) {
        data->fieldMenuState = MENU_INPUT_STATE_BUTTONS;
    }

    u32 newKeys = gSystem.newKeys;

    if (newKeys & PAD_BUTTON_B) {
        return BerryPotsApp_SelectPotAction(data, 5);
    }

    if (newKeys & PAD_BUTTON_A) {
        return BerryPotsApp_SelectPotAction(data, 4);
    }

    if (newKeys & PAD_KEY_LEFT) {
        data->selectedPot = (data->selectedPot + (MAX_BERRY_POT - 1)) % MAX_BERRY_POT;
        flag = TRUE;
    } else if (newKeys & PAD_KEY_RIGHT) {
        data->selectedPot = (data->selectedPot + 1) % MAX_BERRY_POT;
        flag = TRUE;
    }

    if (flag) {
        return BerryPotsApp_SelectPotAction(data, data->selectedPot);
    } else {
        return 2;
    }
}

static u32 BerryPotsApp_HandleSelectPotTouch(BerryPotsAppData *data, BOOL *a1) {
    int unk = BerryPotsApp_GetSelectPotTouch(data);
    if (unk < 0) {
        return 2;
    }

    *a1 = TRUE;
    data->fieldMenuState = MENU_INPUT_STATE_TOUCH;
    return BerryPotsApp_SelectPotAction(data, unk);
}

static BOOL BerryPotsApp_HandleWateringKeys(BerryPotsAppData *data) {
    BOOL unk = FALSE;

    if (data->nextWateringPot != 0xFF) {
        return FALSE;
    }

    if (gSystem.newKeys & (PAD_BUTTON_Y | PAD_BUTTON_X | PAD_KEY_DOWN | PAD_KEY_UP | PAD_KEY_LEFT | PAD_KEY_RIGHT | PAD_BUTTON_B | PAD_BUTTON_A)) {
        data->fieldMenuState = MENU_INPUT_STATE_BUTTONS;
    }

    if (gSystem.newKeys & PAD_KEY_LEFT) {
        data->selectedPot = (data->selectedPot + (MAX_BERRY_POT - 1)) % MAX_BERRY_POT;
        unk = TRUE;
    } else if (gSystem.newKeys & PAD_KEY_RIGHT) {
        data->selectedPot = (data->selectedPot + 1) % MAX_BERRY_POT;
        unk = TRUE;
    }

    if (unk) {
        data->nextWateringPot = data->selectedPot;
        Sprite_SetAnimActiveFlag(data->sprites[1], FALSE);
        BerryPotsApp_UpdateCursor(data);
        return TRUE;
    }

    return FALSE;
}

static BOOL BerryPotsApp_HandleWateringTouch(BerryPotsAppData *data, BOOL *a1) {
    if (data->nextWateringPot != 0xFF) {
        return FALSE;
    }

    int unk = BerryPotsApp_GetTouchedPot(data);
    if (unk < 0 || data->selectedPot == unk) {
        return FALSE;
    }

    *a1 = TRUE;
    data->fieldMenuState = MENU_INPUT_STATE_TOUCH;
    data->selectedPot = unk;
    data->nextWateringPot = data->selectedPot;
    Sprite_SetAnimActiveFlag(data->sprites[1], FALSE);
    BerryPotsApp_UpdateCursor(data);

    return TRUE;
}

static int BerryPotsApp_GetTouchedPot(BerryPotsAppData *data) {
    TouchscreenHitbox touchHitbox;

    for (int i = 0, xOffsetMaybe = 16; i < MAX_BERRY_POT; xOffsetMaybe += 28, i++) {
        touchHitbox.rect.left = xOffsetMaybe;
        touchHitbox.rect.right = touchHitbox.rect.left + 22;
        if (data->pots[i].growthStage < BERRY_POT_GROWTH_STAGE_GROWING) {
            touchHitbox.rect.top = 88;
            touchHitbox.rect.bottom = touchHitbox.rect.top + 24;
        } else {
            touchHitbox.rect.top = 72;
            touchHitbox.rect.bottom = touchHitbox.rect.top + 32;
        }

        if (TouchscreenHitbox_TouchNewIsIn(&touchHitbox)) {
            return i;
        }
    }
    return -1;
}

static int BerryPotsApp_GetSelectPotTouch(BerryPotsAppData *data) {
    if (TouchscreenHitbox_TouchNewIsIn(&sBerryPotsCancelHitbox)) {
        return 5;
    }

    int unk = BerryPotsApp_GetTouchedPot(data);
    if (unk >= 0) {
        data->selectedPot = unk;
        BerryPotsApp_UpdateCursor(data);
        return 4;
    }

    return -1;
}

static u32 BerryPotsApp_GetPotMenuState(BerryPotsAppData *data) {
    BerryPotsAppPot *unk = &data->pots[data->selectedPot];
    switch (unk->growthStage) {
    case BERRY_POT_GROWTH_STAGE_NONE:
        return (unk->mulch != MULCH_NONE) ? 4 : 3;
    case BERRY_POT_GROWTH_STAGE_PLANTED:
    case BERRY_POT_GROWTH_STAGE_SPROUTED:
    case BERRY_POT_GROWTH_STAGE_GROWING:
    case BERRY_POT_GROWTH_STAGE_BLOOM:
        return 5;
    case BERRY_POT_GROWTH_STAGE_BERRIES:
        return 6;
    case 0xFF:
    default:
        return 2;
    }
}
