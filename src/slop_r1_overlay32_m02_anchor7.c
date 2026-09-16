#include "constants/sndseq.h"

#include "msgdata/msg/msg_0196.h"

#include "filesystem_files_def.h"
#include "slop_r1_overlay32_module02_internal.h"
#include "sys_task_api.h"

void ov32_0225DDC4(SysTask *task, void *taskData) {
    NamePickerData *data = taskData;

    switch (data->state) {
    case NAME_PICKER_STATE_INPUT:
        data->state = ov32_0225DE34(data);
        break;
    case NAME_PICKER_STATE_BLINK:
        data->state = ov32_0225DF80(data);
        break;
    case NAME_PICKER_STATE_REDRAW:
        ov32_0225D84C(data);
        ov32_0225D988(data);
        data->state = NAME_PICKER_STATE_INPUT;
        break;
    case NAME_PICKER_STATE_DONE:
        *data->result = data->selection;
        break;
    }

    ov32_0225DD04(data);
    SpriteList_RenderAndAnimateSprites(data->spriteList);
}

int ov32_0225DE34(NamePickerData *data) {
    u32 input;
    int touch;

    touch = TouchscreenHitbox_FindRectAtTouchNew(_0225E15C);
    if (touch == 0) {
        PlaySE(SEQ_SE_DP_SELECT78);
        return ov32_0225DF9C(data, -1);
    }
    if (touch == 1) {
        PlaySE(SEQ_SE_DP_SELECT78);
        return ov32_0225DF9C(data, 1);
    }

    input = GridInputHandler_HandleInput_AllowHold(data->gridInput);
    switch (input) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        PlaySE(SEQ_SE_DP_DECIDE);
        data->selection = input + data->page * NAME_PICKER_NAMES_PER_PAGE;
        return ov32_0225E0FC(data, input % 2 * 16 + 2, input / 2 * 5, NAME_PICKER_STATE_DONE);
    case NAME_PICKER_INPUT_CANCEL:
    case GRID_MENU_CANCEL:
        PlaySE(SEQ_SE_DP_DECIDE);
        data->selection = -2;
        return ov32_0225E0A8(data, NAME_PICKER_SPRITE_CANCEL, 3, 2, NAME_PICKER_STATE_DONE);
    case GRID_MENU_NOTHING_CHOSEN:
        input = GridInputHandler_GetNextInput(data->gridInput);
        if (gSystem.newAndRepeatedKeys & PAD_KEY_RIGHT) {
            if (input == 1 || input == 3 || input == 5 || input == 7) {
                PlaySE(SEQ_SE_DP_SELECT);
                return ov32_0225DF9C(data, 1);
            }
        }
        if (gSystem.newAndRepeatedKeys & PAD_KEY_LEFT) {
            if (input == 0 || input == 2 || input == 4 || input == 6) {
                PlaySE(SEQ_SE_DP_SELECT);
                return ov32_0225DF9C(data, -1);
            }
        }
        break;
    case GRID_MENU_CURSOR_MOVE:
        PlaySE(SEQ_SE_DP_SELECT);
        break;
    case GRID_MENU_BUTTON_MODE:
        break;
    }
    return NAME_PICKER_STATE_INPUT;
}
